//
// Created by omar on 3/6/20.
//

#pragma once
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/mat4x4.hpp>

#include "Mesh.hpp"

namespace aik
{
    // This class loads a .obj and stores most of the useful information about the model such as
    // - Vertices
    // - Indices
    // - Textures
    // - Model Matrix
    // - and more.
    // Note that this object should only be a container and have no rendering logic.
    // TODO: Implement a RenderingManager that accepts a Mesh/Model and renders it.
    class Model
    {
    public:
        Model();
        void loadModel(const std::string& modelPath);
        std::vector<aik::Texture> loadTextures(aiMaterial * material, aiTextureType type, const std::string& typeName);
        inline std::vector<Mesh>& getMeshes() {return meshes_;}
        std::vector<aik::Vertex> getVertices();
        std::vector<unsigned int> getIndices();
        void setRotation(glm::vec3 rotationAxis, float degree);
        inline glm::vec3 getRotation() const {return rotation_;}
        void setScale(glm::vec3 scale);
        inline glm::vec3 getScale() const {return scale_;}
        void setPosition(glm::vec3 position);
        inline glm::vec3 getPosition() const {return position_;}
        void move(glm::vec3 offset);
        void rotate(glm::vec3 rotationAxis, float degree);
        const glm::mat4& getModel();
    private:
        std::vector<Mesh> meshes_;
        std::vector<aik::Texture> texturesLoaded_;
        std::string directory_;
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);

        bool isModified_;
        glm::vec3 rotation_{};
        float rotationAngle_;
        glm::vec3 scale_{};
        glm::vec3 position_{};
        glm::mat4 model_{};
        unsigned int maxIndex_;
        unsigned long verticesCount_;
    };
}
