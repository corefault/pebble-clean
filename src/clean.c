/**
 * clean watchface for the pebble using SDK 3.0
 * supported devices: all
 * 
 * @author Daniel Kagemann @corefault
 * 
 * changelog:
 * v1.0: 
 *   - initial version
 * v1.1: 
 *   - fixed date display bug
 *   - added animations
 */

#include <pebble.h>
#include "animation.h"

enum TextLine {LineDate = 0, LineHour, LineMinute, LineTotal};

static Window    *window;
static TextLayer *text_layer[LineTotal];

animationchain    animDate, animHour, animMinute;

// ==========================================================
// each minute this is called to update the time
static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
   // get current time
   char* day[] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};
   time_t now = time(NULL);
   struct tm * currentTime = localtime(&now);
   char buffer[20];
   static int old_date = 0, old_hour = 100, old_minute = 100;
   
   APP_LOG(APP_LOG_LEVEL_DEBUG, "handleminutetick(%p, %d)", tick_time, units_changed);
   
   if (old_date == 0) {
      
      animDate.prop = animHour.prop = animMinute.prop = NULL;

      snprintf (buffer, sizeof(buffer), "%s %02d", day[currentTime->tm_wday], currentTime->tm_mday);
      ca_initialize(&animDate, text_layer[LineDate], buffer, appearFromTop,800, 0);

      snprintf (buffer, sizeof(buffer), "%d", currentTime->tm_hour);
      ca_initialize(&animHour, text_layer[LineHour], buffer, appearFromRight, 800, 0);
   
      snprintf (buffer, sizeof(buffer), "%d", currentTime->tm_min);
      ca_initialize(&animMinute, text_layer[LineMinute], buffer, appearFromBottom, 800, 0);

      old_date = currentTime->tm_mday;
      old_hour = currentTime->tm_hour;
      old_minute = currentTime->tm_min;

   } else {
      // check old values
      if (currentTime->tm_mday != old_date) {
         old_date = currentTime->tm_mday;
         snprintf (buffer, sizeof(buffer), "%s %02d", day[currentTime->tm_wday], currentTime->tm_mday);
         ca_initialize(&animDate, text_layer[LineDate], buffer, appearFromTop,800, 0);
      }
      
      if (currentTime->tm_hour != old_hour) {
         old_hour = currentTime->tm_hour;
         snprintf (buffer, sizeof(buffer), "%d", currentTime->tm_hour);
         ca_initialize(&animHour, text_layer[LineHour], buffer, appearFromRight, 800, 0);      
      }
      
      if (old_minute != currentTime->tm_min) {
         old_minute = currentTime->tm_min;
         snprintf (buffer, sizeof(buffer), "%d", currentTime->tm_min);
         ca_initialize(&animMinute, text_layer[LineMinute], buffer, appearFromBottom, 800, 0);
      }
   }
}

// ==========================================================
// load window content
static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  int i;

  char* font[] = {FONT_KEY_GOTHIC_14, FONT_KEY_BITHAM_42_BOLD, FONT_KEY_BITHAM_42_LIGHT};
  int size[] = {14, 44, 44};
  int pos[] = {17, 32, 77};

  for (i = 0; i < LineTotal; i++) {
    text_layer[i] = text_layer_create(GRect(0, pos[i], 144, size[i]));
    text_layer_set_text_color(text_layer[i], GColorWhite);
    text_layer_set_background_color(text_layer[i], GColorClear);
    text_layer_set_font(text_layer[i], fonts_get_system_font(font[i]));
    text_layer_set_text(text_layer[i], "---");
    text_layer_set_text_alignment(text_layer[i], GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(text_layer[i]));
  }
  psleep(1000);
  
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

// ==========================================================
// unload window content
static void window_unload(Window *window) {
  for (int i = 0; i < LineTotal; i++) {
    text_layer_destroy(text_layer[i]), text_layer[i] = 0;
  }
}

// ==========================================================
// initialize root window and handler
static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
  window_set_background_color(window, GColorBlack);
  window_set_fullscreen(window, true);
}

// ==========================================================
// destroy all
static void deinit(void) {
  window_destroy(window);
}

// ==========================================================
// main entry point
int main(void) {
  init();
  app_event_loop();
  deinit();
}
