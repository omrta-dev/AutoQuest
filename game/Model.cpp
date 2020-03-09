//
// Created by omar on 3/6/20.
//

#include "Model.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
aik::Model::Model()
{
    model_ = glm::mat4(1.0f);
    position_ = glm::vec3(1.0f);
    scale_ = glm::vec3(1.0f);
    rotation_ = glm::vec3(0.0f, 0.0f, 1.0f);
    rotationAngle_ = 0.0f;
    isModified_ = true;
    maxIndex_ = 0;
    verticesCount_ = 0;
}

void aik::Model::loadModel(const std::string& modelPath)
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

std::vector<aik::Vertex> aik::Model::getVertices()
{
    std::vector<aik::Vertex> vertices;
    vertices.reserve(verticesCount_);
    std::for_each(meshes_.begin(), meshes_.end(), [&](Mesh& mesh){vertices.insert(vertices.end(),
                                                                                 mesh.getVertices().begin(), mesh.getVertices().end());});
    return vertices;
}

std::vector<unsigned int> aik::Model::getIndices()
{
    std::vector<unsigned int> indices;
    indices.reserve(maxIndex_);
    std::for_each(meshes_.begin(), meshes_.end(), [&](Mesh& mesh){indices.insert(indices.end(),
            mesh.getIndices().begin(), mesh.getIndices().end());});
    return indices;
}

std::vector<aik::Texture> aik::Model::loadTextures(aiMaterial *material, aiTextureType type, const std::string& typeName)
{
    std::vector<aik::Texture> textures;
    for(unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString textureName;
        material->GetTexture(type, i, &textureName);
        bool loadTexture = true;
        auto foundTexture = std::find(texturesLoaded_.begin(), texturesLoaded_.end(), std::string(textureName.C_Str()));
        if(foundTexture != texturesLoaded_.end())
        {
            textures.push_back(*foundTexture);
            loadTexture = false;
        }
        if(loadTexture)
        {
            auto texture = aik::Texture::LoadTexture(directory_, textureName.C_Str(), typeName);
            texturesLoaded_.push_back(texture);
            textures.push_back(texture);
        }
    }
    return textures;
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

aik::Mesh aik::Model::processMesh(aiMesh *mesh, const aiScene *scene)
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
        vertex.uv = glm::vec4(1, 1, 1, 1.0f);

        // Normals
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        // tex coords
        if(mesh->mTextureCoords[0])
        {
            vertex.uv.x = mesh->mTextureCoords[0][i].x;
            vertex.uv.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.uv = glm::vec2(0.0f);
        }

        vertices.push_back(vertex);
        verticesCount_++;
    }
    // process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j] + maxIndex_);
        }
    }

    // this is used to calculate the index for the next face since we are storing all model info in only 1 vbo and 1 ebo
    maxIndex_ = *std::max_element(indices.begin(), indices.end()) + 1;

    std::vector<aik::Texture> diffuseMaps =  loadTextures(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE, "diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    return Mesh(vertices, indices, textures);
}

void aik::Model::setRotation(glm::vec3 rotationAxis, float degree)
{
    isModified_ = true;
    rotation_ = rotationAxis;
    rotationAngle_ = degree;
}

void aik::Model::setScale(glm::vec3 scale)
{
    isModified_ = true;
    scale_ = scale;
}

void aik::Model::setPosition(glm::vec3 position)
{
    isModified_ = true;
    position_ = position;
}

void aik::Model::move(glm::vec3 offset)
{
    isModified_ = true;
    position_ += offset;
}

void aik::Model::rotate(glm::vec3 rotationAxis, float degree)
{
    rotation_ = rotationAxis;
    rotationAngle_ += degree;
    isModified_ = true;
}

const glm::mat4& aik::Model::getModel()
{
    if(isModified_)
    {
        model_ = glm::translate(glm::mat4(1.0f), position_);
        model_ = glm::scale(model_, scale_);
        if(rotationAngle_ != 0.0f)
            model_ = glm::rotate(model_, glm::radians(rotationAngle_), rotation_);
        isModified_ = false;
    }
    return model_;
}
