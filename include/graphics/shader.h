#ifndef SHADER_H
#define SHADER_H

unsigned int shader_create(
    const char *vertex_path,
    const char *fragment_path
);

void shader_use(unsigned int program);
void shader_destroy(unsigned int program);

#endif
