#include <Windows.h>
#include <gl/GL.h>
#include "glext.h"
#include "wglext.h"

extern "C" {
	
PFNGLGENBUFFERSARBPROC glGenBuffers = 0;
PFNGLBINDBUFFERARBPROC glBindBuffer = 0;
PFNGLBUFFERDATAARBPROC glBufferData = 0;
PFNGLBUFFERSUBDATAARBPROC glBufferSubData = 0;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffers = 0;
PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameteriv = 0;
PFNGLMAPBUFFERARBPROC glMapBuffer = 0;
PFNGLUNMAPBUFFERARBPROC glUnmapBuffer = 0;
PFNGLISBUFFERARBPROC glIsBuffer = 0;

PFNGLATTACHSHADERPROC glAttachShader = 0;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = 0;
PFNGLCOMPILESHADERPROC glCompileShader = 0;
PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
PFNGLCREATESHADERPROC glCreateShader = 0;
PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
PFNGLDELETESHADERPROC glDeleteShader = 0;
PFNGLDETACHSHADERPROC glDetachShader = 0;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = 0;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = 0;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders = 0;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = 0;
PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0;
PFNGLGETSHADERIVPROC glGetShaderiv = 0;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
PFNGLGETSHADERSOURCEPROC glGetShaderSource = 0;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
PFNGLGETUNIFORMFVPROC glGetUniformfv = 0;
PFNGLGETUNIFORMIVPROC glGetUniformiv = 0;
PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv = 0;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv = 0;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv = 0;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv = 0;
PFNGLISPROGRAMPROC glIsProgram = 0;
PFNGLISSHADERPROC glIsShader = 0;
PFNGLLINKPROGRAMPROC glLinkProgram = 0;
PFNGLSHADERSOURCEPROC glShaderSource = 0;
PFNGLUSEPROGRAMPROC glUseProgram = 0;


PFNGLUNIFORM1IPROC glUniform1i = 0;
PFNGLUNIFORM2IPROC glUniform2i = 0;
PFNGLUNIFORM3IPROC glUniform3i = 0;
PFNGLUNIFORM4IPROC glUniform4i = 0;
PFNGLUNIFORM1IVPROC glUniform1iv = 0;
PFNGLUNIFORM2IVPROC glUniform2iv = 0;
PFNGLUNIFORM3IVPROC glUniform3iv = 0;
PFNGLUNIFORM4IVPROC glUniform4iv = 0;
PFNGLUNIFORM1FPROC glUniform1f = 0;
PFNGLUNIFORM2FPROC glUniform2f = 0;
PFNGLUNIFORM3FPROC glUniform3f = 0;
PFNGLUNIFORM4FPROC glUniform4f = 0;
PFNGLUNIFORM1FVPROC glUniform1fv = 0;
PFNGLUNIFORM2FVPROC glUniform2fv = 0;
PFNGLUNIFORM3FVPROC glUniform3fv = 0;
PFNGLUNIFORM4FVPROC glUniform4fv = 0;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = 0;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = 0;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0;
PFNGLPROGRAMPARAMETERIEXTPROC glProgramParameteri = 0;

PFNGLVALIDATEPROGRAMPROC glValidateProgram = 0;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;


bool InitGLExtension()
{
	glGenBuffers = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
	glBindBuffer = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
	glBufferData = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
	glBufferSubData = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
	glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
	glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
	glMapBuffer = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
	glUnmapBuffer = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");
	glIsBuffer = (PFNGLISBUFFERARBPROC)wglGetProcAddress("glIsBufferARB");

	glGetUniformfv = (PFNGLGETUNIFORMFVPROC)wglGetProcAddress("glGetUniformfv");
	glGetUniformiv = (PFNGLGETUNIFORMIVPROC )wglGetProcAddress("glGetUniformiv");
	glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)wglGetProcAddress("glGetVertexAttribdv");
	glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)wglGetProcAddress("glGetVertexAttribfv");
	glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)wglGetProcAddress("glGetVertexAttribiv");
	glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)wglGetProcAddress("glGetVertexAttribPointerv");
	glIsProgram = (PFNGLISPROGRAMPROC)wglGetProcAddress("glIsProgram");
	glIsShader = (PFNGLISSHADERPROC)wglGetProcAddress("glIsShader");


	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)wglGetProcAddress("glGetAttachedShaders");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	
	// get vertex attribute function pointers
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)wglGetProcAddress("glGetActiveAttrib");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
		
	// uniforms
	glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)wglGetProcAddress("glGetShaderSource");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform2i = (PFNGLUNIFORM2IPROC)wglGetProcAddress("glUniform2i");
	glUniform3i = (PFNGLUNIFORM3IPROC)wglGetProcAddress("glUniform3i");
	glUniform4i = (PFNGLUNIFORM4IPROC)wglGetProcAddress("glUniform4i");
	glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
	glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
	glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress("glUniform2f");
	glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
	glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)wglGetProcAddress("glUniformMatrix2fv");
	glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glProgramParameteri = (PFNGLPROGRAMPARAMETERIEXTPROC)wglGetProcAddress("glProgramParameteri");

	glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)wglGetProcAddress("glValidateProgram");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	

	return true;
}

} // extern "C"