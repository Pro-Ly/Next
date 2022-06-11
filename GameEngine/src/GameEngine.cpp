#include <Next.h>

#include <Next/Runtime/RenderSystem/SceneLayer.h>
#include <Next/Editor/EditorLayer.h>

class GameEngine : public Next::Application
{
public:
	GameEngine()
	{
		PushLayer(new Next::SceneLayer());
		PushOverlay(new Next::EditorLayer());
	}

	~GameEngine()
	{

	}
};

Next::Application* Next::CreateApplication()
{
	return new GameEngine();
}