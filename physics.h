//
//  physics.h
//  RayThrower
//
//  Created by Max Scribner on 4/13/19.
//  Copyright © 2019 Max Scribner. All rights reserved.
//

#ifndef physics_h
#define physics_h

#include "vector.h"

#include <stdio.h>

typedef struct object Object;

typedef struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

typedef struct light {
    vec3 position;
    vec3 velocity;
} Light;

typedef struct object {
    vec3 pos;
    vec3 direction;
    float radius;
    
    vec3 (*normal)(Object*, vec3);
    float (*dist)(Object*, vec3);
    Color (*color)(Object*, vec3);
} Object;

extern vec3 (*acceleration_field)(vec3);

Object *update_light(Light*);

extern Object object;

vec3 flat_field(vec3 pos);
vec3 gravity_field(vec3 pos);



#endif /* physics_h */
