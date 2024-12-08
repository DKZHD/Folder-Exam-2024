#pragma once

struct GLFWwindow;

class Window
{
public:
	void init();
	static void ClearBuffers();
	operator GLFWwindow* () const
	{
		return s_window;
	}
	GLFWwindow* s_window = nullptr;

private:
	static void WindowSizeCallback(GLFWwindow* window, int width, int height);

};

