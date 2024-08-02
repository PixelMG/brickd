#include "brick.hpp"

using namespace Engine;

void Brick::HandleCollision(GameObject* Obj1, Vector2 Center1, GameObject* Obj2, Vector2 Center2, Timestep ts)
{
    if(Health > 0) { Health--; SetColor(HealthColors[Health - 1]); }
    if(Health < 1) isAlive = false;
}