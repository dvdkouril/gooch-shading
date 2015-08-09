//
//  main.cpp
//  opengl-os-x
//
//  Created by David Kouřil on 20/07/15.
//  Copyright (c) 2015 dvdkouril. All rights reserved.
//

// C++ libraries
#include <iostream>
#include <cstring>
#include <vector>
#include <string>

// OpenGL related support libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Obj file loader
#include "tiny_obj_loader.h"


GLuint vboId;
GLuint vboNormalsId;

GLuint programId;
GLuint vertexShaderId;
GLuint fragmentShaderId;
GLuint vertexbuffer;
GLuint vertexArrayId;
GLuint vao = 0;

GLsizeiptr numOfVerticesToRender;

GLuint vertexArrayIds[3];
GLfloat gameTime = 0.0f;

glm::mat4 Projection;
glm::vec4 myVector;

// Vertex Shader source ... TODO: load from file
const GLchar * vsSource[] = {
    "#version 400\n",
    "layout(location = 0) in vec3 vp;\n",
    "layout(location = 1) in vec3 vn;\n",
    //"in vec3 vp;\n",
    "uniform mat4 modelViewProjection;\n",
    "void main(void){\n",
    "   vec4 v = vec4(vp, 1.0);\n",
    //"   gl_Position = vec4(vp, 1.0);\n",
    "   gl_Position = modelViewProjection * v;\n",
    "}"
};

// Fragment Shader source ... TODO: load from file
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

GLfloat * cubeVertexData;
GLfloat * cubeVertexNormalData;

void render(void) {
    
    gameTime += 0.1f;
    glClearColor(0.1f, 0.1f, 0.1f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programId);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, GLsizei(numOfVerticesToRender) );
    
    glm::mat4 Projection    = glm::perspective(45.0f, 4.0f / 3.0f, 1.0f, 100.0f);
    glm::mat4 View          = glm::lookAt(glm::vec3(4,3,3),
                                 glm::vec3(0,0,0),
                                 glm::vec3(0,1,0));
    glm::mat4 rot           = glm::rotate(View, gameTime * 0.1f, glm::vec3(0.0, 1.0, 0.0));
    View = rot;
    glm::mat4 Model         = glm::mat4(1.0f);
    glm::mat4 MVP           = Projection * View * Model;
    
    GLuint mvpLocation = glGetUniformLocation(programId, "modelViewProjection");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &MVP[0][0]);
    
}

void setupVBOs() {
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, numOfVerticesToRender * sizeof(GLfloat), cubeVertexData, GL_STATIC_DRAW);
    
    glGenBuffers(1, &vboNormalsId);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormalsId);
    glBufferData(GL_ARRAY_BUFFER, numOfVerticesToRender * sizeof(GLfloat), cubeVertexNormalData, GL_STATIC_DRAW);
    
    //GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    
    
    
    glBindVertexArray(vao);
    
    glEnableVertexAttribArray(0); // this has to be AFTER glBindVertexArray!!!!!
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormalsId);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
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
    
    glBindAttribLocation(program, 0, "vp");
    glBindAttribLocation(program, 1, "vn");
    
    std::cout << "linking program" << std::endl;
    glLinkProgram(program);
    checkLinkStatus(program);
    
    //glUseProgram(0);
    
}

int setup() {
    
    // Obj file loading
    //std::string pathToFile = "dragon.obj"; // TODO: make so that I don't have to put the file next to the executable
    //std::string pathToFile = "cubeOld.obj";
    std::string pathToFile = "sphere.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string error = tinyobj::LoadObj(shapes, materials, pathToFile.c_str());
    
    std::cout << "loading obj file" << std::endl;
    if (!error.empty()) {
        std::cout << "...error:" << std::endl << error << std::endl;
    } else {
        std::cout << "...successful:" << std:: endl <<
        "# of shapes: " << shapes.size() << std::endl;
    }
    
    /*numOfVerticesToRender = shapes[0].mesh.positions.size();
    cubeVertexData = new GLfloat[numOfVerticesToRender];
    std::cout << "shapes[0].mesh.indices size: " << shapes[0].mesh.indices.size() << std::endl;
    std::cout << "shapes[0].mesh.positions size: " << shapes[0].mesh.positions.size() << std::endl;
    for (size_t f = 0; f < shapes[0].mesh.positions.size(); f++) {
        unsigned int index = shapes[0].mesh.indices[f];
        //cubeVertexData[f] = shapes[0].mesh.positions[index];
        cubeVertexData[f] = shapes[0].mesh.positions[f];
    }*/
    
    for (size_t f = 0; f < shapes[0].mesh.indices.size() / 3; f++) {
        std::cout << "face[" << f << "] = " << shapes[0].mesh.indices[3*f+0] << "," << shapes[0].mesh.indices[3*f+1] << "," << shapes[0].mesh.indices[3*f+2] << "." << std::endl;
        std::cout << shapes[0].mesh.positions[shapes[0].mesh.indices[3*f+0]] << " " << shapes[0].mesh.positions[shapes[0].mesh.indices[3*f+1]] << " " << shapes[0].mesh.positions[shapes[0].mesh.indices[3*f+2]] << std::endl;
    }
    
    numOfVerticesToRender = shapes[0].mesh.indices.size() * 3;
    cubeVertexData = new GLfloat[numOfVerticesToRender];
    size_t dataCurrentIndex = 0;
    for (size_t f = 0; f < shapes[0].mesh.indices.size() / 3; f++) {
        for (size_t i = 0; i < 3; i++) {
            size_t vertexIndex = shapes[0].mesh.indices[3*f+i];
            for (size_t c = 0; c < 3; c++) {
                GLfloat coordinate = shapes[0].mesh.positions[3*vertexIndex+c];
                cubeVertexData[dataCurrentIndex] = coordinate;
                dataCurrentIndex++;
            }
            
            //GLfloat x = shapes[0].mesh.positions[3*vertexIndex+0];
            //GLfloat y = shapes[0].mesh.positions[3*vertexIndex+1];
            //GLfloat z = shapes[0].mesh.positions[3*vertexIndex+2];
            
            
        }
        //size_t aIndex = shapes[0].mesh.indices[3*f+0];
        //size_t bIndex = shapes[0].mesh.indices[3*f+1];
        //size_t cIndex = shapes[0].mesh.indices[3*f+2];
        
    }
    
    cubeVertexNormalData = new GLfloat[numOfVerticesToRender];
    dataCurrentIndex = 0;
    for (size_t f = 0; f < shapes[0].mesh.indices.size() / 3; f++) {
        for (size_t i = 0; i < 3; i++) {
            size_t vertexIndex = shapes[0].mesh.indices[3*f+i];
            for (size_t c = 0; c < 3; c++) {
                GLfloat coordinate = shapes[0].mesh.normals[3*vertexIndex+c];
                cubeVertexNormalData[dataCurrentIndex] = coordinate;
                dataCurrentIndex++;
            }
        }
    }
    
    /*for (size_t v = 0; v < shapes[0].mesh.positions.size() / 3; v++) {
        std::cout << shapes[0].mesh.positions[3*v+0] << "," << shapes[0].mesh.positions[3*v+1] << "," << shapes[0].mesh.positions[3*v+2] << "." << std::endl;
    }*/
    
    /*for (int i = 0; i < numOfVerticesToRender; i++) {
        std::cout << cubeVertexData[i] << std::endl;
    }*/
    
    // Compilation of shaders
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
    
    setup();
    setupVBOs();
    
    
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
