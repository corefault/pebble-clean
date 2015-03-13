/**
 * animation module for handling two animations and an update between both
 * @author Daniel Kagemann @corefault
 * @created 13.03.2015
 *
 */
#include <pebble.h>
#include "animation.h"

/**
 * calculate the rectangle
 */
GRect ca_calc_rect (GRect src, enum animationdirection dir) {
   GRect    target = src;
   
   switch (dir) {
      case moveleft:
      target.origin.x -= 144;
      break;
      case moveright:
      target.origin.x += 144;
      break;
      case moveup:
      target.origin.y -= 168;
      break;
      case movedown:
      target.origin.y += 168;
      break;
   }
   return target;
}

/**
 * stopped last animation
 */
void ca_stopped_all(Animation* animation, bool finished, void* data) {
	// destroy animation
	property_animation_destroy(animation);
}

/**
 * stop animation callback handler for first
 */
void ca_stopped(Animation *animation, bool finished, void *data) {
	animationchain*	self = (animationchain*)data;

	// destroy animation
	property_animation_destroy(animation);
	
	// update text layer
	text_layer_set_text(self->layer, (char*)ca->textbuffer);
	
	// create new
	self->prop  = property_animation_create_layer_frame((Layer*)layer, &self->last_rc_src, &self->last_rc_dest);
	
	animation_set_handlers((Animation*) self->prop, (AnimationHandlers) {
       .started = NULL,
       .stopped = (AnimationStoppedHandler) ca_stopped_all,
     }, prop);
	
	// now start it
    animation_schedule((Animation*) self->prop);
}

/**
 * create propertyanimation
 */
void ca_initialize(animationchain* self, TextLayer* layer, enum animationdirection first_dir, enum animationdirection last_dir) {

	// set the rects
   GRect  frame  = layer_get_frame(layer);
   self->first_rc_src = frame;
   self->first_rc_dest= ca_calc_rect(frame, first_dir);

	// TODO 
   self->last_rc_src = frame;
   self->last_rc_dest= ca_calc_rect(frame, last_dir);
   
   self->prop = property_animation_create_layer_frame((Layer*)layer, &self->first_rc_src, &self->first_rc_dest);
   
   // set handler
   animation_set_handlers((Animation*) self->prop, (AnimationHandlers) {
       .started = NULL,
       .stopped = (AnimationStoppedHandler) ca_stopped,
     }, prop);
   
   // now start it
   animation_schedule((Animation*) self->prop);
}

