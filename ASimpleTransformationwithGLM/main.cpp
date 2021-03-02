#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <GL/glew.h>
#include <GLUT/glut.h>
#include "LoadShaders.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

void init();
void display();

const GLsizei NumVertices = 6;
GLuint program, Buffers[1], VertexArrays[1];

GLfloat vertices[NumVertices][2] = {
    {-0.90f, -0.90f},
    {0.85f, -0.90f},
    {-0.90f, 0.85f},
    {0.90f, -0.85f},
    {0.90f, 0.90f},
    {-0.85f, 0.90f}
};

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_3_2_CORE_PROFILE);
    glutInitWindowSize(512, 512);
    glutCreateWindow(argv[0]);
    
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    
    if (err != GLEW_OK)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    init();

    glutDisplayFunc(display);
    glutMainLoop();


}

void init()
{
    glGenVertexArrays(1, VertexArrays);
    glBindVertexArray(VertexArrays[0]);
    glGenBuffers(1, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "/Users/hyorimkim/Documents/git/OpenGL_Mac/ASimpleTransformationwithGLM/ASimpleTransformationwithGLM/triangles.vert"},
        {GL_FRAGMENT_SHADER, "/Users/hyorimkim/Documents/git/OpenGL_Mac/ASimpleTransformationwithGLM/ASimpleTransformationwithGLM/triangles.frag"},
        {GL_NONE, NULL}
    };

    program = LoadShaders(shaders);
    glUseProgram(program);

    GLuint location = glGetAttribLocation(program, "vPosition");
    glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VertexArrays[0]);

    using namespace glm;

    GLfloat theta = 0.00001f * clock();
    mat4 T(1.0f);
    T = rotate(T, theta, vec3(-1.0f, 1.0f, 0.0f));
    T = scale(T, vec3(0.5f));
    GLuint location = glGetUniformLocation(program, "T");
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(T));

    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();

}
