// =====================================
// animation
#include <pebble.h>
#include "animation.h"

// ======================
// private: target rect depending on direction
static GRect ca_get_target_frame (GRect src, enum AnimationDirection dir) {
   GRect    target = src;
   
   switch (dir) {
      case moveleft:
      target.origin.x -= WIDTH;
      break;
      case moveright:
      target.origin.x += WIDTH;
      break;
      case movetop:
      target.origin.y -= HEIGHT;
      break;
      case movebottom:
      target.origin.y += HEIGHT;
      break;
   }
   
   return target;
}

// ======================
// animation stopped
void ca_stopped(Animation *animation, bool finished, void *data) {
   property_animation_destroy((Animation*)data);
}

// =======================
// create and run animation
void ca_create(Layer* layer,
               enum AnimationDirection direction,
               int duration,
               int delay
                         ) {
   GRect  frame = layer_get_frame(layer);
   GRect  target = ca_get_target_frame(frame, direction);
   
   PropertyAnimation* prop = property_animation_create_layer_frame(layer, &frame, &target);
   
   // set handler
   animation_set_handlers((Animation*) prop, (AnimationHandlers) {
       .started = NULL,
       .stopped = (AnimationStoppedHandler) ca_stopped,
     }, prop);
   
     // some settings
   animation_set_duration((Animation*)prop, duration);
   animation_set_delay((Animation*)prop, delay);
   
   // now start it
   animation_schedule((Animation*) prop);
}

PropertyAnimation* coreanim_create(Layer* layer, )