//
//  GLFW + SimpleUI Test
//
//  2014.04.16 coded by kioku
//
//  GLFW - http://www.glfw.org/
//  SimpleUI - https://github.com/kioku-systemk/SimpleUI
//
//  base code is accuracy.c / GLFW
//
//

#include <GL/glfw.h>

#include <stdio.h>
#include <stdlib.h>

#include "../UI/SimpleGUI.h"

// UI
static SimpleGUI::GUIManager* m_gui = 0;
static SimpleGUI::Slider* m_slider[3];
static float m_col[3];
static void changeRedSlider  (float v, void* thisptr){ m_col[0] = v; }
static void changeGreenSlider(float v, void* thisptr){ m_col[1] = v; }
static void changeBlueSlider (float v, void* thisptr){ m_col[2] = v; }
static void clickBlack(void* thisptr) {
	m_slider[0]->SetValue(0.0f);
	m_slider[1]->SetValue(0.0f);
	m_slider[2]->SetValue(0.0f);
}
static void clickWhite(void* thisptr) {
	m_slider[0]->SetValue(1.0f);
	m_slider[1]->SetValue(1.0f);
	m_slider[2]->SetValue(1.0f);
}

static int cursor_x = 0, cursor_y = 0;
static int window_width = 640, window_height = 480;

static void GLFWCALL window_size_callback(int width, int height)
{
	window_width = width;
	window_height = height;

	glViewport(0, 0, window_width, window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.f, window_width, 0.f, window_height);

	if (m_gui)
		m_gui->Resize(width,height);
}

static void GLFWCALL mouse_pos_callback(int x, int y)
{
    cursor_x = x;
    cursor_y = y;
	if (m_gui)
		m_gui->MouseMove(cursor_x, cursor_y);
}

static void GLFWCALL mouse_button_callback(int btn, int act)
{
	if (!m_gui)
		return;
	
	if (act == GLFW_PRESS)
		m_gui->MouseDown(btn, cursor_x, cursor_y);
	else if(act == GLFW_RELEASE)
		m_gui->MouseUp(btn, cursor_x, cursor_y);
}


void Render()
{
	glClearColor(m_col[0],m_col[1],m_col[2],0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_LINES);
	glVertex2f(0.f, (GLfloat) window_height - cursor_y);
	glVertex2f((GLfloat) window_width, (GLfloat) window_height - cursor_y);
	glVertex2f((GLfloat) cursor_x, 0.f);
	glVertex2f((GLfloat) cursor_x, (GLfloat) window_height);
	glEnd();
		
	glClear(GL_DEPTH_BUFFER_BIT);
	m_gui->Draw();
	
	glfwSwapBuffers();
}

void CreateUI(int width, int height)
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
	m_slider[0]->SetChangedFunc(changeRedSlider,0);
	diag->AddChild(m_slider[0]);
	m_slider[1] = new SimpleGUI::Slider(m_gui,45,52, 100,20,1);
	m_slider[1]->SetValue(m_col[1]);
	m_slider[1]->SetChangedFunc(changeGreenSlider,0);
	diag->AddChild(m_slider[1]);
	m_slider[2] = new SimpleGUI::Slider(m_gui,45,72, 100,20,1);
	m_slider[2]->SetValue(m_col[2]);
	m_slider[2]->SetChangedFunc(changeBlueSlider,0);
	diag->AddChild(m_slider[2]);
	
	SimpleGUI::Button* btn = new SimpleGUI::Button(m_gui, "Black", 20,100,50,18);
	btn->SetClickedFunc(clickBlack, 0);
	diag->AddChild(btn);
	SimpleGUI::Button* btn2 = new SimpleGUI::Button(m_gui, "White", 80,100,50,18);
	btn2->SetClickedFunc(clickWhite, 0);
	diag->AddChild(btn2);
}

int main(void)
{
    if (!glfwInit())
	{
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    if (!glfwOpenWindow(window_width, window_height, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
    {
        glfwTerminate();

        fprintf(stderr, "Failed to open GLFW window\n");
        exit(EXIT_FAILURE);
    }

	CreateUI(window_width, window_height);

    glfwSetWindowTitle("GLFW + SimpleUI");
    glfwSetMousePosCallback(mouse_pos_callback);
	glfwSetMouseButtonCallback(mouse_button_callback);
    glfwSetWindowSizeCallback(window_size_callback);
    glfwSwapInterval(1);

    while (glfwGetWindowParam(GLFW_OPENED))
    {
		Render();
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

