#include <Next.h>

#include <Next/Runtime/RenderSystem/EngineLayer.h>
#include <Next/Editor/EditorLayer.h>
#include <Next/Core/Config.h>

namespace Next {

	class GameEngine : public Application
	{
	public:
		GameEngine(const GameEngineSpecification& engineSpec)
			:Application(engineSpec)
		{
			PushLayer(m_EngineLayer = new EngineLayer());
			PushOverlay(m_EditorLayer = new EditorLayer(*m_EngineLayer));
		}

		~GameEngine()
		{
			PopLayer(m_EngineLayer);
			PopLayer(m_EditorLayer);
		}
	private:
		EngineLayer* m_EngineLayer;
		EditorLayer* m_EditorLayer;
	};
}

Next::Application* Next::CreateApplication()
{
	GameEngineSpecification engineSpec;

	WindowSpecification& windowSpec = engineSpec.windowSpec;
	windowSpec.Width = 1800;
	windowSpec.Height = 1200;
	windowSpec.Title = "Next Engine";

	RendererConfig& rendererSpec = engineSpec.rendererConfig;
	rendererSpec.RendererApi = RendererAPIType::Vulkan;
	rendererSpec.FramesInFlight = 3;

	return new GameEngine(engineSpec);
}