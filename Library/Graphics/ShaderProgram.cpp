// Copyright (C) 2013 Felix Ungman
//
// This file is part of the openwar platform (GPL v3 or later), see LICENSE.txt

#ifdef OPENWAR_USE_NSBUNDLE_RESOURCES
#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#endif
#endif

#include "ShaderProgram.h"

//#include <android/log.h>


#ifndef CHECK_ERROR_GL
void CHECK_ERROR_GL()
{
}
#endif



static void print_log(const char* operation, const char* log)
{
//	__android_log_print(ANDROID_LOG_INFO, "OPENWAR", "%s: %s", operation, log);

#ifdef OPENWAR_USE_NSBUNDLE_RESOURCES
	NSLog(@"RENDERER log (%s):\n%s", operation, log);
#endif
}




ShaderProgramBase::ShaderProgramBase(std::vector<const char*> attrs, const char* vertexshader, const char* fragmentshader) :
_blend_sfactor(GL_ONE),
_blend_dfactor(GL_ZERO)
{
	_program = glCreateProgram();
	CHECK_ERROR_GL();

	GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertexshader);
	GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragmentshader);

    glAttachShader(_program, vertex_shader);
	CHECK_ERROR_GL();
    glAttachShader(_program, fragment_shader);
	CHECK_ERROR_GL();

	for (GLuint index = 0; index < attrs.size(); ++index)
	{
		glBindAttribLocation(_program, index, attrs[index]);
		CHECK_ERROR_GL();
	}

    if (!link_program(_program)) {
        if (_program) {
            glDeleteProgram(_program);
	        CHECK_ERROR_GL();
	        _program = 0;
        }
        return;
    }
	validate_program(_program);

	glDetachShader(_program, vertex_shader);
	CHECK_ERROR_GL();
	glDetachShader(_program, fragment_shader);
	CHECK_ERROR_GL();

	glDeleteShader(vertex_shader);
	CHECK_ERROR_GL();
	glDeleteShader(fragment_shader);
	CHECK_ERROR_GL();
}



ShaderProgramBase::~ShaderProgramBase()
{
    if (_program != 0)
    {
        glDeleteProgram(_program);
	    CHECK_ERROR_GL();
    }
}



float ShaderProgramBase::_pixels_per_point = 0;


float ShaderProgramBase::pixels_per_point()
{
	if (_pixels_per_point == 0)
	{
#if TARGET_OS_IPHONE
		if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)])
			_pixels_per_point = [[UIScreen mainScreen] scale];
		else
			_pixels_per_point = 1;
#else
		_pixels_per_point = 1;
#endif
	}

	return _pixels_per_point;
}


void ShaderProgramBase::set_pixels_per_point(float value)
{
	_pixels_per_point = value;
}


GLuint ShaderProgramBase::compile_shader(GLenum type, const char* source)
{
    std::string str(source);
    
    if (str.size() >= 2 && str[0] == '{' && str[str.size() - 1] == '}')
        str = str.substr(1, str.size() - 2);
    
#ifdef OPENWAR_USE_GLES2
    str.insert(0, "precision highp float; precision lowp int; ");
#else
    str.insert(0, "#version 120\n");
#endif

	const GLchar *src = str.c_str();

	GLuint result = glCreateShader(type);
	CHECK_ERROR_GL();
	glShaderSource(result, 1, &src, NULL);
	CHECK_ERROR_GL();
	glCompileShader(result);
	CHECK_ERROR_GL();

	#if 1 //defined(DEBUG)
	GLint logLength;
	glGetShaderiv(result, GL_INFO_LOG_LENGTH, &logLength);
	CHECK_ERROR_GL();
	if (logLength > 0)
	{
		GLchar *log = (GLchar *)malloc((size_t)logLength);
		glGetShaderInfoLog(result, logLength, &logLength, log);
		CHECK_ERROR_GL();
		print_log("compile", log);
		free(log);
	}
	#endif

	//GLint status;
	//glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

	return result;
}



bool ShaderProgramBase::link_program(GLuint program)
{
    GLint status;
    glLinkProgram(program);
	CHECK_ERROR_GL();

#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	CHECK_ERROR_GL();
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc((size_t)logLength);
        glGetProgramInfoLog(program, logLength, &logLength, log);
	    CHECK_ERROR_GL();
		print_log("log", log);
        free(log);
    }
#endif

    glGetProgramiv(program, GL_LINK_STATUS, &status);
	CHECK_ERROR_GL();
    if (status == 0) {
        return false;
    }

    return true;
}


bool ShaderProgramBase::validate_program(GLuint program)
{
	GLint logLength, status;

	glValidateProgram(program);
	CHECK_ERROR_GL();
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	CHECK_ERROR_GL();
	if (logLength > 0)
	{
		GLchar *log = (GLchar *)malloc((size_t)logLength);
		glGetProgramInfoLog(program, logLength, &logLength, log);
		print_log("validate", log);
		free(log);
	}

	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	CHECK_ERROR_GL();
	if (status == 0) {
		return false;
	}

	return true;
}
