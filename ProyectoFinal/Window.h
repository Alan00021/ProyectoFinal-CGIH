#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevexH() { return muevexH; }	
	bool getplantLight() { return plantLight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool PIniciarNave() { return nave; };				//
	void IniciaNave() { nave = false; }					//
	bool PIniciaVan() { return van; }
	void IniciaVan() { van = true; }
	bool PIniciargemGG() { return gemGG; };				//
	void IniciagemGG() { gemGG = false; }					///
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat muevexH;		
	bool mouseFirstMoved;
	bool plantLight = false;
	bool van = false;
	bool nave = false;
	bool gemGG = false;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

