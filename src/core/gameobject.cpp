#include "gameobject.hpp"

using namespace Engine;

void GameObject::Update(Timestep ts) {}

void GameObject::AttachCollider(PhysicsBody* body) { Body = body; }

void GameObject::HandleCollision(GameObject* Obj1, Vector2 Center1, GameObject* Obj2, Vector2 Center2, Timestep ts) {}

void GameObject::HandleBoundsCollision(float WorldWidth, float WorldHeight)
{
    if(Position.x + Width > WorldWidth)
    {
        float diff = Position.x + Width - WorldWidth;
        SetPosition({ Position.x - diff, Position.y });
    }

    if(Position.x < 0)
    {
        float diff = 0 - Position.x;
        SetPosition({ Position.x + diff, Position.y });
    }

    if(Position.y + Height > WorldHeight)
    {
        float diff = Position.y + Height - WorldHeight;
        SetPosition({ Position.x, Position.y - diff });
    }

    if(Position.y < 0)
    {
        float diff = 0 - Position.y;
        SetPosition({ Position.x, Position.y + diff });
    }
}

int GameObjects::Register(GameObject* gameObject)
{
    ObjectList[o_count] = gameObject;
    gameObject->SetID(o_count);
    o_count++;
    return o_count - 1;
}

void GameObjects::AttachSprite(int ID, unsigned int SpriteID)
{
    ObjectList[ID]->AttachSprite(SpriteID);
}

void GameObjects::Update(Timestep ts)
{
    for(int i = 0; i < o_count; i++)
    {
        ObjectList[i]->Update(ts);
    }
}

GameObject* GameObjects::GetGameObject(int ID)
{
    return ObjectList[ID];
}

void GameObjects::ClearObjects()
{
    for(int i = 0; i < o_count; i++)
    {
        delete ObjectList[i];
    }

    o_count = 0;
}