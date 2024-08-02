#include "framework.hpp"

using namespace Engine;

Base* EngineBase = new Base();

// Engine::Game
Game::Game(int width, int height, const char* title)
{
    WIDTH = width, RES_WIDTH = width, HEIGHT = height, RES_HEIGHT = height, TITLE = title;
}

void Game::Setup()
{
    EngineBase->CreateWindow(WIDTH, HEIGHT, TITLE);
}

void Game::Run()
{
    EngineBase->InitializeRenderer();

    Init();
    PhysicsEngine->SetWorldBounds(RES_WIDTH, RES_HEIGHT);
    Load();
}

void Game::Loop()
{
    float time = EngineBase->GetTime();
    Timestep timestep = time - m_LastFrameTime;
    m_LastFrameTime = time;

    Input::PollInput();
    Update(timestep);
    if(!IsPaused && !IsGameOver)
    {
        PhysicsEngine->Update();
        PostUpdate(timestep);
    }
    Draw();
}

void Game::Stop()
{
    EngineBase->Close();
}

void Game::Init() {}

void Game::Load() {}

void Game::Update(Timestep ts) {}

void Game::PostUpdate(Timestep ts) {}

void Game::Draw() {}
// end Engine::Game

// Engine::Input
void Input::PollInput()
{
    EngineBase->PollInput();
}

bool Input::IsKeyDown(int key)
{
    return EngineBase->KeyDown(key);
}
// end Engine::Input

// Engine::Renderer
void Renderer::Clear(Color color)
{
    EngineBase->Clear(color.R, color.G, color.B, color.A);
}

void Renderer::Begin()
{
    EngineBase->Begin();
}

void Renderer::Draw(unsigned int sprite, Rectangle source, Rectangle clip, Color color)
{
    EngineBase->Draw(sprite, source, clip, color);
}

void Renderer::End()
{
    EngineBase->End();
}
// end Engine::Renderer

// Engine::Content
Content::Content() {}

unsigned int Content::Load(const std::string& path)
{
    return EngineBase->LoadContent(path);
}
// end Engine::Content