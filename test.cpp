//
//  test.cpp
//  SimpleUI Test
//
//  Created by kioku on 12/09/14.
//  Copyright (c) 2012 System K. All rights reserved.
//

// Cross Platform Window header
#include "Core/CoreWindow.h"

// Simple GUI header
#include "UI/SimpleGUI.h"

// GL Headers
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <stdio.h>

class MainWindow : public CoreWindow
{
public:
	MainWindow(int x, int y, int width, int height)
	: CoreWindow(x, y, width, height, "Simple UI Test")
	{
        m_col[0] = 0.5;
        m_col[1] = 0.5;
        m_col[2] = 0.5;
        
        m_gui = new SimpleGUI::GUIManager();
        m_gui->Resize(width, height);
        
        SimpleGUI::Dialog* diag = new SimpleGUI::Dialog(m_gui, 50,50,150,130);
        m_gui->GetRoot()->AddChild(diag);
 
        
        SimpleGUI::Caption* txt = new SimpleGUI::Caption(m_gui,2,2, "Dialog", SimpleGUI::UITextSize);
        diag->AddChild(txt);
        
 
        SimpleGUI::Caption* txt1 = new SimpleGUI::Caption(m_gui,5,30, "Red", SimpleGUI::UITextSize);
        diag->AddChild(txt1);
        SimpleGUI::Caption* txt2 = new SimpleGUI::Caption(m_gui,5,50, "Green", SimpleGUI::UITextSize);
        diag->AddChild(txt2);
        SimpleGUI::Caption* txt3 = new SimpleGUI::Caption(m_gui,5,70, "Blue", SimpleGUI::UITextSize);
        diag->AddChild(txt3);

        m_slider[0] = new SimpleGUI::Slider(m_gui,45,32, 100,20,1);
        m_slider[0]->SetValue(m_col[0]);
        m_slider[0]->SetChangedFunc(changeRedSlider,this);
        diag->AddChild(m_slider[0]);
        m_slider[1] = new SimpleGUI::Slider(m_gui,45,52, 100,20,1);
        m_slider[1]->SetValue(m_col[1]);
        m_slider[1]->SetChangedFunc(changeGreenSlider,this);
        diag->AddChild(m_slider[1]);
        m_slider[2] = new SimpleGUI::Slider(m_gui,45,72, 100,20,1);
        m_slider[2]->SetValue(m_col[2]);
        m_slider[2]->SetChangedFunc(changeBlueSlider,this);
        diag->AddChild(m_slider[2]);

        SimpleGUI::Button* btn = new SimpleGUI::Button(m_gui, "Black", 20,100,50,18);
        btn->SetClickedFunc(clickBlack, this);
        diag->AddChild(btn);
        SimpleGUI::Button* btn2 = new SimpleGUI::Button(m_gui, "White", 80,100,50,18);
        btn2->SetClickedFunc(clickWhite, this);
        diag->AddChild(btn2);

	}
	~MainWindow()
    {
    }
	
    static void changeRedSlider(float v, void* thisptr){
        static_cast<MainWindow*>(thisptr)->changeRedSlider(v);
    }
    static void changeGreenSlider(float v, void* thisptr){
        static_cast<MainWindow*>(thisptr)->changeGreenSlider(v);
    }
    static void changeBlueSlider(float v, void* thisptr){
        static_cast<MainWindow*>(thisptr)->changeBlueSlider(v);
    }
    static void clickBlack(void* thisptr){
        static_cast<MainWindow*>(thisptr)->clickBlack();
    }
    static void clickWhite(void* thisptr){
        static_cast<MainWindow*>(thisptr)->clickWhite();
    }
    
    void changeRedSlider  (float v){ m_col[0] = v; }
    void changeGreenSlider(float v){ m_col[1] = v; }
    void changeBlueSlider (float v){ m_col[2] = v; }
    void clickBlack() {
        m_slider[0]->SetValue(0.0f);
        m_slider[1]->SetValue(0.0f);
        m_slider[2]->SetValue(0.0f);
    }
    void clickWhite() {
        m_slider[0]->SetValue(1.0f);
        m_slider[1]->SetValue(1.0f);
        m_slider[2]->SetValue(1.0f);
    }

	void MouseLeftDown(int x, int y)
	{
        m_gui->MouseDown(0,x,y);
	}
	void MouseLeftUp(int x, int y)
	{
        m_gui->MouseUp(0,x,y);
	}
	void MouseMove(int x, int y)
	{
        m_gui->MouseMove(x,y);
	}
	
    void Draw()
    {
        glClearColor(m_col[0],m_col[1],m_col[2],0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // own graphics code.
        
        glClear(GL_DEPTH_BUFFER_BIT);
        m_gui->Draw();
        
        SwapBuffer();
    }
     
	void Resize(int w, int h)
	{
        glViewport(0, 0, w, h);
        m_gui->Resize(w,h);
        Draw();
	}
    void Idle()
    {
        Draw();
    }
    
private:
    SimpleGUI::GUIManager* m_gui;
    SimpleGUI::Slider* m_slider[3];
    int m_mode;
    float m_col[3];
};

int main(int argc, char *argv[])
{
    MainWindow win(32, 32, 720, 480);
    CoreWindow::MainLoop();
    return 0;
}
