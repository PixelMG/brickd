#pragma once
#include "../common/framework.hpp"

namespace Engine
{
    class GameObject
    {
        public:
        GameObject(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f)
        {
            Position = { x, y }, Velocity = { 0.0f, 0.0f }, Direction = { 1.0f, 1.0f };
            Width = width, Height = height;
            Color = { 1.0f, 1.0f, 1.0f, 1.0f };
            isAlive = true;
            Speed = 1.0f;
        }
        void SetID(int id) { ID = id; }
        int GetID() { return ID; }
        void SetSpeed(float speed) { Speed = speed; }
        void SetMaxVelocity(float maxVelocity) { MaxVelocity = maxVelocity; }
        void SetColor(Color color) { Color = color; }
        Color GetColor() { return Color; }
        void SetOpacity(float alpha) { Color = { Color.R, Color.G, Color.B, alpha }; }
        void SetLabel(const char* label) { Label = label; }
        const char* GetLabel() { return Label; }
        void AttachSprite(unsigned int sprite) { Sprite = sprite; }
        void AttachCollider(PhysicsBody* body);
        unsigned int GetSprite() { return Sprite; }
        void SetClip(Rectangle clip) { Clip = clip; }
        Rectangle GetClip() { return Clip; }
        float GetWidth() { return Width; }
        float GetHeight() { return Height; }
        float GetX() { return Position.x; }
        float GetY() { return Position.y; }
        bool IsAlive() { return isAlive; }
        virtual void Update(Timestep ts);
        virtual void HandleCollision(GameObject* Obj1, Vector2 Center1, GameObject* Obj2, Vector2 Center2, Timestep ts);
        virtual void HandleBoundsCollision(float WorldWidth, float WorldHeight);

        protected:
        int ID;
        Rectangle Clip;
        Vector2 Position;
        Vector2 Velocity;
        Vector2 Direction;
        float Width;
        float Height;
        float Speed;
        float MaxVelocity = 1;
        Color Color;
        bool isAlive;
        const char* Label;
        unsigned int Sprite;
        PhysicsBody* Body;
        // static const size_t MAX_PHYSICS_BODIES = 4;
        void SetPosition(Vector2 position) { Position = position; }
    };

    class GameObjects
    {
        public:
        GameObjects() {}
        int Register(GameObject* gameObject);
        void AttachSprite(int ID, unsigned int SpriteID);
        void Update(Timestep ts);
        GameObject* GetGameObject(int ID);
        void ClearObjects();
        int Count() { return o_count; }

        private:
        GameObject* ObjectList[500];
        size_t o_count = 0;
    };
};