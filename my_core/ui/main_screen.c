#include "stdio.h"
#include "string.h"
#include "lvgl.h"
#include "main_screen.h"
#include "Finger.h"
#include "auto_scr.h"
#include "main.h"
#include "FreeRtos_task.h"
extern Fingertask_t Hand[];
extern volatile uint8_t g_auto_status;
extern volatile uint16_t g_auto_cycles;

static uint8_t rehab_roun = 0;
static uint8_t rehab_total_rounds = 5;

static lv_timer_t *rehab_timer = NULL;
static int32_t rehab_left_ms = 15000;   // 15秒
static uint8_t rehab_running_State = 0;
static uint32_t rehab_start_tick = 0;
static uint32_t rehab_duration_ms = 15000;
static int8_t rehab_last_gesture = -1;
static int32_t rehab_last_show_sec = -1;
static uint16_t rehab_current_score = 0U;
static uint16_t rehab_score_sum = 0U;
static uint8_t rehab_score_count = 0U;
static uint16_t rehab_score_avg = 0U;
static uint16_t rehab_best_score = 0U;
static uint8_t rehab_best_valid = 0U;
static uint16_t rehab_round_best_score = 0U;

Main_scr_t Main_scr;

typedef enum
{
    UI_PAGE_HOME = 0,
    UI_PAGE_FREE,
    UI_PAGE_REHAB,
    UI_PAGE_EMG,
    UI_PAGE_CALIB
} ui_page_t;

typedef struct
{
    lv_obj_t * home_scr;
    lv_obj_t * free_scr;
    lv_obj_t * rehab_scr;
    lv_obj_t * emg_scr;
    lv_obj_t * calib_scr;
    lv_obj_t * header_title;
    lv_obj_t * header_subtitle;
    lv_obj_t * header_badge;
    lv_obj_t * status_chip;
    lv_obj_t * free_bars[5];
    lv_obj_t * free_values[5];
    lv_obj_t * free_status_label;
    lv_obj_t * free_calib_label;
    lv_obj_t * rehab_status_label;
    lv_obj_t * rehab_cycles_label;
    lv_obj_t * rehab_target_text;
    lv_obj_t * rehab_time_label;
    lv_obj_t * rehab_score_label;
    lv_obj_t * rehab_start_btn;
    lv_obj_t * rehab_start_btn_text;
    lv_obj_t * rehab_target_img;
    lv_obj_t * emg_hint_label;
    lv_obj_t * calib_ready_label;
    lv_obj_t * calib_channels_label;
    lv_obj_t * calib_rehab_best_label;
    lv_obj_t * calib_refs_label;
    lv_obj_t * emg_state_label;
    lv_obj_t * emg_level_label;
    lv_obj_t * emg_peak_label;
    lv_obj_t * emg_timer_label;
    lv_obj_t * emg_avg_label;
    lv_obj_t * emg_activity_label;
    lv_obj_t * emg_baseline_label;
    lv_obj_t * emg_delta_label;

    lv_timer_t * refresh_timer;
    ui_page_t current_page;
} ui_context_t;

static ui_context_t s_ui;

static const char * k_finger_names[5] = {
    "Thumb", "Index", "Middle", "Ring", "Little"
};

static hand_app_mode_t ui_page_to_hand_mode(ui_page_t page)
{
    switch(page)
    {
    case UI_PAGE_FREE:  return HAND_APP_MODE_FREE;
    case UI_PAGE_REHAB: return HAND_APP_MODE_REHAB;
    case UI_PAGE_EMG:   return HAND_APP_MODE_EMG;
    case UI_PAGE_CALIB: return HAND_APP_MODE_CALIB;
    case UI_PAGE_HOME:
    default:            return HAND_APP_MODE_IDLE;
    }
}

static const char * ui_emg_state_text(emg_state_t state)
{
    switch(state)
    {
        case EMG_STATE_REST:
            return "Rest";
        case EMG_STATE_ACTIVE:
            return "Active";
        case EMG_STATE_STRONG:
            return "Strong";
        default:
            return "Idle";
    }
}


static lv_color_t color_bg_top(void) { return lv_color_hex(0x000000); }
static lv_color_t color_bg_bot(void) { return lv_color_hex(0x000000); }
static lv_color_t color_panel(void) { return lv_color_hex(0x111111); }
static lv_color_t color_panel_2(void) { return lv_color_hex(0x111111); }
static lv_color_t color_accent(void) { return lv_color_hex(0x4FD1FF); }
static lv_color_t color_green(void) { return lv_color_hex(0x43E0AA); }
static lv_color_t color_warm(void) { return lv_color_hex(0xFFAA4C); }
static lv_color_t color_text(void) { return lv_color_hex(0xF5F7FA); }
static lv_color_t color_muted(void) { return lv_color_hex(0x93A1B3); }
static lv_color_t color_line(void) { return lv_color_hex(0x2D3946); }

static int16_t ui_step_to_angle(uint16_t step)
{
    return (int16_t)(((uint32_t)step * 300U) / 1023U);
}

static uint16_t ui_angle_to_percent(int16_t angle)
{
    if(angle <= 49) return 0U;
    if(angle >= 190) return 100U;
    return (uint16_t)(((uint32_t)(angle - 49) * 100U) / (190U - 49U));
}

static uint16_t ui_finger_percent(uint8_t finger_id)
{
    int16_t angle_deg;

    if(finger_id >= Finger_NUM_ALL)
    {
        return 0U;
    }

    angle_deg = ui_step_to_angle((uint16_t)Hand[finger_id].target_angle);
    return ui_angle_to_percent(angle_deg);
}
static uint8_t ui_enabled_root_count(void)
{
    uint8_t count = 0U;
    uint8_t i;

    for(i = Finger_Thumb; i <= Finger_Little; i++)
    {
        if(Hand[i].adc_scan != 255U)
        {
            count++;
        }
    }
    return count;
}

static uint8_t ui_calibrated_root_count(void)
{
    uint8_t count = 0U;
    uint8_t i;

    for(i = Finger_Thumb; i <= Finger_Little; i++)
    {
        if((Hand[i].adc_scan != 255U) && (Hand[i].calib_ok != 0U))
        {
            count++;
        }
    }
    return count;
}

static void ui_set_label_font(lv_obj_t *obj, const lv_font_t *font, lv_color_t color)
{
    lv_obj_set_style_text_font(obj, font, 0);
    lv_obj_set_style_text_color(obj, color, 0);
}

static void ui_label_set_text_if_changed(lv_obj_t *label, const char *text)
{
    const char *old_text;

    if(label == NULL || text == NULL)
    {
        return;
    }

    old_text = lv_label_get_text(label);
    if(old_text != NULL && strcmp(old_text, text) == 0)
    {
        return;
    }

    lv_label_set_text(label, text);
}

static void ui_label_set_text_fmt_u16(lv_obj_t *label, const char *fmt, unsigned int value)
{
    char buf[48];

    if(label == NULL || fmt == NULL)
    {
        return;
    }

    snprintf(buf, sizeof(buf), fmt, value);
    ui_label_set_text_if_changed(label, buf);
}

static void ui_label_set_text_fmt_u16_u16(lv_obj_t *label, const char *fmt, unsigned int v1, unsigned int v2)
{
    char buf[64];

    if(label == NULL || fmt == NULL)
    {
        return;
    }

    snprintf(buf, sizeof(buf), fmt, v1, v2);
    ui_label_set_text_if_changed(label, buf);
}

static void ui_style_screen(lv_obj_t *scr)
{
    lv_obj_set_style_bg_color(scr, color_bg_top(), 0);
    lv_obj_set_style_bg_grad_color(scr, color_bg_top(), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_100, 0);
    lv_obj_set_style_border_width(scr, 0, 0);
    lv_obj_set_style_pad_all(scr, 0, 0);
}

static lv_obj_t * ui_create_panel(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h)
{
    lv_obj_t *panel = lv_obj_create(parent);
    lv_obj_set_size(panel, w, h);
    lv_obj_set_pos(panel, x, y);
    lv_obj_set_style_radius(panel, 8, 0);
    lv_obj_set_style_bg_color(panel, color_panel(), 0);
    lv_obj_set_style_bg_grad_color(panel, color_panel_2(), 0);
    lv_obj_set_style_bg_opa(panel, LV_OPA_100, 0);
    lv_obj_set_style_border_width(panel, 1, 0);
    lv_obj_set_style_border_color(panel, color_line(), 0);
    lv_obj_set_style_shadow_width(panel, 0, 0);
    lv_obj_set_style_pad_all(panel, 16, 0);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);
    return panel;
}

static void ui_nav_to(lv_event_t *e)
{
    ui_page_t page = (ui_page_t)(uintptr_t)lv_event_get_user_data(e);
    lv_obj_t *target = s_ui.home_scr;

    switch(page)
    {
    case UI_PAGE_HOME:  target = s_ui.home_scr; break;
    case UI_PAGE_FREE:  target = s_ui.free_scr; break;
    case UI_PAGE_REHAB: target = s_ui.rehab_scr; break;
    case UI_PAGE_EMG:   target = s_ui.emg_scr; break;
    case UI_PAGE_CALIB: target = s_ui.calib_scr; break;
    default: break;
    }

    s_ui.current_page = page;
    HandApp_SetMode(ui_page_to_hand_mode(page));
    lv_scr_load(target);
}

static void ui_page_gesture_back_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_GESTURE)
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        if(dir == LV_DIR_RIGHT)
        {
            s_ui.current_page = UI_PAGE_HOME;
            HandApp_SetMode(HAND_APP_MODE_IDLE);
            lv_scr_load(s_ui.home_scr);
        }
    }
}

static lv_obj_t * ui_create_top_bar(lv_obj_t *parent,
                                    const char *title,
                                    const char *subtitle,
                                    const char *badge_text,
                                    ui_page_t back_target)
{
    lv_obj_t *bar = lv_obj_create(parent);
    lv_obj_set_size(bar, 760, 70);
    lv_obj_set_pos(bar, 20, 16);
    lv_obj_set_style_radius(bar, 8, 0);
    lv_obj_set_style_bg_color(bar, lv_color_hex(0x101010), 0);
    lv_obj_set_style_bg_opa(bar, LV_OPA_100, 0);
    lv_obj_set_style_border_width(bar, 1, 0);
    lv_obj_set_style_border_color(bar, lv_color_hex(0x27313B), 0);
    lv_obj_set_style_shadow_width(bar, 0, 0);
    lv_obj_clear_flag(bar, LV_OBJ_FLAG_SCROLLABLE);

    if(back_target != UI_PAGE_HOME)
    {
        lv_obj_t *back_btn = lv_btn_create(bar);
        lv_obj_set_size(back_btn, 104, 40);
        lv_obj_set_pos(back_btn, 12, 14);
        lv_obj_set_style_radius(back_btn, 6, 0);
        lv_obj_set_style_bg_color(back_btn, lv_color_hex(0x2A2A2A), 0);
        lv_obj_set_style_bg_grad_color(back_btn, lv_color_hex(0x2A2A2A), 0);
        lv_obj_set_style_bg_opa(back_btn, LV_OPA_100, 0);
        lv_obj_set_style_border_width(back_btn, 2, 0);
        lv_obj_set_style_border_color(back_btn, color_accent(), 0);
        lv_obj_set_style_bg_color(back_btn, lv_color_hex(0x3A3A3A), LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(back_btn, lv_color_hex(0x323232), LV_STATE_FOCUSED);
        lv_obj_add_event_cb(back_btn, ui_nav_to, LV_EVENT_CLICKED, (void *)(uintptr_t)back_target);
        lv_obj_t *back_lbl = lv_label_create(back_btn);
        lv_label_set_text(back_lbl, LV_SYMBOL_LEFT " Home");
        ui_set_label_font(back_lbl, &lv_font_montserrat_16, color_text());
        lv_obj_center(back_lbl);
    }

    lv_obj_t *label_title = lv_label_create(bar);
    lv_label_set_text(label_title, title);
    ui_set_label_font(label_title, &lv_font_montserrat_26, color_text());
    lv_obj_set_pos(label_title, back_target == UI_PAGE_HOME ? 18 : 118, 10);

    if(subtitle != NULL && subtitle[0] != '\0')
    {
        lv_obj_t *label_sub = lv_label_create(bar);
        lv_label_set_text(label_sub, subtitle);
        ui_set_label_font(label_sub, &lv_font_montserrat_14, color_muted());
        lv_obj_set_pos(label_sub, back_target == UI_PAGE_HOME ? 20 : 120, 40);
    }

    if(badge_text != NULL)
    {
        lv_obj_t *badge = lv_obj_create(bar);
        lv_obj_set_size(badge, 120, 32);
        lv_obj_align(badge, LV_ALIGN_RIGHT_MID, -14, 0);
        lv_obj_set_style_radius(badge, 6, 0);
        lv_obj_set_style_bg_color(badge, lv_color_hex(0x151515), 0);
        lv_obj_set_style_bg_opa(badge, LV_OPA_100, 0);
        lv_obj_set_style_border_width(badge, 1, 0);
        lv_obj_set_style_border_color(badge, color_accent(), 0);
        lv_obj_clear_flag(badge, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_t *badge_lbl = lv_label_create(badge);
        lv_label_set_text(badge_lbl, badge_text);
        ui_set_label_font(badge_lbl, &lv_font_montserrat_14, color_accent());
        lv_obj_center(badge_lbl);
    }

    return bar;
}

static lv_obj_t * ui_create_mode_card(lv_obj_t *parent,
                                      lv_coord_t x,
                                      lv_coord_t y,
                                      lv_coord_t w,
                                      lv_coord_t h,
                                      const char *title,
                                      const char *body,
                                      lv_color_t accent,
                                      ui_page_t target_page)
{
    lv_obj_t *card = lv_btn_create(parent);
    lv_obj_set_size(card, w, h);
    lv_obj_set_pos(card, x, y);
    lv_obj_set_style_radius(card, 8, 0);
    lv_obj_set_style_bg_color(card, color_panel(), 0);
    lv_obj_set_style_bg_grad_color(card, color_panel(), 0);
    lv_obj_set_style_bg_opa(card, LV_OPA_100, 0);
    lv_obj_set_style_border_width(card, 1, 0);
    lv_obj_set_style_border_color(card, lv_color_hex(0x2A3440), 0);
    lv_obj_set_style_shadow_width(card, 0, 0);
    lv_obj_set_style_bg_color(card, lv_color_hex(0x202020), LV_STATE_PRESSED);
    lv_obj_set_style_border_color(card, accent, LV_STATE_PRESSED);
    lv_obj_set_style_outline_width(card, 2, LV_STATE_FOCUSED);
    lv_obj_set_style_outline_color(card, accent, LV_STATE_FOCUSED);
    lv_obj_add_event_cb(card, ui_nav_to, LV_EVENT_CLICKED, (void *)(uintptr_t)target_page);

    lv_obj_t *glow = lv_obj_create(card);
    lv_obj_set_size(glow, 6, h - 28);
    lv_obj_set_pos(glow, 14, 14);
    lv_obj_set_style_radius(glow, 3, 0);
    lv_obj_set_style_bg_color(glow, accent, 0);
    lv_obj_set_style_bg_opa(glow, LV_OPA_100, 0);
    lv_obj_set_style_border_width(glow, 0, 0);
    lv_obj_clear_flag(glow, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *title_lbl = lv_label_create(card);
    lv_label_set_text(title_lbl, title);
    ui_set_label_font(title_lbl, &lv_font_montserrat_22, color_text());
    lv_obj_set_pos(title_lbl, 32, 18);

    lv_obj_t *body_lbl = lv_label_create(card);
    lv_obj_set_width(body_lbl, w - 80);
    lv_label_set_long_mode(body_lbl, LV_LABEL_LONG_WRAP);
    lv_label_set_text(body_lbl, body);
    ui_set_label_font(body_lbl, &lv_font_montserrat_14, color_muted());
    lv_obj_set_pos(body_lbl, 32, 56);

    return card;
}

static void ui_build_home(void)
{
    s_ui.home_scr = lv_obj_create(NULL);
    ui_style_screen(s_ui.home_scr);
    Main_scr.Main_control_scr = s_ui.home_scr;
    ui_create_top_bar(s_ui.home_scr,
                      "Bionic Hand",
                      "",
                      "HOME",
                      UI_PAGE_HOME);

    ui_create_mode_card(s_ui.home_scr, 20, 104, 370, 120,
                        "Free Control",
                        "Monitor the current full-hand pose.",
                        color_accent(), UI_PAGE_FREE);
    ui_create_mode_card(s_ui.home_scr, 410, 104, 370, 120,
                        "Rehab",
                        "Run patient-facing training workflow.",
                        color_green(), UI_PAGE_REHAB);
    ui_create_mode_card(s_ui.home_scr, 20, 242, 370, 120,
                        "EMG",
                        "Show quick on-device force status.",
                        color_warm(), UI_PAGE_EMG);
    ui_create_mode_card(s_ui.home_scr, 410, 242, 370, 120,
                        "Calibration",
                        "Check readiness and sensor setup.",
                        lv_color_hex(0xB48CFF), UI_PAGE_CALIB);

    s_ui.status_chip = lv_label_create(s_ui.home_scr);
    lv_label_set_text(s_ui.status_chip, "--C   --%");
    ui_set_label_font(s_ui.status_chip, &lv_font_montserrat_22, color_text());
    lv_obj_align(s_ui.status_chip, LV_ALIGN_TOP_MID, 0, 40);
}

static void ui_build_free_page(void)
{
    lv_obj_t *left;
    lv_obj_t *bar;
    uint8_t i;

    s_ui.free_scr = lv_obj_create(NULL);
    ui_style_screen(s_ui.free_scr);
    lv_obj_add_event_cb(s_ui.free_scr, ui_page_gesture_back_cb, LV_EVENT_GESTURE, NULL);
    ui_create_top_bar(s_ui.free_scr,
                      "Free Control  v1.1",
                      "",
                      "LIVE",
                      UI_PAGE_HOME);

    left = ui_create_panel(s_ui.free_scr, 20, 104, 760, 356);

    lv_obj_t *title_left = lv_label_create(left);
    lv_label_set_text(title_left, "Finger Open Ratio");
    ui_set_label_font(title_left, &lv_font_montserrat_22, color_text());
    lv_obj_set_pos(title_left, 0, 0);

    for(i = 0; i < 5; i++)
    {
        lv_obj_t *name_lbl = lv_label_create(left);
        lv_label_set_text(name_lbl, k_finger_names[i]);
        ui_set_label_font(name_lbl, &lv_font_montserrat_14, color_muted());
        lv_obj_set_pos(name_lbl, 0, 52 + (lv_coord_t)i * 52);

        bar = lv_bar_create(left);
        lv_obj_set_size(bar, 560, 12);
        lv_obj_set_pos(bar, 90, 56 + (lv_coord_t)i * 52);
        lv_bar_set_range(bar, 0, 100);
        lv_obj_set_style_radius(bar, 6, LV_PART_MAIN);
        lv_obj_set_style_bg_color(bar, lv_color_hex(0x0F1722), LV_PART_MAIN);
        lv_obj_set_style_bg_opa(bar, LV_OPA_90, LV_PART_MAIN);
        lv_obj_set_style_bg_color(bar, (i == 4) ? color_warm() : color_accent(), LV_PART_INDICATOR);
        lv_obj_set_style_bg_grad_color(bar, (i == 4) ? color_warm() : color_accent(), LV_PART_INDICATOR);
        lv_obj_set_style_radius(bar, 6, LV_PART_INDICATOR);
        s_ui.free_bars[i] = bar;

        s_ui.free_values[i] = lv_label_create(left);
        lv_label_set_text(s_ui.free_values[i], "0%");
        ui_set_label_font(s_ui.free_values[i], &lv_font_montserrat_16, color_text());
        lv_obj_set_pos(s_ui.free_values[i], 668, 48 + (lv_coord_t)i * 52);
    }

    s_ui.free_status_label = lv_label_create(left);
    lv_label_set_text(s_ui.free_status_label, "Status: waiting");
    ui_set_label_font(s_ui.free_status_label, &lv_font_montserrat_16, color_text());
    lv_obj_set_pos(s_ui.free_status_label, 0, 312);

    s_ui.free_calib_label = lv_label_create(left);
    lv_label_set_text(s_ui.free_calib_label, "Calibrated: 0 / 0");
    ui_set_label_font(s_ui.free_calib_label, &lv_font_montserrat_14, color_muted());
    lv_obj_set_pos(s_ui.free_calib_label, 0, 334);

    lv_obj_t *ver_label = lv_label_create(s_ui.free_scr);
    lv_label_set_text(ver_label, "v1.1");
    lv_obj_set_style_text_color(ver_label, lv_color_hex(0xFF0000), 0);
    lv_obj_set_style_text_font(ver_label, &lv_font_montserrat_32, 0);
    lv_obj_set_pos(ver_label, 340, 420);
}


static const char * gesture_names[5] =
{
    "Index Straight",
    "Index+Little Straight",
    "All Open",
    "All Close",
    "Peace (V)"
};

/* [0=伸直, 100=弯曲]  拇指  食指  中指  无名指  小指 */
static const uint8_t rehab_target_table[5][5] =
{
    {100U,   0U, 100U, 100U, 100U},  /* index straight  */
    {100U,   0U, 100U, 100U,   0U},  /* index+little    */
    {  0U,   0U,   0U,   0U,   0U},  /* all open        */
    {100U, 100U, 100U, 100U, 100U},  /* all close       */
    {100U,   0U,   0U, 100U, 100U}   /* peace (V)       */
};

static uint16_t ui_abs_diff_u16(uint16_t a, uint16_t b)
{
    return (a >= b) ? (a - b) : (b - a);
}

static uint16_t rehab_calc_score(void)
{
    uint32_t error_sum = 0U;
    uint16_t avg_error;

    if((rehab_last_gesture < 0) || (rehab_last_gesture >= 5))
    {
        return 0U;
    }
    for(uint8_t i = 0U; i < 5U; i++)
    {
        uint16_t now = ui_finger_percent(i);
        uint16_t target = rehab_target_table[(uint8_t)rehab_last_gesture][i];
        error_sum += ui_abs_diff_u16(now, target);
    }

    avg_error = (uint16_t)(error_sum / 5U);
    if(avg_error >= 100U)
    {
        return 0U;
    }

    return (uint16_t)(100U - avg_error);
}

static void rehab_update_score_label(const char *prefix, uint16_t score)
{
    char buf[32];

    if(s_ui.rehab_score_label == NULL || prefix == NULL)
    {
        return;
    }

    snprintf(buf, sizeof(buf), "%s%u", prefix, (unsigned int)score);
    ui_label_set_text_if_changed(s_ui.rehab_score_label, buf);
}

static void rehab_clear_score_label(void)
{
    if(s_ui.rehab_score_label == NULL)
    {
        return;
    }

    ui_label_set_text_if_changed(s_ui.rehab_score_label, "--");
}

static void rehab_commit_round_score(void)
{
    rehab_current_score = rehab_round_best_score;
    rehab_score_sum += rehab_current_score;
    rehab_score_count++;
    rehab_score_avg = (uint16_t)(rehab_score_sum / rehab_score_count);
}



static void rehab_show_next_gesture()
{
    uint8_t index;
    do
    {
        index = lv_rand(0,4);
    } while (index == rehab_last_gesture);
        rehab_last_gesture = index;
        lv_label_set_text(s_ui.rehab_target_text, gesture_names[index]);
}

static void rehab_popup_close_cb(lv_event_t *e)
{
    lv_obj_t *popup = (lv_obj_t *)lv_event_get_user_data(e);
    if(popup != NULL)
    {
        lv_obj_del(popup);
    }
}

static void rehab_timer_cb(lv_timer_t * timer)
{
    int32_t minutes;
    int32_t seconds;

    LV_UNUSED(timer);

    if(rehab_running_State == 0)
    {
        return;
    }

    uint32_t elapsed = HAL_GetTick() - rehab_start_tick;

    if(elapsed >= rehab_duration_ms)
    {
        rehab_left_ms = 0;
    }
    else
    {
        rehab_left_ms = rehab_duration_ms - elapsed;
    }
    {
        uint16_t instant_score = rehab_calc_score();
        if(instant_score > rehab_round_best_score)
        {
            rehab_round_best_score = instant_score;
        }
    }
    int32_t total_sec = (rehab_left_ms + 999) / 1000;
    minutes = total_sec / 60;
    seconds = total_sec % 60;


    if(total_sec != rehab_last_show_sec)
    {
        char time_buf[16];
        snprintf(time_buf, sizeof(time_buf), "%02d:%02d", (int)minutes, (int)seconds);
        lv_label_set_text(s_ui.rehab_time_label, time_buf);
        rehab_last_show_sec = total_sec;
    }

    if(rehab_left_ms == 0)
    {
        rehab_commit_round_score();
        rehab_update_score_label("", rehab_score_avg);

        if(rehab_roun < rehab_total_rounds)
        {
            rehab_roun++;
            g_auto_cycles = rehab_roun;
            rehab_start_tick = lv_tick_get();
            rehab_left_ms = rehab_duration_ms;
            rehab_round_best_score = 0U;
            rehab_show_next_gesture();
        }
        else
        {
            rehab_running_State = 0;
            g_auto_status = 0;
            g_auto_cycles = rehab_total_rounds;
            rehab_round_best_score = 0U;

            if(rehab_best_valid == 0U || rehab_score_avg > rehab_best_score)
            {
                rehab_best_score = rehab_score_avg;
                rehab_best_valid = 1U;
            }

            lv_obj_clear_state(s_ui.rehab_start_btn, LV_STATE_DISABLED);
            lv_label_set_text(s_ui.rehab_status_label, "Finished");
            if(rehab_timer != NULL)
            {
                lv_timer_del(rehab_timer);
                rehab_timer = NULL;
                lv_obj_clear_state(s_ui.rehab_start_btn, LV_STATE_DISABLED);
            }

            /* popup score */
            {
                lv_obj_t *popup = lv_obj_create(lv_scr_act());
                lv_obj_set_size(popup, 320, 200);
                lv_obj_set_style_radius(popup, 12, 0);
                lv_obj_set_style_bg_color(popup, lv_color_hex(0x1A1A2E), 0);
                lv_obj_set_style_border_color(popup, lv_color_hex(0x4FD1FF), 0);
                lv_obj_set_style_border_width(popup, 2, 0);
                lv_obj_set_style_shadow_width(popup, 40, 0);
                lv_obj_set_style_shadow_color(popup, lv_color_hex(0x000000), 0);
                lv_obj_center(popup);

                lv_obj_t *title = lv_label_create(popup);
                lv_label_set_text(title, "Rehab Complete");
                lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
                lv_obj_set_style_text_color(title, color_accent(), 0);
                lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 20);

                lv_obj_t *score_lbl = lv_label_create(popup);
                lv_label_set_text_fmt(score_lbl, "Final Score: %u", (unsigned)rehab_score_avg);
                lv_obj_set_style_text_font(score_lbl, &lv_font_montserrat_32, 0);
                lv_obj_set_style_text_color(score_lbl, color_green(), 0);
                lv_obj_align(score_lbl, LV_ALIGN_CENTER, 0, -10);

                lv_obj_t *close_btn = lv_btn_create(popup);
                lv_obj_set_size(close_btn, 140, 48);
                lv_obj_align(close_btn, LV_ALIGN_BOTTOM_MID, 0, -20);
                lv_obj_set_style_bg_color(close_btn, lv_color_hex(0x1D6b54), 0);
                lv_obj_set_style_bg_color(close_btn, lv_color_hex(0xD6A63A), LV_STATE_PRESSED);

                lv_obj_t *btn_lbl = lv_label_create(close_btn);
                lv_label_set_text(btn_lbl, "CLOSE");
                lv_obj_center(btn_lbl);

                lv_obj_add_event_cb(close_btn, rehab_popup_close_cb, LV_EVENT_CLICKED, popup);
            }
        }
    }
}
static void start_rand_btn(lv_event_t * e)
{
    LV_UNUSED(e);
    if(rehab_running_State != 0)
    {
        return;
    }
    g_auto_status = 1;
    g_auto_cycles = 1;
    rehab_roun = 1;
    rehab_last_show_sec = -1;
    rehab_score_sum = 0U;
    rehab_score_count = 0U;
    rehab_score_avg = 0U;
    rehab_current_score = 0U;
    rehab_round_best_score = 0U;

    lv_obj_add_state(s_ui.rehab_start_btn, LV_STATE_DISABLED);

    rehab_show_next_gesture();

    rehab_running_State = 1;
    rehab_left_ms = 15000;
    rehab_clear_score_label();

    lv_label_set_text(s_ui.rehab_status_label, "Running");


    rehab_start_tick = HAL_GetTick();
    rehab_duration_ms = 15000;

    if(rehab_timer != NULL)
    {
        lv_timer_del(rehab_timer);
        rehab_timer = NULL;
    }

    rehab_timer = lv_timer_create(rehab_timer_cb,100,NULL);

}


static void ui_build_rehab_page(void)
{
    lv_obj_t *left;
    lv_obj_t *right;
    rehab_running_State = 0;

    s_ui.rehab_scr = lv_obj_create(NULL);
    ui_style_screen(s_ui.rehab_scr);
   lv_obj_add_event_cb(s_ui.rehab_scr, ui_page_gesture_back_cb, LV_EVENT_GESTURE, NULL);
    ui_create_top_bar(s_ui.rehab_scr,
                      "Rehab",
                      "",
                      "MODE",
                      UI_PAGE_HOME);

    left = ui_create_panel(s_ui.rehab_scr, 20, 104, 240, 356);
    right = ui_create_panel(s_ui.rehab_scr, 280, 104, 500, 356);

    lv_obj_t *lbl = lv_label_create(left);


    lv_label_set_text(lbl, "Status");
    ui_set_label_font(lbl, &lv_font_montserrat_22, color_text());
    lv_obj_set_pos(lbl, 0, 0);

    s_ui.rehab_status_label = lv_label_create(left);
    lv_label_set_text(s_ui.rehab_status_label, "Standby");
    ui_set_label_font(s_ui.rehab_status_label, &lv_font_montserrat_32, color_green());
    lv_obj_set_pos(s_ui.rehab_status_label, 0, 46);

    s_ui.rehab_cycles_label = lv_label_create(left);
    lv_label_set_text(s_ui.rehab_cycles_label, "Cycles: 0");
    ui_set_label_font(s_ui.rehab_cycles_label, &lv_font_montserrat_20, color_text());
    lv_obj_set_pos(s_ui.rehab_cycles_label, 0, 112);

    lbl = lv_label_create(left);
    lv_obj_set_width(lbl, 220);
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
    lv_label_set_text(lbl, "Complete 5 rounds.\nFinal score after finish.");
    ui_set_label_font(lbl, &lv_font_montserrat_14, color_muted());
    lv_obj_set_pos(lbl, 0, 150);

    lbl = lv_label_create(left);
    lv_obj_set_width(lbl, 220);
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
    lv_label_set_text(lbl, "Patient training page.\nTarget cue and scoring stay here.");
    ui_set_label_font(lbl, &lv_font_montserrat_14, color_muted());
    lv_obj_set_pos(lbl, 0, 270);

    s_ui.rehab_start_btn = lv_btn_create(left);
    lv_obj_align(s_ui.rehab_start_btn,LV_ALIGN_BOTTOM_MID,0,-60);
    lv_obj_set_size(s_ui.rehab_start_btn,180,70);
    lv_obj_set_style_bg_color(s_ui.rehab_start_btn,lv_color_hex(0x1D6b54),0);
    lv_obj_set_style_bg_color(s_ui.rehab_start_btn,lv_color_hex(0xD6A63A),LV_STATE_PRESSED);
    lv_obj_add_event_cb(s_ui.rehab_start_btn,start_rand_btn,LV_EVENT_CLICKED,0);

    s_ui.rehab_start_btn_text = lv_label_create(s_ui.rehab_start_btn);
    lv_obj_center(s_ui.rehab_start_btn_text);
    lv_label_set_text(s_ui.rehab_start_btn_text,"Start");
    ui_set_label_font(s_ui.rehab_start_btn_text,&lv_font_montserrat_16,lv_color_hex(0xFFFFFF));


    lbl = lv_label_create(right);
    lv_label_set_text(lbl, "Target Gesture");
    ui_set_label_font(lbl, &lv_font_montserrat_22, color_text());
    lv_obj_set_pos(lbl, 0, 0);
    lv_obj_t *target_box = lv_obj_create(right);
    lv_obj_set_size(target_box, 468, 148);
    lv_obj_set_pos(target_box, 0, 44);
    lv_obj_set_style_radius(target_box, 14, 0);
    lv_obj_set_style_bg_color(target_box, lv_color_hex(0x0F1724), 0);
    lv_obj_set_style_border_color(target_box, color_line(), 0);
    lv_obj_set_style_shadow_width(target_box, 0, 0);
    lv_obj_clear_flag(target_box, LV_OBJ_FLAG_SCROLLABLE);


    s_ui.rehab_target_text = lv_label_create(target_box);
    lv_obj_align(s_ui.rehab_target_text,LV_ALIGN_BOTTOM_MID,0,-10);
    ui_set_label_font(s_ui.rehab_target_text,&lv_font_montserrat_16,lv_color_hex(0xFFFFFF));
    lv_label_set_text(s_ui.rehab_target_text,"");



    // lbl = lv_label_create(target_box);
    // lv_label_set_text(lbl, "Target area");
    // ui_set_label_font(lbl, &lv_font_montserrat_20, color_text());
    // lv_obj_align(lbl, LV_ALIGN_CENTER, 0, -14);
    // lbl = lv_label_create(target_box);
    // lv_label_set_text(lbl, "Gesture image / guide");
    // ui_set_label_font(lbl, &lv_font_montserrat_14, color_muted());
    // lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 18);




    lv_obj_t *mini1 = ui_create_panel(right, 0, 210, 226, 118);
    lv_obj_t *mini2 = ui_create_panel(right, 242, 210, 226, 118);


    lbl = lv_label_create(mini1);
    lv_label_set_text(lbl, "Timer");
    ui_set_label_font(lbl, &lv_font_montserrat_22, color_accent());
    lv_obj_set_pos(lbl, 0, 0);

    s_ui.rehab_time_label = lv_label_create(mini1);
    lv_label_set_text(s_ui.rehab_time_label,"00:15");
    ui_set_label_font(s_ui.rehab_time_label,&lv_font_montserrat_22,lv_color_hex(0xFFFFFF));
    lv_obj_center(s_ui.rehab_time_label);


    lbl = lv_label_create(mini2);
    lv_label_set_text(lbl, "Score");
    ui_set_label_font(lbl, &lv_font_montserrat_22, color_accent());
    lv_obj_set_pos(lbl, 0, 0);
    s_ui.rehab_score_label = lv_label_create(mini2);
    lv_label_set_text(s_ui.rehab_score_label, "--");
    ui_set_label_font(s_ui.rehab_score_label, &lv_font_montserrat_32, color_green());
    lv_obj_align(s_ui.rehab_score_label, LV_ALIGN_CENTER, 0, -8);

}

static void ui_build_emg_page(void)
{
   lv_obj_t *left;
    lv_obj_t *right;
    lv_obj_t *card1;
    lv_obj_t *card2;
    lv_obj_t *card3;
    lv_obj_t *desc;
    lv_obj_t *lbl;

    s_ui.emg_scr = lv_obj_create(NULL);
    ui_style_screen(s_ui.emg_scr);
    lv_obj_add_event_cb(s_ui.emg_scr, ui_page_gesture_back_cb, LV_EVENT_GESTURE, NULL);
    ui_create_top_bar(s_ui.emg_scr,
                      "EMG",
                      "",
                      "MODE",
                      UI_PAGE_HOME);

    left = ui_create_panel(s_ui.emg_scr, 20, 104, 240, 356);
    right = ui_create_panel(s_ui.emg_scr, 280, 104, 500, 356);

    lbl = lv_label_create(left);
    lv_label_set_text(lbl, "EMG Status");
    ui_set_label_font(lbl, &lv_font_montserrat_22, color_text());
    lv_obj_set_pos(lbl, 0, 0);

    s_ui.emg_state_label = lv_label_create(left);
    lv_label_set_text(s_ui.emg_state_label, "Idle");
    ui_set_label_font(s_ui.emg_state_label, &lv_font_montserrat_32, color_muted());
    lv_obj_set_pos(s_ui.emg_state_label, 0, 44);

    s_ui.emg_hint_label = lv_label_create(left);
    lv_obj_set_width(s_ui.emg_hint_label, 220);
    lv_label_set_long_mode(s_ui.emg_hint_label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(s_ui.emg_hint_label,
                      "Relax first, then grip gradually.\nThe page shows real-time EMG force.");
    ui_set_label_font(s_ui.emg_hint_label, &lv_font_montserrat_14, color_muted());
    lv_obj_set_pos(s_ui.emg_hint_label, 0, 94);

    s_ui.emg_avg_label = lv_label_create(left);
    lv_label_set_text(s_ui.emg_avg_label, "Avg: 0");
    ui_set_label_font(s_ui.emg_avg_label, &lv_font_montserrat_14, color_text());
    lv_obj_set_pos(s_ui.emg_avg_label, 0, 178);

    s_ui.emg_activity_label = lv_label_create(left);
    lv_label_set_text(s_ui.emg_activity_label, "Activity: 0");
    ui_set_label_font(s_ui.emg_activity_label, &lv_font_montserrat_14, color_text());
    lv_obj_set_pos(s_ui.emg_activity_label, 0, 206);

    s_ui.emg_baseline_label = lv_label_create(left);
    lv_label_set_text(s_ui.emg_baseline_label, "Baseline: 0");
    ui_set_label_font(s_ui.emg_baseline_label, &lv_font_montserrat_14, color_text());
    lv_obj_set_pos(s_ui.emg_baseline_label, 0, 234);

    s_ui.emg_delta_label = lv_label_create(left);
    lv_label_set_text(s_ui.emg_delta_label, "Delta: 0");
    ui_set_label_font(s_ui.emg_delta_label, &lv_font_montserrat_14, color_text());
    lv_obj_set_pos(s_ui.emg_delta_label, 0, 262);

    card1 = ui_create_panel(right, 0, 0, 156, 128);
    card2 = ui_create_panel(right, 172, 0, 156, 128);
    card3 = ui_create_panel(right, 344, 0, 156, 128);

    lbl = lv_label_create(card1);
    lv_label_set_text(lbl, "Level");
    ui_set_label_font(lbl, &lv_font_montserrat_20, color_text());
    lv_obj_set_pos(lbl, 0, 0);

    s_ui.emg_level_label = lv_label_create(card1);
    lv_label_set_text(s_ui.emg_level_label, "0%");
    ui_set_label_font(s_ui.emg_level_label, &lv_font_montserrat_32, color_accent());
    lv_obj_set_pos(s_ui.emg_level_label, 0, 42);

    lbl = lv_label_create(card2);
    lv_label_set_text(lbl, "Peak");
    ui_set_label_font(lbl, &lv_font_montserrat_20, color_text());
    lv_obj_set_pos(lbl, 0, 0);

    s_ui.emg_peak_label = lv_label_create(card2);
    lv_label_set_text(s_ui.emg_peak_label, "0%");
    ui_set_label_font(s_ui.emg_peak_label, &lv_font_montserrat_32, color_green());
    lv_obj_set_pos(s_ui.emg_peak_label, 0, 42);

    lbl = lv_label_create(card3);
    lv_label_set_text(lbl, "Timer");
    ui_set_label_font(lbl, &lv_font_montserrat_20, color_text());
    lv_obj_set_pos(lbl, 0, 0);

    s_ui.emg_timer_label = lv_label_create(card3);
    lv_label_set_text(s_ui.emg_timer_label, "00:00");
    ui_set_label_font(s_ui.emg_timer_label, &lv_font_montserrat_32, color_warm());
    lv_obj_set_pos(s_ui.emg_timer_label, 0, 46);

    desc = ui_create_panel(right, 0, 148, 500, 180);

    lbl = lv_label_create(desc);
    lv_obj_set_width(lbl, 450);
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
    lv_label_set_text(lbl,
                      "EMG mode reads the muscle sensor, calculates activity from a sampling window, "
                      "subtracts the resting baseline, and maps the result to a 0-100 force level.");
    ui_set_label_font(lbl, &lv_font_montserrat_16, color_muted());
    lv_obj_set_pos(lbl, 0, 12);
}

static void ui_build_calib_page(void)
{
    lv_obj_t *left;
    lv_obj_t *card;
    lv_obj_t *lbl;

    s_ui.calib_scr = lv_obj_create(NULL);
    ui_style_screen(s_ui.calib_scr);
    lv_obj_add_event_cb(s_ui.calib_scr, ui_page_gesture_back_cb, LV_EVENT_GESTURE, NULL);
    ui_create_top_bar(s_ui.calib_scr,
                      "System Status",
                      "",
                      "STATUS",
                      UI_PAGE_HOME);

    left = ui_create_panel(s_ui.calib_scr, 20, 104, 260, 356);

    lbl = lv_label_create(left);
    lv_label_set_text(lbl, "System Summary");
    ui_set_label_font(lbl, &lv_font_montserrat_22, color_text());
    lv_obj_set_pos(lbl, 0, 0);

    s_ui.calib_ready_label = lv_label_create(left);
    lv_label_set_text(s_ui.calib_ready_label, "READY");
    ui_set_label_font(s_ui.calib_ready_label, &lv_font_montserrat_32, color_green());
    lv_obj_set_pos(s_ui.calib_ready_label, 0, 44);

    s_ui.calib_channels_label = lv_label_create(left);
    lv_obj_set_width(s_ui.calib_channels_label, 220);
    lv_label_set_long_mode(s_ui.calib_channels_label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(s_ui.calib_channels_label,
                      "Flex profiles active\n"
                      "EMG records ready\n"
                      "Rehab scoring ready");
    ui_set_label_font(s_ui.calib_channels_label, &lv_font_montserrat_16, color_muted());
    lv_obj_set_pos(s_ui.calib_channels_label, 0, 108);

    lbl = lv_label_create(left);
    lv_label_set_text(lbl, "Session Records");
    ui_set_label_font(lbl, &lv_font_montserrat_20, color_text());
    lv_obj_set_pos(lbl, 0, 214);

    s_ui.calib_rehab_best_label = lv_label_create(left);
    lv_obj_set_width(s_ui.calib_rehab_best_label, 220);
    lv_label_set_long_mode(s_ui.calib_rehab_best_label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(s_ui.calib_rehab_best_label,
                      "Best EMG peak: --%\n"
                      "Best rehab score: --\n"
                      "Setup: open-close-open");
    ui_set_label_font(s_ui.calib_rehab_best_label, &lv_font_montserrat_16, color_muted());
    lv_obj_set_pos(s_ui.calib_rehab_best_label, 0, 254);

    card = ui_create_panel(s_ui.calib_scr, 300, 104, 480, 356);
    lbl = lv_label_create(card);
    lv_label_set_text(lbl, "Finger IO List");
    ui_set_label_font(lbl, &lv_font_montserrat_20, color_text());
    lv_obj_set_pos(lbl, 0, 0);

    s_ui.calib_refs_label = lv_roller_create(card);
    lv_obj_set_size(s_ui.calib_refs_label, 430, 270);
    lv_obj_set_pos(s_ui.calib_refs_label, 0, 46);
    lv_roller_set_visible_row_count(s_ui.calib_refs_label, 7);
    lv_roller_set_options(s_ui.calib_refs_label,
                          "INDEX ROOT    PA0\n"
                          "INDEX TIP     PA1\n"
                          "MIDDLE ROOT   PA2\n"
                          "MIDDLE TIP    PA3\n"
                          "RING ROOT     PA4\n"
                          "RING TIP      PA5\n"
                          "LITTLE ROOT   PA6\n"
                          "LITTLE TIP    PA7\n"
                          "THUMB ROOT    PC4\n"
                          "THUMB TIP     PC5",
                          LV_ROLLER_MODE_INFINITE);
    lv_obj_set_style_bg_opa(s_ui.calib_refs_label, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(s_ui.calib_refs_label, 0, 0);
    lv_obj_set_style_text_font(s_ui.calib_refs_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(s_ui.calib_refs_label, color_green(), 0);
    lv_obj_set_style_bg_color(s_ui.calib_refs_label, lv_color_hex(0x123A34), LV_PART_SELECTED);
    lv_obj_set_style_text_color(s_ui.calib_refs_label, lv_color_hex(0xFFFFFF), LV_PART_SELECTED);
}

static void ui_refresh_runtime(lv_timer_t *timer)
{
    char buf[96];
    uint8_t enabled;
    uint8_t calibrated;
    uint8_t i;

    LV_UNUSED(timer);

    enabled = ui_enabled_root_count();
    calibrated = ui_calibrated_root_count();

    if((s_ui.current_page == UI_PAGE_HOME) && (s_ui.status_chip != NULL))
    {
        if(g_dht_ok != 0U)
        {
            snprintf(buf,
                     sizeof(buf),
                     "%uC   %u%%",
                     (unsigned int)g_dht_temp,
                     (unsigned int)g_dht_hum);
            ui_label_set_text_if_changed(s_ui.status_chip, buf);
            lv_obj_align(s_ui.status_chip, LV_ALIGN_TOP_MID, 0, 40);
        }
        else
        {
            ui_label_set_text_if_changed(s_ui.status_chip, "--C   --%");
            lv_obj_align(s_ui.status_chip, LV_ALIGN_TOP_MID, 0, 40);
        }
    }

    if(s_ui.current_page == UI_PAGE_FREE)
    {
        for(i = 0; i < 5; i++)
        {
            uint16_t percent = ui_finger_percent(i);
            if(s_ui.free_bars[i] != NULL)
            {
                if(lv_bar_get_value(s_ui.free_bars[i]) != (int32_t)percent)
                {
                    lv_bar_set_value(s_ui.free_bars[i], percent, LV_ANIM_OFF);
                }
            }
            if(s_ui.free_values[i] != NULL)
            {
                ui_label_set_text_fmt_u16(s_ui.free_values[i], "%u%%", (unsigned int)percent);
            }
        }

        if(s_ui.free_status_label != NULL)
        {
            if(enabled == 0U)
            {
                ui_label_set_text_if_changed(s_ui.free_status_label, "Status: no enabled root fingers");
            }
            else if(calibrated == enabled)
            {
                ui_label_set_text_if_changed(s_ui.free_status_label, "Status: mapping task ready");
            }
            else
            {
                ui_label_set_text_if_changed(s_ui.free_status_label, "Status: waiting for calibration");
            }
        }

        if(s_ui.free_calib_label != NULL)
        {
            ui_label_set_text_fmt_u16_u16(s_ui.free_calib_label,
                                          "Calibrated: %u / %u",
                                          (unsigned int)calibrated,
                                          (unsigned int)enabled);
        }
    }

    if(s_ui.current_page == UI_PAGE_REHAB && s_ui.rehab_status_label != NULL)
    {
        const char *status_str = "Standby";
        lv_color_t color = color_text();
        if(g_auto_status == 1U)
        {
            status_str = "Running";
            color = color_green();
        }
        else if(g_auto_status == 2U)
        {
            status_str = "Emergency stop";
            color = lv_color_hex(0xFF7D7D);
        }
        ui_label_set_text_if_changed(s_ui.rehab_status_label, status_str);
        lv_obj_set_style_text_color(s_ui.rehab_status_label, color, 0);

        if(s_ui.rehab_cycles_label != NULL)
        {
            ui_label_set_text_fmt_u16(s_ui.rehab_cycles_label, "Cycles: %u", (unsigned int)g_auto_cycles);
        }

    }


     if(s_ui.current_page == UI_PAGE_EMG)
    {
        emg_metrics_t emg;
        const char *state_text;
        lv_color_t state_color = color_muted();

        EMG_GetMetrics(&emg);
        state_text = ui_emg_state_text(emg.state);

        if(emg.state == EMG_STATE_REST)
        {
            state_color = color_warm();
        }
        else if(emg.state == EMG_STATE_ACTIVE)
        {
            state_color = color_accent();
        }
        else if(emg.state == EMG_STATE_STRONG)
        {
            state_color = color_green();
        }

        if(s_ui.emg_state_label != NULL)
        {
            ui_label_set_text_if_changed(s_ui.emg_state_label, state_text);
            lv_obj_set_style_text_color(s_ui.emg_state_label, state_color, 0);
        }

        if(s_ui.emg_level_label != NULL)
        {
            ui_label_set_text_fmt_u16(s_ui.emg_level_label, "%u%%", (unsigned int)emg.level);
        }

        if(s_ui.emg_peak_label != NULL)
        {
            ui_label_set_text_fmt_u16(s_ui.emg_peak_label, "%u%%", (unsigned int)emg.peak);
        }

        if(s_ui.emg_timer_label != NULL)
        {
            snprintf(buf,
                     sizeof(buf),
                     "%02lu:%02lu",
                     (unsigned long)(emg.elapsed_sec / 60U),
                     (unsigned long)(emg.elapsed_sec % 60U));
            ui_label_set_text_if_changed(s_ui.emg_timer_label, buf);
        }

        if(s_ui.emg_avg_label != NULL)
        {
            ui_label_set_text_fmt_u16(s_ui.emg_avg_label, "Avg: %u", (unsigned int)emg.avg);
        }

        if(s_ui.emg_activity_label != NULL)
        {
            ui_label_set_text_fmt_u16(s_ui.emg_activity_label, "Activity: %u", (unsigned int)emg.activity);
        }

        if(s_ui.emg_baseline_label != NULL)
        {
            ui_label_set_text_fmt_u16(s_ui.emg_baseline_label, "Baseline: %u", (unsigned int)emg.baseline);
        }

        if(s_ui.emg_delta_label != NULL)
        {
            ui_label_set_text_fmt_u16(s_ui.emg_delta_label, "Delta: %u", (unsigned int)emg.delta);
        }

        if(s_ui.emg_hint_label != NULL)
        {
            if(emg.running != 0U)
            {
                ui_label_set_text_if_changed(s_ui.emg_hint_label,
                                             "Relax first, then grip gradually.\nEMG task is running.");
            }
            else
            {
                ui_label_set_text_if_changed(s_ui.emg_hint_label,
                                             "Open this page to start EMG mode.\nWaiting for data.");
            }
        }
    }


    if(s_ui.current_page == UI_PAGE_CALIB && s_ui.calib_ready_label != NULL)
    {
        emg_metrics_t emg;
        const char *status_str = "READY";
        lv_color_t status_color = color_green();
        uint16_t best_rehab = (rehab_best_valid != 0U) ? rehab_best_score : 0U;
        const char *flex_line;
        const char *emg_line;
        const char *rehab_line;

        EMG_GetMetrics(&emg);

        if((enabled == 0U) || (calibrated != enabled))
        {
            status_str = "WAITING";
            status_color = color_warm();
        }

        ui_label_set_text_if_changed(s_ui.calib_ready_label, status_str);
        lv_obj_set_style_text_color(s_ui.calib_ready_label, status_color, 0);

        if(s_ui.calib_channels_label != NULL)
        {
            flex_line = ((enabled != 0U) && (calibrated == enabled))
                        ? "Flex profiles active"
                        : "Flex profiles waiting";
            emg_line = (emg.running != 0U || emg.elapsed_sec > 0U)
                       ? "EMG records ready"
                       : "EMG records idle";
            rehab_line = ((rehab_best_valid != 0U) || (g_auto_status != 0U))
                         ? "Rehab scoring ready"
                         : "Rehab scoring idle";
            snprintf(buf, sizeof(buf), "%s\n%s\n%s", flex_line, emg_line, rehab_line);
            ui_label_set_text_if_changed(s_ui.calib_channels_label, buf);
        }

        if(s_ui.calib_rehab_best_label != NULL)
        {
            snprintf(buf,
                     sizeof(buf),
                     "Best EMG peak: %u%%\nBest rehab score: %u\nSetup: open-close-open",
                     (unsigned int)emg.peak,
                     (unsigned int)best_rehab);
            ui_label_set_text_if_changed(s_ui.calib_rehab_best_label, buf);
        }
    }
}

void Main_control_scr_init(void)
{
    memset(&s_ui, 0, sizeof(s_ui));
    s_ui.current_page = UI_PAGE_HOME;
    HandApp_SetMode(HAND_APP_MODE_IDLE);

    ui_build_home();
    ui_build_free_page();
    ui_build_rehab_page();
    ui_build_emg_page();
    ui_build_calib_page();

    lv_scr_load(s_ui.home_scr);

    if(s_ui.refresh_timer != NULL)
    {
        lv_timer_del(s_ui.refresh_timer);
    }
    s_ui.refresh_timer = lv_timer_create(ui_refresh_runtime, 220, NULL);
}
