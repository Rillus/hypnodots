#include <pebble.h>
#include <time.h>
#include <stdlib.h>
#include <math.h> 

static Window *window;
static TextLayer *text_layer;
static Layer *circle_layer1;
// static Layer *circle_layer2;
// static int *radius;

static void circle_layer_update_callback(Layer *layer, GContext* ctx) {
  // --- Get layer extent --- //
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);

  // --- Graphics draw rectangle code --- //
  // graphics_context_set_fill_color(ctx, GColorWhite);
  // graphics_fill_rect(ctx, GRect(0, 0, 2, 2), 0, GCornerNone);

  // Draw a white filled circle a radius of half the layer height
  graphics_context_set_fill_color(ctx, GColorWhite);

  int i = 0;
  for (i = 0; i < 1; i++){
    const int16_t half_h = rand() % 20;
    graphics_fill_circle(ctx, center, half_h);
  }
}
static void circle1_update_proc(Layer *layer, GContext* ctx) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  static char time_text[] = "00:00:00";
  static char time_text2[] = "00:00:00";
  char *time_format;
  char *time_format2;
  time_format = "%I:%M:%S";
  time_format2 = "%S";

  strftime(time_text, sizeof(time_text), time_format, t);
  strftime(time_text2, sizeof(time_text2), time_format2, t);

  text_layer_set_text(text_layer, time_text);

  int timey = atoi(time_text2);

  graphics_context_set_fill_color(ctx, GColorWhite);

  int i = 0;
  for (i = 0; i < timey; i++){
    //const int16_t half_h = 3 * (i/2);
    const int16_t half_h = 3;
    int x = timey * 2 + (10 * i);
    int y = (timey * 2) * i;

    if (y > 168){
      // To wrap dots.
      int diff = floor (y/168);
      y = y - (168 * diff);
    }
    if (x > 144){
      int diff = floor (x/144);
      x = x - (144 * diff);
    }

    graphics_fill_circle(ctx, GPoint(x, y), half_h);
  }
}
static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  layer_mark_dirty(window_get_root_layer(window));

  // Need to be static because they're used by the system later.
  // static char time_text[] = "00:00:00";
  // static char time_pos[] = "00";

  // char *time_format;

  // if (clock_is_24h_style()) {
  //   time_format = "%R";
  // } else {
  // time_format = "%I:%M:%S";
  // }

  // strftime(time_text, sizeof(time_text), time_format, tick_time);
  // strftime(time_pos, sizeof(time_pos), time_format, tick_time);

  // text_layer_set_text(text_layer, time_text);

  // GRect bounds = layer_get_bounds(circle_layer2);
  // GPoint center = grect_center_point(&bounds);

  // // int timey = atoi(time_pos);

  // Layer *window_layer = window_get_root_layer(window);

  // GRect circle_frame2 = GRect(center.x +1, center.y+35, 144, 168);
  // circle_layer2 = layer_create(circle_frame2);
  // layer_add_child(window_layer, circle_layer2);

  // layer_set_update_proc(circle_layer1, circle_layer_update_callback);
  // layer_set_update_proc(circle_layer2, circle_layer_update_callback);
}

void window_load(){
  Layer *window_layer = window_get_root_layer(window);

  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  int16_t x = 0; //rand() % 144;
  int16_t y = 0; //rand() % 168;
  GRect circle_frame1 = GRect(x, y, 144, 168);
  circle_layer1 = layer_create(circle_frame1);

  // x = 30; //rand() % 144;
  // y = 30; //rand() % 168;
  // GRect circle_frame2 = GRect(x, y, 144, 168);
  // circle_layer1 = layer_create(circle_frame2);
  
  layer_add_child(window_layer, circle_layer1);
  layer_set_update_proc(circle_layer1, circle1_update_proc);
  // layer_add_child(window_layer, circle_layer2);

  text_layer = text_layer_create(GRect(35, 68, 144-8, 168-68));
  text_layer_set_text_color(text_layer, GColorWhite);
  text_layer_set_background_color(text_layer, GColorClear);
  GFont norm18 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  text_layer_set_font(text_layer, norm18);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));

  static char time_text[] = "00:00:00";
  char *time_format;
  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
  time_format = "%I:%M:%S";
  }

  strftime(time_text, sizeof(time_text), time_format, t);
  text_layer_set_text(text_layer, time_text);

  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
}
void window_unload(){
  layer_destroy(circle_layer1);
  text_layer_destroy(text_layer);
}

void handle_init(void) {
  window = window_create();
  
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(window, true /* Animated */);
  window_set_background_color(window, GColorBlack);
}

void handle_deinit(void) {
  tick_timer_service_unsubscribe();
}

int main(void) {
  handle_init();

  app_event_loop();
  
  handle_deinit();
}
