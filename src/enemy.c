#include "enemy.h"
#include "ball.h"
#include "../libs/carrier_app.h"
#include "constants.h"

void init_enemy(enemy* enemy) {
    enemy->shd = cg_load_shader("../shaders/enemy.vert", "../shaders/enemy.frag");

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

    enemy->bind = cg_make_buffer(&(cg_buffer_conf) {
        .vertex_buffer = {
            .size = sizeof(vertices),
            .data = vertices
        },
        .index_buffer = {
            .size = sizeof(indices),
            .data = indices
        }
    });

    enemy->pipe = cg_make_pipeline(&(cg_pipeline_conf) {
        .shader = enemy->shd,
        .primitive_type = GL_TRIANGLES
    });

    enemy->model_loc = cg_get_location(enemy->shd, "u_model");
    enemy->view_loc = cg_get_location(enemy->shd, "u_view");
    enemy->proj_loc = cg_get_location(enemy->shd, "u_proj");
    enemy->color_loc = cg_get_location(enemy->shd, "u_color");

    glm_vec3_zero(enemy->position);
}

void update_enemy(enemy* enemy, ball* ball, float delta_time) {
    if (ball->position[0] > 0.0f) {
        if (enemy->position[1] < ball->position[1]) {
            enemy->position[1] += ENEMY_SPEED * delta_time;
        } else if (enemy->position[1] > ball->position[1]) {
            enemy->position[1] -= ENEMY_SPEED * delta_time;
        }
    }
}

void render_enemy(enemy* enemy, float aspect) {
    const float color_time = cr_get_time();

    // Model matrix (translation, scaling)
    mat4 translation, scaling, model;
    glm_translate_make(translation, (vec3){ 0.95 * aspect, enemy->position[1], 0.0f });
    glm_scale_make(scaling, (vec3){ ENEMY_WIDTH, ENEMY_HEIGHT, 0.0f });
    glm_mat4_mul(translation, scaling, model);

    // View matrix(identity)
    mat4 view;
    glm_mat4_identity(view);

    // Projection matrix(ortho)
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

    // Render enemy
    cg_apply_pipeline(&enemy->pipe);
    cg_apply_bindings(&enemy->bind);
    cg_set_uniform_mat4(enemy->model_loc, (GLfloat*)&model[0]);
    cg_set_uniform_mat4(enemy->view_loc, (GLfloat*)&view[0]);
    cg_set_uniform_mat4(enemy->proj_loc, (GLfloat*)&projection[0]);
    cg_set_uniform_vec4(enemy->color_loc, (GLfloat*)&color[0]);
    cg_render(&enemy->bind, 0, 6, 1);
}
