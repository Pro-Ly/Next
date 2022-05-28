#pragma once
#ifdef NX_PLATFORM_WINDOWS

extern Next::Application* Next::CreateApplication();

int main(int argc,char** argv)
{
	Next::Log::Init();
	NX_CORE_WARN("Initialized Log!");
	int a = 5;
	NX_INFO("Hello! Var={0}", a);

	auto app = Next::CreateApplication();
	app->Run();
	delete app;
}
#endif