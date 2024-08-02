#include "application.hpp"

using namespace Engine;

Game1* game;

Application::Application() {}

void Application::Run()
{
    game = new Game1(1280, 720, "Brick\'d!");
    game->Run();
}

void Application::Quit()
{
    game->Stop();
}