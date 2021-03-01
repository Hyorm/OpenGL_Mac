#include <stdio.h>

#include <GL/glew.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include "LoadShaders.h"

void init();
void display();

const GLsizei NumVertices = 6;

GLfloat vertices[NumVertices][2] = {
    {-0.90f, -0.90f},
    {0.85f, -0.90f},
    {-0.90f, 0.85f},
    {0.90f, -0.85f},
    {0.90f, 0.90f},
    {-0.85f, 0.90f}
};

GLuint Buffers[1], VertexArrays[1];

int main(int argc, char** argv)
{
    glewExperimental = GL_TRUE;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
    glutInitWindowSize(512, 512);
    glutCreateWindow(argv[0]);
    
    glewExperimental = GL_TRUE;
    
    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        //exit(-1);
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
        {GL_VERTEX_SHADER, "/Users/hyorimkim/Documents/git/ComputerGraphics/CG/CG/triangles.vert"},
        {GL_FRAGMENT_SHADER, "/Users/hyorimkim/Documents/git/ComputerGraphics/CG/CG/triangles.frag"},
        {GL_NONE, NULL}
    };

    GLuint program = LoadShaders(shaders);
    glUseProgram(program);

    GLuint location = glGetAttribLocation(program, "vPosition");
    glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VertexArrays[0]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush();
    glutSwapBuffers();
}
