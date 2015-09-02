//
//  Shader.h
//  gooch-shading
//
//  Created by David Kouřil on 23/08/15.
//  Copyright (c) 2015 dvdkouril. All rights reserved.
//

#ifndef __gooch_shading__Shader__
#define __gooch_shading__Shader__

#include <fstream>
#include <iostream>
#include <GL/glew.h>

class Shader {
    
private:
    char ** shaderSource;
    GLenum shaderType;
    GLuint shaderId;
    int numOfLines;
    
    void checkShaderStatus();
    
public:
    Shader() { shaderSource = NULL; }
    Shader(GLenum type, char const * path);
    
    GLuint getShaderId() { return this->shaderId; }
    
    void loadSourceFile(char const * path);
    
};

#endif /* defined(__gooch_shading__Shader__) */
