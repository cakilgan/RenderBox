#pragma once
#include <glad.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader {
	GLint PROGRAM_ID
		, VERTEX_ID,FRAGMENT_ID,GEOMETRY_ID;
	void checkErrors(GLuint id) {
		char error[1000];
		glGetShaderInfoLog(id, 1000, NULL, error);
		if (strlen(error) != 0)
		{
			std::cout << "Shader Compile Status: \n" << error << std::endl << std::endl;
		}
	}
	GLuint loadShader(const char* source,GLenum type) {
		GLuint shaderID;	
		shaderID = glCreateShader(type);
		glShaderSource(shaderID,1,&source,NULL);
		glCompileShader(shaderID);
		checkErrors(shaderID);
		return shaderID;
	}
public:
        
		void compile(const char* VERTEX_SOURCE,const char* FRAGMENT_SOURCE) {
			PROGRAM_ID = glCreateProgram();
			
			VERTEX_ID = loadShader(VERTEX_SOURCE,GL_VERTEX_SHADER);
			FRAGMENT_ID = loadShader(FRAGMENT_SOURCE,GL_FRAGMENT_SHADER);
			glAttachShader(PROGRAM_ID,VERTEX_ID);
			glAttachShader(PROGRAM_ID, FRAGMENT_ID);
			glLinkProgram(PROGRAM_ID);
			
			std::cout << "compiled shader program: " << PROGRAM_ID << " with vertex: " << VERTEX_ID << " fragment: " << FRAGMENT_ID<<std::endl;
		}
		void bind() {
			glUseProgram(PROGRAM_ID);
		}
		void unbind() {
			glUseProgram(NULL);
		}
		void destroy() {
			glDetachShader(PROGRAM_ID, VERTEX_ID);
			glDetachShader(PROGRAM_ID, FRAGMENT_ID);
			glDeleteShader(VERTEX_ID);
			glDeleteShader(FRAGMENT_ID);
			glDeleteProgram(PROGRAM_ID);
		}
		void uniform_1f(const char* uniformName,float value) {
			glUniform1f(glGetUniformLocation(PROGRAM_ID,uniformName), value);
		}
		void uniform_2f(const char* uniformName,float value,float value2) {
			glUniform2f(glGetUniformLocation(PROGRAM_ID, uniformName), value,value2);
		}
		void uniform_3f(const char* uniformName,float value,float value2,float value3) {
			glUniform3f(glGetUniformLocation(PROGRAM_ID, uniformName), value, value2,value3);
		}
		void uniform_4f(const char* uniformName,float value,float value2,float value3,float value4) {
			glUniform4f(glGetUniformLocation(PROGRAM_ID, uniformName), value, value2, value3,value4);
		}
		void uniform_1i(const char* uniformName, int value) {
			glUniform1i(glGetUniformLocation(PROGRAM_ID, uniformName), value);
		}
		void uniform_2i(const char* uniformName, int value, int value2) {
			glUniform2i(glGetUniformLocation(PROGRAM_ID, uniformName), value, value2);
		}
		void uniform_3i(const char* uniformName, int value, int value2, int value3) {
			glUniform3i(glGetUniformLocation(PROGRAM_ID, uniformName), value, value2, value3);
		}
		void uniform_4i(const char* uniformName, float value, int value2, int value3, int value4) {
			glUniform4i(glGetUniformLocation(PROGRAM_ID, uniformName), value, value2, value3, value4);
		}
		void uniform_bool(const char* uniformName,bool value) {
			glUniform1i(glGetUniformLocation(PROGRAM_ID,uniformName),value);
		}
		void uniform_mat4(const char* uniformName,glm::mat4 value) {
			glm::f32* res = glm::value_ptr(value);
			glUniformMatrix4fv(glGetUniformLocation(PROGRAM_ID, uniformName),1,GL_FALSE,res);
		}
		void uniform_mat3(const char* uniformName, glm::mat3 value) {
			glm::f32* res = glm::value_ptr(value);
			glUniformMatrix3fv(glGetUniformLocation(PROGRAM_ID, uniformName), 1, GL_FALSE, res);
		}
		void uniform_mat2(const char* uniformName, glm::mat2 value) {
			glm::f32* res = glm::value_ptr(value);
			glUniformMatrix2fv(glGetUniformLocation(PROGRAM_ID, uniformName), 1, GL_FALSE, res);
		}
};