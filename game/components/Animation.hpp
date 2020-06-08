//
// Created by omar on 6/3/20.
//
#pragma once
#include <vector>

namespace aik::component
{
    /**!
     * @brief Contains the duration and index of each frame
     */
    struct AnimationFrame
    {
        float duration; //< duration in seconds that this frame will be shown for
        int animationIndex; //< the index of the animation in the sprite sheet
    };

    //! @brief An animation is just an array of AnimationFrames with the index of the current frame
    class Animation
    {
    public:
        std::vector<AnimationFrame> animationFrames; //< vector of frames for this animation
        unsigned int currentAnimationIndex; //< contains the current index of the animation
    };
}
