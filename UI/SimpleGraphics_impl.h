/*
 * SimpleGraphics_impl.h
 *
 * Created by kioku on 11/07/06.
 *
 */

// !!! This is included by SimpleGraphics.h !!!
#ifndef INCLUDE_SIMPLEGRAPHICS_H
#error "You must include from SimpleGraphics.h. Do not include this file."
#else

#if defined(_WIN32)
#include <GL/gl.h>
#include <GL/glu.h>
#include "WGLExtention.h"
#elif defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
namespace {
	void InitGLExtention(){}
}
#else // Linux
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
namespace {
	void InitGLExtention(){}
}
#endif

#include <assert.h>

//#define GL_DEBUG

#ifdef GL_DEBUG
#define GLDEBUG	GL_debug_func(this, __LINE__) 
#include <assert.h>
#include <stdio.h>
#else

#define GLDEBUG

#endif

namespace skGUI {
#ifdef GL_DEBUG
	inline void GL_debug_func(skGUI::SimpleGraphics* g, int line)
	{
		skGUI::SGenum e = g->GetError();
		if(e) {
			printf("GL ERROR(0x%X):Line(%d) - %s\n", e, line, gluErrorString(e));
			assert(0);
		}
	}
#endif

inline SimpleGraphics::SimpleGraphics()
{
	InitGLExtention();
}

inline SimpleGraphics::~SimpleGraphics()
{
}



inline void SimpleGraphics::AttachShader (unsigned int program, unsigned int shader){ glAttachShader(program, shader); GLDEBUG; }
inline void SimpleGraphics::BindAttribLocation (unsigned int program, unsigned int index, const char* name){ glBindAttribLocation(program, index, name); GLDEBUG; }
inline void SimpleGraphics::BindBuffer (SGenum target, unsigned int buffer){ glBindBuffer(target, buffer); GLDEBUG;}
inline void SimpleGraphics::BindTexture (SGenum target, unsigned int texture){ glBindTexture(target, texture); GLDEBUG;}
inline void SimpleGraphics::BlendFunc (SGenum sfactor, SGenum dfactor){ glBlendFunc(sfactor, dfactor); GLDEBUG; }
inline void SimpleGraphics::BufferData (SGenum target, int size, const void* data, SGenum usage){ glBufferData(target, size, data, usage); GLDEBUG;}
inline void SimpleGraphics::BufferSubData (SGenum target, int offset, int size, const void* data){ glBufferSubData(target, offset, size, data); GLDEBUG;}
inline void SimpleGraphics::Clear (unsigned int mask){ glClear(mask); GLDEBUG; }
inline void SimpleGraphics::ClearColor (float red, float green, float blue, float alpha){ glClearColor(red, green, blue, alpha); GLDEBUG; }
inline void SimpleGraphics::CompileShader (unsigned int shader){ glCompileShader(shader); GLDEBUG; }
inline unsigned int  SimpleGraphics::CreateProgram (void){ return glCreateProgram(); GLDEBUG;}
inline unsigned int  SimpleGraphics::CreateShader (SGenum type){ return glCreateShader(type); GLDEBUG; }
inline void SimpleGraphics::CullFace (SGenum mode){ glCullFace(mode); GLDEBUG; }
inline void SimpleGraphics::DeleteBuffers (int n, const unsigned int* buffers){ glDeleteBuffers(n, buffers); GLDEBUG; }
inline void SimpleGraphics::DeleteProgram (unsigned int program){ glDeleteProgram(program); GLDEBUG; }
inline void SimpleGraphics::DeleteShader (unsigned int shader){ glDeleteShader(shader); GLDEBUG; }
inline void SimpleGraphics::DeleteTextures (int n, const unsigned int* textures){ glDeleteTextures(n, textures); GLDEBUG; }
//inline void SimpleGraphics::DetachShader (unsigned int program, unsigned int shader){ glDetachShader(program, shader); GLDEBUG; }
inline void SimpleGraphics::Disable (SGenum cap){ glDisable(cap); GLDEBUG; }
inline void SimpleGraphics::DisableVertexAttribArray (unsigned int index){ glDisableVertexAttribArray(index); GLDEBUG; }
inline void SimpleGraphics::DrawArrays (SGenum mode, int first, int count){ glDrawArrays(mode, first, count); GLDEBUG; }
inline void SimpleGraphics::DrawElements (SGenum mode, int count, SGenum type, const void* indices){ glDrawElements(mode, count, type, indices); GLDEBUG; }
inline void SimpleGraphics::Enable (SGenum cap){ glEnable(cap); GLDEBUG; }
inline void SimpleGraphics::EnableVertexAttribArray (unsigned int index){ glEnableVertexAttribArray(index); GLDEBUG; }
inline void SimpleGraphics::GenBuffers (int n, unsigned int* buffers){ glGenBuffers(n, buffers); GLDEBUG; }
inline void SimpleGraphics::GenTextures (int n, unsigned int* textures){ glGenTextures(n, textures); GLDEBUG; }
inline int  SimpleGraphics::GetAttribLocation (unsigned int program, const char* name){ return glGetAttribLocation(program, name); GLDEBUG; }
inline SGenum SimpleGraphics::GetError (void){ return glGetError(); }
inline void SimpleGraphics::GetFloatv (SGenum pname, float* params){ glGetFloatv(pname, params); GLDEBUG; }
inline void SimpleGraphics::GetIntegerv (SGenum pname, int* params){ glGetIntegerv(pname, params); GLDEBUG; }
inline void SimpleGraphics::GetProgramiv (unsigned int program, SGenum pname, int* params){ glGetProgramiv(program, pname, params); GLDEBUG; }
inline void SimpleGraphics::GetProgramInfoLog (unsigned int program, int bufsize, int* length, char* infolog){ glGetProgramInfoLog(program, bufsize, length, infolog); GLDEBUG; }
inline void SimpleGraphics::GetShaderiv (unsigned int shader, SGenum pname, int* params){ glGetShaderiv(shader, pname, params); GLDEBUG; }
inline void SimpleGraphics::GetShaderInfoLog (unsigned int shader, int bufsize, int* length, char* infolog){ glGetShaderInfoLog(shader, bufsize, length, infolog); GLDEBUG; }
inline int  SimpleGraphics::GetUniformLocation (unsigned int program, const char* name){ return glGetUniformLocation(program, name); GLDEBUG; }
inline void SimpleGraphics::LinkProgram (unsigned int program){ glLinkProgram(program); GLDEBUG; }
inline void SimpleGraphics::ShaderSource (unsigned int shader, int count, const char** string, const int* length){ glShaderSource(shader, count, string, length); GLDEBUG; }
inline void SimpleGraphics::TexImage2D (SGenum target, int level, int internalformat, int width, int height, int border, SGenum format, SGenum type, const void* pixels){ glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels); GLDEBUG; }
inline void SimpleGraphics::TexParameterf  (SGenum target, SGenum pname, float param){ glTexParameterf(target, pname, param); GLDEBUG; }
inline void SimpleGraphics::TexParameterfv (SGenum target, SGenum pname, const float* params){ glTexParameterfv(target, pname, params); GLDEBUG; }
inline void SimpleGraphics::TexParameteri  (SGenum target, SGenum pname, int param){ glTexParameteri(target, pname, param); GLDEBUG; }
inline void SimpleGraphics::TexParameteriv (SGenum target, SGenum pname, const int* params){ glTexParameteriv(target, pname, params); GLDEBUG; }
inline void SimpleGraphics::TexSubImage2D  (SGenum target, int level, int xoffset, int yoffset, int width, int height, SGenum format, SGenum type, const void* pixels){ glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels); GLDEBUG; }
inline void SimpleGraphics::Uniform1f  (int location, float x){ glUniform1f(location, x); GLDEBUG; }
inline void SimpleGraphics::Uniform1fv (int location, int count, const float* v){ glUniform1fv(location, count, v); GLDEBUG; }
inline void SimpleGraphics::Uniform1i  (int location, int x){ glUniform1i(location, x); GLDEBUG; }
inline void SimpleGraphics::Uniform1iv (int location, int count, const int* v){ glUniform1iv(location, count, v); GLDEBUG; }
inline void SimpleGraphics::Uniform2f  (int location, float x, float y){ glUniform2f(location, x, y); GLDEBUG; }
inline void SimpleGraphics::Uniform2fv (int location, int count, const float* v){ glUniform2fv(location, count, v); GLDEBUG; }
inline void SimpleGraphics::Uniform2i  (int location, int x, int y){ glUniform2i(location, x, y); GLDEBUG; }
inline void SimpleGraphics::Uniform2iv (int location, int count, const int* v){ glUniform2iv(location, count, v); GLDEBUG; }
inline void SimpleGraphics::Uniform3f  (int location, float x, float y, float z){ glUniform3f(location, x, y, z); GLDEBUG; }
inline void SimpleGraphics::Uniform3fv (int location, int count, const float* v){ glUniform3fv(location, count, v); GLDEBUG; }
inline void SimpleGraphics::Uniform3i  (int location, int x, int y, int z){ glUniform3i(location, x, y, z); GLDEBUG; }
inline void SimpleGraphics::Uniform3iv (int location, int count, const int* v){ glUniform3iv(location, count, v); GLDEBUG; }
inline void SimpleGraphics::Uniform4f  (int location, float x, float y, float z, float w){ glUniform4f(location, x, y, z, w); GLDEBUG; }
inline void SimpleGraphics::Uniform4fv (int location, int count, const float* v){ glUniform4fv(location, count, v); GLDEBUG; }
inline void SimpleGraphics::Uniform4i  (int location, int x, int y, int z, int w){ glUniform4i(location, x, y, z, w); GLDEBUG; }
inline void SimpleGraphics::Uniform4iv (int location, int count, const int* v){ glUniform4iv(location, count, v); GLDEBUG; }
inline void SimpleGraphics::UniformMatrix2fv (int location, int count, unsigned char transpose, const float* value){ glUniformMatrix2fv(location, count, transpose, value); GLDEBUG; }
inline void SimpleGraphics::UniformMatrix3fv (int location, int count, unsigned char transpose, const float* value){ glUniformMatrix3fv(location, count, transpose, value); GLDEBUG; }
inline void SimpleGraphics::UniformMatrix4fv (int location, int count, unsigned char transpose, const float* value){ glUniformMatrix4fv(location, count, transpose, value); GLDEBUG; }
inline void SimpleGraphics::UseProgram (unsigned int program){ glUseProgram(program); GLDEBUG; }
inline void SimpleGraphics::ValidateProgram (unsigned int program){ glValidateProgram(program); GLDEBUG; }
inline void SimpleGraphics::VertexAttribPointer (unsigned int indx, int size, SGenum type, unsigned char normalized, int stride, const void* ptr){ glVertexAttribPointer(indx, size, type, normalized, stride, ptr); GLDEBUG; }
inline void SimpleGraphics::Viewport (int x, int y, int width, int height){ glViewport(x, y, width, height); GLDEBUG; }
	
} // namespace Simple
#endif

