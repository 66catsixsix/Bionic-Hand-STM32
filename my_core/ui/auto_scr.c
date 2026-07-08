#include "algo.h"
#include "auto_scr.h"
#include "lvgl.h"
#include <stddef.h>
#include "main_screen.h"
#include <stdio.h>

extern lv_obj_t *auto_scr;

// --- RTOS 数据解耦全局变量 ---
volatile uint8_t g_auto_status = 0;   // 0: Standby, 1: Running, 2: Error
volatile uint16_t g_auto_cycles = 0;  // 循环次数
volatile uint16_t g_auto_force = 0;   // 力度反馈 %

// --- UI 组件指针与定时器 ---
static lv_obj_t * label_status = NULL;
static lv_obj_t * label_cycles = NULL;
static lv_obj_t * label_force = NULL;
static lv_timer_t * auto_ui_timer = NULL;

// --- 定时器回调：安全刷新UI ---
static void auto_ui_update_cb(lv_timer_t * timer)
{
    if(label_status == NULL || label_cycles == NULL || label_force == NULL) return;

    const char * status_str = "Standby";
    if(g_auto_status == 1) status_str = "Running";
    else if(g_auto_status == 2) status_str = "E-STOP!";

    lv_label_set_text_fmt(label_status, "Status: %s", status_str);
    lv_label_set_text_fmt(label_cycles, "Cycles: %d", g_auto_cycles);
    lv_label_set_text_fmt(label_force, "Current Force: %d %%", g_auto_force);
}

// auto_scr右滑返回主界面回调
 static void auto_scr_gesture_event_cb(lv_event_t* e)
 {
    lv_event_code_t code = lv_event_get_code(e);
     if(code == LV_EVENT_GESTURE) {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        if(dir == LV_DIR_RIGHT) {
            // 右滑返回主界面前，安全销毁定时器，防止后台空跑爆内存
            if(auto_ui_timer != NULL) {
                lv_timer_del(auto_ui_timer);
                auto_ui_timer = NULL;
            }
            lv_scr_load(Main_scr.Main_control_scr);
         }
     }
 }

//自动模式按钮回调，创建auto_scr并注册右滑返回
void serival_auto(lv_event_t* e)
{
    lv_obj_t *auto_scr = lv_obj_create(NULL); // 局部变量
    lv_scr_load(auto_scr);
    
    // --- 自动模式 UI 构建开始 --- //
    // 1. 标题
    lv_obj_t * title_label = lv_label_create(auto_scr);
    lv_label_set_text(title_label, "Auto Mode (Rehab)");
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);

    // 2. 状态显示面板区域
    lv_obj_t * status_panel = lv_obj_create(auto_scr);
    lv_obj_set_size(status_panel, 300, 120);
    lv_obj_align(status_panel, LV_ALIGN_CENTER, 0, -50);

    label_status = lv_label_create(status_panel);
    lv_label_set_text(label_status, "Status: Standby");
    lv_obj_align(label_status, LV_ALIGN_TOP_LEFT, 10, 10);

    label_cycles = lv_label_create(status_panel);
    lv_obj_align(label_cycles, LV_ALIGN_TOP_LEFT, 10, 40);

    label_force = lv_label_create(status_panel);
    lv_label_set_text(label_force, "Current Force: 0 %");
    lv_obj_align(label_force, LV_ALIGN_TOP_LEFT, 10, 70);

    // 3. 启动/停止按钮
    lv_obj_t * start_btn = lv_btn_create(auto_scr);
    lv_obj_set_size(start_btn, 120, 50);
    lv_obj_align(start_btn, LV_ALIGN_CENTER, -80, 80);
    lv_obj_t * start_label = lv_label_create(start_btn);
    lv_label_set_text(start_label, "START");
    lv_obj_center(start_label);

    // 4. 紧急停止按钮 (红色)
    lv_obj_t * stop_btn = lv_btn_create(auto_scr);
    lv_obj_set_size(stop_btn, 120, 50);
    lv_obj_align(stop_btn, LV_ALIGN_CENTER, 80, 80);
    lv_obj_set_style_bg_color(stop_btn, lv_color_hex(0xFF0000), 0);
    lv_obj_t * stop_label = lv_label_create(stop_btn);
    lv_label_set_text(stop_label, "E-STOP");
    lv_obj_center(stop_label);

    // 5. 提示信息 (右滑返回)
    lv_obj_t * back_hint = lv_label_create(auto_scr);
    lv_label_set_text(back_hint, ">> Swipe Right to Return >>");
    lv_obj_set_style_text_color(back_hint, lv_color_hex(0x888888), 0);
    lv_obj_align(back_hint, LV_ALIGN_BOTTOM_MID, 0, -20);
    // --- 自动模式 UI 构建结束 --- //

    // 启动解耦定时器，每 200 毫秒(即 5Hz)执行一次 UI 刷新
    if(auto_ui_timer == NULL) {
        auto_ui_timer = lv_timer_create(auto_ui_update_cb, 200, NULL);
    }

    lv_obj_add_event_cb(auto_scr, auto_scr_gesture_event_cb, LV_EVENT_GESTURE, NULL);
}

void auto_scr_init()
{
    char cycle_str[32];

    // 创建自动模式界面
    lv_obj_t *auto_scr = lv_obj_create(NULL);
    lv_scr_load(auto_scr);
    
    // --- 自动模式 UI 构建开始 --- //

    // 1. 标题
    lv_obj_t * title_label = lv_label_create(auto_scr);
    lv_label_set_text(title_label, "Auto Mode (Rehab)");
    // lv_obj_set_style_text_font(title_label, &lv_font_montserrat_24, 0); // 取消强制指定字体，防止未启用引发编译报错
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);

    // 2. 状态显示面板区域
    lv_obj_t * status_panel = lv_obj_create(auto_scr);
    lv_obj_set_size(status_panel, 300, 120);
    lv_obj_align(status_panel, LV_ALIGN_CENTER, 0, -50);

    lv_obj_t * status_label1 = lv_label_create(status_panel);
    lv_label_set_text(status_label1, "Status: Standby");
    lv_obj_align(status_label1, LV_ALIGN_TOP_LEFT, 10, 10);

    lv_obj_t * status_label2 = lv_label_create(status_panel);
    sprintf(cycle_str, "Cycles: %d", g_auto_cycles);
    lv_label_set_text(status_label2, cycle_str);
    lv_obj_align(status_label2, LV_ALIGN_TOP_LEFT, 10, 40);

    lv_obj_t * status_label3 = lv_label_create(status_panel);
    lv_label_set_text(status_label3, "Current Force: 0 %");
    lv_obj_align(status_label3, LV_ALIGN_TOP_LEFT, 10, 70);

    // 3. 启动/停止按钮
    lv_obj_t * start_btn = lv_btn_create(auto_scr);
    lv_obj_set_size(start_btn, 120, 50);
    lv_obj_align(start_btn, LV_ALIGN_CENTER, -80, 80);
    lv_obj_t * start_label = lv_label_create(start_btn);
    lv_label_set_text(start_label, "START");
    lv_obj_center(start_label);

    // 4. 紧急停止按钮 (红色)
    lv_obj_t * stop_btn = lv_btn_create(auto_scr);
    lv_obj_set_size(stop_btn, 120, 50);
    lv_obj_align(stop_btn, LV_ALIGN_CENTER, 80, 80);
    lv_obj_set_style_bg_color(stop_btn, lv_color_hex(0xFF0000), 0);
    lv_obj_t * stop_label = lv_label_create(stop_btn);
    lv_label_set_text(stop_label, "E-STOP");
    lv_obj_center(stop_label);

    // 5. 提示信息 (右滑返回)
    lv_obj_t * back_hint = lv_label_create(auto_scr);
    lv_label_set_text(back_hint, ">> Swipe Right to Return >>");
    lv_obj_set_style_text_color(back_hint, lv_color_hex(0x888888), 0);
    lv_obj_align(back_hint, LV_ALIGN_BOTTOM_MID, 0, -20);
    
    // --- 自动模式 UI 构建结束 --- //

      lv_obj_add_event_cb(auto_scr, auto_scr_gesture_event_cb, LV_EVENT_GESTURE, NULL);

}
