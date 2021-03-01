# CG_install_check

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
- fix
<pre> 
// Fix include code.
#include <OpenGL/gl.h> // in LoadShaders.h
#include <GL/glew.h> // in main.cpp
#include <GLUT/GLUT.h> // in main.cpp
</pre>
<pre>
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
</pre>
<pre>
// ... in display function ...
glFlush();
glutSwapBuffers(); // Add this line.
// ...
</pre>
<pre>
// ... in display function ...
glFlush();
glutSwapBuffers(); // Add this line.
// ...
</pre>
<pre>
ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "{absolute path}.vert"},
        {GL_FRAGMENT_SHADER, "{absolute path}.frag"},
        {GL_NONE, NULL}
};
</pre>
