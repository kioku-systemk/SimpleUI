/*
 *  SimpleShader.h
 *
 *  Created by kioku on 2012/08/20.
 *
 */

#ifndef INCLUDE_SIMPLESHADER_H
#define INCLUDE_SIMPLESHADER_H

#include "SimpleGraphics.h"
#include <string.h>

namespace skGUI {
	
class SimpleShader
{
public:
	SimpleShader(SimpleGraphics* sg, const char* vsshader, const char* fsshader)
	{
		g = sg;
		m_program = 0;
		m_oldProgram = 0;
		m_binding = false;
		const int vshader = createShader(vsshader, SG_VERTEX_SHADER);
		const int fshader = createShader(fsshader, SG_FRAGMENT_SHADER);
		link(vshader, fshader);
		g->DeleteShader(vshader);
		g->DeleteShader(fshader);
	}
	~SimpleShader()
	{
		g->DeleteProgram(m_program);
	}
	
	void Bind()
	{
		g->GetIntegerv(SG_CURRENT_PROGRAM, reinterpret_cast<int*>(&m_oldProgram));
		g->UseProgram(m_program);
		m_binding = true;
	}
	void Unbind()
	{
		g->UseProgram(m_oldProgram);
		m_binding = false;
	}
	void SetUniform(const char* name, const int i0)
	{
		if (m_program && m_binding)
			g->Uniform1i(g->GetUniformLocation(m_program, name), i0);
	}
	
	void SetUniform(const char* name, const float f0)
	{
		if (m_program && m_binding)
			g->Uniform1f(g->GetUniformLocation(m_program, name), f0);
	}
	void SetUniform(const char* name, const float f0, const float f1, const float f2, const float f3)
	{
		if (m_program && m_binding)
			g->Uniform4f(g->GetUniformLocation(m_program, name), f0, f1, f2, f3);
	}

	void SetUniformMatrix4x4(const char* name, const int count, const bool transpose, const float* val)
	{
		if (m_program && m_binding)
			g->UniformMatrix4fv(g->GetUniformLocation(m_program, name), count, transpose, val);
	}

	void BindAttribLocation(unsigned int index, const char* name)
	{
		if (m_program && m_binding)
			g->BindAttribLocation(m_program, index, name);
	}
	
	unsigned int GetAttribLocation(const char* name)
	{
		if (m_program && m_binding)
			return g->GetAttribLocation(m_program, name);
		else
			return -1;
	}
	
	
private:
	void printShaderInfoLog(SimpleGraphics* g, int shader) const
	{
		int bufSize = 0;
		g->GetShaderiv(shader, SG_INFO_LOG_LENGTH , &bufSize);
		if (bufSize > 1) {
			char *infoLog;
			infoLog = new char[bufSize];
			if (infoLog != NULL) {
				int length;
				g->GetShaderInfoLog(shader, bufSize, &length, infoLog);
				printf("InfoLog:\n%s\n",infoLog);
				delete [] infoLog;
			} else {
				printf("Could not allocate InfoLog buffer.");
			}
		}
	}
	
	void printProgramInfoLog(SimpleGraphics* g, GLuint program) const
	{
		int bufSize;
		g->GetProgramiv(program, SG_INFO_LOG_LENGTH , &bufSize);
		if (bufSize > 1) {
			char *infoLog;
			infoLog = new char[bufSize];
			if (infoLog != NULL) {
				int length;
				g->GetProgramInfoLog(program, bufSize, &length, infoLog);
				printf("InfoLog:\n%s\n",infoLog);
				delete [] infoLog;
			} else {
				printf("Could not allocate InfoLog buffer.");
			}
		}
	}
	
	int createShader(const char* programSource, SGenum shaderType) const
	{
		const char* s = programSource;
		const int l = static_cast<int>(strlen(programSource));
		
		int shader = g->CreateShader(shaderType);
		g->ShaderSource( shader, 1, &s, &l );
		if ( g->GetError() != SG_NO_ERROR ) {
			printf("cannot set shader source: %s", s);
			return -1;
		}
		
		// compile
		int compiled = 0;
		g->CompileShader(shader);
		g->GetShaderiv(shader, SG_COMPILE_STATUS, &compiled);
		printShaderInfoLog(g, shader);
		if (!compiled)
		{
			printf("Compile is failed");
			return -1;
		}
		
		return shader;
	}
	
	bool link(const int vertexShader, const int fragmentShader)
	{
		if (!m_program) {
			unsigned int program = g->CreateProgram();
			g->AttachShader(program, vertexShader);
			g->AttachShader(program, fragmentShader);
			m_program = program;
		}
		
		g->LinkProgram(m_program);
		GLint linked;
		g->GetProgramiv(m_program, SG_LINK_STATUS, &linked);
		printProgramInfoLog(g, m_program);
		if (linked == SG_FALSE) {
			printf("Link error.");
			return false;
		}
		return true;
	}
	
	unsigned int m_program, m_oldProgram;
	bool m_binding;
	SimpleGraphics* g;
};
	
} // namespace skGUI

#endif // INCLUDE_SIMPLESHADER_H

