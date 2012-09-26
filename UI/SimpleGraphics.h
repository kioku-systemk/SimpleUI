/*
 * SimpleGraphics.h
 *
 * coded by kioku / System K
 * 
 */

#ifndef INCLUDE_SIMPLEGRAPHICS_H
#define INCLUDE_SIMPLEGRAPHICS_H

namespace skGUI {

typedef unsigned int SGenum;

class SimpleGraphics
{
public:
	SimpleGraphics();
	~SimpleGraphics();
		
public:
	void AttachShader (unsigned int program, unsigned int shader);
	void BindAttribLocation (unsigned int program, unsigned int index, const char* name);
	void BindBuffer (SGenum target, unsigned int buffer);
	void BindTexture (SGenum target, unsigned int texture);
	void BlendFunc (SGenum sfactor, SGenum dfactor);
	void BufferData (SGenum target, int size, const void* data, SGenum usage);
	void BufferSubData (SGenum target, int offset, int size, const void* data);
	void Clear (unsigned int mask);
	void ClearColor (float red, float green, float blue, float alpha);
	void CompileShader (unsigned int shader);
	unsigned int  CreateProgram (void);
	unsigned int  CreateShader (SGenum type);
	void CullFace (SGenum mode);
	void DeleteBuffers (int n, const unsigned int* buffers);
	void DeleteProgram (unsigned int program);
	void DeleteShader (unsigned int shader);
	void DeleteTextures (int n, const unsigned int* textures);
	void Disable (SGenum cap);
	void DisableVertexAttribArray (unsigned int index);
	void DrawArrays (SGenum mode, int first, int count);
	void DrawElements (SGenum mode, int count, SGenum type, const void* indices);
	void Enable (SGenum cap);
	void EnableVertexAttribArray (unsigned int index);
	void GenBuffers (int n, unsigned int* buffers);
	void GenTextures (int n, unsigned int* textures);
	int  GetAttribLocation (unsigned int program, const char* name);
	SGenum GetError (void);
	void GetFloatv (SGenum pname, float* params);
	void GetIntegerv (SGenum pname, int* params);
	void GetProgramiv (unsigned int program, SGenum pname, int* params);
	void GetProgramInfoLog (unsigned int program, int bufsize, int* length, char* infolog);
	void GetShaderiv (unsigned int shader, SGenum pname, int* params);
	void GetShaderInfoLog (unsigned int shader, int bufsize, int* length, char* infolog);
	int  GetUniformLocation (unsigned int program, const char* name);
	void LinkProgram (unsigned int program);
	void ShaderSource (unsigned int shader, int count, const char** string, const int* length);
	void TexImage2D (SGenum target, int level, int internalformat, int width, int height, int border, SGenum format, SGenum type, const void* pixels);
	void TexParameterf  (SGenum target, SGenum pname, float param);
	void TexParameterfv (SGenum target, SGenum pname, const float* params);
	void TexParameteri  (SGenum target, SGenum pname, int param);
	void TexParameteriv (SGenum target, SGenum pname, const int* params);
	void TexSubImage2D  (SGenum target, int level, int xoffset, int yoffset, int width, int height, SGenum format, SGenum type, const void* pixels);
	void Uniform1f  (int location, float x);
	void Uniform1fv (int location, int count, const float* v);
	void Uniform1i  (int location, int x);
	void Uniform1iv (int location, int count, const int* v);
	void Uniform2f  (int location, float x, float y);
	void Uniform2fv (int location, int count, const float* v);
	void Uniform2i  (int location, int x, int y);
	void Uniform2iv (int location, int count, const int* v);
	void Uniform3f  (int location, float x, float y, float z);
	void Uniform3fv (int location, int count, const float* v);
	void Uniform3i  (int location, int x, int y, int z);
	void Uniform3iv (int location, int count, const int* v);
	void Uniform4f  (int location, float x, float y, float z, float w);
	void Uniform4fv (int location, int count, const float* v);
	void Uniform4i  (int location, int x, int y, int z, int w);
	void Uniform4iv (int location, int count, const int* v);
	void UniformMatrix2fv (int location, int count, unsigned char transpose, const float* value);
	void UniformMatrix3fv (int location, int count, unsigned char transpose, const float* value);
	void UniformMatrix4fv (int location, int count, unsigned char transpose, const float* value);
	void UseProgram (unsigned int program);
	void ValidateProgram (unsigned int program);
	void VertexAttribPointer (unsigned int indx, int size, SGenum type, unsigned char normalized, int stride, const void* ptr);
	void Viewport (int x, int y, int width, int height);

};

} // namespace SG

/* ClearBufferMask */
#define SG_DEPTH_BUFFER_BIT               0x00000100
#define SG_STENCIL_BUFFER_BIT             0x00000400
#define SG_COLOR_BUFFER_BIT               0x00004000

/* Boolean */
#define SG_FALSE                          0
#define SG_TRUE                           1

/* BeginMode */
#define SG_POINTS                         0x0000
#define SG_LINES                          0x0001
#define SG_LINE_LOOP                      0x0002
#define SG_LINE_STRIP                     0x0003
#define SG_TRIANGLES                      0x0004
#define SG_TRIANGLE_STRIP                 0x0005
#define SG_TRIANGLE_FAN                   0x0006

/* AlphaFunction (not supported in ES20) */
/*      SG_NEVER */
/*      SG_LESS */
/*      SG_EQUAL */
/*      SG_LEQUAL */
/*      SG_GREATER */
/*      SG_NOTEQUAL */
/*      SG_GEQUAL */
/*      SG_ALWAYS */

/* BlendingFactorDest */
#define SG_ZERO                           0
#define SG_ONE                            1
#define SG_SRC_COLOR                      0x0300
#define SG_ONE_MINUS_SRC_COLOR            0x0301
#define SG_SRC_ALPHA                      0x0302
#define SG_ONE_MINUS_SRC_ALPHA            0x0303
#define SG_DST_ALPHA                      0x0304
#define SG_ONE_MINUS_DST_ALPHA            0x0305

/* BlendingFactorSrc */
/*      SG_ZERO */
/*      SG_ONE */
#define SG_DST_COLOR                      0x0306
#define SG_ONE_MINUS_DST_COLOR            0x0307
#define SG_SRC_ALPHA_SATURATE             0x0308
/*      SG_SRC_ALPHA */
/*      SG_ONE_MINUS_SRC_ALPHA */
/*      SG_DST_ALPHA */
/*      SG_ONE_MINUS_DST_ALPHA */

/* BlendEquationSeparate */
#define SG_FUNC_ADD                       0x8006
#define SG_BLEND_EQUATION                 0x8009
#define SG_BLEND_EQUATION_RGB             0x8009    /* same as BLEND_EQUATION */
#define SG_BLEND_EQUATION_ALPHA           0x883D

/* BlendSubtract */
#define SG_FUNC_SUBTRACT                  0x800A
#define SG_FUNC_REVERSE_SUBTRACT          0x800B

/* Separate Blend Functions */
#define SG_BLEND_DST_RGB                  0x80C8
#define SG_BLEND_SRC_RGB                  0x80C9
#define SG_BLEND_DST_ALPHA                0x80CA
#define SG_BLEND_SRC_ALPHA                0x80CB
#define SG_CONSTANT_COLOR                 0x8001
#define SG_ONE_MINUS_CONSTANT_COLOR       0x8002
#define SG_CONSTANT_ALPHA                 0x8003
#define SG_ONE_MINUS_CONSTANT_ALPHA       0x8004
#define SG_BLEND_COLOR                    0x8005

/* Buffer Objects */
#define SG_ARRAY_BUFFER                   0x8892
#define SG_ELEMENT_ARRAY_BUFFER           0x8893
#define SG_ARRAY_BUFFER_BINDING           0x8894
#define SG_ELEMENT_ARRAY_BUFFER_BINDING   0x8895

#define SG_STREAM_DRAW                    0x88E0
#define SG_STATIC_DRAW                    0x88E4
#define SG_DYNAMIC_DRAW                   0x88E8

#define SG_BUFFER_SIZE                    0x8764
#define SG_BUFFER_USAGE                   0x8765

#define SG_CURRENT_VERTEX_ATTRIB          0x8626

/* CullFaceMode */
#define SG_FRONT                          0x0404
#define SG_BACK                           0x0405
#define SG_FRONT_AND_BACK                 0x0408

/* PolygonMode */
#define SG_POINT                          0x1B00
#define SG_LINE                           0x1B01
#define SG_FILL                           0x1B02

/* DepthFunction */
/*      SG_NEVER */
/*      SG_LESS */
/*      SG_EQUAL */
/*      SG_LEQUAL */
/*      SG_GREATER */
/*      SG_NOTEQUAL */
/*      SG_GEQUAL */
/*      SG_ALWAYS */

/* EnableCap */
#define SG_TEXTURE_2D                     0x0DE1
#define SG_CULL_FACE                      0x0B44
#define SG_BLEND                          0x0BE2
#define SG_DITHER                         0x0BD0
#define SG_STENCIL_TEST                   0x0B90
#define SG_DEPTH_TEST                     0x0B71
#define SG_SCISSOR_TEST                   0x0C11
#define SG_POLYGON_OFFSET_FILL            0x8037
#define SG_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define SG_SAMPLE_COVERAGE                0x80A0

/* ErrorCode */
#define SG_NO_ERROR                       0
#define SG_INVALID_ENUM                   0x0500
#define SG_INVALID_VALUE                  0x0501
#define SG_INVALID_OPERATION              0x0502
#define SG_OUT_OF_MEMORY                  0x0505

/* FrontFaceDirection */
#define SG_CW                             0x0900
#define SG_CCW                            0x0901

/* GetPName */
#define SG_LINE_WIDTH                     0x0B21
#define SG_ALIASED_POINT_SIZE_RANGE       0x846D
#define SG_ALIASED_LINE_WIDTH_RANGE       0x846E
#define SG_CULL_FACE_MODE                 0x0B45
#define SG_FRONT_FACE                     0x0B46
#define SG_DEPTH_RANGE                    0x0B70
#define SG_DEPTH_WRITEMASK                0x0B72
#define SG_DEPTH_CLEAR_VALUE              0x0B73
#define SG_DEPTH_FUNC                     0x0B74
#define SG_STENCIL_CLEAR_VALUE            0x0B91
#define SG_STENCIL_FUNC                   0x0B92
#define SG_STENCIL_FAIL                   0x0B94
#define SG_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define SG_STENCIL_PASS_DEPTH_PASS        0x0B96
#define SG_STENCIL_REF                    0x0B97
#define SG_STENCIL_VALUE_MASK             0x0B93
#define SG_STENCIL_WRITEMASK              0x0B98
#define SG_STENCIL_BACK_FUNC              0x8800
#define SG_STENCIL_BACK_FAIL              0x8801
#define SG_STENCIL_BACK_PASS_DEPTH_FAIL   0x8802
#define SG_STENCIL_BACK_PASS_DEPTH_PASS   0x8803
#define SG_STENCIL_BACK_REF               0x8CA3
#define SG_STENCIL_BACK_VALUE_MASK        0x8CA4
#define SG_STENCIL_BACK_WRITEMASK         0x8CA5
#define SG_VIEWPORT                       0x0BA2
#define SG_SCISSOR_BOX                    0x0C10
/*      SG_SCISSOR_TEST */
#define SG_COLOR_CLEAR_VALUE              0x0C22
#define SG_COLOR_WRITEMASK                0x0C23
#define SG_UNPACK_ALIGNMENT               0x0CF5
#define SG_PACK_ALIGNMENT                 0x0D05
#define SG_MAX_TEXTURE_SIZE               0x0D33
#define SG_MAX_VIEWPORT_DIMS              0x0D3A
#define SG_SUBPIXEL_BITS                  0x0D50
#define SG_RED_BITS                       0x0D52
#define SG_GREEN_BITS                     0x0D53
#define SG_BLUE_BITS                      0x0D54
#define SG_ALPHA_BITS                     0x0D55
#define SG_DEPTH_BITS                     0x0D56
#define SG_STENCIL_BITS                   0x0D57
#define SG_POLYGON_OFFSET_UNITS           0x2A00
/*      SG_POLYGON_OFFSET_FILL */
#define SG_POLYGON_OFFSET_FACTOR          0x8038
#define SG_TEXTURE_BINDING_2D             0x8069
#define SG_SAMPLE_BUFFERS                 0x80A8
#define SG_SAMPLES                        0x80A9
#define SG_SAMPLE_COVERAGE_VALUE          0x80AA
#define SG_SAMPLE_COVERAGE_INVERT         0x80AB

/* GetTextureParameter */
/*      SG_TEXTURE_MAG_FILTER */
/*      SG_TEXTURE_MIN_FILTER */
/*      SG_TEXTURE_WRAP_S */
/*      SG_TEXTURE_WRAP_T */

#define SG_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define SG_COMPRESSED_TEXTURE_FORMATS     0x86A3

/* HintMode */
#define SG_DONT_CARE                      0x1100
#define SG_FASTEST                        0x1101
#define SG_NICEST                         0x1102

/* HintTarget */
#define SG_GENERATE_MIPMAP_HINT            0x8192

/* DataType */
#define SG_BYTE                           0x1400
#define SG_UNSIGNED_BYTE                  0x1401
#define SG_SHORT                          0x1402
#define SG_UNSIGNED_SHORT                 0x1403
#define SG_INT                            0x1404
#define SG_UNSIGNED_INT                   0x1405
#define SG_FLOAT                          0x1406
#define SG_FIXED                          0x140C

/* PixelFormat */
#define SG_DEPTH_COMPONENT                0x1902
#define SG_ALPHA                          0x1906
#define SG_RGB                            0x1907
#define SG_BGRA                           0x80E1 // will support extension!
#define SG_RGBA                           0x1908
#define SG_LUMINANCE                      0x1909
#define SG_LUMINANCE_ALPHA                0x190A

/* PixelType */
/*      SG_UNSIGNED_BYTE */
#define SG_UNSIGNED_SHORT_4_4_4_4         0x8033
#define SG_UNSIGNED_SHORT_5_5_5_1         0x8034
#define SG_UNSIGNED_SHORT_5_6_5           0x8363

/* Shaders */
#define SG_FRAGMENT_SHADER                0x8B30
#define SG_VERTEX_SHADER                  0x8B31
#define SG_MAX_VERTEX_ATTRIBS             0x8869
#define SG_MAX_VERTEX_UNIFORM_VECTORS     0x8DFB
#define SG_MAX_VARYING_VECTORS            0x8DFC
#define SG_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define SG_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define SG_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define SG_MAX_FRAGMENT_UNIFORM_VECTORS   0x8DFD
#define SG_SHADER_TYPE                    0x8B4F
#define SG_DELETE_STATUS                  0x8B80
#define SG_LINK_STATUS                    0x8B82
#define SG_VALIDATE_STATUS                0x8B83
#define SG_ATTACHED_SHADERS               0x8B85
#define SG_ACTIVE_UNIFORMS                0x8B86
#define SG_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define SG_ACTIVE_ATTRIBUTES              0x8B89
#define SG_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A
#define SG_SHADING_LANGUAGE_VERSION       0x8B8C
#define SG_CURRENT_PROGRAM                0x8B8D

/* StencilFunction */
#define SG_NEVER                          0x0200
#define SG_LESS                           0x0201
#define SG_EQUAL                          0x0202
#define SG_LEQUAL                         0x0203
#define SG_GREATER                        0x0204
#define SG_NOTEQUAL                       0x0205
#define SG_GEQUAL                         0x0206
#define SG_ALWAYS                         0x0207

/* StencilOp */
/*      SG_ZERO */
#define SG_KEEP                           0x1E00
#define SG_REPLACE                        0x1E01
#define SG_INCR                           0x1E02
#define SG_DECR                           0x1E03
#define SG_INVERT                         0x150A
#define SG_INCR_WRAP                      0x8507
#define SG_DECR_WRAP                      0x8508

/* StringName */
#define SG_VENDOR                         0x1F00
#define SG_RENDERER                       0x1F01
#define SG_VERSION                        0x1F02
#define SG_EXTENSIONS                     0x1F03

/* TextureMagFilter */
#define SG_NEAREST                        0x2600
#define SG_LINEAR                         0x2601

/* TextureMinFilter */
/*      SG_NEAREST */
/*      SG_LINEAR */
#define SG_NEAREST_MIPMAP_NEAREST         0x2700
#define SG_LINEAR_MIPMAP_NEAREST          0x2701
#define SG_NEAREST_MIPMAP_LINEAR          0x2702
#define SG_LINEAR_MIPMAP_LINEAR           0x2703

/* TextureParameterName */
#define SG_TEXTURE_MAG_FILTER             0x2800
#define SG_TEXTURE_MIN_FILTER             0x2801
#define SG_TEXTURE_WRAP_S                 0x2802
#define SG_TEXTURE_WRAP_T                 0x2803

/* TextureTarget */
/*      SG_TEXTURE_2D */
#define SG_TEXTURE                        0x1702

#define SG_TEXTURE_CUBE_MAP               0x8513
#define SG_TEXTURE_BINDING_CUBE_MAP       0x8514
#define SG_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define SG_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define SG_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define SG_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define SG_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define SG_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define SG_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C

/* TextureUnit */
#define SG_TEXTURE0                       0x84C0
#define SG_TEXTURE1                       0x84C1
#define SG_TEXTURE2                       0x84C2
#define SG_TEXTURE3                       0x84C3
#define SG_TEXTURE4                       0x84C4
#define SG_TEXTURE5                       0x84C5
#define SG_TEXTURE6                       0x84C6
#define SG_TEXTURE7                       0x84C7
#define SG_TEXTURE8                       0x84C8
#define SG_TEXTURE9                       0x84C9
#define SG_TEXTURE10                      0x84CA
#define SG_TEXTURE11                      0x84CB
#define SG_TEXTURE12                      0x84CC
#define SG_TEXTURE13                      0x84CD
#define SG_TEXTURE14                      0x84CE
#define SG_TEXTURE15                      0x84CF
#define SG_TEXTURE16                      0x84D0
#define SG_TEXTURE17                      0x84D1
#define SG_TEXTURE18                      0x84D2
#define SG_TEXTURE19                      0x84D3
#define SG_TEXTURE20                      0x84D4
#define SG_TEXTURE21                      0x84D5
#define SG_TEXTURE22                      0x84D6
#define SG_TEXTURE23                      0x84D7
#define SG_TEXTURE24                      0x84D8
#define SG_TEXTURE25                      0x84D9
#define SG_TEXTURE26                      0x84DA
#define SG_TEXTURE27                      0x84DB
#define SG_TEXTURE28                      0x84DC
#define SG_TEXTURE29                      0x84DD
#define SG_TEXTURE30                      0x84DE
#define SG_TEXTURE31                      0x84DF
#define SG_ACTIVE_TEXTURE                 0x84E0

/* TextureWrapMode */
#define SG_REPEAT                         0x2901
#define SG_CLAMP_TO_EDGE                  0x812F
#define SG_MIRRORED_REPEAT                0x8370

/* Uniform Types */
#define SG_FLOAT_VEC2                     0x8B50
#define SG_FLOAT_VEC3                     0x8B51
#define SG_FLOAT_VEC4                     0x8B52
#define SG_INT_VEC2                       0x8B53
#define SG_INT_VEC3                       0x8B54
#define SG_INT_VEC4                       0x8B55
#define SG_BOOL                           0x8B56
#define SG_BOOL_VEC2                      0x8B57
#define SG_BOOL_VEC3                      0x8B58
#define SG_BOOL_VEC4                      0x8B59
#define SG_FLOAT_MAT2                     0x8B5A
#define SG_FLOAT_MAT3                     0x8B5B
#define SG_FLOAT_MAT4                     0x8B5C
#define SG_SAMPLER_2D                     0x8B5E
#define SG_SAMPLER_CUBE                   0x8B60

/* Vertex Arrays */
#define SG_VERTEX_ATTRIB_ARRAY_ENABLED    0x8622
#define SG_VERTEX_ATTRIB_ARRAY_SIZE       0x8623
#define SG_VERTEX_ATTRIB_ARRAY_STRIDE     0x8624
#define SG_VERTEX_ATTRIB_ARRAY_TYPE       0x8625
#define SG_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define SG_VERTEX_ATTRIB_ARRAY_POINTER    0x8645
#define SG_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F

/* Read Format */
#define SG_IMPLEMENTATION_COLOR_READ_TYPE   0x8B9A
#define SG_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B

/* Shader Source */
#define SG_COMPILE_STATUS                 0x8B81
#define SG_INFO_LOG_LENGTH                0x8B84
#define SG_SHADER_SOURCE_LENGTH           0x8B88
#define SG_SHADER_COMPILER                0x8DFA

/* Shader Binary */
#define SG_SHADER_BINARY_FORMATS          0x8DF8
#define SG_NUM_SHADER_BINARY_FORMATS      0x8DF9

/* Shader Precision-Specified Types */
#define SG_LOW_FLOAT                  0x8DF0
#define SG_MEDIUM_FLOAT               0x8DF1
#define SG_HIGH_FLOAT                 0x8DF2
#define SG_LOW_INT                    0x8DF3
#define SG_MEDIUM_INT                 0x8DF4
#define SG_HIGH_INT                   0x8DF5

/* Framebuffer Object. */
#define SG_FRAMEBUFFER                                      0x8D40
#define SG_RENDERBUFFER                                     0x8D41

#define SG_RGBA4                                            0x8056
#define SG_RGB5_A1                                          0x8057
#define SG_RGB565                                           0x8D62
#define SG_DEPTH_COMPONENT16                                0x81A5
#define SG_STENCIL_INDEX                                    0x1901
#define SG_STENCIL_INDEX8                                   0x8D48

#define SG_RENDERBUFFER_WIDTH                               0x8D42
#define SG_RENDERBUFFER_HEIGHT                              0x8D43
#define SG_RENDERBUFFER_INTERNAL_FORMAT                     0x8D44
#define SG_RENDERBUFFER_RED_SIZE                            0x8D50
#define SG_RENDERBUFFER_GREEN_SIZE                          0x8D51
#define SG_RENDERBUFFER_BLUE_SIZE                           0x8D52
#define SG_RENDERBUFFER_ALPHA_SIZE                          0x8D53
#define SG_RENDERBUFFER_DEPTH_SIZE                          0x8D54
#define SG_RENDERBUFFER_STENCIL_SIZE                        0x8D55

#define SG_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE               0x8CD0
#define SG_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME               0x8CD1
#define SG_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL             0x8CD2
#define SG_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE     0x8CD3

#define SG_COLOR_ATTACHMENT0                                0x8CE0
#define SG_DEPTH_ATTACHMENT                                 0x8D00
#define SG_STENCIL_ATTACHMENT                               0x8D20

#define SG_NONE                                             0

#define SG_FRAMEBUFFER_COMPLETE                             0x8CD5
#define SG_FRAMEBUFFER_INCOMPLETE_ATTACHMENT                0x8CD6
#define SG_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT        0x8CD7
#define SG_FRAMEBUFFER_INCOMPLETE_DIMENSIONS                0x8CD9
#define SG_FRAMEBUFFER_UNSUPPORTED                          0x8CDD

#define SG_FRAMEBUFFER_BINDING                              0x8CA6
#define SG_RENDERBUFFER_BINDING                             0x8CA7
#define SG_MAX_RENDERBUFFER_SIZE                            0x84E8

#define SG_INVALID_FRAMEBUFFER_OPERATION                    0x0506


#include "SimpleGraphics_impl.h"

#endif // INCLUDE_SIMPLEGRAPHICS_H
