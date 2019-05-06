//
//  vector.c
//  RayThrower
//
//  Created by Max Scribner on 4/13/19.
//  Copyright © 2019 Max Scribner. All rights reserved.
//

#include "vector.h"
#include <math.h>

vec3 add(vec3 a, vec3 b) {
    vec3 out;
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    out.z = a.z + b.z;
    
    return out;
}

vec3 scale(vec3 a, float scalar) {
    vec3 out;
    out.x = a.x * scalar;
    out.y = a.y * scalar;
    out.z = a.z * scalar;
    
    return out;

}

vec3 subtract(vec3 a, vec3 b) {
    vec3 out;
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    out.z = a.z - b.z;
    
    return out;
}

float length (vec3 a) {
    return (float) sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

vec3 normalize(vec3 a) {
    float len = length(a);
    vec3 out;
    
    out.x = a.x / len;
    out.y = a.y / len;
    out.z = a.z / len;
    
    return out;
}

vec3 cross(vec3 a, vec3 b) {
    vec3 out;
    out.x = a.y*b.z - a.z*b.y;
    out.y = a.z*b.x - a.x*b.z;
    out.z = a.x*b.y - a.y*b.x;
    
    return out;
}

float dot(vec3 a, vec3 b) {
    return a.x*b.x+a.y*b.y+a.z*b.z;
}

vec3 proj(vec3 a, vec3 b) {
    return scale(b, dot(a, b)/length(b));
}
