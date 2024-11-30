#ifndef PLAYER_H
#define PLAYER_H

#include "../libs/carrier_gfx.h"
#include <cglm/cglm.h>

typedef struct player {
    cg_pipeline pipe;
    cg_bindings bind;
    cg_uniform model_loc;
    cg_uniform view_loc;
    cg_uniform proj_loc;
    cg_uniform color_loc;
    cg_shader shd;
    vec3 position;
} player;

void init_player(player* player);
void update_player(player* player, float delta_time, const capp_window* window);
void render_player(player* player, float aspect);

#endif // PLAYER_H
