// Copyright (c) 2021 Inaba
// This software is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#define LGFX_AUTODETECT
#define LGFX_USE_V1
#include <lvgl.h>
#include <M5ModuleDisplay.h>
#include <M5Unified.h>

namespace {

constexpr uint16_t screenWidth  = 800;
constexpr uint16_t screenHeight = 480;
constexpr uint16_t refreshRate  = 30;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

M5ModuleDisplay gfx(screenWidth, screenHeight, refreshRate);

void my_disp_flush(
    lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p) {
  const u_long w = area->x2 - area->x1 + 1;
  const u_long h = area->y2 - area->y1 + 1;

  if (gfx.getStartCount() <= 0) {
    gfx.startWrite();
  }
  gfx.setAddrWindow(area->x1, area->y1, w, h);
  gfx.pushPixels(static_cast<uint16_t *>(&color_p->full), w * h);

  if (lv_disp_flush_is_last(disp)) {
    gfx.endWrite();
  }
  ::lv_disp_flush_ready(disp);
}

void btn_event_cb(lv_event_t * e) {
  lv_event_code_t code = ::lv_event_get_code(e);
  lv_obj_t * btn = ::lv_event_get_target(e);
  uint8_t* d = static_cast<uint8_t*>(lv_event_get_user_data(e));
  if (code == LV_EVENT_CLICKED) {
    *d += 1;
    lv_obj_t * label = ::lv_obj_get_child(btn, 0);
    ::lv_label_set_text_fmt(label, "Button: %d", *d);
  }
}

static lv_obj_t * meter;
static lv_obj_t * meter2;

static void set_value(void * indic, int32_t v)
{
    lv_meter_set_indicator_value(meter, (lv_meter_indicator_t*)indic, v);
}

static void set_value2(void * indic2, int32_t v)
{
    lv_meter_set_indicator_value(meter2, (lv_meter_indicator_t*)indic2, v);
}

void create_objects() {

  meter = lv_meter_create(lv_scr_act());
  meter2 = lv_meter_create(lv_scr_act());
  // lv_obj_center(meter);
  lv_obj_align(meter, LV_ALIGN_LEFT_MID, 25, -25);
  lv_obj_set_size(meter, 300, 300);
  lv_obj_align(meter2, LV_ALIGN_RIGHT_MID, -25, -25);
  lv_obj_set_size(meter2, 300, 300);

  /*Add a scale first*/
  lv_meter_scale_t * scale = lv_meter_add_scale(meter);
  lv_meter_set_scale_ticks(meter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_PINK));
  lv_meter_set_scale_major_ticks(meter, scale, 8, 4, 15, lv_color_black(), 10);

  lv_meter_indicator_t * indic;

  /*Add a blue arc to the start*/
  indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
  lv_meter_set_indicator_start_value(meter, indic, 0);
  lv_meter_set_indicator_end_value(meter, indic, 20);

  /*Make the tick lines blue at the start of the scale*/
  indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_BLUE), false, 0);
  lv_meter_set_indicator_start_value(meter, indic, 0);
  lv_meter_set_indicator_end_value(meter, indic, 20);

  /*Add a red arc to the end*/
  indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
  lv_meter_set_indicator_start_value(meter, indic, 80);
  lv_meter_set_indicator_end_value(meter, indic, 100);

  /*Make the tick lines red at the end of the scale*/
  indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
  lv_meter_set_indicator_start_value(meter, indic, 80);
  lv_meter_set_indicator_end_value(meter, indic, 100);

  /*Add a needle line indicator*/
  indic = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

  /*Add a scale first*/
  lv_meter_scale_t * scale2 = lv_meter_add_scale(meter2);
  lv_meter_set_scale_ticks(meter2, scale2, 41, 2, 10, lv_palette_main(LV_PALETTE_PINK));
  lv_meter_set_scale_major_ticks(meter2, scale2, 8, 4, 15, lv_color_black(), 10);

  lv_meter_indicator_t * indic2;

  /*Add a blue arc to the start*/
  indic2 = lv_meter_add_arc(meter2, scale2, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
  lv_meter_set_indicator_start_value(meter2, indic2, 0);
  lv_meter_set_indicator_end_value(meter2, indic2, 20);

  /*Make the tick lines blue at the start of the scale*/
  indic2 = lv_meter_add_scale_lines(meter2, scale, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_BLUE), false, 0);
  lv_meter_set_indicator_start_value(meter2, indic2, 0);
  lv_meter_set_indicator_end_value(meter2, indic2, 20);

  /*Add a red arc to the end*/
  indic2 = lv_meter_add_arc(meter2, scale2, 3, lv_palette_main(LV_PALETTE_RED), 0);
  lv_meter_set_indicator_start_value(meter2, indic2, 80);
  lv_meter_set_indicator_end_value(meter2, indic2, 100);

  /*Make the tick lines red at the end of the scale*/
  indic2 = lv_meter_add_scale_lines(meter2, scale2, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
  lv_meter_set_indicator_start_value(meter2, indic2, 80);
  lv_meter_set_indicator_end_value(meter2, indic2, 100);

  /*Add a needle line indicator*/
  indic2 = lv_meter_add_needle_line(meter2, scale2, 4, lv_palette_main(LV_PALETTE_GREY), -10);

  lv_obj_t * label1 = lv_label_create(lv_scr_act());
  lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
  lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
  lv_label_set_text(label1, LV_SYMBOL_LOOP);
  lv_obj_set_style_text_font(label1, &lv_font_montserrat_42, 0);
  lv_obj_set_width(label1, 150);  /*Set smaller width to make the lines wrap*/
  lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(label1, LV_ALIGN_BOTTOM_MID, 0, -40);
  /*Create an animation to set the value*/
  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_exec_cb(&a, set_value);
  lv_anim_set_var(&a, indic);
  lv_anim_set_values(&a, 0, 100);
  lv_anim_set_time(&a, 2000);
  lv_anim_set_repeat_delay(&a, 100);
  lv_anim_set_playback_time(&a, 500);
  lv_anim_set_playback_delay(&a, 100);
  lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
  lv_anim_start(&a);

  /*Create an animation to set the value*/
  lv_anim_t a2;
  lv_anim_init(&a2);
  lv_anim_set_exec_cb(&a2, set_value2);
  lv_anim_set_var(&a2, indic2);
  lv_anim_set_values(&a2, 100, 0);
  lv_anim_set_time(&a2, 2000);
  lv_anim_set_repeat_delay(&a2, 100);
  lv_anim_set_playback_time(&a2, 500);
  lv_anim_set_playback_delay(&a2, 100);
  lv_anim_set_repeat_count(&a2, LV_ANIM_REPEAT_INFINITE);
  lv_anim_start(&a2);

}

}  // namespace

void setup() {
  M5.begin();

  gfx.begin();
  gfx.setSwapBytes(true);

  ::lv_init();
  ::lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

  static lv_disp_drv_t disp_drv;
  ::lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  ::lv_disp_drv_register(&disp_drv);

  create_objects();
}

void loop() {
  ::lv_timer_handler();
  ::delay(5);
}