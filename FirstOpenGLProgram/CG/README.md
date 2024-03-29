# First OpenGL Program

## terminal
1. Install package
<pre>
brew install GLEW
</pre>
<pre>
brew install glut
</pre>
<pre>
brew install glm
</pre>

## X code
### General
1. Deployment info<br>
  10.14
2. Framework and library<br>
- button [+]
- select 'Add others'
- select 'Add file'
- 'command + shift + G' {path}/glew && {path}/glfw
- ~/lib/*.dylib open
- button [+]
- *GL > Add GLUT.framework && OpenGL.framework

### Build Setting
- Search path > Header Search Path > {path}/glew/{version}/include

### FILE
- fix LoadShaders.h
```
#include <OpenGL/gl.h>
```
- fix main.cpp
```
//order is very important
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <LoadShaders.h>
```
```
// ... in main function ...
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|
				GLUT_DEPTH|GLUT_3_2_CORE_PROFILE); // Add this line.
glutInitWindowSize(512, 512);
glutCreateWindow(argv[0]);


glewExperimental = GL_TRUE; // Add this line.
GLenum err = glewInit();
if (err != GLEW_OK)
{
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    exit(-1);
}
// ...
```
```
// ... in display function ...
glFlush();
glutSwapBuffers(); // Add this line.
// ...
```
- fix vertex shader and fragment shader
```
#version 410 // please set version to 410. 
// This is because macOS graphics driver only supports up to 4.1 version.
```
```
ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "{absolute path}.vert"},
        {GL_FRAGMENT_SHADER, "{absolute path}.frag"},
        {GL_NONE, NULL}
};
```

Reference: https://www.notion.so/MacOS-OpenGL-Setting-Guide-4952565e5db1464b85047ee29b928814


## Result

<p align="center"><img width="514" alt="image" src="https://user-images.githubusercontent.com/28642467/109499871-c87b9900-7ad8-11eb-8978-e1a120e2624a.JPG")</p>
<p align="center">Figure 1. Program Result</p>
