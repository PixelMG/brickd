#include "gameobject.hpp"

namespace Engine
{
    class Ball : public GameObject
    {
        public:
        Ball(float x, float y, float width, float height) : GameObject(x, y, width, height) {}
        void Update(Timestep ts);
        void HandleCollision(GameObject* Obj1, Vector2 Center1, GameObject* Obj2, Vector2 Center2, Timestep ts);
        void HandleBoundsCollision(float WorldWidth, float WorldHeight);

        private:
    };
};