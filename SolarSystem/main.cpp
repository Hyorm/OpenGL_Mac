#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLUT/glut.h>
#include "LoadShaders.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdexcept>

#define BUFF_SIZE 1024

typedef std::vector<GLfloat> GLvec;

GLuint program;

void init();
void display();

int build_program();
void bind_buffer(GLint buffer, GLvec& vec, int program, const GLchar* attri_name, GLint attri_size);
void get_color_3d_by_pos(GLvec& c, GLvec& p, GLfloat offset);
void get_sphere_3d(GLvec& p, float r, int subh, int suba);
void draw_sphere(const GLfloat* trans_mat);
void keyboard(unsigned char key, int x, int y);

#define FPUSH_VTX3(p, vx, vy, vz) do{p.push_back(vx); p.push_back(vy); p.push_back(vz);}while(0)
#define FSET_VTX3(vx, vy, vz, valx, valy, valz)do{vx = (float)(valx); vy = (float)(valy); vz = (float)(valz);}while(0)

GLuint num_of_vertices = 0;

GLvec vertices;
GLvec colors;
GLuint show_wireframe = 0, show_vertices = 0;
GLuint vao;
GLuint vbo[2];

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_3_2_CORE_PROFILE);
    glutInitWindowSize(512, 512);

    glutCreateWindow(argv[1]);
    glewExperimental = GL_TRUE;
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

void get_sphere_3d(GLvec& p, float r, int subh, int suba) {

    for (int i = 1; i <= subh; ++i) {
    
        double theta0 = M_PI * (i - 1) / subh;
        double theta1 = M_PI * i / subh;

        double y0 = r * cos(theta0);
        double rst0 = r * sin(theta0);
        double y1 = r * cos(theta1);
        double rst1 = r * sin(theta1);

        for (int j = 1; j <= suba; ++j) {

            double phi0 = 2 * M_PI * (j - 1) / suba;
            double phi1 = 2 * M_PI * j / suba;

            double cp0 = cos(phi0);
            double sp0 = sin(phi0);
            double cp1 = cos(phi1);
            double sp1 = sin(phi1);

            float vx0, vy0, vz0, vx1, vy1, vz1;
            float vx2, vy2, vz2, vx3, vy3, vz3;

            FSET_VTX3(vx0, vy0, vz0, sp0 * rst0, y0, cp0 * rst0);
            FSET_VTX3(vx1, vy1, vz1, sp0 * rst1, y1, cp0 * rst1);
            FSET_VTX3(vx2, vy2, vz2, sp1 * rst0, y0, cp1 * rst0);
            FSET_VTX3(vx3, vy3, vz3, sp1 * rst1, y1, cp1 * rst1);

            if (i < subh) {
                
                FPUSH_VTX3(p, vx0, vy0, vz0);
                FPUSH_VTX3(p, vx1, vy1, vz1);
                FPUSH_VTX3(p, vx3, vy3, vz3);
            
            }

            if (1 < i) {

                FPUSH_VTX3(p, vx3, vy3, vz3);
                FPUSH_VTX3(p, vx2, vy2, vz2);
                FPUSH_VTX3(p, vx0, vy0, vz0);

            }
        
        }
    
    }

}

void init() {

    srand(clock());
    program = build_program();
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    get_sphere_3d(vertices, 1.0, 30, 30);
    get_color_3d_by_pos(colors, vertices, 0);//offset

    glGenBuffers(2, vbo);
    bind_buffer(vbo[0], vertices, program, "vPosition", 3);
    bind_buffer(vbo[1], colors, program, "vColor", 3);
    

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //glLineWidth(3.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

}
void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1, 1);
    
    glBindVertexArray(vao);
    num_of_vertices = vertices.size()/3;

    GLfloat theta = 0.00001f * clock();
    using namespace glm;
    mat4 T_sun(1.0f);
    mat4 T_earth(1.0f);
    mat4 T_moon(1.0f);

    T_sun = rotate(T_sun, theta, vec3(0.0f, 1.0f, 0.0f));
    T_sun = translate(T_sun, vec3(0.0f, 0.0f, 0.0f));
    T_sun = rotate(T_sun, theta, vec3(1.0f, 0.0f, 0.0f));
    T_sun = scale(T_sun, vec3(0.5f));

    T_earth = rotate(T_earth, theta, vec3(0.0f, 1.0f, 0.0f));
    T_earth = translate(T_earth, vec3(0.8f, 0.0f, 0.0f));
    T_earth = rotate(T_earth, theta, vec3(0.0f, 1.0f, 0.0f));
    T_earth = scale(T_earth, vec3(0.1f));

    T_moon = rotate(T_moon, theta, vec3(0.0f, 1.0f, 0.0f));
    T_moon = translate(T_earth, vec3(1.7f, 0.0f, 0.0f));
    T_moon = rotate(T_moon, theta, vec3(0.0f, 1.0f, 0.0f));
    T_moon = scale(T_moon, vec3(0.4f));

    GLint crLoc = glGetUniformLocation(program, "draw_mode");
    glUniform1i(crLoc, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    draw_sphere(value_ptr(T_sun));
    draw_sphere(value_ptr(T_earth));
    draw_sphere(value_ptr(T_moon));

    glDisable(GL_POLYGON_OFFSET_FILL);

    if (show_wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1);
        
        crLoc = glGetUniformLocation(program, "draw_mode");
        glUniform1i(crLoc, 1);
        draw_sphere(value_ptr(T_sun));
        draw_sphere(value_ptr(T_earth));
        draw_sphere(value_ptr(T_moon));
    }

    if (show_vertices) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glPointSize(3);

        crLoc = glGetUniformLocation(program, "draw_mode");
        glUniform1i(crLoc, 1);
        draw_sphere(value_ptr(T_sun));
        draw_sphere(value_ptr(T_earth));
        draw_sphere(value_ptr(T_moon));
    }

    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

int build_program() {
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "/Users/hyorimkim/Documents/SolarSystem/SolarSystem/vPosition.vert" },
        {GL_FRAGMENT_SHADER, "/Users/hyorimkim/Documents/SolarSystem/SolarSystem/vColor.frag"},
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

void get_color_3d_by_pos(GLvec& c, GLvec& p, GLfloat offset) {

    GLfloat max_val[3] = { -INFINITY, -INFINITY, -INFINITY};
    GLfloat min_val[3] = { INFINITY, INFINITY, INFINITY };

    int n = (int)(p.size() / 3);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 3; ++j) {
        
            GLfloat val = p[i * 3 + j];
            if (max_val[j] < val) max_val[j] = val;
            else if (min_val[j] > val) min_val[j] = val;
        
        }
    
    }

    GLfloat width[3] = {

        max_val[0] - min_val[0],
        max_val[1] - min_val[1],
        max_val[2] - min_val[2]

    };

    c.resize(p.size());

    for (int i = 0; i < n; ++i) {
    
        for (int j = 0; j < 3; ++j) {
        
            int k = i * 3 + j;
            c[k] = std::fmin((p[k] - min_val[j]) / width[j] + offset, 1.0f);

        }

    }

}

void draw_sphere(const GLfloat* trans_mat) {

    GLint crLoc = glGetUniformLocation(program, "T");
    glUniformMatrix4fv(crLoc, 1, GL_FALSE, trans_mat);
    glDrawArrays(GL_TRIANGLES, 0, num_of_vertices);

}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        show_wireframe = 1;
        show_vertices = 0;
        glutPostRedisplay();
        break;
    case 'v':
        show_wireframe = 0;
        show_vertices = 1;
        glutPostRedisplay();
        break;
    default:
        show_wireframe = 0;
        show_vertices = 0;
        glutPostRedisplay();
        break;
    }

}
