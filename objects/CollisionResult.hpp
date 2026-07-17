#pragma once 

class CollisionResult {
public:
    bool hit;
    float minOverlap;
    glm::vec2 bestAxis;
};

