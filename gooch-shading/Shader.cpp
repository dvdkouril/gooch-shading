//
//  Shader.cpp
//  gooch-shading
//
//  Created by David Kouřil on 23/08/15.
//  Copyright (c) 2015 dvdkouril. All rights reserved.
//

#include "Shader.h"


/*
 vertexShader = glCreateShader(GL_VERTEX_SHADER);
 glShaderSource(vertexShader, sizeof(vsSource) / sizeof(*vsSource), vsSource, NULL);
 glCompileShader(vertexShader);
 std::cout << "compiling vertex shader" << std::endl;
 checkShaderStatus(vertexShader);
 */

Shader::Shader(GLenum type, char const * path) {
    loadSourceFile(path);
    
    this->shaderId = glCreateShader(type);
    //glShaderSource(shaderId, sizeof(shaderSource) / sizeof(*shaderSource), shaderSource, NULL);
    glShaderSource(shaderId, this->numOfLines, shaderSource, NULL);
    glCompileShader(shaderId);
    std::cout << "compiling shader..." << std::endl;
    // TODO: checkShaderStatus
    this->checkShaderStatus();
    
}

void Shader::checkShaderStatus() {
    
    GLint status = 0;
    glGetShaderiv(this->shaderId, GL_COMPILE_STATUS, &status);
    
    if (status == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(this->shaderId, GL_INFO_LOG_LENGTH, &maxLength);
        
        GLchar * errorLog = new GLchar[maxLength];
        glGetShaderInfoLog(this->shaderId, maxLength, &maxLength, errorLog);
        
        glDeleteShader(this->shaderId);
        std::cerr << "...failed: " << errorLog << std::endl;
    } else {
        std::cout << "...successful" << std::endl;
    }
    
}

void Shader::loadSourceFile(char const * path) {
    std::ifstream infile(path);
    if (!infile) {
        std::cout << "Could not load shader source file " << path << std::endl;
    }
    
    this->shaderSource = new char*[1024];
    
    uint maxLineChar = 1024;
    //char * lineBuf = new char[maxLineChar];
    
    int lineIndex = 0;
    while (infile.peek() != -1) {
        char * lineBuf = new char[maxLineChar];
        infile.getline(lineBuf, maxLineChar);
        size_t lastIndex = strlen(lineBuf);
        lineBuf[lastIndex] = '\n';
        lineBuf[lastIndex + 1] = '\0';
        
        shaderSource[lineIndex] = lineBuf;
        lineIndex += 1;
        //std::cout << lineBuf;
    }
    this->numOfLines = lineIndex;
    
    std::cout << "loading shader successful" << std::endl;
    //delete lineBuf;
}