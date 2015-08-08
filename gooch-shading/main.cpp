//
//  main.cpp
//  opengl-os-x
//
//  Created by David Kouřil on 20/07/15.
//  Copyright (c) 2015 dvdkouril. All rights reserved.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cstring>
#include <vector>


GLuint vboId;
GLuint programId;
GLuint vertexShaderId;
GLuint fragmentShaderId;
GLuint vertexbuffer;
GLuint vertexArrayId;
GLuint vao = 0;

GLuint vertexArrayIds[3];

glm::mat4 Projection;
glm::vec4 myVector;

/*const GLchar * vsSource[] = {
    "#version 150\n",
    "uniform mat4 modelViewProjection;\n",
    "void main(void) {\n",
    "   gl_FrontColor = gl_Color;\n",
    "\n",
    "}"
};

const GLchar * vsSourceNew[] = {
    "#version 150\n",
    "in vec3 position;\n",
    "uniform mat4 modelViewProjection;\n",
    "void main(void) {\n",
    "   vec4 v = vec4(position, 1);\n",
    "   gl_Position = modelViewProjection * v;\n"
    "}\n"
};

const GLchar * fsSource[] = {
    "#version 150\n",
    "void main(void) {\n",
    "   gl_FragColor = gl_Color;\n",
    "}"
};*/
const GLchar * vsSource[] = {
    "#version 400\n",
    "in vec3 vp;\n",
    "void main(void){\n",
    "   gl_Position = vec4(vp, 1.0);\n",
    "}"
};

const GLchar * fsSource[] = {
    "#version 400\n",
    "out vec4 frag_color;\n"
    "void main(void){\n",
    "   frag_color = vec4(0.5, 0.0, 0.5, 1.0);\n",
    "}"
};

static const GLfloat vertexData [] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

void render(void) {
    
    glClearColor(0.1f, 0.1f, 0.1f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programId);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    /*//glBindVertexArray(vertexArrayId);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    
    glm::mat4 Projection    = glm::perspective(45.0f, 4.0f / 3.0f, 1.0f, 100.0f);
    glm::mat4 View          = glm::lookAt(glm::vec3(4,3,3),
                                 glm::vec3(0,0,0),
                                 glm::vec3(0,1,0));
    glm::mat4 Model         = glm::mat4(1.0f);
    glm::mat4 MVP           = Projection * View * Model;
    
    GLuint mvpLocation = glGetUniformLocation(programId, "modelViewProjection");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &MVP[0][0]);
    
    glBegin(GL_LINES);
    glVertex3d(-1.0, -1.0, 0.0);
    glVertex3d(1.0, 1.0, 0.0);
    glVertex3d(-1.0, 1.0, 0.0);
    glVertex3d(1.0, -1.0, 0.0);
    glEnd();*/
}

void setupVBOs() {
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
    
    //GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
}

void checkShaderStatus(GLuint shaderId) {
    
    GLint status = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    
    if (status == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);
        
        //std::vector<GLchar> errorLog(maxLength);
        GLchar * errorLog = new GLchar[maxLength];
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, errorLog);
        
        glDeleteShader(shaderId);
        std::cerr << "...failed: " << errorLog << std::endl;
    } else {
        std::cout << "...successful" << std::endl;
    }
    
}

void checkLinkStatus(GLuint program) {
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    
    if (status == GL_FALSE) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        
        GLchar * log = new GLchar[length];
        glGetProgramInfoLog(program, length, &length, log);
        
        std::cerr << "...failed: " << log << std::endl;
        glDeleteProgram(program);
    } else {
        std::cout << "...successful" << std::endl;
    }
}

void setupShaders(GLuint & program, GLuint &  vertexShader, GLuint & fragmentShader) {
    
    // compiling vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, sizeof(vsSource) / sizeof(*vsSource), vsSource, NULL);
    glCompileShader(vertexShader);
    std::cout << "compiling vertex shader" << std::endl;
    checkShaderStatus(vertexShader);
    
    // compiling fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, sizeof(fsSource) / sizeof(*fsSource), fsSource, NULL);
    glCompileShader(fragmentShader);
    std::cout << "compiling fragment shader" << std::endl;
    checkShaderStatus(fragmentShader);
    
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    
    std::cout << "linking program" << std::endl;
    glLinkProgram(program);
    checkLinkStatus(program);
    
    //glUseProgram(0);
    
}

int setup() {
    
    //GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    //glGenVertexArrays(3, vertexArrayIds);
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << err << std::endl;
    }
    
    //GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    
    programId = glCreateProgram();
    setupShaders(programId, vertexShaderId, fragmentShaderId);
    glUseProgram(programId);
    
    return 0;
}

int main(int argc, char * argv[]) {
    
    GLFWwindow *win;
    
    if (!glfwInit()) {
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    
    win = glfwCreateWindow(640, 480, "dvdkouril.", NULL, NULL);
    if(!win) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(win);
    
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "something went wrong: " << glewGetErrorString(err) << std::endl;
    }
    
    
    const GLubyte * version;
    version = glGetString(GL_VERSION);
    std::cout << version << std::endl;
    
    GLenum glerr = glGetError();
    if (glerr != GL_NO_ERROR) {
        std::cout << glerr << std::endl;
    }
    
    setupVBOs();
    setup();
    
    glUseProgram(programId);
    while (!glfwWindowShouldClose(win)) {
        render();
        
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
    
    return 0;
}
