#include "gameobject.hpp"

namespace Engine
{
    class Brick : public GameObject
    {
        public:
        Brick(float x, float y, float width, float height) : GameObject(x, y, width, height) {}
        void SetHealth(int health) { Health = health; SetColor(HealthColors[Health - 1]); }
        void HandleCollision(GameObject* Obj1, Vector2 Center1, GameObject* Obj2, Vector2 Center2, Timestep ts);

        private:
        int Health;
        struct::Color HealthColors[6] = {
            { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 0.5f, 0.5f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f },
            { 0.5f, 0.5f, 0.0f, 1.0f },
            { 1.0f, 0.0f, 0.0f, 1.0f },
            { 0.5f, 0.0f, 0.0f, 1.0f }
        };
    };
};