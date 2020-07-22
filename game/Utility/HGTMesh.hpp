//
// Created by omar on 7/18/20.
//

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <glm/vec3.hpp>

namespace aik
{
    union BigEndianNumber
    {
        // contains the 2 bytes
        char buffer[2];
        // contains the number representation of the buffer
        int16_t number;
    };

    //N20 -> N59
    //W053 -> W0138
    class HGTMesh
    {
    public:
        HGTMesh(const std::string& hgtFile);
        std::vector<glm::vec3> getVertices();
        std::vector<unsigned int> getIndicies();
    private:
        int16_t bigToLittleEndian(BigEndianNumber& bigEndianNumber);
        std::vector<glm::vec3> vertices_;
        std::vector<unsigned int> indicies_;
        static constexpr int xyDimension = 1201;
    };
}
