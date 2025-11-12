#ifndef MESH_UTILITY_H
#define MESH_UTILITY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_utility.h"
#include "texture_utility.h"
#include "dyn_array_utility.h"
#include "string_utility.h"
#include "arena_utility.h"

// Use assimp for model loading
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>

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
    DynArray vertices;     // holds vertex structs
    DynArray indices;      // holds unsigned ints
    DynArray textures;     // holds textures
    //std::vector<float> vertices;
    //std::vector<unsigned int> indices;
    //std::vector<Texture> textures;

    bool initialized = false;
    bool use_indices = false;

} Mesh;

// ALWAYS SET THE SHAPE BEFORE YOU INITIALIZE

static inline void Mesh_CreateTriangle(Mesh* mesh)
{
    mesh->vertices = DynArray_Create_T(Vertex, 3, NULL);
    mesh->textures = DynArray_Create_T(Texture, 1, NULL);

    Vertex v1 = {{-0.5f, -0.5f, 0.0f},   {0.0f,0.0f},  {0.0f, 0.0f, 1.0f}};
    Vertex v2 = {{0.5f, -0.5f, 0.0f},   {1.0f,0.0f},  {0.0f, 0.0f, 1.0f}};
    Vertex v3 = {{0.0f, 0.5f, 0.0f},   {0.5f,1.0f},  {0.0f, 0.0f, 1.0f}};
    DynArray_Push_T(Vertex, &mesh->vertices, v1);
    DynArray_Push_T(Vertex, &mesh->vertices, v2);
    DynArray_Push_T(Vertex, &mesh->vertices, v3);

    mesh->use_indices = false;

    mesh->VAO = 0;
    mesh->VBO = 0;
    mesh->EBO = 0;

    mesh->initialized = true;
}

static inline void Mesh_CreateRectangle(Mesh* mesh)
{
    mesh->vertices = DynArray_Create_T(Vertex, 4, NULL);
    mesh->indices = DynArray_Create_T(unsigned int, 6, NULL);
    mesh->textures = DynArray_Create_T(Texture, 1, NULL);

    Vertex v0 = {{0.5f,  0.5f, 0.0f},   {1.0f,1.0f},  {0.0f, 0.0f, 1.0f}};
    Vertex v1 = {{0.5f, -0.5f, 0.0f},   {1.0f,0.0f},  {0.0f, 0.0f, 1.0f}};
    Vertex v2 = {{-0.5f, -0.5f, 0.0f},   {0.0f,0.0f},  {0.0f, 0.0f, 1.0f}};
    Vertex v3 = {{-0.5f,  0.5f, 0.0f},   {0.0f,1.0f},  {0.0f, 0.0f, 1.0f}};

    DynArray_Push_T(Vertex, &mesh->vertices, v0); DynArray_Push_T(Vertex, &mesh->vertices, v1);
    DynArray_Push_T(Vertex, &mesh->vertices, v2); DynArray_Push_T(Vertex, &mesh->vertices, v3);

    DynArray_Push_T(unsigned int, &mesh->indices, 0); DynArray_Push_T(unsigned int, &mesh->indices, 1); DynArray_Push_T(unsigned int, &mesh->indices, 3);
    DynArray_Push_T(unsigned int, &mesh->indices, 1); DynArray_Push_T(unsigned int, &mesh->indices, 2); DynArray_Push_T(unsigned int, &mesh->indices, 3);

    mesh->use_indices = true;

    mesh->VAO = 0;
    mesh->VBO = 0;
    mesh->EBO = 0;

    mesh->initialized = true;
}

// // TODO
static inline void Mesh_CreateCircle(Mesh* mesh, float radius, int sectors)
{
    mesh->vertices = DynArray_Create_T(Vertex, 10, NULL);
    mesh->indices = DynArray_Create_T(unsigned int, 10, NULL);
    mesh->textures = DynArray_Create_T(Texture, 1, NULL);

    // Center vertex
    Vertex center = { {0.0f, 0.0f, 0.0f}, {0.5f, 0.5f}, {0.0f, 0.0f, 1.0f} };
    DynArray_Push_T(Vertex, &mesh->vertices, center);

    for (int i = 0; i <= sectors; ++i)
    {
        float theta = (float)i / (float)sectors * 2.0f * PI;
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        // position, texcoords, normal
        Vertex vert = 
        {
            {x, y, 0.0f},
            {0.5f + 0.5f * cosf(theta), 0.5f + 0.5f * sinf(theta)},
            {0.0f, 0.0f, 1.0f}
        };

        DynArray_Push_T(Vertex, &mesh->vertices, vert);
    }

    // Indices (triangle fan)
    for (int i = 1; i <= sectors; ++i)
    {
        DynArray_Push_T(unsigned int, &mesh->indices, 0);
        DynArray_Push_T(unsigned int, &mesh->indices, i);
        DynArray_Push_T(unsigned int, &mesh->indices, i + 1);
    }

    mesh->use_indices = true;
    mesh->VAO = 0;
    mesh->VBO = 0;
    mesh->EBO = 0;
    mesh->initialized = true;
}

static inline void Mesh_CreateCube(Mesh* mesh)
{
    mesh->vertices = DynArray_Create_T(Vertex, 24, NULL);
    mesh->indices = DynArray_Create_T(unsigned int, 36, NULL);
    mesh->textures = DynArray_Create_T(Texture, 1, NULL);

    // pos                  // uv       // normal
    // Front face
    Vertex v0 = {{-0.5f,-0.5f, 0.5f},      {0.0f,0.0f},  {0.0f, 0.0f, 1.0f}};
    Vertex v1 = {{0.5f,-0.5f, 0.5f},      {1.0f,0.0f},  {0.0f, 0.0f, 1.0f}};
    Vertex v2 = {{0.5f, 0.5f, 0.5f},      {1.0f,1.0f},  {0.0f, 0.0f, 1.0f}};
    Vertex v3 = {{-0.5f, 0.5f, 0.5f},      {0.0f,1.0f},  {0.0f, 0.0f, 1.0f}};

    // Back face
    Vertex v4 = {{-0.5f,-0.5f,-0.5f},      {1.0f,0.0f},  {0.0f, 0.0f,-1.0f}};
    Vertex v5 = {{0.5f,-0.5f,-0.5f},      {0.0f,0.0f},  {0.0f, 0.0f,-1.0f}};
    Vertex v6 = {{0.5f, 0.5f,-0.5f},      {0.0f,1.0f},  {0.0f, 0.0f,-1.0f}};
    Vertex v7 = {{-0.5f, 0.5f,-0.5f},      {1.0f,1.0f},  {0.0f, 0.0f,-1.0f}};

    // Left face
    Vertex v8 = {{-0.5f,-0.5f,-0.5f},      {0.0f,0.0f}, {-1.0f, 0.0f, 0.0f}};
    Vertex v9 = {{-0.5f,-0.5f, 0.5f},      {1.0f,0.0f}, {-1.0f, 0.0f, 0.0f}};
    Vertex v10 = {{-0.5f, 0.5f, 0.5f},      {1.0f,1.0f}, {-1.0f, 0.0f, 0.0f}};
    Vertex v11 = {{-0.5f, 0.5f,-0.5f},      {0.0f,1.0f}, {-1.0f, 0.0f, 0.0f}};

    // Right face
    Vertex v12 = {{0.5f,-0.5f,-0.5f},      {1.0f,0.0f},  {1.0f, 0.0f, 0.0f}};
    Vertex v13 = {{0.5f,-0.5f, 0.5f},      {0.0f,0.0f},  {1.0f, 0.0f, 0.0f}};
    Vertex v14 = {{0.5f, 0.5f, 0.5f},      {0.0f,1.0f},  {1.0f, 0.0f, 0.0f}};
    Vertex v15 = {{0.5f, 0.5f,-0.5f},      {1.0f,1.0f},  {1.0f, 0.0f, 0.0f}};

    // Top face
    Vertex v16 = {{-0.5f, 0.5f,-0.5f},      {0.0f,0.0f},  {0.0f, 1.0f, 0.0f}};
    Vertex v17 = {{0.5f, 0.5f,-0.5f},      {1.0f,0.0f},  {0.0f, 1.0f, 0.0f}};
    Vertex v18 = {{0.5f, 0.5f, 0.5f},      {1.0f,1.0f},  {0.0f, 1.0f, 0.0f}};
    Vertex v19 = {{-0.5f, 0.5f, 0.5f},      {0.0f,1.0f},  {0.0f, 1.0f, 0.0f}};

    // Bottom face
    Vertex v20 = {{-0.5f,-0.5f,-0.5f},      {1.0f,1.0f},  {0.0f,-1.0f, 0.0f}};
    Vertex v21 = {{0.5f,-0.5f,-0.5f},      {0.0f,1.0f},  {0.0f,-1.0f, 0.0f}};
    Vertex v22 = {{0.5f,-0.5f, 0.5f},      {0.0f,0.0f},  {0.0f,-1.0f, 0.0f}};
    Vertex v23 = {{-0.5f,-0.5f, 0.5f},      {1.0f,0.0f},  {0.0f,-1.0f, 0.0f}};

    DynArray_Push_T(Vertex, &mesh->vertices, v0); DynArray_Push_T(Vertex, &mesh->vertices, v1); DynArray_Push_T(Vertex, &mesh->vertices, v2);
    DynArray_Push_T(Vertex, &mesh->vertices, v3); DynArray_Push_T(Vertex, &mesh->vertices, v4); DynArray_Push_T(Vertex, &mesh->vertices, v5);
    DynArray_Push_T(Vertex, &mesh->vertices, v6); DynArray_Push_T(Vertex, &mesh->vertices, v7); DynArray_Push_T(Vertex, &mesh->vertices, v8);
    DynArray_Push_T(Vertex, &mesh->vertices, v9); DynArray_Push_T(Vertex, &mesh->vertices, v10); DynArray_Push_T(Vertex, &mesh->vertices, v11);
    DynArray_Push_T(Vertex, &mesh->vertices, v12); DynArray_Push_T(Vertex, &mesh->vertices, v13); DynArray_Push_T(Vertex, &mesh->vertices, v14);
    DynArray_Push_T(Vertex, &mesh->vertices, v15); DynArray_Push_T(Vertex, &mesh->vertices, v16); DynArray_Push_T(Vertex, &mesh->vertices, v17);
    DynArray_Push_T(Vertex, &mesh->vertices, v18); DynArray_Push_T(Vertex, &mesh->vertices, v19); DynArray_Push_T(Vertex, &mesh->vertices, v20);
    DynArray_Push_T(Vertex, &mesh->vertices, v21); DynArray_Push_T(Vertex, &mesh->vertices, v22); DynArray_Push_T(Vertex, &mesh->vertices, v23);

    unsigned int indices[36] = 
    {
        0,1,2, 2,3,0,        // front
        4,5,6, 6,7,4,        // back
        8,9,10,10,11,8,      // left
        12,13,14,14,15,12,   // right
        16,17,18,18,19,16,   // top
        20,21,22,22,23,20    // bottom
    };

    for (size_t i = 0; i < 36; ++i)
    {
        DynArray_Push_T(unsigned int, &mesh->indices, indices[i]);
    }

    mesh->use_indices = true;

    mesh->VAO = 0;
    mesh->VBO = 0;
    mesh->EBO = 0;

    mesh->initialized = true;
}

static inline void Mesh_CreateSphere(Mesh* mesh, float radius, int stacks, int sectors)
{
    mesh->vertices = DynArray_Create_T(Vertex, 10, NULL);
    mesh->indices = DynArray_Create_T(unsigned int, 10, NULL);
    mesh->textures = DynArray_Create_T(Texture, 1, NULL);

    for (int i = 0; i <= stacks; ++i)
    {
        float phi = (float)i / (float)stacks * PI;
        float y = radius * cosf(phi);
        float r = radius * sinf(phi);

        for (int j = 0; j <= sectors; ++j)
        {
            float theta = (float)j / (float)sectors * 2.0f * PI;
            float x = r * cosf(theta);
            float z = r * sinf(theta);

            float u = (float)j / (float)sectors;
            float v = 1.0f - (float)i / (float)stacks;

            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;

            Vertex vert = { {x, y, z}, {u, v}, {nx, ny, nz} };
            DynArray_Push_T(Vertex, &mesh->vertices, vert);
        }
    }

    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < sectors; ++j)
        {
            int a = i * (sectors + 1) + j;
            int b = a + sectors + 1;

            DynArray_Push_T(unsigned int, &mesh->indices, a);
            DynArray_Push_T(unsigned int, &mesh->indices, b);
            DynArray_Push_T(unsigned int, &mesh->indices, a + 1);

            DynArray_Push_T(unsigned int, &mesh->indices, a + 1);
            DynArray_Push_T(unsigned int, &mesh->indices, b);
            DynArray_Push_T(unsigned int, &mesh->indices, b + 1);
        }
    }

    mesh->use_indices = true;
    mesh->VAO = 0;
    mesh->VBO = 0;
    mesh->EBO = 0;
    mesh->initialized = true;
}

static inline void Mesh_CreateDome(Mesh* mesh, float radius, int stacks, int sectors)
{
    mesh->vertices = DynArray_Create_T(Vertex, 10, NULL);
    mesh->indices = DynArray_Create_T(unsigned int, 10, NULL);
    mesh->textures = DynArray_Create_T(Texture, 1, NULL);

    for (int i = 0; i <= stacks; ++i)
    {
        float phi = (float)i / (float)stacks * PI;
        float y = radius * cosf(phi);
        float r = radius * sinf(phi);

        for (int j = 0; j <= sectors; ++j)
        {
            float theta = (float)j / (float)sectors * 2.0f * PI;
            float x = r * cosf(theta);
            float z = r * sinf(theta);

            float u = 1.0f - ((float)j / (float)sectors);
            float v = (float)i / (float)stacks;

            float nx = -(x / radius);
            float ny = -(y / radius);
            float nz = -(z / radius);

            Vertex vert = { {x, y, z}, {u, v}, {nx, ny, nz} };
            DynArray_Push_T(Vertex, &mesh->vertices, vert);
        }
    }

    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < sectors; ++j)
        {
            int a = i * (sectors + 1) + j;
            int b = a + sectors + 1;

            DynArray_Push_T(unsigned int, &mesh->indices, a);
            DynArray_Push_T(unsigned int, &mesh->indices, a + 1);
            DynArray_Push_T(unsigned int, &mesh->indices, b);

            DynArray_Push_T(unsigned int, &mesh->indices, a + 1);
            DynArray_Push_T(unsigned int, &mesh->indices, b + 1);
            DynArray_Push_T(unsigned int, &mesh->indices, b);
        }
    }

    mesh->use_indices = true;
    mesh->VAO = 0;
    mesh->VBO = 0;
    mesh->EBO = 0;
    mesh->initialized = true;
}

static inline void Mesh_CreateModel(Mesh* mesh, const std::string& obj_path)
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
        mesh->initialized = false;
        return;
    }

    mesh->vertices = DynArray_Create_T(Vertex, 100, NULL);
    mesh->indices = DynArray_Create_T(unsigned int, 100, NULL);
    mesh->textures = DynArray_Create_T(Texture, 1, NULL);

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

            Vertex vert = { (Vector3){pos.x, pos.y, pos.z}, (Vector2){tex.x, tex.y}, (Vector3){normal.x, normal.y, normal.z} };
            DynArray_Push_T(Vertex, &mesh->vertices, vert);
        }

        for (unsigned int f = 0; f < aimesh->mNumFaces; ++f)
        {
            const aiFace face = aimesh->mFaces[f];
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
            {
                DynArray_Push_T(unsigned int, &mesh->indices, face.mIndices[j] + indexOffset);
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
                    Texture_Create(&tex, full_tex_path.c_str(), true);
                    DynArray_Push_T(Texture, &mesh->textures, tex);
                }   
            }
        }
    }

    mesh->use_indices = true;
    mesh->VAO = 0;
    mesh->VBO = 0;
    mesh->EBO = 0;
    mesh->initialized = true;
}

static inline void Mesh_Upload(Mesh* mesh)
{
    if (!mesh->initialized)
    {
        std::cout<< "Mesh not initialized with shape" << std::endl;
        return;
    } 

    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    if (mesh->use_indices) glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, DynArray_Size(&mesh->vertices) * sizeof(Vertex), mesh->vertices.data, GL_STATIC_DRAW);

    if (mesh->use_indices)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, DynArray_Size(&mesh->indices) * sizeof(unsigned int), mesh->indices.data, GL_STATIC_DRAW);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

static inline void Mesh_Draw(const Mesh* mesh)
{
    if (!mesh->initialized)
    {
        std::cout<< "Mesh not initialized with shape" << std::endl;
        return;
    } 

    glBindVertexArray(mesh->VAO);

    if (mesh->use_indices)
        glDrawElements(GL_TRIANGLES, DynArray_Size(&mesh->indices), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, DynArray_Size(&mesh->vertices));

    glBindVertexArray(0);
}

static inline void Mesh_DrawWireFrame(const Mesh* mesh)
{
    if (!mesh->initialized)
    {
        std::cout<< "Mesh not initialized with shape" << std::endl;
        return;
    } 

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Mesh_Draw(mesh);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

static inline void Mesh_Delete(Mesh* mesh)
{
    if (mesh->EBO) glDeleteBuffers(1, &mesh->EBO);
    if (mesh->VAO) glDeleteVertexArrays(1, &mesh->VAO);
    if (mesh->VBO) glDeleteBuffers(1, &mesh->VBO);
    
    DynArray_Free(&mesh->vertices);
    DynArray_Free(&mesh->textures);
    if (mesh->use_indices) DynArray_Free(&mesh->indices);
    
    mesh->VAO = 0;
    mesh->VBO = 0;
    mesh->EBO = 0;
}

#endif