#include <stdio.h>
#include <stdlib.h>

#include <glad/gl.h>

#include "graphics/shader.h"

static char *read_file(const char *path)
{
    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        printf("Failed to open shader: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = malloc((size_t)size + 1);

    if (buffer == NULL)
    {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, (size_t)size, file);
    buffer[size] = '\0';

    fclose(file);

    return buffer;
}

static unsigned int compile_shader(
    unsigned int type,
    const char *source
)
{
    unsigned int shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char info_log[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(
            shader,
            sizeof(info_log),
            NULL,
            info_log
        );

        printf("Shader compilation failed:\n%s\n", info_log);

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

unsigned int shader_create(
    const char *vertex_path,
    const char *fragment_path
)
{
    char *vertex_source = read_file(vertex_path);
    char *fragment_source = read_file(fragment_path);

    if (vertex_source == NULL || fragment_source == NULL)
    {
        free(vertex_source);
        free(fragment_source);

        return 0;
    }

    unsigned int vertex_shader =
        compile_shader(GL_VERTEX_SHADER, vertex_source);

    unsigned int fragment_shader =
        compile_shader(GL_FRAGMENT_SHADER, fragment_source);

    free(vertex_source);
    free(fragment_source);

    if (vertex_shader == 0 || fragment_shader == 0)
    {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        return 0;
    }

    unsigned int program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    int success;
    char info_log[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(
            program,
            sizeof(info_log),
            NULL,
            info_log
        );

        printf("Shader linking failed:\n%s\n", info_log);

        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}

void shader_use(unsigned int program)
{
    glUseProgram(program);
}

void shader_destroy(unsigned int program)
{
    glDeleteProgram(program);
}
