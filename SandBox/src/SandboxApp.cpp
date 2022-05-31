#include <Next.h>

#include "imgui\imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class GameLayer : public Next::Layer
{
public:
	GameLayer()
	{
	

	}


private:

};

class Sandbox : public Next::Application
{
public:
	Sandbox()
	{
		PushLayer(new GameLayer());
	}

	~Sandbox()
	{

	}
};

Next::Application* Next::CreateApplication()
{
	return new Sandbox();
}