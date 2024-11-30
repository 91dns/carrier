#include "player.h"
#include "ball.h"
#include "constants.h"
#include "../libs/carrier_app.h"

void init_player(player* player) {
    player->shd = cg_load_shader("../shaders/player.vert", "../shaders/player.frag");

    const GLfloat vertices[] = {
        // Position (quad)
        -0.5f, -0.5f, 0.0f, // bottom left
         0.5f, -0.5f, 0.0f, // bottom right
         0.5f,  0.5f, 0.0f, // top right
        -0.5f,  0.5f, 0.0f  // top left
    };

    const GLint indices[] = {
        // first triangle
        0, 1, 2,
        // second triangle
        2, 3, 0
    };

    player->bind = cg_make_buffer(&(cg_buffer_conf) {
        .vertex_buffer = {
            .size = sizeof(vertices),
            .data = vertices
        },
        .index_buffer = {
            .size = sizeof(indices),
            .data = indices
        }
    });

    player->pipe = cg_make_pipeline(&(cg_pipeline_conf) {
        .shader = player->shd,
        .primitive_type = GL_TRIANGLES
    });

    player->model_loc = cg_get_location(player->shd, "u_model");
    player->view_loc = cg_get_location(player->shd, "u_view");
    player->proj_loc = cg_get_location(player->shd, "u_proj");
    player->color_loc = cg_get_location(player->shd, "u_color");

    glm_vec3_zero(player->position);
}

void update_player(player* player, float delta_time, const capp_window* window) {
    int direction = 0;

    if (cr_get_key(window, CAPP_KEY_W) == GLFW_PRESS) {
        direction = 1;
    } else if (cr_get_key(window, CAPP_KEY_S) == GLFW_PRESS) {
        direction = -1;
    }

    if (direction == 1) {
        player->position[1] += PLAYER_SPEED * delta_time;
    } else if (direction == -1) {
        player->position[1] -= PLAYER_SPEED * delta_time;
    }

    if (player->position[1] > TOP_BOUNDARY - PLAYER_HALF_HEIGHT) {
        player->position[1] = TOP_BOUNDARY - PLAYER_HALF_HEIGHT;
    } else if (player->position[1] < BOTTOM_BOUNDARY + PLAYER_HALF_HEIGHT) {
        player->position[1] = BOTTOM_BOUNDARY + PLAYER_HALF_HEIGHT;
    }
}

void render_player(player* player, float aspect) {
    const float color_time = cr_get_time();

    // Model matrix (translation, scaling)
    mat4 translation, scaling, model;
    glm_translate_make(translation, (vec3){ -0.95f * aspect, player->position[1], 0.0f });
    glm_scale_make(scaling, (vec3){ PLAYER_WIDTH, PLAYER_HEIGHT, 0.0f });
    glm_mat4_mul(translation, scaling, model);

    // View matrix (identity)
    mat4 view;
    glm_mat4_identity(view);

    // Projection Matrix (ortho)
    const float left = -aspect;
    const float right = aspect;
    const float bottom = -1.0f;
    const float top = 1.0f;
    const float near = -1.0f;
    const float far = 1.0f;
    mat4 projection;
    glm_ortho(left, right, bottom, top, near, far, projection);

    // Calculate color based on time
    vec4 color = {
        (float)(sin(color_time) * 0.5f) + 0.5f,       // Red channel
        (float)(cos(color_time) * 0.5f) + 0.5f,       // Green channel
        (float)(sin(color_time * 0.5) * 0.5f) + 0.5f, // Blue channel
        1.0f                                          // Alpha channel
    };

    // Render player
    cg_apply_pipeline(&player->pipe);
    cg_apply_bindings(&player->bind);
    cg_set_uniform_mat4(player->model_loc, (GLfloat*)&model[0]);
    cg_set_uniform_mat4(player->view_loc, (GLfloat*)&view[0]);
    cg_set_uniform_mat4(player->proj_loc, (GLfloat*)&projection[0]);
    cg_set_uniform_vec4(player->color_loc, (GLfloat*)&color[0]);
    cg_render(&player->bind, 0, 6, 1);
}
