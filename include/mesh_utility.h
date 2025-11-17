#ifndef MESH_UTILITY_H
#define MESH_UTILITY_H

#include <stdbool.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_utility.h"
#include "texture_utility.h"
#include "darray_utility.h"
#include "string_utility.h"
#include "arena_utility.h"
#include <math.h>

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
    DArray vertices;     // holds vertex structs
    DArray indices;      // holds unsigned ints
    DArray textures;     // holds textures
    //std::vector<float> vertices;
    //std::vector<unsigned int> indices;
    //std::vector<Texture> textures;

    bool initialized;
    bool use_indices;

} Mesh;

// ALWAYS SET THE SHAPE BEFORE YOU INITIALIZE

static inline void Mesh_CreateTriangle(Mesh* mesh, Arena* allocator)
{
    mesh->vertices = DArray_Create_T(Vertex, 3, allocator);
    mesh->textures = DArray_Create_T(Texture, 1, allocator);

    Vertex v[3] = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f,0.0f}, {0.0f,0.0f,1.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {1.0f,0.0f}, {0.0f,0.0f,1.0f}},
        {{ 0.0f,  0.5f, 0.0f}, {0.5f,1.0f}, {0.0f,0.0f,1.0f}}
    };

    for (int i=0;i<3;i++) DArray_Push_T(Vertex,&mesh->vertices,v[i]);

    mesh->use_indices = false;
    mesh->VAO = mesh->VBO = mesh->EBO = 0;
    mesh->initialized = true;
}

static inline void Mesh_CreateRectangle(Mesh* mesh, Arena* allocator)
{
    mesh->vertices = DArray_Create_T(Vertex, 4, allocator);
    mesh->indices = DArray_Create_T(unsigned int, 6, allocator);
    mesh->textures = DArray_Create_T(Texture, 1, allocator);

    Vertex v[4] = {
        {{ 0.5f,  0.5f, 0.0f}, {1.0f,1.0f}, {0.0f,0.0f,1.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {1.0f,0.0f}, {0.0f,0.0f,1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f,0.0f}, {0.0f,0.0f,1.0f}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f,1.0f}, {0.0f,0.0f,1.0f}}
    };
    for(int i=0;i<4;i++) DArray_Push_T(Vertex,&mesh->vertices,v[i]);

    unsigned int idx[6] = {0,1,3,1,2,3};
    for(int i=0;i<6;i++) DArray_Push_T(unsigned int,&mesh->indices,idx[i]);

    mesh->use_indices = true;
    mesh->VAO = mesh->VBO = mesh->EBO = 0;
    mesh->initialized = true;
}

static inline void Mesh_CreateCircle(Mesh* mesh, float radius, int sectors, Arena* allocator)
{
    int vertex_count = sectors + 2; // center + perimeter + duplicate for closure
    int index_count = sectors * 3;

    mesh->vertices = DArray_Create_T(Vertex, vertex_count, allocator);
    mesh->indices  = DArray_Create_T(unsigned int, index_count, allocator);
    mesh->textures = DArray_Create_T(Texture, 1, allocator);

    Vertex center = {{0,0,0},{0.5f,0.5f},{0,0,1}};
    DArray_Push_T(Vertex,&mesh->vertices,center);

    for(int i=0;i<=sectors;i++)
    {
        float theta = (float)i/sectors * 2*PI;
        Vertex v = {{radius*cosf(theta),radius*sinf(theta),0},
                    {0.5f + 0.5f*cosf(theta),0.5f + 0.5f*sinf(theta)},
                    {0,0,1}};
        DArray_Push_T(Vertex,&mesh->vertices,v);
    }

    for(int i=1;i<=sectors;i++)
    {
        DArray_Push_T(unsigned int,&mesh->indices,0);
        DArray_Push_T(unsigned int,&mesh->indices,i);
        DArray_Push_T(unsigned int,&mesh->indices,i+1);
    }

    mesh->use_indices = true;
    mesh->VAO = mesh->VBO = mesh->EBO = 0;
    mesh->initialized = true;
}

static inline void Mesh_CreateCube(Mesh* mesh, Arena* allocator)
{
    int vertex_count = 24;
    int index_count  = 36;

    mesh->vertices = DArray_Create_T(Vertex, vertex_count, allocator);
    mesh->indices  = DArray_Create_T(unsigned int, index_count, allocator);
    mesh->textures = DArray_Create_T(Texture, 1, allocator);

    Vertex verts[24] = {
        // front
        {{-0.5f,-0.5f,0.5f},{0,0},{0,0,1}},{{0.5f,-0.5f,0.5f},{1,0},{0,0,1}},{{0.5f,0.5f,0.5f},{1,1},{0,0,1}},{{-0.5f,0.5f,0.5f},{0,1},{0,0,1}},
        // back
        {{-0.5f,-0.5f,-0.5f},{1,0},{0,0,-1}},{{0.5f,-0.5f,-0.5f},{0,0},{0,0,-1}},{{0.5f,0.5f,-0.5f},{0,1},{0,0,-1}},{{-0.5f,0.5f,-0.5f},{1,1},{0,0,-1}},
        // left
        {{-0.5f,-0.5f,-0.5f},{0,0},{-1,0,0}},{{-0.5f,-0.5f,0.5f},{1,0},{-1,0,0}},{{-0.5f,0.5f,0.5f},{1,1},{-1,0,0}},{{-0.5f,0.5f,-0.5f},{0,1},{-1,0,0}},
        // right
        {{0.5f,-0.5f,-0.5f},{1,0},{1,0,0}},{{0.5f,-0.5f,0.5f},{0,0},{1,0,0}},{{0.5f,0.5f,0.5f},{0,1},{1,0,0}},{{0.5f,0.5f,-0.5f},{1,1},{1,0,0}},
        // top
        {{-0.5f,0.5f,-0.5f},{0,0},{0,1,0}},{{0.5f,0.5f,-0.5f},{1,0},{0,1,0}},{{0.5f,0.5f,0.5f},{1,1},{0,1,0}},{{-0.5f,0.5f,0.5f},{0,1},{0,1,0}},
        // bottom
        {{-0.5f,-0.5f,-0.5f},{1,1},{0,-1,0}},{{0.5f,-0.5f,-0.5f},{0,1},{0,-1,0}},{{0.5f,-0.5f,0.5f},{0,0},{0,-1,0}},{{-0.5f,-0.5f,0.5f},{1,0},{0,-1,0}}
    };
    for(int i=0;i<24;i++) DArray_Push_T(Vertex,&mesh->vertices,verts[i]);

    unsigned int idx[36] = {
        0,1,2,2,3,0, 4,5,6,6,7,4, 8,9,10,10,11,8, 12,13,14,14,15,12, 16,17,18,18,19,16, 20,21,22,22,23,20
    };
    for(int i=0;i<36;i++) DArray_Push_T(unsigned int,&mesh->indices,idx[i]);

    mesh->use_indices = true;
    mesh->VAO = mesh->VBO = mesh->EBO = 0;
    mesh->initialized = true;
}

static inline void Mesh_CreateSphere(Mesh* mesh, float radius, int stacks, int sectors, Arena* allocator)
{
    int vertex_count = (stacks+1)*(sectors+1);
    int index_count = stacks*sectors*6;

    mesh->vertices = DArray_Create_T(Vertex, vertex_count, allocator);
    mesh->indices  = DArray_Create_T(unsigned int, index_count, allocator);
    mesh->textures = DArray_Create_T(Texture, 1, allocator);

    for(int i=0;i<=stacks;i++)
    {
        float phi = (float)i/stacks*PI;
        float y = radius*cosf(phi);
        float r = radius*sinf(phi);

        for(int j=0;j<=sectors;j++)
        {
            float theta = (float)j/sectors*2*PI;
            float x = r*cosf(theta), z = r*sinf(theta);
            Vertex v = {{x,y,z},{(float)j/sectors,1.0f-(float)i/stacks},{x/radius,y/radius,z/radius}};
            DArray_Push_T(Vertex,&mesh->vertices,v);
        }
    }

    for(int i=0;i<stacks;i++)
    {
        for(int j=0;j<sectors;j++)
        {
            int a=i*(sectors+1)+j;
            int b=a+sectors+1;
            DArray_Push_T(unsigned int,&mesh->indices,a); DArray_Push_T(unsigned int,&mesh->indices,b); DArray_Push_T(unsigned int,&mesh->indices,a+1);
            DArray_Push_T(unsigned int,&mesh->indices,a+1); DArray_Push_T(unsigned int,&mesh->indices,b); DArray_Push_T(unsigned int,&mesh->indices,b+1);
        }
    }

    mesh->use_indices = true;
    mesh->VAO = mesh->VBO = mesh->EBO = 0;
    mesh->initialized = true;
}

static inline void Mesh_CreateDome(Mesh* mesh, float radius, int stacks, int sectors, Arena* allocator)
{
    int vertex_count = (stacks + 1) * (sectors + 1);
    int index_count  = stacks * sectors * 6;

    mesh->vertices = DArray_Create_T(Vertex, vertex_count, allocator);
    mesh->indices  = DArray_Create_T(unsigned int, index_count, allocator);
    mesh->textures = DArray_Create_T(Texture, 1, allocator);

    // Generate vertices
    for (int i = 0; i <= stacks; i++)
    {
        float phi = (float)i / stacks * PI; // full sphere
        float y = radius * cosf(phi);
        float r = radius * sinf(phi);

        for (int j = 0; j <= sectors; j++)
        {
            float theta = (float)j / sectors * 2.0f * PI;
            float x = r * cosf(theta);
            float z = r * sinf(theta);

            // Inward normals for skybox
            Vertex v = {
                { x, y, z },
                { 1.0f - (float)j / sectors, (float)i / stacks },
                { -x / radius, -y / radius, -z / radius } // inward
            };
            DArray_Push_T(Vertex, &mesh->vertices, v);
        }
    }

    // Generate indices
    for (int i = 0; i < stacks; i++)
    {
        for (int j = 0; j < sectors; j++)
        {
            int a = i * (sectors + 1) + j;
            int b = a + sectors + 1;

            // Clockwise winding for inward-facing triangles
            DArray_Push_T(unsigned int, &mesh->indices, a);
            DArray_Push_T(unsigned int, &mesh->indices, b);
            DArray_Push_T(unsigned int, &mesh->indices, a + 1);

            DArray_Push_T(unsigned int, &mesh->indices, a + 1);
            DArray_Push_T(unsigned int, &mesh->indices, b);
            DArray_Push_T(unsigned int, &mesh->indices, b + 1);
        }
    }

    mesh->use_indices = true;
    mesh->VAO = mesh->VBO = mesh->EBO = 0;
    mesh->initialized = true;
}

static inline void Mesh_Upload(Mesh* mesh)
{
    if (!mesh->initialized)
    {
        printf("Mesh not initialized with shape\n");
        return;
    } 

    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    if (mesh->use_indices) glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, DArray_Size(&mesh->vertices) * sizeof(Vertex), mesh->vertices.data, GL_STATIC_DRAW);

    if (mesh->use_indices)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, DArray_Size(&mesh->indices) * sizeof(unsigned int), mesh->indices.data, GL_STATIC_DRAW);
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
        printf("Mesh not initialized with shape\n");
        return;
    } 

    glBindVertexArray(mesh->VAO);

    if (mesh->use_indices)
        glDrawElements(GL_TRIANGLES, DArray_Size(&mesh->indices), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, DArray_Size(&mesh->vertices));

    glBindVertexArray(0);
}

static inline void Mesh_DrawWireFrame(const Mesh* mesh)
{
    if (!mesh->initialized)
    {
        printf("Mesh not initialized with shape\n");
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
    
    DArray_Free(&mesh->vertices);
    DArray_Free(&mesh->textures);
    if (mesh->use_indices) DArray_Free(&mesh->indices);
    
    mesh->VAO = 0;
    mesh->VBO = 0;
    mesh->EBO = 0;
}

#endif