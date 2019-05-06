//
//  vector.h
//  RayThrower
//
//  Created by Max Scribner on 4/13/19.
//  Copyright © 2019 Max Scribner. All rights reserved.
//

#ifndef vector_h
#define vector_h

#include <stdio.h>

typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

vec3 add(vec3, vec3);
vec3 scale(vec3, float);
vec3 subtract(vec3, vec3);
float length (vec3);
vec3 normalize(vec3);
vec3 cross(vec3, vec3);
float dot(vec3, vec3);

vec3 proj(vec3 a, vec3 b);


#endif /* vector_h */
