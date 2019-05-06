//
//  quartic.c
//  RayThrower
//
//  Created by Max Scribner on 4/13/19.
//  Copyright © 2019 Max Scribner. All rights reserved.
//

#include "quartic.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

float* error(float *curr, float *init){
    float *err = calloc(4, sizeof(float));
    err[0] = init[0]-curr[0]-curr[2];
    err[1] = init[1]-curr[1]-curr[0]*curr[2]-curr[3];
    err[2] = init[2]-curr[1]*curr[2]-curr[0]*curr[3];
    err[3] = init[3]-curr[1]*curr[3];
    return err;
}

float *quad(float b, float c) {
    float *roots = calloc(2, sizeof(float));
    if(b*b - 4*c >= 0) {
        roots[0] = (-b-sqrt(b*b-4*c))/2.0;
        roots[1] = (-b+sqrt(b*b-4*c))/2.0;
    } else {
        roots[0] = -1000000;
        roots[0] = -1000000;
    }
    return roots;
}

float *quartic(float init_vars[]) {
    float guess[] = {0, init_vars[3]/init_vars[1], 1, -init_vars[3]/init_vars[1]};
    float specs[] = {0,0,0,0};
    float x[] = {0,0,0,0};
    float y[] = {0,0,0,0};
    float e[] = {10000,10000,10000,10000};
    float *sols = calloc(4, sizeof(float));
    sols[0] = -1000;
    sols[1] = -1000;
    sols[2] = -1000;
    sols[3] = -1000;

    int num_iter = 0;
    
    if (abs(init_vars[0] * (2<<6)) < init_vars[1]) {
        sols[0] = sqrt(-init_vars[3]/init_vars[1]);
    } else {
        for(int i=0; i<50 && e[0]*e[0] + e[1]*e[1] + e[2]*e[2] + e[3]*e[3] > 0.1; i++) {
            float *er = error(guess,init_vars);
            e[0] = er[0];
            e[1] = er[1];
            e[2] = er[2];
            e[3] = er[3];
            free(er);
            
            specs[0] = guess[0]-guess[2];
            specs[1] = guess[1]-guess[3]-guess[2]*specs[0];
            specs[2] = -guess[3]*specs[0]/specs[1];
            specs[3] = guess[1]-guess[3]-specs[0]*specs[2];
            x[0] = e[0];
            x[1] = e[1]-guess[2]*x[0];
            x[2] = e[2]-guess[3]*x[0]-guess[2]*x[1];
            x[3] = e[3]-guess[3]*x[1]-specs[2]*x[2];
            y[3] = x[3]/specs[3];
            y[2] = (x[2]-specs[0]*y[3])/specs[1];
            y[1] = x[1]-specs[0]*y[2]-y[3];
            y[0] = x[0]-y[2];
            for(int j=0; j<4; j++) {
                guess[j] += y[j];
                num_iter += 1;
            }
        }
        
        float *first = quad(guess[0],guess[1]);
        float *last = quad(guess[2],guess[3]);
        sols[0] = first[0];
        sols[1] = first[1];
        sols[2] = last[0];
        sols[3] = last[1];
        
        free(first);
        free(last);
    }
    
    return sols;
}

float min_pos(float a, float b) {
    if(a > b) {
        if (b <= 0) {
            return a;
        } else {
            return b;
        }
    } else {
        if (a <= 0) {
            return b;
        } else {
            return a;
        }
    }
}

float first_quartic_soln (float *init_vars) {
    float *sols = quartic(init_vars);
    float min1 = min_pos(sols[0], sols[1]);
    float min2 = min_pos(sols[2], sols[3]);
    float minA = min_pos(min1, min2);
    free(sols);
    return minA;
}

float f (float a, float b, float x) {
    return a*x*x + b*x;
}

/*  int main(void) {
        float ins[] = {-3,2,0.5,-0.4};
        float *sols;
        sols = quartic(ins);
    
        for(int i=0; i<4; i++) {
            printf("%f, ",
                sols[i]);
        }
        printf("\b\b\n");
        free(sols);
        return 0;
    }
 */
