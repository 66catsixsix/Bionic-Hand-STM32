#include "Finger.h"
#include "lvgl.h"
#include "main_screen.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIM_HOR_RES 800
#define SIM_VER_RES 480
#define DRAW_BUF_LINES 80

static SDL_Window *s_window;
static SDL_Renderer *s_renderer;
static SDL_Texture *s_texture;
static uint32_t s_framebuffer[SIM_HOR_RES * SIM_VER_RES];
static lv_color_t s_draw_buf_1[SIM_HOR_RES * DRAW_BUF_LINES];
static lv_disp_draw_buf_t s_draw_buf;
static lv_disp_drv_t s_disp_drv;
static lv_indev_drv_t s_indev_drv;
static bool s_quit;
static bool s_pointer_pressed;
static int16_t s_pointer_x = SIM_HOR_RES / 2;
static int16_t s_pointer_y = SIM_VER_RES / 2;
static uint32_t s_last_tick;

static void usage(const char *prog)
{
    fprintf(stderr,
            "Usage: %s [--page home|free|rehab|emg|calib] [--screenshot file.bmp]\n",
            prog);
}

static void display_present(void)
{
    if(s_texture == NULL || s_renderer == NULL) {
        return;
    }

    SDL_UpdateTexture(s_texture, NULL, s_framebuffer, SIM_HOR_RES * (int)sizeof(uint32_t));
    SDL_RenderClear(s_renderer);
    SDL_RenderCopy(s_renderer, s_texture, NULL, NULL);
    SDL_RenderPresent(s_renderer);
}

static void display_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    int32_t x1 = area->x1 < 0 ? 0 : area->x1;
    int32_t y1 = area->y1 < 0 ? 0 : area->y1;
    int32_t x2 = area->x2 >= SIM_HOR_RES ? SIM_HOR_RES - 1 : area->x2;
    int32_t y2 = area->y2 >= SIM_VER_RES ? SIM_VER_RES - 1 : area->y2;

    for(int32_t y = area->y1; y <= area->y2; y++) {
        for(int32_t x = area->x1; x <= area->x2; x++) {
            lv_color_t c = *color_p++;
            if(x >= x1 && x <= x2 && y >= y1 && y <= y2) {
                s_framebuffer[y * SIM_HOR_RES + x] =
                    0xFF000000U |
                    ((uint32_t)LV_COLOR_GET_R(c) << 16) |
                    ((uint32_t)LV_COLOR_GET_G(c) << 8) |
                    ((uint32_t)LV_COLOR_GET_B(c));
            }
        }
    }

    display_present();
    lv_disp_flush_ready(disp_drv);
}

static void pointer_read_cb(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    LV_UNUSED(indev_drv);

    data->point.x = s_pointer_x;
    data->point.y = s_pointer_y;
    data->state = s_pointer_pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

static bool sim_sdl_init(bool hidden)
{
    uint32_t flags = hidden ? SDL_WINDOW_HIDDEN : SDL_WINDOW_SHOWN;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }

    s_window = SDL_CreateWindow("LVGL Bionic Hand UI",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                SIM_HOR_RES,
                                SIM_VER_RES,
                                flags);
    if(s_window == NULL) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }

    s_renderer = SDL_CreateRenderer(s_window, -1, SDL_RENDERER_SOFTWARE);
    if(s_renderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return false;
    }

    s_texture = SDL_CreateTexture(s_renderer,
                                  SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STREAMING,
                                  SIM_HOR_RES,
                                  SIM_VER_RES);
    if(s_texture == NULL) {
        fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

static void sim_lvgl_init(void)
{
    lv_init();

    lv_disp_draw_buf_init(&s_draw_buf, s_draw_buf_1, NULL, SIM_HOR_RES * DRAW_BUF_LINES);

    lv_disp_drv_init(&s_disp_drv);
    s_disp_drv.hor_res = SIM_HOR_RES;
    s_disp_drv.ver_res = SIM_VER_RES;
    s_disp_drv.draw_buf = &s_draw_buf;
    s_disp_drv.flush_cb = display_flush_cb;
    lv_disp_drv_register(&s_disp_drv);

    lv_indev_drv_init(&s_indev_drv);
    s_indev_drv.type = LV_INDEV_TYPE_POINTER;
    s_indev_drv.read_cb = pointer_read_cb;
    lv_indev_drv_register(&s_indev_drv);

    s_last_tick = SDL_GetTicks();
}

static void sim_poll_events(void)
{
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            s_quit = true;
            break;
        case SDL_MOUSEMOTION:
            s_pointer_x = (int16_t)event.motion.x;
            s_pointer_y = (int16_t)event.motion.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
            s_pointer_x = (int16_t)event.button.x;
            s_pointer_y = (int16_t)event.button.y;
            s_pointer_pressed = true;
            break;
        case SDL_MOUSEBUTTONUP:
            s_pointer_x = (int16_t)event.button.x;
            s_pointer_y = (int16_t)event.button.y;
            s_pointer_pressed = false;
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q) {
                s_quit = true;
            }
            break;
        default:
            break;
        }
    }
}

static void sim_run_for(uint32_t ms)
{
    uint32_t start = SDL_GetTicks();

    while(!s_quit && SDL_GetTicks() - start < ms) {
        uint32_t now = SDL_GetTicks();
        uint32_t diff = now - s_last_tick;
        if(diff > 0) {
            lv_tick_inc(diff);
            s_last_tick = now;
        }

        sim_poll_events();
        lv_timer_handler();
        SDL_Delay(5);
    }
}

static void sim_click(int16_t x, int16_t y)
{
    s_pointer_x = x;
    s_pointer_y = y;
    s_pointer_pressed = false;
    sim_run_for(60);
    s_pointer_pressed = true;
    sim_run_for(100);
    s_pointer_pressed = false;
    sim_run_for(320);
}

static void sim_select_page(const char *page)
{
    if(strcmp(page, "home") == 0) {
        return;
    }
    if(strcmp(page, "free") == 0) {
        sim_click(205, 164);
    }
    else if(strcmp(page, "rehab") == 0) {
        sim_click(595, 164);
    }
    else if(strcmp(page, "emg") == 0) {
        sim_click(205, 302);
    }
    else if(strcmp(page, "calib") == 0) {
        sim_click(595, 302);
    }
}

static bool save_screenshot(const char *path)
{
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(s_framebuffer,
                                                    SIM_HOR_RES,
                                                    SIM_VER_RES,
                                                    32,
                                                    SIM_HOR_RES * (int)sizeof(uint32_t),
                                                    0x00FF0000,
                                                    0x0000FF00,
                                                    0x000000FF,
                                                    0xFF000000);
    if(surface == NULL) {
        fprintf(stderr, "SDL_CreateRGBSurfaceFrom failed: %s\n", SDL_GetError());
        return false;
    }

    int rc = SDL_SaveBMP(surface, path);
    SDL_FreeSurface(surface);
    if(rc != 0) {
        fprintf(stderr, "SDL_SaveBMP failed: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    const char *page = "home";
    const char *screenshot = NULL;

    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--page") == 0 && i + 1 < argc) {
            page = argv[++i];
        }
        else if(strcmp(argv[i], "--screenshot") == 0 && i + 1 < argc) {
            screenshot = argv[++i];
        }
        else if(strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        }
        else {
            usage(argv[0]);
            return 2;
        }
    }

    if(strcmp(page, "home") != 0 &&
       strcmp(page, "free") != 0 &&
       strcmp(page, "rehab") != 0 &&
       strcmp(page, "emg") != 0 &&
       strcmp(page, "calib") != 0) {
        usage(argv[0]);
        return 2;
    }

    if(!sim_sdl_init(screenshot != NULL)) {
        return 1;
    }

    sim_lvgl_init();
    Hand_Init();
    Main_control_scr_init();
    sim_run_for(300);
    sim_select_page(page);
    sim_run_for(500);

    if(screenshot != NULL) {
        bool ok = save_screenshot(screenshot);
        SDL_Quit();
        return ok ? 0 : 1;
    }

    while(!s_quit) {
        sim_run_for(16);
    }

    SDL_Quit();
    return 0;
}
