#include "gameobject.hpp"

namespace Engine
{
    class Paddle : public GameObject
    {
        public:
        Paddle(float x, float y, float width, float height) : GameObject(x, y, width, height) {}
        void Update(Timestep ts);
    };
};