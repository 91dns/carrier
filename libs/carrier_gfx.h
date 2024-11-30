#ifndef CARRIER_GFX_H
#define CARRIER_GFX_H

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libs/carrier_types.h"
#include "../libs/carrier_log.h"

// PUBLIC API
// These functions are intended to be used by the users of the library.
// === === === === === ===
// === === === === === ===

static void cg_setup(const cg_conf* conf);
static void cg_shutdown(void);
static void cg_begin_pass(const cg_pass_action* action);
static void cg_end_pass(void);
static void cg_commit();
static cg_shader cg_load_shader(const char* vertex_path, const char* fragment_path);
static cg_bindings cg_make_buffer(const cg_buffer_conf* buffer_conf);
static cg_pipeline cg_make_pipeline(const cg_pipeline_conf* conf);
static void cg_apply_pipeline(cg_pipeline* pipeline);
static void cg_apply_bindings(cg_bindings* bindings);
static void cg_render(cg_bindings* bindings, int base_element, int num_elements, int num_instances);
static cg_uniform cg_get_location(cg_shader shader, const char* name);
static void cg_set_uniform_mat4(cg_uniform location, const GLfloat* value);
static void cg_set_uniform_vec4(cg_uniform location, const GLfloat* value);
static void cg_set_wireframe(bool enable);

// INTERNAL
// These functions are intended for internal use within the library.
// === === === === === ===
// === === === === === ===

static char* read_file(const char* path);
static GLuint compile_shader(const char* source, GLenum type);

// Global variable to hold the graphics context
static cg_context context = {0};

// PUBLIC API IMPLEMENTATION
// === === === === === ===
// === === === === === ===

static void cg_setup(const cg_conf *conf) {
    if (glewInit() != GLEW_OK) {
        cr_log(CR_ERROR, "Failed to initialize [carrier graphics module]");
        exit(EXIT_FAILURE);
    }

    bool depth_test = conf->depth_test;
    bool blend = conf->blend;

    if (depth_test) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    if (blend) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }

    context.shaders = NULL;
    context.shader_count = 0;
    context.pipelines = NULL;
    context.pipeline_count = 0;
    context.bindings = NULL;
    context.binding_count = 0;
    cr_log(CR_SUCCESS, "Successfully initialized [carrier graphics module]");
}

static void cg_shutdown(void) {
    for (size_t i = 0; i < context.shader_count; i++) {
        glDeleteProgram(context.shaders[i].program);
    }
    free(context.shaders);

    for (size_t i = 0; i < context.binding_count; i++) {
        glDeleteVertexArrays(1, &context.bindings[i].vao);
        glDeleteBuffers(1, &context.bindings[i].vbo);
        if (context.bindings[i].ebo != 0) {
            glDeleteBuffers(1, &context.bindings[i].ebo);
        }
    }
    free(context.bindings);

    free(context.pipelines);
    cr_log(CR_SUCCESS, "Successfully shutdown [carrier graphics module]");
}

static void cg_begin_pass(const cg_pass_action* action) {
    glClearColor(action->clear_color.r, action->clear_color.g, action->clear_color.b, action->clear_color.a);

    GLbitfield clear_mask = GL_COLOR_BUFFER_BIT;

    if (action->clear_depth >= 0.0f && action->clear_depth <= 1.0f) {
        glClearDepth(action->clear_depth);
        clear_mask |= GL_DEPTH_BUFFER_BIT;
    }

    if (action->clear_stencil >= 0) {
        glClearStencil(action->clear_stencil);
        clear_mask |= GL_STENCIL_BUFFER_BIT;
    }

    glClear(clear_mask);
}

static void cg_end_pass(void) {
    glBindVertexArray(0);
    glUseProgram(0);
}

static void cg_commit() {
    glfwSwapBuffers(glfwGetCurrentContext());
}

static cg_shader cg_load_shader(const char* vertex_path, const char* fragment_path) {
    char* vertex_source = read_file(vertex_path);
    char* fragment_source = read_file(fragment_path);

    if (!vertex_source || !fragment_source) {
        free(vertex_source);
        free(fragment_source);
        cr_log(CR_ERROR, "Failed to load shaders");
        return (cg_shader){ 0 };
    }

    GLuint vertex_shader = compile_shader(vertex_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(fragment_source, GL_FRAGMENT_SHADER);

    free(vertex_source);
    free(fragment_source);

    if (!vertex_shader || !fragment_shader) {
        return (cg_shader){ 0 };
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(program, 512, NULL, info_log);
        cr_log(CR_ERROR, info_log);
        glDeleteProgram(program);
        return (cg_shader){ 0 };
    }

    context.shaders = (cg_shader*)realloc(context.shaders, (context.shader_count + 1) * sizeof(cg_shader));
    context.shaders[context.shader_count++] = (cg_shader){program};
    cr_log(CR_SUCCESS, "Successfully loaded shaders");
    return (cg_shader){program};
}

static cg_bindings cg_make_buffer(const cg_buffer_conf* buffer_conf) {
    cg_bindings bindings;

    glGenVertexArrays(1, &bindings.vao);
    glGenBuffers(1, &bindings.vbo);

    glBindVertexArray(bindings.vao);

    glBindBuffer(GL_ARRAY_BUFFER, bindings.vbo);
    glBufferData(GL_ARRAY_BUFFER, buffer_conf->vertex_buffer.size, buffer_conf->vertex_buffer.data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    if (buffer_conf->index_buffer.data != NULL) {
        glGenBuffers(1, &bindings.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bindings.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer_conf->index_buffer.size, buffer_conf->index_buffer.data, GL_STATIC_DRAW);
    } else {
        bindings.ebo = 0;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    context.bindings = (cg_bindings*)realloc(context.bindings, (context.binding_count + 1) * sizeof(cg_bindings));
    context.bindings[context.binding_count++] = bindings;
    return bindings;
}

static cg_pipeline cg_make_pipeline(const cg_pipeline_conf* conf) {
    cg_pipeline pipeline;

    pipeline.shader = conf->shader;
    pipeline.primitive_type = conf->primitive_type;

    context.pipelines = (cg_pipeline*)realloc(context.pipelines, (context.pipeline_count + 1) * sizeof(cg_pipeline));
    context.pipelines[context.pipeline_count++] = pipeline;
    return pipeline;
}

static void cg_apply_pipeline(cg_pipeline* pipeline) {
    glUseProgram(pipeline->shader.program);
}

static void cg_apply_bindings(cg_bindings* bindings) {
    glBindVertexArray(bindings->vao);
}

static void cg_render(cg_bindings* bindings, int base_element, int num_elements, int num_instances) {
    if (num_instances > 1) {
        if (bindings->ebo != 0) {
            glDrawElementsInstanced(GL_TRIANGLES, num_elements, GL_UNSIGNED_INT, (void*)(base_element * sizeof(unsigned int)), num_instances);
        } else {
            glDrawArraysInstanced(GL_TRIANGLES, base_element, num_elements, num_instances);
        }
    } else {
        if (bindings->ebo != 0) {
            glDrawElements(GL_TRIANGLES, num_elements, GL_UNSIGNED_INT, (void*)(base_element * sizeof(unsigned int)));
        } else {
            glDrawArrays(GL_TRIANGLES, base_element, num_elements);
        }
    }
}

static cg_uniform cg_get_location(cg_shader shader, const char* name) {
    return glGetUniformLocation(shader.program, name);
}

static void cg_set_uniform_mat4(cg_uniform location, const GLfloat* value) {
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

static void cg_set_uniform_vec4(cg_uniform location, const GLfloat* value) {
    glUniform4fv(location, 1, value);
}

static void cg_set_wireframe(bool enable) {
    glPolygonMode(GL_FRONT_AND_BACK, enable ? GL_LINE : GL_FILL);
}

// INTERNAL IMPLEMENTATION
// === === === === === ===
// === === === === === ===

static char* read_file(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        cr_log(CR_ERROR, "Failed to open shader file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        cr_log(CR_ERROR, "Failed to allocate memory for shader file");
        fclose(file);
        return NULL;
    }

    if (fread(buffer, 1, length, file) != length) {
        cr_log(CR_ERROR, "Failed to read shader file");
        fclose(file);
        free(buffer);
        return NULL;
    }
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

static GLuint compile_shader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        cr_log(CR_ERROR, info_log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

#endif // CARRIER_GFX_H
