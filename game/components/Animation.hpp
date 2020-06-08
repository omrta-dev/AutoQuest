//
// Created by omar on 6/3/20.
//
#pragma once
#include <vector>

namespace aik::component
{
    struct AnimationFrame
    {
        float duration;
        int animationIndex;
    };

    class Animation
    {
    public:
        std::vector<AnimationFrame> animationFrames;
        unsigned int currentAnimationIndex;
    };
}
