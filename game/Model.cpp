//
// Created by omar on 3/6/20.
//

#include "Model.hpp"

void aik::Model::loadModel(std::string modelPath)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory_ = modelPath.substr(0, modelPath.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void aik::Model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

aik::Entity aik::Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<aik::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<aik::Texture> textures;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // process vertex positions
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;
        vertex.color = glm::vec4(1, 1, 1, 1.0f);
        vertices.push_back(vertex);
    }
    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Entity(vertices, indices, textures);
}