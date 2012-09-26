/*
 *  SimpleVB.h
 *
 *  Created by kioku on 2012/08/20.
 *
 */

#ifndef INCLUDE_SIMPLEVB_H
#define INCLUDE_SIMPLEVB_H

#include <vector>

namespace skGUI {
	
class SimpleVB
{
public:
	SimpleVB(SimpleGraphics* sg, SimpleShader* shader)
	{
		g = sg;
		m_color = 0xFFFFFFFF;
		m_uv[0] = 0.0f;
		m_uv[1] = 0.0f;
		g->GenBuffers(1, &m_vb);
        g->GenBuffers(1, &m_ib);
		shader->Bind();
		m_att_pos = shader->GetAttribLocation("pos");
		m_att_col = shader->GetAttribLocation("col");
		m_att_uv  = shader->GetAttribLocation("uv");
		m_shader = shader;
		m_shader->Unbind();
	}
	~SimpleVB()
	{
		g->DeleteBuffers(1, &m_vb);
        g->DeleteBuffers(1, &m_ib);
	}
	
	class VertexFormat
	{
	public:
		VertexFormat(float x_, float y_, float z_, unsigned int color, float uv[2])
		{
			x = x_; y = y_; z = z_;
			col = color;
			u = uv[0]; v = uv[1];
		}
		float x,y,z;
		unsigned int col;
		float u,v;
	};
	
	void BeginTriangle(){
		
	}
	void End()
	{
		
	}
	void Vertex3f(float x, float y, float z)
	{
		m_buffer.push_back(VertexFormat(x,y,z, m_color, m_uv));
	}
	
	void Color4f(float r, float g, float b, float a)
	{
		unsigned int rc = static_cast<unsigned int>(r * 255.0);
		unsigned int gc = static_cast<unsigned int>(g * 255.0);
		unsigned int bc = static_cast<unsigned int>(b * 255.0);
		unsigned int ac = static_cast<unsigned int>(a * 255.0);
		if (rc > 255) rc = 255;
		if (gc > 255) gc = 255;
		if (bc > 255) bc = 255;
		if (ac > 255) ac = 255;
		m_color = (ac << 24) | (bc << 16) | (gc << 8) | rc;
	}
	void Texcoord2f(float u, float v)
	{
		m_uv[0] = u;
		m_uv[1] = v;
	}

	void Rect2f(float sx, float sy, float ex, float ey, float z)
	{
		BeginTriangle();
        const size_t baseIndex = m_buffer.size();
		Vertex3f(sx,sy,z);
        Vertex3f(ex,sy,z);
		Vertex3f(sx,ey,z);
		Vertex3f(ex,ey,z);
        m_indexbuffer.push_back(baseIndex  );
        m_indexbuffer.push_back(baseIndex+1);
        m_indexbuffer.push_back(baseIndex+2);
        m_indexbuffer.push_back(baseIndex+2);
        m_indexbuffer.push_back(baseIndex+1);
        m_indexbuffer.push_back(baseIndex+3);
		End();
	}
	
	void RectUV2f(float sx, float sy, float ex, float ey, float z, float su, float sv, float eu, float ev)
	{
		BeginTriangle();
		const size_t baseIndex = m_buffer.size();
        Texcoord2f(su, sv); Vertex3f(sx,sy,z);
        Texcoord2f(eu, sv); Vertex3f(ex,sy,z);
        Texcoord2f(su, ev); Vertex3f(sx,ey,z);
        Texcoord2f(eu, ev); Vertex3f(ex,ey,z);
        m_indexbuffer.push_back(baseIndex  );
        m_indexbuffer.push_back(baseIndex+1);
        m_indexbuffer.push_back(baseIndex+2);
        m_indexbuffer.push_back(baseIndex+2);
        m_indexbuffer.push_back(baseIndex+1);
        m_indexbuffer.push_back(baseIndex+3);
		End();
	}
  
    void RectUV9Grid(float sx, float sy, float ex, float ey, float z, float dxy, float su, float sv, float eu, float ev, float du, float dv)
    {
        BeginTriangle();
        const size_t baseIndex = m_buffer.size();
		Texcoord2f(su    , sv       );
		Vertex3f  (sx    , sy    , z);
		Texcoord2f(su+du , sv       );
		Vertex3f  (sx+dxy, sy    , z);
        Texcoord2f(eu-du , sv       );
		Vertex3f  (ex-dxy, sy    , z);
        Texcoord2f(eu    , sv       );
		Vertex3f  (ex    , sy    , z);
        
        Texcoord2f(su    , sv+dv    );
		Vertex3f  (sx    , sy+dxy, z);
		Texcoord2f(su+du , sv+dv    );
		Vertex3f  (sx+dxy, sy+dxy, z);
        Texcoord2f(eu-du , sv+dv    );
		Vertex3f  (ex-dxy, sy+dxy, z);
        Texcoord2f(eu    , sv+dv    );
		Vertex3f  (ex    , sy+dxy, z);

        Texcoord2f(su    , ev-dv    );
		Vertex3f  (sx    , ey-dxy, z);
		Texcoord2f(su+du , ev-dv    );
		Vertex3f  (sx+dxy, ey-dxy, z);
        Texcoord2f(eu-du , ev-dv    );
		Vertex3f  (ex-dxy, ey-dxy, z);
        Texcoord2f(eu    , ev-dv    );
		Vertex3f  (ex    , ey-dxy, z);

        Texcoord2f(su    , ev       );
		Vertex3f  (sx    , ey    , z);
		Texcoord2f(su+du , ev       );
		Vertex3f  (sx+dxy, ey    , z);
        Texcoord2f(eu-du , ev       );
		Vertex3f  (ex-dxy, ey    , z);
        Texcoord2f(eu    , ev       );
		Vertex3f  (ex    , ey    , z);

        for (size_t y = 0; y < 3; ++y) {
            for (size_t x = 0; x < 3; ++x) {
                m_indexbuffer.push_back(baseIndex  +x + 4*y);
                m_indexbuffer.push_back(baseIndex+1+x + 4*y);
                m_indexbuffer.push_back(baseIndex+4+x + 4*y);
                m_indexbuffer.push_back(baseIndex+4+x + 4*y);
                m_indexbuffer.push_back(baseIndex+1+x + 4*y);
                m_indexbuffer.push_back(baseIndex+5+x + 4*y);
            }
        }
        End();

    }

	void Clear()
	{
		m_buffer.clear();
        m_indexbuffer.clear();
	}
	
	void Update()
	{
		g->BindBuffer(SG_ARRAY_BUFFER, m_vb);
		g->BufferData(SG_ARRAY_BUFFER, static_cast<int>(m_buffer.size()) * sizeof(VertexFormat), &m_buffer[0], SG_STATIC_DRAW);
        g->BindBuffer(SG_ELEMENT_ARRAY_BUFFER, m_ib);
		g->BufferData(SG_ELEMENT_ARRAY_BUFFER, static_cast<int>(m_indexbuffer.size()) * sizeof(unsigned short), &m_indexbuffer[0], SG_STATIC_DRAW);
		g->BindBuffer(SG_ARRAY_BUFFER, 0);
        g->BindBuffer(SG_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	void Draw()
	{
		g->BindBuffer(SG_ARRAY_BUFFER, m_vb);
        g->BindBuffer(SG_ELEMENT_ARRAY_BUFFER, m_ib);
		m_shader->Bind();
		g->EnableVertexAttribArray(m_att_pos);
		g->VertexAttribPointer(m_att_pos, 3, SG_FLOAT, SG_FALSE, sizeof(VertexFormat), 0);
		g->EnableVertexAttribArray(m_att_col);
		g->VertexAttribPointer(m_att_col, 4, SG_UNSIGNED_BYTE, SG_TRUE, sizeof(VertexFormat), (const void*)(sizeof(float)*3));
		g->EnableVertexAttribArray(m_att_uv);
		g->VertexAttribPointer(m_att_uv, 2, SG_FLOAT, SG_FALSE, sizeof(VertexFormat), (const void*)(sizeof(float)*4));
		//g->DrawArrays(SG_TRIANGLES, 0, static_cast<int>(m_buffer.size()));
        g->DrawElements(SG_TRIANGLES, static_cast<int>(m_indexbuffer.size()), SG_UNSIGNED_SHORT, 0);
		g->DisableVertexAttribArray(m_att_pos);
		g->DisableVertexAttribArray(m_att_col);
		g->DisableVertexAttribArray(m_att_uv);
		m_shader->Unbind();
        g->BindBuffer(SG_ARRAY_BUFFER, 0);
        g->BindBuffer(SG_ELEMENT_ARRAY_BUFFER, 0);
	}
private:
	SimpleGraphics* g;
	unsigned int m_color;
	float m_uv[2];
	std::vector<VertexFormat>   m_buffer;
    std::vector<unsigned short> m_indexbuffer;
	unsigned int m_vb,m_ib;
	int m_att_pos, m_att_col, m_att_uv;
	SimpleShader* m_shader;
};
	
} // namespace skGUI

#endif // INCLUDE_SIMPLEVB_H

