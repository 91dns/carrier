#ifndef ENEMY_H
#define ENEMY_H

#include "../libs/carrier_gfx.h"
#include <cglm/cglm.h>

typedef struct ball ball;

typedef struct enemy {
    cg_pipeline pipe;
    cg_bindings bind;
    cg_uniform model_loc;
    cg_uniform view_loc;
    cg_uniform proj_loc;
    cg_uniform color_loc;
    cg_shader shd;
    vec3 position;
} enemy;

void init_enemy(enemy* enemy);
void update_enemy(enemy* enemy, ball* ball, float delta_time);
void render_enemy(enemy* enemy, float aspect);

#endif // ENEMY_H
