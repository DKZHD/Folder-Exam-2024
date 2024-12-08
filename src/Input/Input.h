#pragma once
class Camera;

struct GLFWwindow;


namespace Input
{
	void processInput(GLFWwindow* window, Camera& camera);
	class Keyboard
	{
	public:
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		inline static bool pointCloud = false;

	private:
		inline static bool wireframe = false;
	};

	class Mouse
	{
	public:
		static void MouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
		static void MouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset);
		static void MouseClickCallback(GLFWwindow* window, int key, int action, int mods);
		inline static Camera* cameraPtr = nullptr;
		inline static bool firstMouse = true;
	private:
		inline static double prevX = 0, prevY = 0;
		inline static float pitch = 0.f, roll = 0.f, yaw = -90.f;
	};
	
};

