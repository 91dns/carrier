#ifndef BALL_H
#define BALL_H

#include "../libs/carrier_gfx.h"
#include <cglm/cglm.h>

typedef struct player player;
typedef struct enemy enemy;

typedef struct ball {
    cg_pipeline pipe;
    cg_bindings bind;
    cg_uniform model_loc;
    cg_uniform view_loc;
    cg_uniform proj_loc;
    cg_uniform color_loc;
    cg_shader shd;
    vec3 position;
    vec3 velocity;
} ball;

void init_ball(ball* ball);
void update_ball(ball* ball, player* player, enemy* enemy, float delta_time, float aspect);
void render_ball(ball* ball, float aspect);

void ball_reset(ball* ball, float aspect);

#endif // BALL_H
