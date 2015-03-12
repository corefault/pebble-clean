#define WIDTH 144
#define HEIGHT 168

enum AnimationDirection {
   moveleft = 0,
   moveright,
   movetop,
   movebottom
};

void ca_create(Layer* layer,
               enum AnimationDirection direction,
               int duration,
               int delay);