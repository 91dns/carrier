#include "player.h"
#include "enemy.h"
#include "ball.h"
#include "../libs/carrier_app.h"

static struct {
    cg_pass_action pass_action;
    player player;
    enemy enemy;
    ball ball;
    float aspect;
    float width, height;
} state;

void init(void) {
    cg_setup(&(cg_conf) {
        .blend = true,
        .depth_test = true
    });

    state.pass_action = (cg_pass_action) {
        .clear_color = { 0.1f, 0.1f, 0.15f, 1.0f },
        .clear_depth = 1.0f,
        .clear_stencil = 0
    };

    state.width = cr_get_width();
    state.height = cr_get_height(); 

    init_player(&state.player);
    init_enemy(&state.enemy);
    init_ball(&state.ball);
}

void frame(void) {
    // Calculate delta time
    static float last_time = 0.0f;
    const float current_time = cr_get_time();
    const float delta_time = current_time - last_time;
    last_time = current_time;

    // Get window aspect ratio
    state.aspect = state.width / state.height;

    // Update clear color based on time
    const float color_time = current_time * 0.5f;
    const float anthracite[3] = { 0.1f, 0.1f, 0.1f };
    const float dark_blue[3] = { 0.0f, 0.0f, 0.3f };
    const float t = (sin(color_time) * 0.5f) + 0.5f;

    state.pass_action.clear_color.r = anthracite[0] * (1.0f - t) + dark_blue[0] * t;
    state.pass_action.clear_color.g = anthracite[1] * (1.0f - t) + dark_blue[1] * t;
    state.pass_action.clear_color.b = anthracite[2] * (1.0f - t) + dark_blue[2] * t;

    // Begin pass
    cg_begin_pass(&state.pass_action);

    // Update here
    update_player(&state.player, delta_time, cwindow);
    update_enemy(&state.enemy, &state.ball, delta_time);
    update_ball(&state.ball, &state.player, &state.enemy, delta_time, state.aspect);

    // Render here
    render_player(&state.player, state.aspect);
    render_enemy(&state.enemy, state.aspect);
    render_ball(&state.ball, state.aspect);

    // End pass and commit frame
    cg_end_pass();
    cg_commit();
}

void cleanup(void) {
    cg_shutdown();
}

void event(const capp_event* e) {
    static bool wireframe = false;

    if (e->type == CAPP_EVENT_KEY_DOWN) {
        switch (e->input.key_code) {
            case CAPP_KEY_ESCAPE:
                cr_set_window_should_close(true);
                break;

            case CAPP_KEY_G:
                wireframe = !wireframe;
                cg_set_wireframe(wireframe);
                break;

            default:
                break;
        }
    }
}

capp_conf carrier_main(int argc, char* argv[]) {
    (void)argc, (void)argv;

    return (capp_conf) {
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = 800,
        .height = 600,
        .window_title = "Pong",
        .resizable = true,
        .fullscreen = false,
        .gl_major = 4,
        .gl_minor = 6,
    };
}

CARRIER_MAIN_FUNC(argc, argv)
