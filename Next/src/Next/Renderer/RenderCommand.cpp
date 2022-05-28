#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Next {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}