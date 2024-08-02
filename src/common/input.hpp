#pragma once
#include "inputmap.hpp"

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
};