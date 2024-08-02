#pragma once
#include "types.hpp"

namespace Engine
{
    class PhysicsBody
    {
        public:
        PhysicsBody(Rectangle position = { 0.0f, 0.0f, 0.0f, 0.0f }, Rectangle object = { 0.0f, 0.0f, 0.0f, 0.0f }, int objectID = -1);
        int GetObjectID() { return ObjectID; }
        void SetID(int id) { ID = id; }
        int GetID() { return ID; }
        void SetObjectPosition(Vector2 position);
        float GetX() { return ObjectPosition.x; }
        float GetY() { return ObjectPosition.y; }
        Vector2 GetCenter() { return Center; }
        float GetWidth() { return Width; }
        float GetHeight() { return Height; }
        void SetOutofBoundsFlag(bool flag) { OutofBounds = flag; }
        void SetOverlappingFlag(bool flag) { Overlapping = flag; }
        void SetOverlappingWith(int id) { OverlappingWith = id; }
        bool IsOutofBounds() { return OutofBounds; }
        bool IsOverlapping() { return Overlapping; }
        int IsOverlappingWith() { return OverlappingWith; }
        void Disable() { isEnabled = isEnabled ? false : true; }
        bool IsEnabled() { return isEnabled; }

        private:
        Vector2 Position;
        Vector2 Center;
        Vector2 ObjectPosition;
        float Width;
        float Height;
        int ID;
        int ObjectID;
        bool OutofBounds = false;
        bool Overlapping = false;
        bool isEnabled = true;
        int OverlappingWith = -1;
    };

    class Physics
    {
        public:
        Physics() {}
        void SetWorldBounds(float width, float height);
        float GetWorldWidth() { return WorldWidth; }
        float GetWorldHeight() { return WorldHeight; }
        void AddBody(PhysicsBody* Body);
        PhysicsBody* GetBody(int ID) { return PhysicsBodies[ID]; }
        void Update();
        void ClearBodies();
        int PhysicsBodyCount() { return b_count; }

        private:
        std::array<PhysicsBody*, 1000> PhysicsBodies;
        int b_count = 0;
        float WorldWidth;
        float WorldHeight;
        void CheckWorldBounds(PhysicsBody* b);
    };
};