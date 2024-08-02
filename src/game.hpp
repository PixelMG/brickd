#include "common/framework.hpp"
#include "core/ball.hpp"
#include "core/paddle.hpp"
#include "core/brick.hpp"

namespace Engine
{
    class Game1 : public Game
    {
        public:
        Game1(int width, int height, const char* title) : Game::Game(width, height, title) {}
        
        protected:
        void Init();
        void Load();
        void Update(Timestep ts);
        void PostUpdate(Timestep ts);
        void Draw();
        GameObjects* ObjectList = new GameObjects();

        private:
        int Score = 0;
        int Chances = 3;
        int Debounce = 0;
        int DebounceSet = 70;
        bool GameStarted = false;
        void SetupLevel();
        void LoadSprites(unsigned int s);
        void DestroyLevel();
    };
};