/*
 *  skBaseGUI.h
 *  skBaseGUI
 *
 *  Created by kioku on 2010/05/04.
 *  Copyright 2010 System K. All rights reserved.
 *
 */

#ifndef INCLUDE_SKBASEGUI_H
#define INCLUDE_SKBASEGUI_H

#include <vector>
#include <algorithm>
#include <assert.h>

namespace skGUI
{
class RefPtr
{
protected:
	RefPtr()           { m_ref = 0; }
	virtual ~RefPtr()  {};
public:
	int Ref()    const { return ++m_ref; }
	int GetRef() const { return m_ref;   }
	int Unref()  const {
		const int ref = --m_ref;
		assert(ref>=0);
		
		if (!ref)
			delete this;
		return ref;
	}
	
private:
	mutable int m_ref;
};

template<class T>
class SharedRefPtr
{
public:
	SharedRefPtr() { m_p = 0; }
	SharedRefPtr(T* s) {
		m_p = s;
		if (m_p)
			m_p->Ref();
	}
	SharedRefPtr(const SharedRefPtr& p)
	{
		m_p = p;
		if (m_p)
			m_p->Ref();
	}
	~SharedRefPtr()
	{
		if (m_p)
			m_p->Unref();
	}
		
	T* Get() const               { return m_p; }
	operator T*() const          { return m_p; }
	T& operator *() const        { assert(m_p); return *m_p; }
	T* operator->() const        { assert(m_p); return m_p;  }
	bool operator !() const      { return m_p == 0; }
	bool operator ==(T* p) const { return m_p == p; }
	bool operator !=(T* p) const { return m_p != p; }
	bool operator <(T* p) const  { return m_p < p;  }
	T* operator =(T* p)
	{
		if (p)
			p->Ref();
		if (m_p)
			m_p->Unref();
		m_p = p;
		return m_p;
	}
private:
	T* m_p;
};


class BaseWindow : public RefPtr
{
public:
	bool IsEnable()            { return m_enable;   }
	void SetEnable(bool enable){ m_enable = enable; }
	bool IsShow()              { return m_show;     }
	void SetShow(bool show)    { m_show = show;     }
	
	size_t GetNumChildren()
	{
		return m_children.size();
	}
	BaseWindow* GetChild(size_t i)
	{
		if (i < GetNumChildren())
			return m_children[i];
		else
			return NULL;
	}
	void AddChild(BaseWindow* win)
	{
		win->setParent(this);
		m_children.push_back(win);
	}
	bool RemoveChild(BaseWindow* win)
	{
		std::vector< SharedRefPtr<BaseWindow> >::iterator it = std::find(m_children.begin(), m_children.end(), win);
		if (it != m_children.end())
		{
			m_children.erase(it);
			return true;
		}
		return false;
	}
	void ClearChild()
	{
		destory();
	}
	
	BaseWindow* GetParent()
	{
		return m_parent;
	}
	
	int GetX()        { return m_x;      }
	int GetY()        { return m_y;      }
	int GetWidth()    { return m_width;  }
	int GetHeight()   { return m_height; }
	int GetDrawOrder(){ return m_order;  }
	float GetScale()  { return m_scale;  }
	void SetPos(int x, int y)   { m_x = x; m_y = y;          }
	void SetSize(int w, int h)  { m_width = w; m_height = h; ownResized(); }
	void SetScale(float s)      { m_scale = s;               }
	void SetDrawOrder(int order){ m_order = order;           }
	int GetAbsoluteX()
	{
		int x = GetX();
		if (m_parent)
			x += m_parent->GetAbsoluteX();
		return x;
	}
	int GetAbsoluteY()
	{
		int y = GetY();
		if (m_parent)
			y += m_parent->GetAbsoluteY();
		return y;
	}

	void Draw(int parent_x = 0, int parent_y = 0)
	{
		if (m_show)
		{
			ownDraw(parent_x, parent_y);
			std::vector< SharedRefPtr<BaseWindow> >::iterator it, eit = m_children.end();
			for (it = m_children.begin(); it != eit; ++it)
			{
				(*it)->Draw(parent_x + m_x, parent_y + m_y);
			}
			ownDrawAfter(parent_x, parent_y);
		}
	}
	void MouseDown(int button, int x, int y)
	{
		if (m_show && m_enable)
		{
			transformScale(x,y);
			ownMouseDown(button, x, y);
            if (isChildEvent(x,y))
            {
                std::vector< SharedRefPtr<BaseWindow> >::iterator it, eit = m_children.end();
                for (it = m_children.begin(); it != eit; ++it)
                {
                    (*it)->MouseDown(button, x - (*it)->GetX(), y - (*it)->GetY());
                }
            }
		}
	}
	void MouseUp(int button, int x, int y)
	{
		if (m_show && m_enable)
		{
			transformScale(x,y);
			ownMouseUp(button, x, y);
			if (isChildEvent(x,y))
            {
				std::vector< SharedRefPtr<BaseWindow> >::iterator it, eit = m_children.end();
				for (it = m_children.begin(); it != eit; ++it)
				{
					(*it)->MouseUp(button,  x - (*it)->GetX(), y - (*it)->GetY());
				}
			}
		}
	}
	void MouseMove(int x, int y)
	{
		if (m_show && m_enable)
		{
			transformScale(x,y);
			ownMouseMove(x, y);
			if (isChildEvent(x,y))
            {
				std::vector< SharedRefPtr<BaseWindow> >::iterator it, eit = m_children.end();
				for (it = m_children.begin(); it != eit; ++it)
				{
					(*it)->MouseMove(x - (*it)->GetX(), y - (*it)->GetY());
				}
			}
		}
	}
	void MouseDropped(BaseWindow* w)
	{
		ownMouseDropped(w);
	};

	void KeyInput (int key)
	{
		if (m_show && m_enable)
		{
			ownKeyInput(key);
			if (isChildEvent(0,0))
            {
				std::vector< SharedRefPtr<BaseWindow> >::iterator it, eit = m_children.end();
				for (it = m_children.begin(); it != eit; ++it)
				{
					(*it)->KeyInput(key);
				}
			}
		}
	}
	
	void KeyDown (int key)
	{
		if (m_show && m_enable)
		{
			ownKeyDown(key);
			if (isChildEvent(0,0))
            {
				std::vector< SharedRefPtr<BaseWindow> >::iterator it, eit = m_children.end();
				for (it = m_children.begin(); it != eit; ++it)
				{
					(*it)->KeyDown(key);
				}
			}
		}
	}
	
	void KeyUp (int key)
	{
		if (m_show && m_enable)
		{
			ownKeyUp(key);
			if (isChildEvent(0,0))
            {
				std::vector< SharedRefPtr<BaseWindow> >::iterator it, eit = m_children.end();
				for (it = m_children.begin(); it != eit; ++it)
				{
					(*it)->KeyUp(key);
				}
			}
		}
	}

	
	BaseWindow* Hit(int x, int y)
	{
		BaseWindow* w = NULL;
		if (m_show)
		{
			transformScale(x, y);
			if (isChildEvent(x,y))
            {
				std::vector< SharedRefPtr<BaseWindow> >::iterator it, eit = m_children.end();
				for (it = m_children.begin(); it != eit; ++it)
				{
					BaseWindow* t =	(*it)->Hit(x - (*it)->GetX(), y - (*it)->GetY());
					if (w == NULL
					|| (t != NULL && w->GetDrawOrder() <= t->GetDrawOrder()))
						w = t;
				}
			}
			if (!w)
				w = ownHit(x, y);
		}
		return w;
	}
	void SetActive(bool active)
	{
		m_active = active;
	}

	virtual ~BaseWindow()
	{
		destory();
	}

	int GetType()
	{
		return m_uiType;
	}
	void SetUserData(void* data)
	{
		m_userData = data;
	}
	void* GetUserData()
	{
		return m_userData;
	}
protected:
	BaseWindow(int uiType)
	{
		m_uiType   = uiType;
		m_enable   = true;
		m_show     = true;
		m_active   = false;
		m_userData = NULL;
		
		m_order  = 0;
		m_x = m_y = m_z = 0;
		m_width  = m_height = 100;
		m_scale = 1.0f;

		m_parent = NULL;
	}
	
	void destory()
	{
		m_children.clear();
	}
	
	void transformScale(int& x, int& y)
	{
		if (m_scale != 1.0f)
		{
			x += static_cast<int>(m_x - m_width  * 0.5f);
			y += static_cast<int>(m_y - m_height * 0.5f);
			x = static_cast<int>(x / m_scale);
			y = static_cast<int>(y / m_scale);
			x -= static_cast<int>(m_x - m_width  * 0.5f);
			y -= static_cast<int>(m_y - m_height * 0.5f);
		}
	}
	
	void setParent(BaseWindow* parent){ m_parent = parent; }
	
	virtual BaseWindow* ownHit(int x, int y)               = 0;
	virtual void ownDraw      (int parent_x, int parent_y) = 0;
	virtual void ownDrawAfter (int parent_x, int parent_y) {};
	virtual void ownResized   () {};
	virtual void ownMouseDown (int button, int x, int y)   = 0;
	virtual void ownMouseUp   (int button, int x, int y)   = 0;
	virtual void ownMouseMove (int x, int y)               = 0;
	virtual void ownKeyInput  (int key){};
	virtual void ownKeyDown   (int key){};
	virtual void ownKeyUp     (int key){};
	
	virtual void ownMouseDropped(BaseWindow* w){};
    virtual bool isChildEvent (int x, int y)   { return true; };

	bool m_enable;
	bool m_show;
	bool m_active;
	int  m_uiType;
	void* m_userData;
	
	int m_order;// high order is over
	int m_x, m_y, m_z;
	int m_width, m_height;
	float m_scale;
	
	BaseWindow* m_parent;
	std::vector< SharedRefPtr<BaseWindow> > m_children;
};
	
class BlankWindow : public BaseWindow
{
public:
	BlankWindow() : BaseWindow(-1)
	{
	}
	~BlankWindow()
	{
	}
	BaseWindow* ownHit(int x, int y){ return NULL; }
	void ownDraw(int parent_x, int parent_y){};
	void ownMouseDown(int button, int x, int y){};
	void ownMouseUp  (int button, int x, int y){};
	void ownMouseMove(int x, int y){};
};
	
class GUIManager
{
protected:
	GUIManager()
	{
		m_root = static_cast<BaseWindow*>(new BlankWindow());
		m_activeWindow = NULL;
	}
	~GUIManager()
	{
	}
	
public:
	void Draw()
	{
		beginUI();
		if (m_root)
		{
			m_root->Draw();
		}
		endUI();
	}
	BaseWindow* Hit(int x, int y)
	{
		if (m_root)
		{
			return m_root->Hit(x, y);
		}
		return NULL;
	}

	BaseWindow* GetRoot()
	{
		return m_root;
	}
	
	void Deactive()
	{
		m_activeWindow = NULL;
	}
	BaseWindow* GetActiveWindow()
	{
		return m_activeWindow;
	}

	void MouseDown(int button, int x, int y)
	{
		if (m_activeWindow)
			m_activeWindow->SetActive(false);		
		m_activeWindow = Hit(x, y);
		if (m_activeWindow)
			m_activeWindow->SetActive(true);
		
		if (m_root)
		{
			m_root->MouseDown(button, x, y);
		}

	}
	void MouseUp(int button, int x, int y)
	{
		if (m_root)
		{
			m_root->MouseUp(button, x, y);
			BaseWindow* w = Hit(x, y);
			if (w && m_activeWindow != w)
			{
				w->MouseDropped(m_activeWindow);
			}
		}
	}
	void MouseMove(int x, int y)
	{
		if (m_root)
		{
			m_root->MouseMove(x, y);
		}
	}
	void KeyInput(int key)
	{
		if (m_root)
			m_root->KeyInput(key);
	}
	void KeyDown(int key)
	{
		if (m_root)
			m_root->KeyDown(key);
	}
	void KeyUp(int key)
	{
		if (m_root)
			m_root->KeyUp(key);
	}

	
	int GetWidth() { return m_width;  }
	int GetHeight(){ return m_height; }
	
	void Resize(int w, int h){ m_width = w; m_height = h; }
protected:
	virtual void beginUI() = 0;
	virtual void endUI() = 0;

	SharedRefPtr<BaseWindow> m_root;
	SharedRefPtr<BaseWindow> m_activeWindow;
	int m_width, m_height;
	
};


}// skGUI

#endif // INCLUDE_SKBASEGUI_H

