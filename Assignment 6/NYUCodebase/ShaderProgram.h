#pragma once

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL_opengl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Matrix.h"

class ShaderProgram {
    public:
        ShaderProgram(const char *vertexShaderFile, const char *fragmentShaderFile);
        ~ShaderProgram();
    
        void setModelMatrix(const Matrix &matrix);
        void setProjectionMatrix(const Matrix &matrix);
        void setViewMatrix(const Matrix &matrix);
		void setColor(float red, float green, float blue, float alpha = 1.f);
		void setEffectCutoff(float c);
    
        GLuint loadShaderFromString(const std::string &shaderContents, GLenum type);
        GLuint loadShaderFromFile(const std::string &shaderFile, GLenum type);
    
        GLuint programID;
    
        GLuint projectionMatrixUniform;
        GLuint modelMatrixUniform;
        GLuint viewMatrixUniform;
		GLuint colorUniform;
		GLuint effectUniform;
    
        GLuint positionAttribute;
        GLuint texCoordAttribute;
    
        GLuint vertexShader;
        GLuint fragmentShader;
};