#include <pebble.h>

enum TextLine {LineDate = 0, LineHour, LineMinute, LineTotal};

static Window    *window;
static TextLayer *text_layer[LineTotal];

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
    text_layer_set_text(text_layer[i], "");
    text_layer_set_text_alignment(text_layer[i], GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(text_layer[i]));
  }
}

// ==========================================================
// unload window content
static void window_unload(Window *window) {
  for (int i = 0; i < LineTotal; i++) {
    text_layer_destroy(text_layer[i]), text_layer[i] = 0;
  }
}

// ==========================================================
// each minute this is called to update the time
static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
   // get current time
   char* day[] = {"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa"};
   time_t now = time(NULL);
   struct tm * currentTime = localtime(&now);
   static char text[10][3];

   snprintf (text[0], 10, "%s %02d", day[currentTime->tm_wday], currentTime->tm_mday);
   text_layer_set_text(text_layer[LineDate], text[0]);

   snprintf (text[1], 10, "%d", currentTime->tm_hour);
   text_layer_set_text(text_layer[LineHour], text[1]);

   snprintf (text[2], 10, "%d", currentTime->tm_min);
   text_layer_set_text(text_layer[LineMinute], text[2]);
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

  handle_minute_tick(NULL, 0);
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
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
