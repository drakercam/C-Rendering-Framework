#ifndef FRAMEWORK_H
#define FRAMEWORK_H

/*
    In this file, the framework is born. All the utility files are utilized together to create functions that will be used to render shit
    It wraps key utility functions I wrote which in themselves wrap OpenGL and glfw. The framework internally handles a default shader and
    the render context (the window). Its very reminiscent of Raylib and OpenFrameworks, in regards to the Push/Pop matrix functionality, and C-Like function
    calling and procedural style.
    - The framework itself uses C++ in a c-style, only utilizing key C++ features when necessary, those of which include:
        - std::string, std::vector, std::stack, namespaces, file streams, assimp, and templates
        - Otherwise, everything is C-like in structure and architecture.
        - math_utility.h is a custom vector, matrix and quaternion library I created alongside the framework to support it, its readily evolving and constantly
          being added to
*/

#include "master.h"

// The framework internally handles the window and a default shader
namespace FrameworkState
{
    static inline Window window;
    static inline Shader default_shader;
}

static inline void Framework_InitDefaultShader()
{
    Shader_Init(FrameworkState::default_shader, "shaders/vertex.glsl", "shaders/fragment.glsl");
}

static inline void Framework_InitWindow(int width, int height, float fov, std::string title)
{
    FrameworkState::window = Window_Init(width, height, fov, title);
    if (!Window_Generate(FrameworkState::window))
    {
        std::cerr << "Window was not created" << std::endl;
        exit(-1);
    }

    Framework_InitDefaultShader();
}

static inline bool Framework_WindowShouldClose()
{
    return Window_IsOpen(FrameworkState::window);
}

template<typename CameraType>
static inline void Framework_BeginFrame(CameraType& camera, const Vector4& colour)
{
    Time_Update();

    if constexpr (std::is_same_v<CameraType, Camera3D>)
    {
        Camera3D_Update(FrameworkState::window, camera, Time_Delta());
        Window_EnableDepthTesting();
    }
    else if constexpr (std::is_same_v<CameraType, Camera2D>)
    {
        Camera2D_Update(FrameworkState::window, camera, Time_Delta());
        Window_DisableDepthTesting();
    }

    Window_Clear(colour);
}

static inline void Framework_EndFrame()
{
    Window_PollEvents();
    Window_SwapBuffers(FrameworkState::window);
}

static inline void Framework_CloseWindow()
{
    Shader_Delete(FrameworkState::default_shader);
    Window_Delete();
}

template<typename CameraType>
static inline void Framework_SetMVP(Shader& shader, CameraType& camera, const Window& window)
{
    Shader_SetUniformMat4(shader, "uModel",      Transform_ModelMatrix());

    if constexpr (std::is_same_v<CameraType, Camera3D>)
    {
        Shader_SetUniformMat4(shader, "uView",       Camera3D_ViewMatrix(camera));
        Shader_SetUniformMat4(shader, "uProjection", Math_ProjectionMatrix(window.fov, window.aspect, 0.1f, 100.0f));
    }
    else if constexpr (std::is_same_v<CameraType, Camera2D>)
    {
        Shader_SetUniformMat4(shader, "uView",       Camera2D_ViewMatrix(camera));

        float left = -1.0f;
        float right = 1.0f;
        float bottom = -1.0f;
        float top = 1.0f;
        float near = -1.0f;
        float far = 1.0f;

        Shader_SetUniformMat4(shader, "uProjection", Math_OrthographicMatrix(left, right, bottom, top, near, far));
    }
}

template<typename CameraType>
static inline void Framework_DrawMesh(Mesh& mesh, const Transform& transform, CameraType& camera, Shader* shader = nullptr, Texture* tex = nullptr, Vector4 colour = Colour::White, bool push_pop = true, bool draw_fill = true, bool draw_wire = false)
{
    if (push_pop) Transform_PushMatrix();

        Transform_Translate(transform.position);

        if constexpr (std::is_same_v<CameraType, Camera3D>)
        {
            Transform_RotateX(transform.rotation.x);
            Transform_RotateY(transform.rotation.y);
            Transform_RotateZ(transform.rotation.z);
        }
        else
        {
            Transform_RotateZ(transform.rotation.z);
        }

        Transform_Scale(transform.scale);

        Shader active_shader = shader ? *shader : FrameworkState::default_shader;
        Shader_Enable(active_shader);

        Framework_SetMVP(active_shader, camera, FrameworkState::window);

        // CASE ONE: Mesh is imported and has its own textures
        if (!mesh.textures.empty())
        {
            Texture_Enable(mesh.textures[0], 0);

            Shader_SetUniform1i(active_shader, "uUseTexture", 1);
            Shader_SetUniform1i(active_shader, "uTexture", 0);
            Shader_SetUniform4f(active_shader, "uColor", colour);
            if (!draw_wire && draw_fill) Mesh_Draw(mesh);
            else if (draw_wire && !draw_fill) Mesh_DrawWireFrame(mesh);
            else
            {
                Mesh_Draw(mesh);
                Shader_SetUniform4f(active_shader, "uColor", Colour::Black);
                Mesh_DrawWireFrame(mesh);
            }

            Texture_Disable();
        }

        // CASE TWO: User provided an external texture
        else if (tex)
        {
            Texture_Enable(*tex, 0);

            Shader_SetUniform1i(active_shader, "uUseTexture", 1);
            Shader_SetUniform1i(active_shader, "uTexture", 0);
            Shader_SetUniform4f(active_shader, "uColor", colour);
            
            if (!draw_wire && draw_fill) Mesh_Draw(mesh);
            else if (draw_wire && !draw_fill) Mesh_DrawWireFrame(mesh);
            else
            {
                Mesh_Draw(mesh);
                Shader_SetUniform4f(active_shader, "uColor", Colour::Black);
                Mesh_DrawWireFrame(mesh);
            }

            Texture_Disable();
        }

        // CASE THREE: No textures provided, use a flat colour
        else
        {
            Shader_SetUniform1i(active_shader, "uUseTexture", 0);
            Shader_SetUniform4f(active_shader, "uColor", colour);

            if (!draw_wire && draw_fill) Mesh_Draw(mesh);
            else if (draw_wire && !draw_fill) Mesh_DrawWireFrame(mesh);
            else
            {
                Mesh_Draw(mesh);
                Shader_SetUniform4f(active_shader, "uColor", Colour::Black);
                Mesh_DrawWireFrame(mesh);
            }
        }
 
        Shader_Disable();

    if (push_pop) Transform_PopMatrix();
}

#endif