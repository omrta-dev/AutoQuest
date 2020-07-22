//
// Created by omar on 7/18/20.
//

#include "HGTMesh.hpp"
#include <fstream>
#include <iostream>

// A Hgt file contains 1201x1201 items, so we should allocate that much in out vector
aik::HGTMesh::HGTMesh(const std::string &hgtFile)
{
    vertices_.reserve((xyDimension * xyDimension) + 1);
    std::ifstream hgtData(hgtFile, std::ifstream::binary);
    if(hgtData.is_open())
    {
        float vertexStep = 1.0f / static_cast<float>(xyDimension);
        for(int row = 0; row < xyDimension; row++) // generate vertex information
        {
            for(unsigned int column = 0; column < xyDimension; column++)
            {
                if(hgtData.eof())
                {
                    return;
                }
                float x = static_cast<float>(column) * vertexStep;
                float y = static_cast<float>(row) * vertexStep;
                // extract the elevation from the hgtFile and then swap endianness
                BigEndianNumber bigEndianNumber{};
                hgtData.read(bigEndianNumber.buffer, 2);
                // HGT format is at most a int16_t value, so we can clamp it down to -1,1
                auto z = static_cast<float>(bigToLittleEndian(bigEndianNumber)) / static_cast<float>(std::numeric_limits<int16_t>::max());
                vertices_.emplace_back(x,y,z);
            }
        }
    }
    // generate index information
    glm::ivec3 triangle1, triangle2;
    for (int row = 0; row < xyDimension - 1; row++)
    {
        for (int column = 0; column < xyDimension - 1; column++)
        {
            // Triangle 1
            triangle1.x = column + row * xyDimension;
            triangle1.y = column + (row + 1) * xyDimension;
            triangle1.z = (column + 1) + row * xyDimension;
            // Triangle 2
            triangle2.x = triangle1.y;
            triangle2.y = (column + 1) + (row + 1) * xyDimension;
            triangle2.z = triangle1.z;

            // add our data to the vector
            indicies_.emplace_back(triangle1.x);
            indicies_.emplace_back(triangle1.y);
            indicies_.emplace_back(triangle1.z);

            indicies_.emplace_back(triangle2.x);
            indicies_.emplace_back(triangle2.y);
            indicies_.emplace_back(triangle2.z);
        }
    }
}

std::vector<glm::vec3> aik::HGTMesh::getVertices()
{
    return vertices_;
}

std::vector<unsigned int> aik::HGTMesh::getIndicies()
{
    return indicies_;
}

int16_t aik::HGTMesh::bigToLittleEndian(aik::BigEndianNumber &bigEndianNumber)
{
    // swap the first and second byte to convert to little endian
    std::swap(bigEndianNumber.buffer[0], bigEndianNumber.buffer[1]);
    return bigEndianNumber.number;
}
