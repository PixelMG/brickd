#include "game.hpp"
// #include <cassert>
#include <map>

using namespace Engine;

unsigned int sprite;
PhysicsBody* PaddleCollider;
Rectangle Numbers[10] = {
    { 37, 57, 3, 7 },
    { 1, 57, 3, 7 },
    { 5, 57, 3, 7 },
    { 9, 57, 3, 7 },
    { 13, 57, 3, 7 },
    { 17, 57, 3, 7 },
    { 21, 57, 3, 7 },
    { 25, 57, 3, 7 },
    { 29, 57, 3, 7 },
    { 33, 57, 3, 7 }
};
std::map<char, Rectangle> Letters {
    { 'a', { 1, 65, 3, 7 } },
    { 'b', { 5, 65, 3, 7 } },
    { 'c', { 9, 65, 3, 7 } },
    { 'd', { 13, 65, 3, 7 } },
    { 'e', { 17, 65, 3, 7 } },
    { 'f', { 21, 65, 3, 7 } },
    { 'g', { 25, 65, 3, 7 } },
    { 'h', { 29, 65, 3, 7 } },
    { 'i', { 33, 65, 3, 7 } },
    { 'j', { 37, 65, 3, 7 } },
    { 'k', { 1, 73, 3, 7 } },
    { 'l', { 5, 73, 3, 7 } },
    { 'm', { 9, 73, 3, 7 } },
    { 'n', { 13, 73, 3, 7 } },
    { 'o', { 17, 73, 3, 7 } },
    { 'p', { 21, 73, 3, 7 } },
    { 'q', { 25, 73, 3, 7 } },
    { 'r', { 29, 73, 3, 7 } },
    { 's', { 33, 73, 3, 7 } },
    { 't', { 37, 73, 3, 7 } },
    { 'u', { 1, 81, 3, 7 } },
    { 'v', { 5, 81, 3, 7 } },
    { 'w', { 9, 81, 3, 7 } },
    { 'x', { 13, 81, 3, 7 } },
    { 'y', { 17, 81, 3, 7 } },
    { 'z', { 21, 81, 3, 7 } }
};
int ScoreArray[] = { 0, 0 };

int FlashCounter = 30;
float fA = 1.0f; // flash alpha

void Game1::Init()
{
    SetResolution(640, 360);

    SetupLevel();

    IsGameOver = true;
}

void Game1::Load()
{
    sprite = ContentManager->Load("src/assets/sprites/objects.png");

    LoadSprites(sprite);
}

void Game1::Update(Timestep ts)
{
    if(Debounce > 0) Debounce--;

    if(IsGameOver || !GameStarted)
    {
        if(FlashCounter > 0) FlashCounter--;
        else
        {
            FlashCounter = 30;
            fA = fA == 1.0f ? 0.0f : 1.0f;
        }

        if(Input::IsKeyDown(KEYS_SPACE) && Debounce == 0)
        {
            Debounce = DebounceSet;
            DestroyLevel();
            SetupLevel();
            LoadSprites(sprite);
            Score = 0;
            Chances = 3;
            ScoreArray[0] = 0, ScoreArray[1] = 0;
            IsGameOver = false;
            GameStarted = true;
            Update(ts);
        }
        return;
    }

    if(Input::IsKeyDown(KEYS_SPACE) && Debounce == 0)
    {
        IsPaused = IsPaused == true ? false : true;
        Debounce = DebounceSet;
    }

    if(IsPaused) return;

    ObjectList->Update(ts);

    for(int i = 0; i < PhysicsEngine->PhysicsBodyCount(); i++)
    {
        auto b = PhysicsEngine->GetBody(i);
        auto o = ObjectList->GetGameObject(b->GetObjectID());

        b->SetObjectPosition({ o->GetX(), o->GetY() });
    }
}

void Game1::PostUpdate(Timestep ts)
{
    for(int i = 0; i < PhysicsEngine->PhysicsBodyCount(); i++)
    {
        auto b = PhysicsEngine->GetBody(i);

        if(b->IsOutofBounds())
        {
            auto o = ObjectList->GetGameObject(b->GetObjectID());
            if(b->GetObjectID() == 73 && o->GetY() + o->GetHeight() > PhysicsEngine->GetWorldHeight())
            {
                Chances--;
                if(Chances == 0) IsGameOver = true;
            }
            o->HandleBoundsCollision(PhysicsEngine->GetWorldWidth(), PhysicsEngine->GetWorldHeight());
        }

        if(b->IsOverlapping())
        {
            auto o1 = ObjectList->GetGameObject(b->GetObjectID());
            auto b2 = PhysicsEngine->GetBody(b->IsOverlappingWith());
            auto o2 = ObjectList->GetGameObject(b2->GetObjectID());

            o1->HandleCollision(o1, b->GetCenter(), o2, b2->GetCenter(), ts);
            o2->HandleCollision(o2, b2->GetCenter(), o1, b->GetCenter(), ts);

            b->SetOverlappingFlag(false);
            b2->SetOverlappingFlag(false);

            if(!o1->IsAlive()) { b->Disable(); Score++; }
            if(!o2->IsAlive()) { b2->Disable(); Score++; }

            ScoreArray[0] = Score / 10;
            ScoreArray[1] = Score % 10;
        }
    }
}

size_t stlen(const char* str)
{
    size_t l = 0;
    while(str[l] != '\0') l++;
    return l;
}

void Game1::Draw()
{
    Renderer::Clear({ 0.25f, 0.1f, 0.65f, 1.0f });
    Renderer::Begin();
    for(int i = 0; i < ObjectList->Count(); i++)
    {
        auto o = ObjectList->GetGameObject(i);

        if(o->IsAlive()) Renderer::Draw(o->GetSprite(), { o->GetX(), o->GetY(), o->GetWidth(), o->GetHeight() }, o->GetClip(), o->GetColor());
    }

    // draw score text
    Renderer::Draw(sprite, {535.0f, 330.0f, 66.0f, 16.0f}, { 32.0f, 7.0f, 33.0f, 8.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

    // draw score
    for(int i = 0; i < 2; i++)
    {
        float x = 605 + 10 * i;
        if(ScoreArray[0] == 0) i++;
        Renderer::Draw(sprite, { x, 330.0f, 6.0f, 14.0f }, Numbers[ScoreArray[i]], { 1.0f, 1.0f, 1.0f, 1.0f });
    }

    // draw life label
    const char* lLabel = "life";
    size_t lLen = stlen(lLabel);

    for(int i = 0; i < lLen; i++)
    {
        char l = lLabel[i];
        Renderer::Draw(sprite, { 33.0f + 4.0f * i, 320.0f, 3.0f, 7.0f }, Letters[l], { 1.0f, 1.0f, 1.0f, 1.0f });
    }

    // draw chances
    for(int i = 0; i < Chances; i++)
    {
        float x = 16.0f + 16.0f * i;
        Renderer::Draw(sprite, { x, 330.0f, 16.0f, 16.0f }, { 16.0f, 0.0f, 16.0f, 16.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
    }

    // draw game over
    if(IsGameOver && GameStarted)
    {
        float x = 320.0f - (53.0f * 7.0f) / 2;
        float y = 180.0f - (9.0f * 7.0f) / 2;
        Renderer::Draw(sprite, { x, y, 53.0f * 7, 9.0f * 7 }, { 1.0f, 47.0f, 53.0f, 9.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

        float lW = 8.0f;
        float lY = 329.0f;
        const char* phr = "press space to play again";
        size_t siz = stlen(phr);
        float lX = GetResolution().x / 2 - (lW * siz) / 2;
        
        for(int i = 0; i < siz; i++)
        {
            if(phr[i] == ' ') i++;
            char l = phr[i];
        
            Renderer::Draw(sprite, { lX + lW * i, lY, 6.0f, 14.0f }, Letters[l], { 1.0f, 1.0f, 1.0f, fA });
        }
    }

    if(!GameStarted)
    {
        float lW = 8.0f;
        float lY = 329.0f;
        const char* phr = "press space to play";
        size_t siz = stlen(phr);
        float lX = GetResolution().x / 2 - (lW * siz) / 2;
        
        for(int i = 0; i < siz; i++)
        {
            if(phr[i] == ' ') i++;
            char l = phr[i];
        
            Renderer::Draw(sprite, { lX + lW * i, lY, 6.0f, 14.0f }, Letters[l], { 1.0f, 1.0f, 1.0f, fA });
        }
    }

    Renderer::End();
}

// Setup Game Board
void Game1::SetupLevel()
{
    Ball* ball = new Ball(312.0f, 220.0f, 16.0f, 16.0f);
    ball->SetClip({ 0.0f, 0.0f, 16.0f, 16.0f });
    ball->SetSpeed(40.0f);
    ball->SetMaxVelocity(500.0f);
    ball->SetLabel("ball");

    Paddle* paddle = new Paddle(280.0f, 300.0f, 48.0f, 14.0f);
    paddle->SetClip({ 0.0f, 16.0f, 48.0f, 14.0f });
    paddle->SetSpeed(10.0f);
    paddle->SetMaxVelocity(650.0f);
    paddle->SetLabel("paddle");

    for(int y = 0; y < 6; y++)
    {
        for(int x = 0; x < 12; x++)
        {
            int stagger = y % 2;
            float bX = x * 40 + 2 * x + 50 + 30 * stagger;
            float bY = y * 16 + 2 * y;

            int health = 6 - y;

            auto brick = new Brick(bX, bY, 41.0f, 17.0f);
            brick->SetClip({ 0.0f, 30.0f, 41.0f, 17.0f });
            brick->SetHealth(health);
            brick->SetLabel("brick");

            PhysicsEngine->AddBody(new PhysicsBody(
                { 0.0f, 0.0f, brick->GetWidth(), brick->GetHeight() },
                { brick->GetX(), brick->GetY(), brick->GetWidth(), brick->GetHeight() },
                ObjectList->Register(brick)
            ));
        }
    }

    PhysicsEngine->AddBody(new PhysicsBody(
        { 0.0f, 0.0f, paddle->GetWidth(), paddle->GetHeight() },
        { paddle->GetX(), paddle->GetY(), paddle->GetWidth(), paddle->GetHeight() },
        ObjectList->Register(paddle)
    ));
    
    PhysicsEngine->AddBody(new PhysicsBody(
        { 0.0f, 0.0f, ball->GetWidth(), ball->GetHeight() },
        { ball->GetX(), ball->GetY(), ball->GetWidth(), ball->GetHeight() },
        ObjectList->Register(ball)
    ));
}

void Game1::LoadSprites(unsigned int s)
{
    for(int i = 0; i < ObjectList->Count(); i++)
    {
        ObjectList->AttachSprite(i, s);
    }
}

void Game1::DestroyLevel()
{
    ObjectList->ClearObjects();
    PhysicsEngine->ClearBodies();
}