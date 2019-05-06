//
//  main.c
//  RayThrower
//
//  Created by Max Scribner on 4/12/19.
//  Copyright © 2019 Max Scribner. All rights reserved.
//

#define false 0
#define true 1

#include <SDL2/SDL.h>
#include <stdio.h>
#include "physics.h"

#define RECURSION_COUNT 8

#define FLAT_WARP 0
#define GRAVITY_WARP 1

#define COLOR_RENDER 0
#define ITERATIONS_RENDER 1
#define DISTANCE_RENDER 2

const int screenHeight = 512;
const int screenWidth = 512;

const float traceRatio = 1.0/8;

const int traceHeight = screenHeight * traceRatio;
const int traceWidth = screenWidth * traceRatio;

const float frustum_dist = 1;

int draw_type = 0;
int warp_type = FLAT_WARP;

extern Object object;

extern vec3 (*acceleration_field)(vec3);

int main(int argc, const char * argv[]) {
    if (SDL_Init (SDL_INIT_VIDEO) == 0) {
        SDL_Window *window;
        SDL_Renderer *renderer;

        int exit = false;
        int change = true;
        SDL_Event event;

        if (SDL_CreateWindowAndRenderer(screenWidth, screenHeight, 0, &window, &renderer) == 0) {
            while (!exit) {
                SDL_Rect drawRect;
                drawRect.w = drawRect.h = 1/traceRatio;

                if (change) {
                    SDL_RenderClear(renderer);

                    for(int x=0; x<traceWidth; x++) {
                        for(int y=0; y<traceHeight; y++) {
                            drawRect.x = x/traceRatio;
                            drawRect.y = y/traceRatio;

                            Light l;
                            l.position.x = l.position.y = 0;
                            l.position.z = -frustum_dist;
                            vec3 start_v = {(x-traceWidth/2.0)/traceWidth, (y-traceHeight/2.0)/traceHeight, frustum_dist};
                            start_v = normalize(start_v);
                            l.velocity = start_v;
                            
                            Object *intersect;
                            int count = RECURSION_COUNT;
                            
                            acceleration_field = warp_type == FLAT_WARP ? &flat_field: &gravity_field;
                            
                            while ((intersect = update_light(&l)) == NULL && (count--) > 0 ) {}
                            
                            Color c;
                            if (intersect) {
                                switch(draw_type) {
                                    case COLOR_RENDER:
                                        c = intersect->color(intersect, l.position);
                                        break;
                                        
                                    case ITERATIONS_RENDER:
                                        c.r = c.b = c.g = 256*((float) count)/RECURSION_COUNT;
                                        break;
                                        
                                    default: break;
                                }
                            } else {
                                c.r = 16;
                                c.g = 16;
                                c.b = 16;
                            }
                            SDL_SetRenderDrawColor(renderer,
                                                   c.r,
                                                   c.g,
                                                   c.b,
                                                   SDL_ALPHA_OPAQUE);
                            SDL_RenderFillRect(renderer, &drawRect);
                        }
                    }
                    
                    SDL_RenderPresent(renderer);
                    change = true;
                }
                
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        exit = true;
                    } else if(event.type == SDL_KEYDOWN){
                        switch(event.key.keysym.scancode) {
                            case SDL_SCANCODE_PERIOD:
                                object.pos.z += 0.1;
                                change = true;
                                break;

                            case SDL_SCANCODE_COMMA:
                                object.pos.z -= 0.1;
                                change = true;
                                break;

                            case SDL_SCANCODE_P:
                                printf("Dist = %f\n", object.pos.z);
                                break;

                            case SDL_SCANCODE_F:
                                warp_type = FLAT_WARP;
                                break;
                                
                            case SDL_SCANCODE_G:
                                warp_type = GRAVITY_WARP;
                                break;

                            case SDL_SCANCODE_C:
                                draw_type = COLOR_RENDER;
                                break;

                            case SDL_SCANCODE_I:
                                draw_type = ITERATIONS_RENDER;
                                break;
                                
                            default:
                                break;
                        }
                    }
                }
            }
            
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
    }
    printf("Hello, World!\n");
    return 0;
}
