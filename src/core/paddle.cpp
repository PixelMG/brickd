#include "paddle.hpp"

using namespace Engine;

void Paddle::Update(Timestep ts)
{
    if(Input::IsKeyDown(KEYS_D) || Input::IsKeyDown(KEYS_RIGHT))
        Velocity.x += Speed;

    else if(Input::IsKeyDown(KEYS_A) || Input::IsKeyDown(KEYS_LEFT))
        Velocity.x -= Speed;

    else
    {
        if(Velocity.x > 0)
        {
            float diff = Velocity.x - Speed < 0 ? Velocity.x - Speed : 0;
            Velocity.x -= Speed + diff;
        }
        if(Velocity.x < 0)
        {
            float diff = Velocity.x + Speed > 0 ? Velocity.x + Speed : 0;
            Velocity.x += Speed + diff;
        }
    }

    if(Velocity.x > MaxVelocity) Velocity.x = MaxVelocity;
    if(Velocity.x < -MaxVelocity) Velocity.x = -MaxVelocity;

    SetPosition({ Position.x + Velocity.x * ts.GetSeconds(), Position.y + Velocity.y * ts.GetSeconds() });
}