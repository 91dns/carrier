#include "ball.h"
#include "player.h"
#include "enemy.h"
#include "constants.h"
#include "../libs/carrier_app.h"
#include <cglm/vec3.h>

void init_ball(ball* ball) {
    ball->shd = cg_load_shader("../shaders/ball.vert", "../shaders/ball.frag");

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

    ball->bind = cg_make_buffer(&(cg_buffer_conf) {
        .vertex_buffer = {
            .size = sizeof(vertices),
            .data = vertices
        },
        .index_buffer = {
            .size = sizeof(indices),
            .data = indices
        }
    });

    ball->pipe = cg_make_pipeline(&(cg_pipeline_conf) {
        .shader = ball->shd,
        .primitive_type = GL_TRIANGLES
    });

    ball->model_loc = cg_get_location(ball->shd, "u_model");
    ball->view_loc = cg_get_location(ball->shd, "u_view");
    ball->proj_loc = cg_get_location(ball->shd, "u_proj");
    ball->color_loc = cg_get_location(ball->shd, "u_color");

    glm_vec3_zero(ball->position);
    glm_vec3_copy((vec3){ BALL_SPEED, BALL_SPEED, 0.0f }, ball->velocity);
}

void update_ball(ball* ball, player* player, enemy* enemy, float delta_time, float aspect) {
    ball->position[0] += ball->velocity[0] * delta_time;
    ball->position[1] += ball->velocity[1] * delta_time;

    if (ball->position[1] + BALL_HALF_SIZE > TOP_BOUNDARY) {
        ball->velocity[1] = -BALL_SPEED;
    } else if (ball->position[1] - BALL_HALF_SIZE < BOTTOM_BOUNDARY) {
        ball->velocity[1] = BALL_SPEED;
    }

    // Check collision with player
    if (ball->position[0] - BALL_HALF_SIZE < -0.95f * aspect + PLAYER_WIDTH / 2.0f) {
        if (ball->position[1] + BALL_HALF_SIZE > player->position[1] - PLAYER_HEIGHT / 2.0f &&
            ball->position[1] - BALL_HALF_SIZE < player->position[1] + PLAYER_HEIGHT / 2.0f) {
            ball->velocity[0] = BALL_SPEED;
        }
    }

    // Check collision with enemy
    if (ball->position[0] + BALL_HALF_SIZE > 0.95f * aspect - ENEMY_WIDTH / 2.0f) {
        if (ball->position[1] + BALL_HALF_SIZE > enemy->position[1] - ENEMY_HEIGHT / 2.0f &&
            ball->position[1] - BALL_HALF_SIZE < enemy->position[1] + ENEMY_HEIGHT / 2.0f) {
            ball->velocity[0] = -BALL_SPEED;
        }
    }

    // Check if ball is out of bounds and reset
    if (ball->position[0] + BALL_HALF_SIZE > aspect || ball->position[0] - BALL_HALF_SIZE < -aspect) {
        ball_reset(ball, aspect);
    }
}

void render_ball(ball* ball, float aspect) {
    const float color_time = cr_get_time();

    // Model matrix (translation, scaling)
    mat4 translation, scaling, model;
    glm_translate_make(translation, (vec3){ ball->position[0], ball->position[1], 0.0f });
    glm_scale_make(scaling, (vec3){ BALL_SIZE, BALL_SIZE, 0.0f });
    glm_mat4_mul(translation, scaling, model);

    // View matrix (identity)
    mat4 view;
    glm_mat4_identity(view);

    // Projection matrix (ortho)
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

    // Render ball
    cg_apply_pipeline(&ball->pipe);
    cg_apply_bindings(&ball->bind);
    cg_set_uniform_mat4(ball->model_loc, (GLfloat*)&model[0]);
    cg_set_uniform_mat4(ball->view_loc, (GLfloat*)&view[0]);
    cg_set_uniform_mat4(ball->proj_loc, (GLfloat*)&projection[0]);
    cg_set_uniform_vec4(ball->color_loc, (GLfloat*)&color[0]);
    cg_render(&ball->bind, 0, 6, 1);
}

void ball_reset(ball *ball, float aspect) {
    glm_vec3_zero(ball->position);
    glm_vec3_copy((vec3){ BALL_SPEED, BALL_SPEED, 0.0f }, ball->velocity);
}
