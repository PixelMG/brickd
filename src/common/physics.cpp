#include "physics.hpp"

using namespace Engine;

PhysicsBody::PhysicsBody(Rectangle position, Rectangle object, int objectID)
{
    Position = { object.x, object.y }, Width = object.w, Height = object.h, ObjectID = objectID;
    Center = { Position.x + Width / 2, Position.y + Height / 2 };
}

void PhysicsBody::SetObjectPosition(Vector2 position)
{
    ObjectPosition = position;
    Center = { ObjectPosition.x + Width / 2, ObjectPosition.y + Height / 2 };
}

void Physics::SetWorldBounds(float width, float height)
{
    WorldWidth = width, WorldHeight = height;
}

void Physics::CheckWorldBounds(PhysicsBody* b)
{
    if(b->GetX() + b->GetWidth() > WorldWidth ||
        b->GetX() < 0 ||
        b->GetY() + b->GetHeight() > WorldHeight ||
        b->GetY() < 0)
    {
       b->SetOutofBoundsFlag(true);
       return;
    }

    b->SetOutofBoundsFlag(false);
}

void Physics::AddBody(PhysicsBody* Body)
{
    Body->SetID(b_count);
    PhysicsBodies[b_count] = Body;
    b_count++;
}

void Physics::Update()
{
    for(int a = 0; a < b_count; a++)
    {
        auto b1 = PhysicsBodies[a];

        CheckWorldBounds(b1);

        for(int b = 0; b < b_count; b++)
        {
            auto b2 = PhysicsBodies[b];

            if(PhysicsBodies[a]->GetID() != PhysicsBodies[b]->GetID() &&
                PhysicsBodies[a]->IsEnabled() && PhysicsBodies[b]->IsEnabled())
            {
                if(b1->GetX() + b1->GetWidth() > b2->GetX() &&
                    b1->GetX() < b2->GetX() + b2->GetWidth() &&
                    b1->GetY() + b1->GetHeight() > b2->GetY() &&
                    b1->GetY() < b2->GetY() + b2->GetHeight())
                {
                    b1->SetOverlappingFlag(true);
                    b1->SetOverlappingWith(b2->GetID());
                    b2->SetOverlappingFlag(true);
                    b2->SetOverlappingWith(b1->GetID());
                }
            }
        }
    }
}

void Physics::ClearBodies()
{
    for(int i = 0; i < b_count; i++)
    {
        delete PhysicsBodies[i];
    }

    b_count = 0;
}