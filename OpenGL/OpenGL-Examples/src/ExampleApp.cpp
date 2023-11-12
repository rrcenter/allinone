#include "GLCore.h"
#include "ExampleLayer.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace GLCore;

class Example : public Application
{
public:
	Example()
		: Application("OpenGL Examples")
	{
		PushLayer(new ExampleLayer());
	}
};

int main()
{
	std::unique_ptr<Example> app = std::make_unique<Example>();
	app->Run();
}