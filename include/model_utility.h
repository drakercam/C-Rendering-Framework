#ifndef MODEL_UTILITY_H
#define MODEL_UTILITY_H

#include "mesh_utility.h"
#include "dyn_array_utility.h"

#ifdef __cplusplus

// ===========================================================
//  C++ version — Uses Assimp
// ===========================================================

#include <string>
#include <stdio.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>

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
        printf("Assimp Error: %s\n", importer.GetErrorString());
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
#else
// ===========================================================
//  C version — Tiny OBJ loader (no Assimp)
// ===========================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static inline void Mesh_CreateModel(Mesh* mesh, const char* obj_path)
{
    FILE* file = fopen(obj_path, "r");
    if (!file)
    {
        printf("Failed to open model file: %s\n", obj_path);
        mesh->initialized = false;
        return;
    }

    mesh->vertices = DynArray_Create_T(Vertex, 256, NULL);
    mesh->indices = DynArray_Create_T(unsigned int, 256, NULL);
    mesh->textures = DynArray_Create_T(Texture, 1, NULL);

    float vx[10000][3];
    float vt[10000][2];
    float vn[10000][3];
    unsigned int vCount = 0, vtCount = 0, vnCount = 0;

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, "v ", 2) == 0)
        {
            sscanf(line, "v %f %f %f", &vx[vCount][0], &vx[vCount][1], &vx[vCount][2]);
            vCount++;
        }
        else if (strncmp(line, "vt ", 3) == 0)
        {
            sscanf(line, "vt %f %f", &vt[vtCount][0], &vt[vtCount][1]);
            vtCount++;
        }
        else if (strncmp(line, "vn ", 3) == 0)
        {
            sscanf(line, "vn %f %f %f", &vn[vnCount][0], &vn[vnCount][1], &vn[vnCount][2]);
            vnCount++;
        }
        else if (strncmp(line, "f ", 2) == 0)
        {
            unsigned int vIdx[3], tIdx[3], nIdx[3];
            int count = sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u",
                               &vIdx[0], &tIdx[0], &nIdx[0],
                               &vIdx[1], &tIdx[1], &nIdx[1],
                               &vIdx[2], &tIdx[2], &nIdx[2]);
            if (count == 9)
            {
                for (int i = 0; i < 3; ++i)
                {
                    Vertex vert = {
                        {vx[vIdx[i]-1][0], vx[vIdx[i]-1][1], vx[vIdx[i]-1][2]},
                        {vt[tIdx[i]-1][0], vt[tIdx[i]-1][1]},
                        {vn[nIdx[i]-1][0], vn[nIdx[i]-1][1], vn[nIdx[i]-1][2]}
                    };
                    DynArray_Push_T(Vertex, &mesh->vertices, vert);
                    DynArray_Push_T(unsigned int, &mesh->indices, DynArray_Size(&mesh->vertices) - 1);
                }
            }
        }
    }

    fclose(file);

    mesh->use_indices = true;
    mesh->initialized = true;
}

#endif // __cplusplus

#endif // MODEL_UTILITY_H