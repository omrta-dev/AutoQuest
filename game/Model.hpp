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
#include "Entity.hpp"

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
    class Model : public aik::Entity
    {
    public:
        Model();
        void loadModel(const std::string& modelPath);
        std::vector<aik::Texture> loadTextures(aiMaterial * material, aiTextureType type, const std::string& typeName);
        inline std::vector<Mesh>& getMeshes() {return meshes_;}
        std::vector<aik::Vertex> getVertices() override ;
        std::vector<unsigned int> getIndices() override ;
    private:
        std::vector<Mesh> meshes_;
        std::vector<aik::Texture> texturesLoaded_;
        std::string directory_;
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    };
}
