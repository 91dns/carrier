#ifndef CARRIER_TYPES_H
#define CARRIER_TYPES_H

#include <GLFW/glfw3.h>
#include <stdbool.h>

// ENUMERATIONS
// === === === === === ===
// === === === === === ===

// Event types for the application
typedef enum {
    CAPP_EVENT_KEY_DOWN = 0,
    CAPP_EVENT_KEY_UP,
    CAPP_EVENT_KEY_REPEAT,
    CAPP_EVENT_MOUSE_DOWN,
    CAPP_EVENT_MOUSE_UP,
    CAPP_EVENT_MOUSE_MOVE,
    CAPP_EVENT_MOUSE_SCROLL,
    CAPP_EVENT_RESIZE
} capp_event_type;

// Key codes for the application
typedef enum {
    CAPP_KEY_UNKNOWN = -1,
    CAPP_KEY_SPACE = GLFW_KEY_SPACE,
    CAPP_KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE,
    CAPP_KEY_COMMA = GLFW_KEY_COMMA,
    CAPP_KEY_MINUS = GLFW_KEY_MINUS,
    CAPP_KEY_PERIOD = GLFW_KEY_PERIOD,
    CAPP_KEY_SLASH = GLFW_KEY_SLASH,
    CAPP_KEY_0 = GLFW_KEY_0,
    CAPP_KEY_1 = GLFW_KEY_1,
    CAPP_KEY_2 = GLFW_KEY_2,
    CAPP_KEY_3 = GLFW_KEY_3,
    CAPP_KEY_4 = GLFW_KEY_4,
    CAPP_KEY_5 = GLFW_KEY_5,
    CAPP_KEY_6 = GLFW_KEY_6,
    CAPP_KEY_7 = GLFW_KEY_7,
    CAPP_KEY_8 = GLFW_KEY_8,
    CAPP_KEY_9 = GLFW_KEY_9,
    CAPP_KEY_SEMICOLON = GLFW_KEY_SEMICOLON,
    CAPP_KEY_EQUAL = GLFW_KEY_EQUAL,
    CAPP_KEY_A = GLFW_KEY_A,
    CAPP_KEY_B = GLFW_KEY_B,
    CAPP_KEY_C = GLFW_KEY_C,
    CAPP_KEY_D = GLFW_KEY_D,
    CAPP_KEY_E = GLFW_KEY_E,
    CAPP_KEY_F = GLFW_KEY_F,
    CAPP_KEY_G = GLFW_KEY_G,
    CAPP_KEY_H = GLFW_KEY_H,
    CAPP_KEY_I = GLFW_KEY_I,
    CAPP_KEY_J = GLFW_KEY_J,
    CAPP_KEY_K = GLFW_KEY_K,
    CAPP_KEY_L = GLFW_KEY_L,
    CAPP_KEY_M = GLFW_KEY_M,
    CAPP_KEY_N = GLFW_KEY_N,
    CAPP_KEY_O = GLFW_KEY_O,
    CAPP_KEY_P = GLFW_KEY_P,
    CAPP_KEY_Q = GLFW_KEY_Q,
    CAPP_KEY_R = GLFW_KEY_R,
    CAPP_KEY_S = GLFW_KEY_S,
    CAPP_KEY_T = GLFW_KEY_T,
    CAPP_KEY_U = GLFW_KEY_U,
    CAPP_KEY_V = GLFW_KEY_V,
    CAPP_KEY_W = GLFW_KEY_W,
    CAPP_KEY_X = GLFW_KEY_X,
    CAPP_KEY_Y = GLFW_KEY_Y,
    CAPP_KEY_Z = GLFW_KEY_Z,
    CAPP_KEY_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
    CAPP_KEY_BACKSLASH = GLFW_KEY_BACKSLASH,
    CAPP_KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
    CAPP_KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
    CAPP_KEY_WORLD_1 = GLFW_KEY_WORLD_1,
    CAPP_KEY_WORLD_2 = GLFW_KEY_WORLD_2,
    CAPP_KEY_ESCAPE = GLFW_KEY_ESCAPE,
    CAPP_KEY_ENTER = GLFW_KEY_ENTER,
    CAPP_KEY_TAB = GLFW_KEY_TAB,
    CAPP_KEY_BACKSPACE = GLFW_KEY_BACKSPACE,
    CAPP_KEY_INSERT = GLFW_KEY_INSERT,
    CAPP_KEY_DELETE = GLFW_KEY_DELETE,
    CAPP_KEY_RIGHT = GLFW_KEY_RIGHT,
    CAPP_KEY_LEFT = GLFW_KEY_LEFT,
    CAPP_KEY_DOWN = GLFW_KEY_DOWN,
    CAPP_KEY_UP = GLFW_KEY_UP,
    CAPP_KEY_PAGE_UP = GLFW_KEY_PAGE_UP,
    CAPP_KEY_PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
    CAPP_KEY_HOME = GLFW_KEY_HOME,
    CAPP_KEY_END = GLFW_KEY_END,
    CAPP_KEY_CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
    CAPP_KEY_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
    CAPP_KEY_NUM_LOCK = GLFW_KEY_NUM_LOCK,
    CAPP_KEY_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
    CAPP_KEY_PAUSE = GLFW_KEY_PAUSE,
    CAPP_KEY_F1 = GLFW_KEY_F1,
    CAPP_KEY_F2 = GLFW_KEY_F2,
    CAPP_KEY_F3 = GLFW_KEY_F3,
    CAPP_KEY_F4 = GLFW_KEY_F4,
    CAPP_KEY_F5 = GLFW_KEY_F5,
    CAPP_KEY_F6 = GLFW_KEY_F6,
    CAPP_KEY_F7 = GLFW_KEY_F7,
    CAPP_KEY_F8 = GLFW_KEY_F8,
    CAPP_KEY_F9 = GLFW_KEY_F9,
    CAPP_KEY_F10 = GLFW_KEY_F10,
    CAPP_KEY_F11 = GLFW_KEY_F11,
    CAPP_KEY_F12 = GLFW_KEY_F12,
    CAPP_KEY_F13 = GLFW_KEY_F13,
    CAPP_KEY_F14 = GLFW_KEY_F14,
    CAPP_KEY_F15 = GLFW_KEY_F15,
    CAPP_KEY_F16 = GLFW_KEY_F16,
    CAPP_KEY_F17 = GLFW_KEY_F17,
    CAPP_KEY_F18 = GLFW_KEY_F18,
    CAPP_KEY_F19 = GLFW_KEY_F19,
    CAPP_KEY_F20 = GLFW_KEY_F20,
    CAPP_KEY_F21 = GLFW_KEY_F21,
    CAPP_KEY_F22 = GLFW_KEY_F22,
    CAPP_KEY_F23 = GLFW_KEY_F23,
    CAPP_KEY_F24 = GLFW_KEY_F24,
    CAPP_KEY_F25 = GLFW_KEY_F25,
    CAPP_KEY_KP_0 = GLFW_KEY_KP_0,
    CAPP_KEY_KP_1 = GLFW_KEY_KP_1,
    CAPP_KEY_KP_2 = GLFW_KEY_KP_2,
    CAPP_KEY_KP_3 = GLFW_KEY_KP_3,
    CAPP_KEY_KP_4 = GLFW_KEY_KP_4,
    CAPP_KEY_KP_5 = GLFW_KEY_KP_5,
    CAPP_KEY_KP_6 = GLFW_KEY_KP_6,
    CAPP_KEY_KP_7 = GLFW_KEY_KP_7,
    CAPP_KEY_KP_8 = GLFW_KEY_KP_8,
    CAPP_KEY_KP_9 = GLFW_KEY_KP_9,
    CAPP_KEY_KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
    CAPP_KEY_KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
    CAPP_KEY_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
    CAPP_KEY_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
    CAPP_KEY_KP_ADD = GLFW_KEY_KP_ADD,
    CAPP_KEY_KP_ENTER = GLFW_KEY_KP_ENTER,
    CAPP_KEY_KP_EQUAL = GLFW_KEY_KP_EQUAL,
    CAPP_KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
    CAPP_KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
    CAPP_KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT,
    CAPP_KEY_LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
    CAPP_KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
    CAPP_KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
    CAPP_KEY_RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
    CAPP_KEY_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
    CAPP_KEY_MENU = GLFW_KEY_MENU
} capp_keycode;

// Mouse button codes for the application
typedef enum {
    CAPP_MOUSE_LEFT = GLFW_MOUSE_BUTTON_LEFT,
    CAPP_MOUSE_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
    CAPP_MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
    CAPP_MOUSE_LAST = GLFW_MOUSE_BUTTON_LAST,
    CAPP_MOUSE_1 = GLFW_MOUSE_BUTTON_1,
    CAPP_MOUSE_2 = GLFW_MOUSE_BUTTON_2,
    CAPP_MOUSE_3 = GLFW_MOUSE_BUTTON_3,
    CAPP_MOUSE_4 = GLFW_MOUSE_BUTTON_4,
    CAPP_MOUSE_5 = GLFW_MOUSE_BUTTON_5,
    CAPP_MOUSE_6 = GLFW_MOUSE_BUTTON_6,
    CAPP_MOUSE_7 = GLFW_MOUSE_BUTTON_7,
    CAPP_MOUSE_8 = GLFW_MOUSE_BUTTON_8
} capp_mousecode;

// STRUCTURES
// === === === === === ===
// === === === === === ===

// Color structure for the graphics module
typedef struct {
    float r, g, b, a;
} cg_color;

// Input structure for the application
typedef struct {
    capp_keycode key_code;
    capp_mousecode mouse_code;
    int scancode;
    int action;
    int mods;
    int mouse_button;
    double mouse_x, mouse_y;
    double scroll_x, scroll_y;
} capp_input;

// Window structure for the application
typedef struct {
    GLFWwindow* glfw_window;
    int width, height;
    const char* window_title;
    bool fullscreen;
    int gl_major, gl_minor;
} capp_window;

// Event structure for the application
typedef struct {
    capp_event_type type;
    capp_window window;
    capp_input input;
} capp_event;

// Configuration structure for the application
typedef struct {
    void (*init_cb)(void);
    void (*frame_cb)(void);
    void (*cleanup_cb)(void);
    void (*event_cb)(const capp_event* event);
    int width, height;
    const char* window_title;
    bool resizable, fullscreen;
    int gl_major, gl_minor;
} capp_conf;

// Configuration structure for the graphics module
typedef struct {
    bool depth_test;
    bool blend;
} cg_conf;

// Pass action structure for the graphics module
typedef struct {
    cg_color clear_color;
    float clear_depth;
    int clear_stencil;
} cg_pass_action;

// Shader structure for the graphics module
typedef struct {
    GLuint program;
} cg_shader;

// Bindings structure for the graphics module
typedef struct {
    GLuint vao, vbo, ebo;
} cg_bindings;

// Pipeline structure for the graphics module
typedef struct {
    cg_shader shader;
    GLenum primitive_type;
} cg_pipeline;

// Vertex buffer configuration structure for the graphics module
typedef struct {
    size_t size;
    const void* data;
} cg_vertex_conf;

// Index buffer configuration structure for the graphics module
typedef struct {
    size_t size;
    const void* data;
} cg_index_conf;

// Buffer configuration structure for the graphics module
typedef struct {
    cg_vertex_conf vertex_buffer;
    cg_index_conf index_buffer;
} cg_buffer_conf;

// Pipeline configuration structure for the graphics module
typedef struct {
    cg_shader shader;
    GLenum primitive_type;
} cg_pipeline_conf;

// Context structure for the graphics module
typedef struct {
    cg_shader* shaders;
    size_t shader_count;
    cg_pipeline* pipelines;
    size_t pipeline_count;
    cg_bindings* bindings;
    size_t binding_count;
} cg_context;


// Uniform type for the graphics module
typedef GLint cg_uniform;

#endif // CARRIER_TYPES_H
