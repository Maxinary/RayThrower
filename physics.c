//
//  physics.c
//  RayThrower
//
//  Created by Max Scribner on 4/13/19.
//  Copyright © 2019 Max Scribner. All rights reserved.
//

#include "physics.h"
#include "vector.h"

#define DEBUG 0
#define TUNNELDEBUG 0

#include "quartic.h"

#include <math.h>
#include <stdlib.h>

extern Object object;
extern vec3 (*acceleration_field)(vec3);


vec3 sphereNormal(Object *o, vec3 point) {
    return normalize(subtract(point, o->pos));
}

float sphereDist(Object *o, vec3 point) {
    return length(subtract(point, o->pos)) - (o->radius);
}

vec3 discNormal(Object *o, vec3 point) {
    return o->direction;
}

Color normalColor(Object *o, vec3 point) {
    Color out;
    vec3 half = {128,128,128};
    vec3 norm = add(half, scale((*(o->normal))(o, point), 128));
    
    out.r = (int)norm.x;
    out.g = (int)norm.y;
    out.b = (int)norm.z;
    
    return out;
}

Color black(Object *o, vec3 point) {
    Color out;
    
    out.r = (int)(0);
    out.g = (int)(0);
    out.b = (int)(0);
    
    return out;
}

Object object = {{0.0,0.0,15.450148}, {1.0, 0.0, 0.0}, 2.0, &sphereNormal, &sphereDist, &normalColor};

vec3 flat_field(vec3 pos) {
    vec3 out =  {0.0, 0.0, 0.0};
    return out;
}

float G = 30;

vec3 gravity_field (vec3 pos) {
    vec3 between = subtract(object.pos, pos);
#if DEBUG
    printf("between, <%f, %f,%f>\n", between.x, between.y, between.z);
#endif
    float distance = length(between);
    vec3 direction = normalize(between);
    vec3 out = scale(direction, G/(distance*distance));
    
    return out;
}

vec3 (*acceleration_field)(vec3) = &gravity_field;

Object* update_light(Light* l) {
#if DEBUG
    printf("\n\nStarting Update Light\n");
#endif
    float r;
    if((r = object.dist(&object, l->position)) > 1.0/16) {
        if (length((*acceleration_field)(l->position)) > 0.00001) {
            vec3 a_vec = (*acceleration_field)(l->position);
            vec3 v_vec = l->velocity;
#if DEBUG
            printf("radius of freedom = %f\n", r);
            printf("velocity = <%f, %f, %f>\n", v_vec.x, v_vec.y, v_vec.z);
            printf("acceleration = <%f, %f, %f>\n", a_vec.x, a_vec.y, a_vec.z);
#endif
            float acceleration = length(a_vec);
            
            vec3 p_vec = cross(cross(a_vec, v_vec), a_vec);
            
            float a = acceleration/2;
            float b = dot(v_vec, normalize(a_vec))/dot(v_vec, normalize(p_vec));

            float quart_ins[] = {2*b/a, (b*b+1)/(a*a), 0, -r*r/(a*a)};
#if DEBUG
            printf("a, b: %f, %f\n", a, b);
            printf("quartic inputs: 1, %f, %f, %f, %f\n", 2.0*b/a, (b*b+1)/(a*a), 0.0, -(r*r)/(a*a));
            
            float *qs = quartic(quart_ins);
            for(int i=0; i<4; i++) {
                printf ("quart val: %f\n", qs[i]);
            }
            free(qs);
#endif
            
            float x_sol = first_quartic_soln(quart_ins);
            float y_sol = f(a, b, x_sol);
#if DEBUG
            printf("solution = <%f, %f>\n", x_sol, y_sol);
#endif
            
#if TUNNELDEBUG
            if (x_sol > r) {
                printf("\n\nERROR\n");
                printf("radius of freedom = %f\n", r);
                printf("velocity = <%f, %f, %f>\n", v_vec.x, v_vec.y, v_vec.z);
                printf("acceleration = <%f, %f, %f>\n", a_vec.x, a_vec.y, a_vec.z);
                printf("a, b: %f, %f\n", a, b);
                printf("quartic inputs: 1, %f, %f, %f, %f\n", 2.0*b/a, (b*b+1)/(a*a), 0.0, -(r*r)/(a*a));
                printf("solution = <%f, %f>\n", x_sol, y_sol);
            }
#endif
            float deltav = sqrt(pow(dot(l->velocity, normalize(a_vec)), 2) + 2 * acceleration * y_sol)-length(proj(l->velocity,a_vec));
            
            vec3 delta_position = add(scale(normalize(p_vec), x_sol),
                                      scale(normalize(a_vec), y_sol));
#if DEBUG
            printf("delta_v = %f\n", deltav);
            printf("delta_position = <%f, %f, %f>\n", delta_position.x, delta_position.y, delta_position.z);
#endif
            
            l->velocity = add(l->velocity, scale(normalize(a_vec), deltav));
            l->position = add(l->position, delta_position);
        } else {
            l->position = add(l->position, scale(normalize(l->velocity), r));
        }
        return NULL;
    } else {
        return &object;
    }
}
