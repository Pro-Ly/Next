#pragma once

#include "Next/Runtime/RenderSystem/SceneRenderer.h"

namespace Next {

	class Scene : public RefCounted
	{
	public:
		Scene();
		~Scene();

		void OnRender(Ref<SceneRenderer> renderer);
	};
}


