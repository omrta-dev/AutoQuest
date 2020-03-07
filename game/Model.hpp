//
// Created by omar on 3/6/20.
//

#pragma once
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Entity.hpp"

namespace aik
{
    class Model
    {
    public:
        void loadModel(std::string modelPath);
        inline std::vector<Entity>& getMeshes() {return meshes_;}
    private:
        std::vector<Entity> meshes_;
        std::string directory_;
        void processNode(aiNode *node, const aiScene *scene);
        Entity processMesh(aiMesh *mesh, const aiScene *scene);
        //std::vector<aik::Texture> loadMaterialTextures(aiMaterial* material, aiTextureType textureType, std::string typeName);
    };
}
