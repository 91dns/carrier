#ifndef CARRIER_APP_H
#define CARRIER_APP_H

#include <GL/glew.h>
#include "../libs/carrier_types.h"
#include "../libs/carrier_log.h"
#include <stdlib.h>

// PUBLIC API
// These functions are intended to be used by the users of the library.
// === === === === === ===
// === === === === === ===

static void cr_set_window_title(const char* new_title);
static void cr_set_window_should_close(bool should_close);
static float cr_get_time(void);
static float cr_get_width(void);
static float cr_get_height(void);
static GLFWwindow* cr_get_window(void);
static int cr_get_key(const capp_window* window, capp_keycode key);

// INTERNAL
// These functions are intended for internal use within the library.
// === === === === === ===
// === === === === === ===

static void cr_setup(const capp_conf* conf);
static void cr_run(const capp_conf* conf);
static void cr_setup_window(const capp_conf* conf, capp_window* cwindow);
static void error_callback(int error, const char* desc);
static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_callback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Global variable to hold the window context
static capp_window* cwindow = {0};

// PUBLIC API IMPLEMENTATION
// === === === === === ===
// === === === === === ===

static void cr_set_window_title(const char* new_title) {
    if (cwindow->glfw_window) {
        glfwSetWindowTitle(cwindow->glfw_window, new_title);
        cwindow->window_title = new_title;
        cr_log(CR_SUCCESS, "Successfully updated window title");
    } else {
        cr_log(CR_ERROR, "Failed to update window title: invalid window");
    }
}

static void cr_set_window_should_close(bool should_close) {
    if (cwindow->glfw_window) {
        glfwSetWindowShouldClose(cwindow->glfw_window, should_close);
    } else {
        cr_log(CR_ERROR, "Failed to set window should close: invalid window");
    }
}

static float cr_get_time(void) {
    return glfwGetTime();
}

static float cr_get_width(void) {
    return cwindow->width;
}

static float cr_get_height(void) {
    return cwindow->height;
}

static GLFWwindow* cr_get_window(void) {
    return cwindow->glfw_window;
}

static int cr_get_key(const capp_window* window, capp_keycode key) {
    return glfwGetKey(window->glfw_window, key);
}

static void cr_setup(const capp_conf* conf) {
    if (!glfwInit()) {
        cr_log(CR_ERROR, "Failed to initialize [carrier app module]");
        exit(EXIT_FAILURE);
    }
    cr_log(CR_SUCCESS, "Successfully initialized [carrier app module]");

    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, conf->gl_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, conf->gl_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, conf->resizable ? GLFW_TRUE : GLFW_FALSE);

    cwindow = (capp_window*)malloc(sizeof(capp_window));
    if (!cwindow) {
        cr_log(CR_ERROR, "Failed to allocate memory for window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    cr_setup_window(conf, cwindow);

    glfwMakeContextCurrent(cwindow->glfw_window);
    glfwSwapInterval(1);
    glfwSetWindowUserPointer(cwindow->glfw_window, (void*)conf);
    glfwSetKeyCallback(cwindow->glfw_window, key_callback);
    glfwSetMouseButtonCallback(cwindow->glfw_window, mouse_callback);
    glfwSetCursorPosCallback(cwindow->glfw_window, cursor_callback);
    glfwSetScrollCallback(cwindow->glfw_window, scroll_callback);
    glfwSetFramebufferSizeCallback(cwindow->glfw_window, framebuffer_size_callback);
    glViewport(0, 0, cwindow->width, cwindow->height);

    if (conf->init_cb) { conf->init_cb(); }
}

static void cr_run(const capp_conf* conf) {
    while (!glfwWindowShouldClose(cwindow->glfw_window)) {
        if (conf->frame_cb) { conf->frame_cb(); }
        glfwPollEvents();
    }

    if (conf->cleanup_cb) { conf->cleanup_cb(); }

    glfwDestroyWindow(cwindow->glfw_window);
    glfwTerminate();
    free(cwindow);
    cr_log(CR_SUCCESS, "Successfully shutdown [carrier app module]");
}

// INTERNAL IMPLEMENTATION
// === === === === === ===
// === === === === === ===

static void cr_setup_window(const capp_conf* conf, capp_window* cwindow) {
    GLFWmonitor* monitor = NULL;

    if (conf->fullscreen) {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        cwindow->glfw_window = glfwCreateWindow(mode->width, mode->height, conf->window_title, monitor, NULL);
        cwindow->width = mode->width;
        cwindow->height = mode->height;
        cwindow->fullscreen = true;
    } else {
        cwindow->glfw_window = glfwCreateWindow(conf->width, conf->height, conf->window_title, NULL, NULL);
        cwindow->width = conf->width;
        cwindow->height = conf->height;
        cwindow->fullscreen = false;
    }

    if (!cwindow->glfw_window) {
        cr_log(CR_ERROR, "Failed to create window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    cwindow->window_title = conf->window_title;
    cwindow->gl_major = conf->gl_major;
    cwindow->gl_minor = conf->gl_minor;
    cr_log(CR_SUCCESS, "Successfully created window");
}

static void error_callback(int error, const char* desc) {
    char message[256];
    snprintf(message, sizeof(message), "Error %d: %s", error, desc);
    cr_log(CR_ERROR, message);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    const capp_conf* conf = (capp_conf*)glfwGetWindowUserPointer(window);

    if (conf->event_cb) {
        capp_event event;

        if (action == GLFW_PRESS) {
            event.type = CAPP_EVENT_KEY_DOWN;
        } else if (action == GLFW_RELEASE) {
            event.type = CAPP_EVENT_KEY_UP;
        } else if (action == GLFW_REPEAT) {
            event.type = CAPP_EVENT_KEY_REPEAT;
        }

        event.input.key_code = (capp_keycode)key;
        event.input.scancode = scancode;
        event.input.action = action;
        event.input.mods = mods;
        conf->event_cb(&event);
    }
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    const capp_conf* conf = (capp_conf*)glfwGetWindowUserPointer(window);

    if (conf->event_cb) {
        capp_event event;
        event.type = action == GLFW_PRESS ? CAPP_EVENT_MOUSE_DOWN : CAPP_EVENT_MOUSE_UP;
        event.input.mouse_code = (capp_mousecode)button;
        event.input.action = action;
        event.input.mods = mods;
        glfwGetCursorPos(window, &event.input.mouse_x, &event.input.mouse_y);
        conf->event_cb(&event);
    }
}

static void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    const capp_conf* conf = (capp_conf*)glfwGetWindowUserPointer(window);

    if (conf->event_cb) {
        capp_event event;
        event.type = CAPP_EVENT_MOUSE_MOVE;
        event.input.mouse_x = xpos;
        event.input.mouse_y = ypos;
        conf->event_cb(&event);
    }
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    const capp_conf* conf = (capp_conf*)glfwGetWindowUserPointer(window);

    if (conf->event_cb) {
        capp_event event;
        event.type = CAPP_EVENT_MOUSE_SCROLL;
        event.input.scroll_x = xoffset;
        event.input.scroll_y = yoffset;
        conf->event_cb(&event);
    }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    const capp_conf* conf = (capp_conf*)glfwGetWindowUserPointer(window);

    if (conf->event_cb) {
        capp_event event;
        event.type = CAPP_EVENT_RESIZE;
        event.window.width = width;
        event.window.height = height;
        conf->event_cb(&event);
    }
    glViewport(0, 0, width, height);
}

// Macro to define the main function for the application
#define CARRIER_MAIN_FUNC(argc, argv) \
    int main(int argc, char* argv[]) { \
        capp_conf conf = carrier_main(argc, argv); \
        cr_setup(&conf); \
        cr_run(&conf); \
        return 0; \
}

#endif // CARRIER_APP_H
