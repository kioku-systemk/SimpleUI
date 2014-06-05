/*
 *  SimpleTex.h
 *
 *  Created by kioku on 2012/08/20.
 *
 */

#ifndef INCLUDE_SIMPLETEX_H
#define INCLUDE_SIMPLETEX_H

namespace skGUI {
	
class SimpleTex
{
public:
	enum COLORTYPE {
		COLOR_RGBA8888,
		COLOR_R8
	};

	SimpleTex(SimpleGraphics* sg, int w, int h, COLORTYPE col = COLOR_RGBA8888)
	{
		g = sg;
		m_tex    = -1;
		m_width  = w;
		m_height = h;
		m_color  = col;
		const int colsize = (col == COLOR_RGBA8888 ? 4 : 1);
		m_buffer = new unsigned char[colsize * w * h];
	}
	~SimpleTex()
	{
		delete [] m_buffer;
		g->DeleteTextures(1, &m_tex);
	}
	int GetWidth () const          { return m_width;  }
	int GetHeight() const          { return m_height; }
	COLORTYPE GetColorType() const { return m_color; }
	void Resize(int w, int h)
	{
		delete [] m_buffer;
		const int colsize = (m_color == COLOR_RGBA8888 ? 4 : 1);
		m_buffer = new unsigned char[colsize * w * h];
		m_width  = w;
		m_height = h;
	}
	unsigned char* Map()
	{
		return m_buffer;
	}
	void Unmap()
	{
		if (m_tex == -1){
			g->GenTextures(1, &m_tex);
			g->BindTexture(SG_TEXTURE_2D, m_tex);
			g->TexParameteri(SG_TEXTURE_2D, SG_TEXTURE_MIN_FILTER, SG_NEAREST);// SG_LINEAR);
			g->TexParameteri(SG_TEXTURE_2D, SG_TEXTURE_MAG_FILTER, SG_NEAREST);//SG_LINEAR);
			g->TexParameteri(SG_TEXTURE_2D, SG_TEXTURE_WRAP_S, SG_CLAMP_TO_EDGE);
			g->TexParameteri(SG_TEXTURE_2D, SG_TEXTURE_WRAP_T, SG_CLAMP_TO_EDGE);
		}
		// Update
		g->BindTexture(SG_TEXTURE_2D, m_tex);
		if (m_color == COLOR_RGBA8888)
			g->TexImage2D(SG_TEXTURE_2D, 0, SG_RGBA, m_width, m_height, 0, SG_RGBA, SG_UNSIGNED_BYTE, m_buffer);
		else
			g->TexImage2D(SG_TEXTURE_2D, 0, SG_LUMINANCE, m_width, m_height, 0, SG_LUMINANCE, SG_UNSIGNED_BYTE, m_buffer);
	}
	unsigned int GetID()
	{
		return m_tex;
	}
private:
	COLORTYPE m_color;
	
	SimpleGraphics* g;
	unsigned int m_tex;
	int m_width;
	int m_height;
	unsigned char* m_buffer;
};
	
} // namespace skGUI

#endif // INCLUDE_SIMPLETEX_H

