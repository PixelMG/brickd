#include "ball.hpp"

using namespace Engine;

void Ball::Update(Timestep ts)
{
    if(Speed > MaxVelocity) Speed = MaxVelocity;

    Velocity.x = Speed * ts.GetSeconds() * Direction.x;
    Velocity.y = Speed * ts.GetSeconds() * Direction.y;

    SetPosition({ Position.x + Velocity.x, Position.y + Velocity.y });
}

void Ball::HandleBoundsCollision(float WorldWidth, float WorldHeight)
{
    if(Position.x + Width > WorldWidth)
    {
        float diff = Position.x + Width - WorldWidth;
        Direction.x *= -1;
        Speed += 10.0f;

        SetPosition({ Position.x - diff, Position.y });
    }

    if(Position.x < 0)
    {
        float diff = 0 - Position.x;
        Direction.x *= -1;
        Speed += 10.0f;
        SetPosition({ Position.x + diff, Position.y });
    }

    if(Position.y + Height > WorldHeight)
    {
        float diff = Position.y + Height - WorldHeight;
        Direction.y *= -1;
        Speed = 40.0f;
        SetPosition({ 312.0f, 240.0f });
    }

    if(Position.y < 0)
    {
        float diff = 0 - Position.y;
        Direction.y *= -1;
        Speed += 10.0f;
        SetPosition({ Position.x, Position.y + diff });
    }
}

void Ball::HandleCollision(GameObject* Obj1, Vector2 Center1, GameObject* Obj2, Vector2 Center2, Timestep ts)
{
    bool tEdge = Center1.y < Center2.y && Obj2->GetY() >= Obj1->GetY() + Obj1->GetHeight() - (Speed * ts.GetSeconds());
    bool bEdge = Center1.y > Center2.y && Obj2->GetY() + Obj2->GetHeight() - (Speed * ts.GetSeconds()) <= Obj1->GetY();
    bool lEdge = Center1.x < Center2.x && Obj2->GetX() + (Speed * ts.GetSeconds()) >= Obj1->GetX() + Obj1->GetWidth();
    bool rEdge = Center1.x > Center2.x && Obj2->GetX() + Obj2->GetWidth() <= Obj1->GetX() + (Speed * ts.GetSeconds());

    if(lEdge && Direction.x > 0)
    {
        Direction.x *= -1;
        Speed += 10.0f;
        SetPosition({ Obj2->GetX() - Obj1->GetWidth(), Position.y });
    }

    if(rEdge && Direction.x < 0)
    {
        Direction.x *= -1;
        Speed += 10.0f;
        SetPosition({ Obj2->GetX() + Obj2->GetWidth(), Position.y });
    }

    if(tEdge && Direction.y > 0)
    {
        float diff = Obj2->GetY() - Obj1->GetY();
        Direction.y *= -1;
        Speed += 10.0f;
        SetPosition({ Position.x, Obj2->GetY() - Obj1->GetHeight() });
    }

    if(bEdge && Direction.y < 0)
    {
        float diff = Obj2->GetY() - Position.y;
        Direction.y *= -1;
        Speed += 10.0f;
        SetPosition({ Position.x, Obj2->GetY() + Obj2->GetHeight() });
    }
}