#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GLUT/glut.h>
#include "LoadShaders.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

typedef std::vector<GLfloat> GLvec;
using namespace glm;

void init();
void display();
void get_box_3d(std::vector<GLfloat>& p, GLfloat lx, GLfloat ly, GLfloat lz);
void get_box_random_color(std::vector<GLfloat>& color);
void keyboard(unsigned char key, int x, int y);
int build_program();
void bind_buffer(GLint buffer, GLvec& vec, int program, const GLchar* attri_name, GLint attri_size);
void share_buffer(GLint buffer, int program, const GLchar* attri_name, GLint attri_size);

GLuint active_vao = 0;
GLfloat scaling_factor = 1.0f;
GLuint drawing_mode = 0;
GLuint vao, vbo[2];
GLvec vertices, colors;

GLuint program;

int main(int argc, char** argv) {
    glewExperimental = GL_TRUE;

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_3_2_CORE_PROFILE);
    glutInitWindowSize(512, 512);

    glutCreateWindow("VAO example");

    GLenum err = glewInit();

    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}

void init() {

    srand(clock());
    program = build_program();

    get_box_3d(vertices, 1, 1, 1);
    get_box_random_color(colors);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(2, vbo);

    bind_buffer(vbo[0], vertices, program, "vPosition", 3);
    bind_buffer(vbo[1], colors, program, "vColor", 3);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

}
void display() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao);

    GLint location;
    GLfloat theta = 0.00001f * clock();
    location = glGetUniformLocation(program, "drawing_mode");
    glUniform1i(location, drawing_mode);

    mat4 T(1.0f);
    T = rotate(T, theta, vec3(1.0f, 0.0f, 0.0f));
    T = rotate(T, theta, vec3(0.0f, 0.1f, 0.0f));
    T = rotate(T, theta, vec3(1.0f, 0.0f, 0.0f));
    T = scale(T, vec3(scaling_factor));//TODO: change this variables

    location = glGetUniformLocation(program, "T");
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(T));

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        drawing_mode = 0;
        glutPostRedisplay();
        break;
    case '2':
        drawing_mode = 1;
        glutPostRedisplay();
        break;
    case '3':
        drawing_mode = 2;
        glutPostRedisplay();
        break;
    case '+':
        scaling_factor+=0.1f;
        if (scaling_factor > 1.2f)scaling_factor = 1.2f;
        glutPostRedisplay();
        break;
    case '-':
        scaling_factor-= 0.1f;
        if (scaling_factor < 0.1f)scaling_factor = 0.1f;
        glutPostRedisplay();
        break;
    }
}
int build_program() {
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "/Users/hyorimkim/Documents/RotatingACube/RotatingACube/vPosition.vert" },
        {GL_FRAGMENT_SHADER, "/Users/hyorimkim/Documents/RotatingACube/RotatingACube/vColor.frag"},
        {GL_NONE, NULL}
    };
    program = LoadShaders(shaders);
    glUseProgram(program);
    return program;

}
void bind_buffer(GLint buffer, GLvec& vec, int program, const GLchar* attri_name, GLint attri_size) {

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vec.size(), vec.data(), GL_STATIC_DRAW);

    GLuint location = glGetAttribLocation(program, attri_name);

    glVertexAttribPointer(location, attri_size, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);

}
void share_buffer(GLint buffer, int program, const GLchar* attri_name, GLint attri_size) {
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vec.size(), vec.data(), GL_STATIC_DRAW);
    GLuint location = glGetAttribLocation(program, attri_name);
    glVertexAttribPointer(location, attri_size, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);

}

void get_box_3d(std::vector<GLfloat>& p, GLfloat lx, GLfloat ly, GLfloat lz) {

    static const GLfloat box_vertices[] = {

        0.5f, 0.5f, -0.5f,    -0.5f, -0.5f, -0.5f,    -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,    0.5f, -0.5f, -0.5f,    -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,    -0.5f, -0.5f, 0.5f,        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,    -0.5f, 0.5f, 0.5f,        -0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,        -0.5f, -0.5f,-0.5f,        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,        -0.5f, -0.5f, 0.5f,        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,        -0.5f, -0.5f, 0.5f,        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,        -0.5f, 0.5f, 0.5f,        0.5f,-0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,    0.5f, -0.5f, -0.5f,        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,        0.5f, 0.5f, 0.5f,    0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,        0.5f, 0.5f, -0.5f,        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,        -0.5f, 0.5f, -0.5f,        -0.5f, 0.5f, 0.5f,

    };

    p.resize(sizeof(box_vertices) / sizeof(GLfloat));
    memcpy(p.data(), box_vertices, sizeof(box_vertices));
    size_t n = p.size() / 3;

    for (int i = 0; i < n; ++i) {

        p[3 * i + 0] *= lx;
        p[3 * i + 1] *= ly;
        p[3 * i + 2] *= lz;
    }

}

void get_box_random_color(std::vector<GLfloat>& color) {

    color.resize(108);

    GLfloat* p = color.data();
    for (size_t i = 0; i < 6; ++i) {
    
        GLfloat side_color[3] = {

            0.5f * rand() / RAND_MAX + 0.5f,
            0.5f * rand() / RAND_MAX + 0.5f,
            0.5f * rand() / RAND_MAX + 0.5f

        };

        for (size_t j = 0; j < 6; ++j) {
        
            *p++ = side_color[0];
            *p++ = side_color[1];
            *p++ = side_color[2];
        
        }

    }

}


