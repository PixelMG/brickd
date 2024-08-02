#pragma once
#if OPENGL
#include "../ogl/base.hpp"
#elif METAL
#include "../mtl/base.hpp"
#endif

#include "types.hpp"
#include "inputmap.hpp"
#include "physics.hpp"
#include "timestep.hpp"

namespace Engine
{
    class Input
    {
        private:
        Input();

        public:
        static void PollInput();
        static bool IsKeyDown(int key);
    };

    class Renderer
    {
        private:
        Renderer();

        public:
        static void Clear(Color color);
        static void Begin();
        static void Draw(unsigned int sprite, Rectangle source, Rectangle clip, Color color);
        static void End();
    };

    class Content
    {
        public:
        Content();
        unsigned int Load(const std::string& path);
    };

    class Game
    {
        public:
        Game(int width, int height, const char* title);
        void SetResolution(int width, int height) { RES_WIDTH = width; RES_HEIGHT = height; }
        void Setup();
        void Run();
        void Loop();
        void Stop();

        protected:
        Content* ContentManager = new Content();
        Physics* PhysicsEngine = new Physics();
        virtual void Init();
        virtual void Load();
        virtual void Update(Timestep ts);
        virtual void PostUpdate(Timestep ts);
        virtual void Draw();
        Vector2 GetResolution() { return { (float)RES_WIDTH, (float)RES_HEIGHT }; }
        bool IsGameOver = false;
        bool IsPaused = false;

        private:
        int WIDTH;
        int HEIGHT;
        int RES_WIDTH;
        int RES_HEIGHT;
        const char* TITLE;
        float m_LastFrameTime = 0.0f;
    };
};