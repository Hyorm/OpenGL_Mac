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
#include <stdexcept>
#include <iostream>
#include <fstream>

#define BUFF_SIZE 1024

using namespace std;
typedef vector<GLfloat> GLvec;

void init();
void display();

void keyboard(unsigned char key, int x, int y);
int build_program();
void bind_buffer(GLint buffer, GLvec& vec, int program, const GLchar* attri_name, GLint attri_size);
void bind_buffer(GLint buffer, vector<GLuint>& vec, int program);
bool load_obj(GLvec& vertices_, vector<GLuint>& faces_, const char* filepath);

const GLuint num_of_models = 3;
GLuint active_vao = 0;

const char* obj_filepath[num_of_models] = {

        "/Users/hyorimkim/Documents/SimpleOBJLoader/SimpleOBJLoader/house.obj",
        "/Users/hyorimkim/Documents/SimpleOBJLoader/SimpleOBJLoader/sphere.obj",
        "/Users/hyorimkim/Documents/SimpleOBJLoader/SimpleOBJLoader/teapot.obj"

};

vector<GLuint> faces[num_of_models];
GLuint vao[num_of_models];
GLuint vbo[num_of_models][2];
GLvec vertices[num_of_models];

GLuint program;

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_3_2_CORE_PROFILE);
    glutInitWindowSize(512, 512);

    glutCreateWindow("VAO example");
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

bool load_obj(GLvec& vertices_, vector<GLuint>& faces_, const char* filepath) {

    ifstream fp;
    fp.open(filepath);
    if(!fp.is_open()){
        printf("file open error\n");
        return false;
    }
    string str;
    while (getline(fp, str)) {
        if(str.empty())continue;
        if (str.at(0) == '#')continue;
        if (str.at(0) == 'm')continue;
        if (str.at(0) == 'v' && str.at(1) == ' '){
            int cur, pre;
            cur = str.find(' ');
            pre = cur + 1;
            string sub;
            int id = 0;
            while(cur != string::npos){
                if(id == 0){
                    id++;
                    pre = cur + 1;
                    cur = str.find(' ', pre);
                    continue;
                }
                sub = str.substr(pre, cur - pre);
                vertices_.push_back(stof(sub));
                pre = cur + 1;
                cur = str.find(' ', pre);
                id++;
            }
            if(id < 4){
                sub = str.substr(pre, cur - pre);
                vertices_.push_back(stof(sub));
            }
        }
        else if (str.at(0) == 'v' && str.at(1) == 't') continue;
        else if (str.at(0) == 'v' && str.at(1) == 'n') continue;
        else if (str.at(0) == 'g')continue;
        else if (str.at(0) == 'u')continue;
        else if (str.at(0) == 'f'){
            int cur, pre;
            cur = str.find(' ');
            pre = cur + 1;
            string sub;
            int id = 0;
            while(cur != string::npos){
                if(id == 0){
                    pre = cur + 1;
                    cur = str.find(' ', pre);
                    id++;
                    continue;
                }
                sub = str.substr(pre, cur - pre);
                int t_cur = sub.find('/');
                sub = sub.substr(0, t_cur);
                faces_.push_back(stoi(sub)-1);
                pre = cur + 1;
                cur = str.find(' ', pre);
                id++;
            }
            if(id < 4){
                sub = str.substr(pre, cur - pre);
                int t_cur = sub.find('/', pre);
                sub = sub.substr(0, t_cur);
                faces_.push_back(stoi(sub)-1);
            }
        }
        else continue;
    }
    fp.close();
    return true;
}

void init() {

    srand(clock());
    program = build_program();

    for (int i = 0; i < num_of_models; ++i) {
        load_obj(vertices[i], faces[i], obj_filepath[i]);
        
        glGenVertexArrays(1, &vao[i]);
        glBindVertexArray(vao[i]);
        glGenBuffers(2, vbo[i]);

        bind_buffer(vbo[i][0], vertices[i], program, "vPosition", 3);
        bind_buffer(vbo[i][1], faces[i], program);

    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glLineWidth(1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

}
void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao[active_vao]);
    
    GLfloat theta = 0.00001f * clock();
    using namespace glm;
    mat4 T(1.0f);

    T = rotate(T, theta, vec3(0.0f, 0.1f, 0.0f));

    GLint location = glGetUniformLocation(program, "T");
    glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(T));
    
    glDrawElements(GL_TRIANGLES, faces[active_vao].size(), GL_UNSIGNED_INT, (void*)0);
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        active_vao = 0;
        glutPostRedisplay();
        break;
    case '2':
        active_vao = 1;
        glutPostRedisplay();
        break;
    case '3':
        active_vao = 2;
        glutPostRedisplay();
        break;
    }

}
int build_program() {
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "/Users/hyorimkim/Documents/SimpleOBJLoader/SimpleOBJLoader/vPosition.vert" },
        {GL_FRAGMENT_SHADER, "/Users/hyorimkim/Documents/SimpleOBJLoader/SimpleOBJLoader/vColor.frag"},
        {GL_NONE, NULL}
    };
    program = LoadShaders(shaders);
    glUseProgram(program);
    return program;

}
void bind_buffer(GLint buffer, vector<GLuint>& vec, int program) {

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * vec.size(), vec.data(), GL_STATIC_DRAW);

}

void bind_buffer(GLint buffer, GLvec& vec, int program, const GLchar* attri_name, GLint attri_size) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vec.size(), vec.data(), GL_STATIC_DRAW);

    GLuint location = glGetAttribLocation(program, attri_name);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(location, attri_size, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(location);

}

