#ifndef MESH_UTILITY_H
#define MESH_UTILITY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <filesystem>

// Use assimp for model loading
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader_utility.h"
#include "texture_utility.h"

// For better readabilty and easier to reuse
typedef struct
{
    Vector3 pos;
    Vector2 uv;
    Vector3 normal;

} Vertex;

typedef struct
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    bool initialized = false;
    bool use_indices = false;

} Mesh;

// ALWAYS SET THE SHAPE BEFORE YOU INITIALIZE

static inline void Mesh_LoadTriangle(Mesh& mesh)
{
    mesh.vertices = 
    {
        // pos                // uv       // normal
        -0.5f, -0.5f, 0.0f,   0.0f,0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   1.0f,0.0f,  0.0f, 0.0f, 1.0f,
         0.0f,  0.5f, 0.0f,   0.5f,1.0f,  0.0f, 0.0f, 1.0f
    };
    mesh.use_indices = false;

    mesh.VAO = 0;
    mesh.VBO = 0;
    mesh.EBO = 0;

    mesh.initialized = true;
    mesh.textures.clear();
}

static inline void Mesh_LoadRectangle(Mesh& mesh)
{
    mesh.vertices = 
    {
        // pos                // uv       // normal
         0.5f,  0.5f, 0.0f,   1.0f,1.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f,   1.0f,0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,   0.0f,0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,   0.0f,1.0f,  0.0f, 0.0f, 1.0f
    };

    mesh.indices =
    {
        0, 1, 3,    // first triangle
        1, 2, 3     // second triangle
    };

    mesh.use_indices = true;

    mesh.VAO = 0;
    mesh.VBO = 0;
    mesh.EBO = 0;

    mesh.initialized = true;
    mesh.textures.clear();
}

// TODO
static inline void Mesh_LoadCircle(Mesh& mesh, float radius, int sectors)
{
    mesh.vertices.clear();
    mesh.indices.clear();

    // Center vertex
    mesh.vertices.insert(mesh.vertices.end(),
        { 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f });

    for (int i = 0; i <= sectors; ++i)
    {
        float theta = (float)i / (float)sectors * 2.0f * consts::PI;
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        // position, texcoords, normal
        mesh.vertices.insert(mesh.vertices.end(),
        {
            x, y, 0.0f,
            0.5f + 0.5f * cosf(theta),
            0.5f + 0.5f * sinf(theta),
            0.0f, 0.0f, 1.0f
        });
    }

    // Indices (triangle fan)
    for (int i = 1; i <= sectors; ++i)
    {
        mesh.indices.push_back(0);
        mesh.indices.push_back(i);
        mesh.indices.push_back(i + 1);
    }

    mesh.use_indices = true;
    mesh.VAO = 0;
    mesh.VBO = 0;
    mesh.EBO = 0;
    mesh.initialized = true;
    mesh.textures.clear();
}

static inline void Mesh_LoadCube(Mesh& mesh)
{
    mesh.vertices = 
    {
        // pos                  // uv       // normal
        // Front face
        -0.5f,-0.5f, 0.5f,      0.0f,0.0f,  0.0f, 0.0f, 1.0f,
         0.5f,-0.5f, 0.5f,      1.0f,0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, 0.5f, 0.5f,      1.0f,1.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,      0.0f,1.0f,  0.0f, 0.0f, 1.0f,

        // Back face
        -0.5f,-0.5f,-0.5f,      1.0f,0.0f,  0.0f, 0.0f,-1.0f,
         0.5f,-0.5f,-0.5f,      0.0f,0.0f,  0.0f, 0.0f,-1.0f,
         0.5f, 0.5f,-0.5f,      0.0f,1.0f,  0.0f, 0.0f,-1.0f,
        -0.5f, 0.5f,-0.5f,      1.0f,1.0f,  0.0f, 0.0f,-1.0f,

        // Left face
        -0.5f,-0.5f,-0.5f,      0.0f,0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f,-0.5f, 0.5f,      1.0f,0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,      1.0f,1.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,      0.0f,1.0f, -1.0f, 0.0f, 0.0f,

        // Right face
         0.5f,-0.5f,-0.5f,      1.0f,0.0f,  1.0f, 0.0f, 0.0f,
         0.5f,-0.5f, 0.5f,      0.0f,0.0f,  1.0f, 0.0f, 0.0f,
         0.5f, 0.5f, 0.5f,      0.0f,1.0f,  1.0f, 0.0f, 0.0f,
         0.5f, 0.5f,-0.5f,      1.0f,1.0f,  1.0f, 0.0f, 0.0f,

        // Top face
        -0.5f, 0.5f,-0.5f,      0.0f,0.0f,  0.0f, 1.0f, 0.0f,
         0.5f, 0.5f,-0.5f,      1.0f,0.0f,  0.0f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,      1.0f,1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,      0.0f,1.0f,  0.0f, 1.0f, 0.0f,

        // Bottom face
        -0.5f,-0.5f,-0.5f,      1.0f,1.0f,  0.0f,-1.0f, 0.0f,
         0.5f,-0.5f,-0.5f,      0.0f,1.0f,  0.0f,-1.0f, 0.0f,
         0.5f,-0.5f, 0.5f,      0.0f,0.0f,  0.0f,-1.0f, 0.0f,
        -0.5f,-0.5f, 0.5f,      1.0f,0.0f,  0.0f,-1.0f, 0.0f
    };

    mesh.indices = 
    {
        0,1,2, 2,3,0,        // front
        4,5,6, 6,7,4,        // back
        8,9,10,10,11,8,      // left
        12,13,14,14,15,12,   // right
        16,17,18,18,19,16,   // top
        20,21,22,22,23,20    // bottom
    };

    mesh.use_indices = true;

    mesh.VAO = 0;
    mesh.VBO = 0;
    mesh.EBO = 0;

    mesh.initialized = true;
    mesh.textures.clear();
}

static inline void Mesh_LoadSphere(Mesh& mesh, float radius, int stacks, int sectors)
{
    mesh.vertices.clear();
    mesh.indices.clear();

    for (int i = 0; i <= stacks; ++i)
    {
        float phi = (float)i / (float)stacks * consts::PI;
        float y = radius * cosf(phi);
        float r = radius * sinf(phi);

        for (int j = 0; j <= sectors; ++j)
        {
            float theta = (float)j / (float)sectors * 2.0f * consts::PI;
            float x = r * cosf(theta);
            float z = r * sinf(theta);

            float u = (float)j / (float)sectors;
            float v = 1.0f - (float)i / (float)stacks;

            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;

            mesh.vertices.insert(mesh.vertices.end(),
                { x, y, z, u, v, nx, ny, nz });
        }
    }

    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < sectors; ++j)
        {
            int a = i * (sectors + 1) + j;
            int b = a + sectors + 1;

            mesh.indices.push_back(a);
            mesh.indices.push_back(b);
            mesh.indices.push_back(a + 1);

            mesh.indices.push_back(a + 1);
            mesh.indices.push_back(b);
            mesh.indices.push_back(b + 1);
        }
    }

    mesh.use_indices = true;
    mesh.VAO = 0;
    mesh.VBO = 0;
    mesh.EBO = 0;
    mesh.initialized = true;
    mesh.textures.clear();
}

static inline void Mesh_LoadModel(Mesh& mesh, const std::string& obj_path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile
    (
        obj_path,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_FlipUVs
    );

    if (!scene || !scene->HasMeshes())
    {
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        mesh.initialized = false;
        return;
    }

    mesh.vertices.clear();
    mesh.indices.clear();

    unsigned int indexOffset = 0;

    // Fill in the vertex data
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh* aimesh = scene->mMeshes[i];

        for (unsigned int v = 0; v < aimesh->mNumVertices; ++v)
        {
            aiVector3D pos = aimesh->mVertices[v];
            aiVector3D normal = aimesh->HasNormals() ? aimesh->mNormals[v] : aiVector3D(0, 1, 0);
            aiVector3D tex = aimesh->HasTextureCoords(0) ? aimesh->mTextureCoords[0][v] : aiVector3D(0, 0, 0);

            mesh.vertices.insert(mesh.vertices.end(), {
                pos.x, pos.y, pos.z,
                tex.x, tex.y,
                normal.x, normal.y, normal.z
            });
        }

        for (unsigned int f = 0; f < aimesh->mNumFaces; ++f)
        {
            const aiFace& face = aimesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
            {
                mesh.indices.push_back(face.mIndices[j] + indexOffset);
            }
        }

        indexOffset += aimesh->mNumVertices;
    }

    if (scene->HasMaterials())
    {
        for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
        {
            aiMaterial* material = scene->mMaterials[i];
            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiString texture_path;
                if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) == AI_SUCCESS)
                {
                    std::filesystem::path modelDir = std::filesystem::path(obj_path).parent_path();
                    std::string full_tex_path = (modelDir / texture_path.C_Str()).string();

                    Texture tex;
                    Texture_Init(tex, full_tex_path);
                    mesh.textures.push_back(tex);
                }   
            }
        }
    }

    mesh.use_indices = true;
    mesh.VAO = 0;
    mesh.VBO = 0;
    mesh.EBO = 0;
    mesh.initialized = true;
}

static inline void Mesh_Generate(Mesh& mesh)
{
    if (!mesh.initialized)
    {
        std::cout<< "Mesh not initialized with shape" << std::endl;
        return;
    } 

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    if (mesh.use_indices) glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);

    if (mesh.use_indices)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);
    }

    GLsizei stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

static inline void Mesh_Draw(const Mesh& mesh)
{
    if (!mesh.initialized)
    {
        std::cout<< "Mesh not initialized with shape" << std::endl;
        return;
    } 

    glBindVertexArray(mesh.VAO);

    if (mesh.use_indices)
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size() / 3 * sizeof(float));

    glBindVertexArray(0);
}

static inline void Mesh_DrawWireFrame(const Mesh& mesh)
{
    if (!mesh.initialized)
    {
        std::cout<< "Mesh not initialized with shape" << std::endl;
        return;
    } 

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Mesh_Draw(mesh);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

static inline void Mesh_Delete(Mesh& mesh)
{
    if (mesh.EBO) glDeleteBuffers(1, &mesh.EBO);
    if (mesh.VAO) glDeleteVertexArrays(1, &mesh.VAO);
    if (mesh.VBO) glDeleteBuffers(1, &mesh.VBO);
    
    mesh.vertices.clear();
    mesh.indices.clear();
    mesh.textures.clear();
    mesh.VAO = 0;
    mesh.VBO = 0;
    mesh.EBO = 0;
}

#endif