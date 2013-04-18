/*
 *  SimpleGUI.h
 *  SimpleGUI
 *
 *  Created by kioku on 2012/09/14.
 *
 */

#ifndef INCLUDE_SIMGPLE_GUI_H
#define INCLUDE_SIMGPLE_GUI_H

#include "skBaseGUI.h"
#include "SimpleGraphics.h"
#include "SimpleShader.h"
#include "SimpleTex.h"
#include "SimpleVB.h"

#include "icons.h"
#define STRINGIFY(A) #A

// Font data(TrueType font)
#include "mplus1cmedium.h"

// We use STB Truetype font lib
#define STB_TRUETYPE_IMPLEMENTATION 
#include "stb_truetype.h"

namespace SimpleGUI
{
    const float UITextSize = 16;
		
bool TGALoader(const unsigned char* buffer, int& w, int& h, const unsigned char** buf)
{
#ifdef __GNUC__
#pragma pack(push, 1)
#define ALIGNMENT __attribute__((packed))
#else
#pragma pack(1)
#define ALIGNMENT 
#endif // __GNUC__	
	typedef struct
	{
		unsigned char identsize;
		unsigned char colourmaptype;
		unsigned char imagetype;
		short colourmapstart;
		short colourmaplength;
		unsigned char colourmapbits;
		short xstart;
		short ystart;
		short width;
		short height;
		unsigned char bits;
		unsigned char descriptor;
	} TGA_HEADER;
#ifdef __GNUC__
#pragma pack(pop)
#else
#pragma pack() 
#endif // __GNUC__

	const TGA_HEADER* header = reinterpret_cast<const TGA_HEADER*>(buffer);
	w = header->width;
	h = header->height;
	if (header->bits != 32 || header->colourmaptype != 0 || header->imagetype != 2)
		return false;
	*buf = buffer;
	*buf += 18;
	return true;
}
    
class GUIManager : public skGUI::GUIManager
{
public:
	GUIManager() : skGUI::GUIManager()
	{
		static const char* vshader = STRINGIFY(
			uniform mat4 proj;
			attribute vec3 pos;
			attribute vec4 col;
			attribute vec2 uv;
			varying vec4 color;
			varying vec2 tex;
			void main(void) {
				color = col;
				tex   = uv;
				gl_Position = proj * vec4(pos,1);
			}
		);
		static const char* fshader = STRINGIFY(
		uniform sampler2D texture;
		uniform float mono;
	    varying vec4 color;
		varying vec2 tex;
		void main(void) {
			vec4 t = texture2D(texture, tex);
			gl_FragColor = color * ((t.rrrr - t.rgba) * mono + t.rgba);
		}
		);

		g = new skGUI::SimpleGraphics();
		m_defshader = new skGUI::SimpleShader(g, vshader, fshader);
		m_defvb = new skGUI::SimpleVB(g, m_defshader);
		
		m_deftex = 0;
		int tw, th;
		const unsigned char* buf;
		if (TGALoader(icons, tw, th, &buf)) {
			m_deftex = new skGUI::SimpleTex(g, tw, th);
			unsigned char* data = m_deftex->Map();
			// upsidedown
			for (int y = 0; y < th; ++y)
				memcpy(data + tw * (th - y - 1) * 4, buf + tw * y * 4, tw * 4);
			m_deftex->Unmap();
		}
	}
	~GUIManager()
	{
		delete m_deftex;
		delete m_defvb;
		delete m_defshader;
		delete g;
	}
	
    skGUI::SimpleGraphics* GetGraphics() const
	{
		return g;
	}
	
	skGUI::SimpleVB* GetDefaultVB() const
	{
		return m_defvb;
	}
	
	skGUI::SimpleShader* GetDefaultShader() const
	{
		return m_defshader;
	}
	
	const skGUI::SimpleTex* GetTex() const
	{
		return m_deftex;
	}
	
protected:
	void Ortho(float l, float r, float t, float b, float nearval, float farval, float mat[16])
	{
		const float x =  2.0f / (r - l);
		const float y =  2.0f / (t - b);
		const float z =  -2.0f / (farval - nearval);
		const float tx = - (r + l) / (r - l);
		const float ty = - (t + b) / (t - b);
		const float tz = - (farval + nearval) / (farval - nearval);
		for (int i = 0; i < 16; ++i)
			mat[i] = 0;
		mat[ 0] = x;
		mat[ 5] = y;
		mat[10] = z;
		mat[12] = tx;
		mat[13] = ty;
		mat[14] = tz;
		mat[15] = 1.0f;
	}

	void beginUI()
	{
		g->Clear(SG_DEPTH_BUFFER_BIT);
		m_defvb->Clear();
		g->BlendFunc(SG_SRC_ALPHA, SG_ONE_MINUS_SRC_ALPHA);
		g->Enable(SG_BLEND);
		float proj[16];
		Ortho(0.0f, static_cast<float>(m_width), 0.0f, static_cast<float>(m_height), -100.0f, 100.0f, proj);
		m_defshader->Bind();
		m_defshader->SetUniformMatrix4x4("proj", 1, false, proj);
		m_defshader->SetUniform("texture", 0);
		m_defshader->SetUniform("mono",0.0f);
		m_defshader->Unbind();
		g->BindTexture(SG_TEXTURE_2D, m_deftex->GetID());
	}
	void endUI()
	{
		m_defvb->Update();
		m_defvb->Draw();

		g->BlendFunc(SG_ONE, SG_ONE);//_MINUS_SRC_ALPHA);
        m_defshader->Bind();
		m_defshader->SetUniform("mono",1.0f);
		m_defshader->Unbind();
		const int n = static_cast<int>(m_drawsets.size());
		for (int i = 0; i < n; ++i) {
			g->BindTexture(SG_TEXTURE_2D, m_drawsets[i].tex->GetID());
			m_drawsets[i].vb->Draw();
		}
        
		g->Disable(SG_BLEND);
	}
	
	friend class Caption;
	void addDrawSet(skGUI::SimpleVB* vb, skGUI::SimpleTex* tex)
	{
		m_drawsets.push_back(DrawSet(vb, tex));
	}
	void removeDrawSet(skGUI::SimpleVB* vb, skGUI::SimpleTex* tex)
	{
		size_t n = m_drawsets.size();
		for (size_t i = 0; i < n; ++i){
			if( m_drawsets[i].vb == vb && m_drawsets[i].tex == tex) {
				m_drawsets.erase(m_drawsets.begin() + i);
				return;
			}
		}
	}
	
private:
	skGUI::SimpleGraphics* g;
	skGUI::SimpleShader* m_defshader;
	skGUI::SimpleVB* m_defvb;
	skGUI::SimpleTex* m_deftex;
	class DrawSet{
	public:
		DrawSet(skGUI::SimpleVB* vb_, skGUI::SimpleTex* tex_):vb(vb_),tex(tex_){}
		skGUI::SimpleVB* vb;
		skGUI::SimpleTex* tex;
	};
	std::vector<DrawSet> m_drawsets;
	
};
	
enum UITYPE{
	UITYPE_FRAME,
   	UITYPE_DIALOG,
	UITYPE_GRAPHIC,
	UITYPE_BUTTON,
    UITYPE_SLIDER,
	UITYPE_CAPTION,
	UITYPE_CHECK
};
	
class Frame : public skGUI::BaseWindow
{
public:
	Frame(GUIManager* mgr, int x, int y, int w, int h, const float color[4], int z = 0) : skGUI::BaseWindow(UITYPE_FRAME)
	{
		m_defvb = mgr->GetDefaultVB();
		m_x = x; m_y = y; m_width = w; m_height = h;
		m_color[0] = color[0];
		m_color[1] = color[1];
		m_color[2] = color[2];
		m_color[3] = color[3];
		m_z = z;
	}
	~Frame()
	{
	}
	void SetColor(const float color[4])
	{
		m_color[0] = color[0];
		m_color[1] = color[1];
		m_color[2] = color[2];
		m_color[3] = color[3];		
	}
protected:
	skGUI::BaseWindow* ownHit(int x, int y){
		if (x >= 0 && x < m_width
		&&  y >= 0 && y < m_height) return this;
		else return 0;
	}
	void ownDraw      (int parent_x, int parent_y)
	{
		const int sx = parent_x + m_x;
		const int sy = parent_y + m_y;
		m_defvb->Color4f(m_color[0], m_color[1], m_color[2], m_color[3]);
		m_defvb->Texcoord2f(1.0f, 1.0f);
		m_defvb->Rect2f(static_cast<float>(sx), static_cast<float>(sy),
			static_cast<float>(sx + m_width), static_cast<float>(sy + m_height), static_cast<float>(m_z));
	}
	bool ownMouseDown (int button, int x, int y){ return ownHit(x,y) > 0; }
	bool ownMouseUp   (int button, int x, int y){ return ownHit(x,y) > 0; }
	void ownMouseMove (int x, int y){}
private:
	skGUI::SimpleVB* m_defvb;
	float m_color[4];
	int m_z;
};

class Dialog : public skGUI::BaseWindow
{
public:
    Dialog(GUIManager* mgr, int x, int y, int w, int h, int z = 0)
    : skGUI::BaseWindow(UITYPE_DIALOG)
    {
        m_defvb = mgr->GetDefaultVB();
        m_tex_w = static_cast<float>(mgr->GetTex()->GetWidth());
        m_tex_h = static_cast<float>(mgr->GetTex()->GetHeight());
        m_x = x; m_y = y;
        m_tx = 0; m_ty = 0;
        m_width = w; m_height = h;
        m_z = z;
        m_press = false;
		m_alpha = 1.0f;
    }
    ~Dialog()
    {
    }
	
	void SetAlpha(float alpha)
	{
		m_alpha = alpha;
	}
protected:
    BaseWindow* ownHit(int x, int y){
        if (x >= 0 && x < m_width
        &&  y >= 0 && y < m_height)
            return this;
        else
            return 0;
    }
    void ownDraw      (int parent_x, int parent_y)
    {
        const int sx = parent_x + m_x;
        const int sy = parent_y + m_y;
        const int ex = parent_x + m_x + m_width;
        const int ey = parent_y + m_y + m_height;
        const int dxy = 4;
        const float su =       m_tx / m_tex_w;
        const float sv = 1.0f -  m_ty / m_tex_h;
        const float eu =        (m_tx + 12) / m_tex_w;
        const float ev = 1.0f - (m_ty + 12) / m_tex_h;
        const float du =  4.0f / m_tex_w;
        const float dv = -4.0f / m_tex_h;

        m_defvb->Color4f(1.0f,1.0f,1.0f,m_alpha);
        m_defvb->RectUV9Grid(static_cast<const float>(sx), static_cast<const float>(sy),
                             static_cast<const float>(ex), static_cast<const float>(ey), static_cast<const float>(m_z), static_cast<const float>(dxy),
                             su, sv, eu, ev, du,dv);
                             
    }
    bool ownMouseDown (int button, int x, int y)
    {
        if (ownHit(x,y))
        {
            m_press = true;
            m_mousex = x;
            m_mousey = y;
			return true;
        }
		return false;
    }
    bool ownMouseUp   (int button, int x, int y)
    {
		bool r = false;
		if (m_press)
			r = true;
        m_press = false;
		return r;
    }
    void ownMouseMove (int x, int y)
    {
        if (m_press && m_active)
        {
            int dx = x - m_mousex;
            int dy = y - m_mousey;
            this->m_x += dx;
            this->m_y += dy;
        }
    }
    
    skGUI::SimpleVB* m_defvb;
    int m_tx, m_ty;
    int m_z;
    float m_tex_w,m_tex_h;
    bool m_press;
    int m_mousex,m_mousey;
	float m_alpha;
};

class textRasterizer
{
private:
	textRasterizer()
	{
		const unsigned char* fontdata = mplus1cmedium;
		stbtt_InitFont(&font, fontdata, 0);
	}
	~textRasterizer()
	{
	}
public:
	static textRasterizer& GetInstance()
	{
		static textRasterizer singleton;
		return singleton;
	}
	
	void GetTextSize(const char* text, float fontsize, int& w, int& h, int& rw) const
	{
		int ascent,baseline,ch=0;
		float scale, xpos=0;
		scale = stbtt_ScaleForPixelHeight(&font, fontsize);
		stbtt_GetFontVMetrics(&font, &ascent,0,0);
		baseline = (int) (ascent * scale);
	
		while (text[ch]) {
			int chcode = static_cast<int>(*reinterpret_cast<const unsigned char*>(&text[ch]));
			int advance,lsb,x0,y0,x1,y1;
			float x_shift = xpos - (float) floor(xpos);
			stbtt_GetCodepointHMetrics(&font,chcode, &advance, &lsb);
			stbtt_GetCodepointBitmapBoxSubpixel(&font, chcode, scale,scale,x_shift,0, &x0,&y0,&x1,&y1);
			xpos += (advance * scale);
			if (chcode)
				xpos += scale * stbtt_GetCodepointKernAdvance(&font, chcode,chcode);
			ch++;
		}
		int ixpos = static_cast<int>(xpos);
        rw = ixpos;
		w = (((ixpos+3)>>2)<<2); // 4byte align
		h = static_cast<int>(fontsize);
		w = (w < 4 ? 4 : w);
		h = (h < 4 ? 4 : h);
		w += 4; // not just size?
	}
	
	void DrawText(const char* text, float fontsize, unsigned char* buf, int buf_w) const
	{
		int ascent,baseline,ch=0;
		float scale, xpos=0;
		scale = stbtt_ScaleForPixelHeight(&font, fontsize);
		stbtt_GetFontVMetrics(&font, &ascent,0,0);
		baseline = (int) (ascent * scale);
		
		while (text[ch]) {
			int chcode = static_cast<int>(*reinterpret_cast<const unsigned char*>(&text[ch]));
			int advance,lsb,x0,y0,x1,y1;
			float x_shift = xpos - (float) floor(xpos);
			stbtt_GetCodepointHMetrics(&font,chcode, &advance, &lsb);
			stbtt_GetCodepointBitmapBoxSubpixel(&font, chcode, scale,scale,x_shift,0, &x0,&y0,&x1,&y1);
			const float nextstep = scale*stbtt_GetCodepointKernAdvance(&font, chcode,chcode) + (advance * scale);
			if (xpos + nextstep > buf_w)
				break;
			unsigned char* bit = &buf[((baseline + y0) * buf_w + (int)xpos + x0)];
			stbtt_MakeCodepointBitmapSubpixel(&font, bit, x1-x0,y1-y0, buf_w, scale,scale,x_shift,0, chcode);
			xpos += nextstep;
			ch++;
		}
	}

private:
	stbtt_fontinfo font;
};
	
class Caption : public skGUI::BaseWindow
{
public:
	Caption(GUIManager* mgr, int x, int y, const char* text, float size, int z = 0, int maxwsize = 0) : BaseWindow(UITYPE_CAPTION)
	{
        const float col[] = {1.0f,1.0f,1.0f,1.0f};
		m_mgr = mgr;
		m_vb = new skGUI::SimpleVB(mgr->GetGraphics(), mgr->GetDefaultShader());
		m_x = x; m_y = y;
		m_z = z;
		m_size = size;
		m_color[0] = col[0];
		m_color[1] = col[1];
		m_color[2] = col[2];
		m_color[3] = col[3];
		
		textRasterizer& txt = textRasterizer::GetInstance();
		int w,h,rw;
		txt.GetTextSize(text, size, w, h, rw);
        m_realwidth = rw;
		m_width  = w;
		m_maxwsize = maxwsize = (((maxwsize + 3) >> 2) << 2);
		if (maxwsize != 0)
			m_width = (maxwsize < m_width ? maxwsize : m_width);
		m_height = h;
		m_tex = new skGUI::SimpleTex(mgr->GetGraphics(), m_width, m_height, skGUI::SimpleTex::COLOR_R8);
		unsigned char* ptr = m_tex->Map();
		memset(ptr, 0x00, w*h);
		txt.DrawText(text,size,ptr,m_width);
		m_tex->Unmap();
		m_mgr->addDrawSet(m_vb, m_tex);
		//ownDraw(0,0);
	}
	~Caption()
	{
		m_mgr->removeDrawSet(m_vb,m_tex);
		delete m_tex;
		delete m_vb;
	}
	void SetText(const char* text)
	{
		textRasterizer& txt = textRasterizer::GetInstance();
		int w,h,rw;
		txt.GetTextSize(text, static_cast<float>(m_size), w, h,rw);
		m_width = w;
		if (m_maxwsize != 0)
			m_width = (m_maxwsize < m_width ? m_maxwsize : m_width);
		m_height = h;
        m_realwidth = rw;
		m_tex->Resize(m_width,m_height);
		unsigned char* ptr = m_tex->Map();
		memset(ptr, 0x00, w*h);
		txt.DrawText(text,static_cast<float>(m_size),ptr,m_width);
		m_tex->Unmap();
	}
    
    int GetRealWidth() const
    {
        return m_realwidth;
    }
protected:
	BaseWindow* ownHit(int x, int y){
		if (x >= 0 && x < m_width &&  y >= 0 && y < m_height) return this;
		else return 0;
	}
	void ownDraw      (int parent_x, int parent_y)
	{
		const int sx = parent_x + m_x;
		const int sy = parent_y + m_y;
		if (m_cache_x != sx || m_cache_y != sy || m_cache_w != m_width || m_cache_h != m_height) {
			m_vb->Clear();
			m_vb->Color4f(m_color[0],m_color[1],m_color[2],m_color[3]);
			m_vb->RectUV2f(static_cast<float>(sx), static_cast<float>(sy),
						   static_cast<float>(sx + m_width), static_cast<float>(sy + m_height), static_cast<float>(m_z), 0.0f, 0.0f, 1.0f, 1.0f);
			m_vb->Update();
		}
		m_cache_x = sx;
		m_cache_y = sy;
		m_cache_w = m_width;
		m_cache_h = m_height;
	}
	bool ownMouseDown (int button, int x, int y) { return false; }
	bool ownMouseUp   (int button, int x, int y) { return false; }
	void ownMouseMove (int x, int y) {}
private:
	GUIManager* m_mgr;
    skGUI::SimpleGraphics* g;
	skGUI::SimpleVB* m_vb;
	skGUI::SimpleTex* m_tex;
	float m_color[4];
	int m_cache_x, m_cache_y, m_cache_w, m_cache_h;
	float m_size;
	int m_maxwsize;
	int m_z;
    int m_realwidth;
};

    
class Button : public skGUI::BaseWindow
{
public:
    Button(GUIManager* mgr, const char* caption, int x, int y, int w, int h, int z = 0)
    : skGUI::BaseWindow(UITYPE_BUTTON)
    {
        m_defvb = mgr->GetDefaultVB();
        m_tex_w = static_cast<float>(mgr->GetTex()->GetWidth());
        m_tex_h = static_cast<float>(mgr->GetTex()->GetHeight());
        m_x = x; m_y = y;
        m_tx = 14; m_ty = 0;
        m_width = w; m_height = h;
        m_z = z;
        m_over = false;
        m_press = false;
        m_func = 0;
        m_thisptr = 0;
        Caption* txt = new Caption(mgr, 0,0,caption, UITextSize, z+1);
        int txt_x = (w - txt->GetRealWidth()) / 2;
        int txt_y = (h - txt->GetHeight()) / 2;
        txt->SetPos(txt_x, txt_y);
        AddChild(txt);
    }
    ~Button()
    {
    }
    void SetClickedFunc(void (*func)(void*), void* thisptr)
    {
        m_func = func;
        m_thisptr = thisptr;
    }
protected:
    BaseWindow* ownHit(int x, int y){
        if (x >= 0 && x < m_width
            &&  y >= 0 && y < m_height)
            return this;
        else
            return 0;
    }
    void ownDraw      (int parent_x, int parent_y)
    {
        const int sx = parent_x + m_x;
        const int sy = parent_y + m_y;
        const int ex = parent_x + m_x + m_width;
        const int ey = parent_y + m_y + m_height;
        const int dxy = 4;
        const float su = m_tx / m_tex_w;
        const float sv = 1.0f - m_ty / m_tex_h;
        const float eu = (m_tx + 12) / m_tex_w;
        const float ev = 1.0f - (m_ty + 12) / m_tex_h;
        const float du = 4.0f / m_tex_w;
        const float dv = -4.0f / m_tex_h;
        
		if (m_press)
			m_defvb->Color4f(1.0f,0.5f,0.4f,1.0f);
        else if (m_over)
            m_defvb->Color4f(1.0f,0.8f,0.7f,1.0f);
        else
            m_defvb->Color4f(1.0f,1.0f,1.0f,1.0f);

        m_defvb->RectUV9Grid(static_cast<const float>(sx), static_cast<const float>(sy),
                             static_cast<const float>(ex), static_cast<const float>(ey), static_cast<const float>(m_z), static_cast<const float>(dxy),
                             su, sv, eu, ev, du,dv);
        
    }
    bool ownMouseDown (int button, int x, int y)
    {
        if (ownHit(x,y)) {
            m_press = true;
			return true;
		}
		return false;
    }
    bool ownMouseUp   (int button, int x, int y)
    {
		bool r = false;
        if (m_press && ownHit(x,y) && m_func) {
            (*m_func)(m_thisptr);
			r = true;
		}
        m_press = false;
		return r;
    }
    void ownMouseMove (int x, int y)
    {
        if (ownHit(x,y))
            m_over = true;
        else
            m_over = false;
            
    }
    
    skGUI::SimpleVB* m_defvb;
    int m_tx, m_ty;
    int m_z;
    float m_tex_w,m_tex_h;
    bool m_over, m_press;
    void (*m_func)(void*);
    void* m_thisptr;
};

class Slider : public skGUI::BaseWindow
{
public:
    Slider(GUIManager* mgr, int x, int y, int w, int h, int z = 0)
    : skGUI::BaseWindow(UITYPE_SLIDER)
    {
        m_defvb = mgr->GetDefaultVB();
        m_tex_w = static_cast<float>(mgr->GetTex()->GetWidth());
        m_tex_h = static_cast<float>(mgr->GetTex()->GetHeight());
        m_x = x; m_y = y;
        m_tx = 14; m_ty = 0;
        m_width = w; m_height = h;
        m_z = z;
        m_over = false;
        m_press = false;
        m_val = 0.0f;
        m_func = 0;
        m_thisptr = 0;
    }
    ~Slider()
    {
    }
    
    float GetValue() const
    {
        return m_val;
    }
    void SetValue(float val)
    {
        changeVal(val);
    }
    void SetChangedFunc(void (*func)(float, void*), void* thisptr)
    {
        m_func = func;
        m_thisptr = thisptr;
    }
protected:
    BaseWindow* ownHit(int x, int y){
        if (x >= 0 && x < m_width
        &&  y >= 0 && y < 10)
            return this;
        else
            return 0;
    }
    void ownDraw      (int parent_x, int parent_y)
    {
        const int sx = parent_x + m_x;
        const int sy = parent_y + m_y;
        const int ex = parent_x + m_x + m_width;
        //const int ey = parent_y + m_y + m_height;
        
        m_defvb->Texcoord2f(1.0f,1.0f);
        if (m_over)
            m_defvb->Color4f(0.59f,0.33f,0.23f,1.0f);
        else
            m_defvb->Color4f(0.0f,0.0f,0.0f,1.0f);
        m_defvb->Rect2f(static_cast<float>(sx), static_cast<float>(sy),
                        static_cast<float>(ex), static_cast<float>(sy+10),
                        static_cast<float>(m_z));

        m_defvb->Color4f(0.91f,0.31f,0.10f,1.0f);
        m_defvb->Rect2f(static_cast<float>(sx+1), static_cast<float>(sy+1),
                        static_cast<float>(sx+1)+m_val*(ex-sx-1), static_cast<float>(sy+9),
                        static_cast<float>(m_z));
        
        m_defvb->Color4f(0.21f,0.21f,0.21f,1.0f);
        m_defvb->Rect2f(static_cast<float>(sx+1)+m_val*(ex-sx-1), static_cast<float>(sy+1),
                        static_cast<float>(ex-1), static_cast<float>(sy+9),
                        static_cast<float>(m_z));
        
        
    }
    
    void changeVal(float v)
    {
        m_val = v;
        m_val = m_val < 0.0f ? 0.0f : m_val;
        m_val = m_val > 1.0f ? 1.0f : m_val;
        if (m_func)
            (*m_func)(m_val, m_thisptr);
    }
    bool ownMouseDown (int button, int x, int y)
    {
        if (ownHit(x,y)) {
            m_press = true;
            changeVal(x / static_cast<float>(m_width));
			return true;
        }
		return false;
    }
    bool ownMouseUp   (int button, int x, int y)
    {
		bool r = false;
		if (m_press)
			r = true;
        m_press = false;
		return r;
    }
    void ownMouseMove (int x, int y)
    {
        bool r = (ownHit(x,y) != 0);
        m_over = r;

        if (!m_press)
            return;
        
        if (m_active)
        {
            changeVal(x / static_cast<float>(m_width));
        }
    }
    
    skGUI::SimpleVB* m_defvb;
    int m_tx, m_ty;
    int m_z;
    float m_tex_w,m_tex_h;
    
    bool m_over;
    bool m_press;
    float m_val;
    
    void (*m_func)(float, void*);
    void* m_thisptr;
};

class Check : public skGUI::BaseWindow
{
public:
	Check(GUIManager* mgr, const char* caption, int x, int y, int z = 0)
	: skGUI::BaseWindow(UITYPE_CHECK)
	{
		m_defvb = mgr->GetDefaultVB();
		m_tex_w = static_cast<float>(mgr->GetTex()->GetWidth());
		m_tex_h = static_cast<float>(mgr->GetTex()->GetHeight());
		m_x = x; m_y = y;
		m_z = z;
		m_over  = false;
		m_press = false;
		m_state = false;
		m_func = 0;
		m_thisptr = 0;
		Caption* txt = new Caption(mgr, 0,0,caption, UITextSize, z+1);
		m_width  = 16 + txt->GetRealWidth();
		m_height = static_cast<int>(UITextSize);
		int txt_x = 16;
		int txt_y = (16 - txt->GetHeight()) / 2;
		txt->SetPos(txt_x, txt_y);
		AddChild(txt);
	}
	~Check()
	{
	}
	void SetChangedFunc(void (*func)(bool, void*), void* thisptr)
	{
		m_func = func;
		m_thisptr = thisptr;
	}
protected:
	BaseWindow* ownHit(int x, int y){
		if (x >= 0 && x < m_width
			&&  y >= 0 && y < m_height)
			return this;
		else
			return 0;
	}
	void ownDraw      (int parent_x, int parent_y)
	{
		const int sx = parent_x + m_x + 3;
		const int sy = parent_y + m_y + 3;
		const int ex = parent_x + m_x + 15;
		const int ey = parent_y + m_y + 15;
		const int tx = 28;
		int ty = 0;
		if (m_state)
			ty = 12;
		const float su = tx / m_tex_w;
		const float sv = 1.0f - ty / m_tex_h;
		const float eu = (tx + 12) / m_tex_w;
		const float ev = 1.0f - (ty + 12) / m_tex_h;
		
		if (m_over)
			m_defvb->Color4f(1.0f,0.8f,0.7f,1.0f);
		else
			m_defvb->Color4f(1.0f,1.0f,1.0f,1.0f);
		
		m_defvb->RectUV2f(static_cast<const float>(sx), static_cast<const float>(sy),
						static_cast<const float>(ex), static_cast<const float>(ey), static_cast<const float>(m_z),
						su, sv, eu, ev);
		
	}
	bool ownMouseDown (int button, int x, int y)
	{
		if (ownHit(x,y)) {
			m_press = true;
			return true;
		}
		return false;
	}
	bool ownMouseUp   (int button, int x, int y)
	{
		bool r = false;
		if (m_press && ownHit(x,y)){
			m_state = !m_state;
			if (m_func)
				(*m_func)(m_state,m_thisptr);
			r = true;
		}
		m_press = false;
		return r;
	}
	void ownMouseMove (int x, int y)
	{
		if (ownHit(x,y))
			m_over = true;
		else
			m_over = false;
		
	}
	
	void SetState(bool state)
	{
		m_state = state;
		if (m_func)
			(*m_func)(state,m_thisptr);
	}
	
	bool GetState() const
	{
		return m_state;
	}
	
	skGUI::SimpleVB* m_defvb;
	int m_z;
	float m_tex_w,m_tex_h;
	bool m_over, m_press, m_state;
	void (*m_func)(bool,void*);
	void* m_thisptr;
};

}// SimpleGUI

#endif // INCLUDE_SIMGPLE_GUI_H

