#include "application.hpp"

int main()
{
    Engine::Application* application = new Engine::Application();
    application->Run();
    application->Quit();

    return 0;
}