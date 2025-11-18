#include <stdio.h>
#include <stdlib.h>
#include "master.h"

int main(void)
{
    Window window;
    if (!Window_Create(&window, 1280, 720, 60.0f, "C_Framework")) 
        return -1;

    Window_DisableDepthTest();

    // Create texture shader
    Shader tex_shader;
    Shader_Create(&tex_shader, "shaders/texture_vertex.glsl", "shaders/texture_fragment.glsl");

    // Lets create an arena to handle the mesh data
    Arena arena = Arena_Create(4096*4096); // 4MB

    // Create the rectangle
    Mesh rectangle;
    Mesh_CreateRectangle(&rectangle, &arena);
    Mesh_Upload(&rectangle);

    // Create the camera (2D)
    Camera2D camera;
    Camera2D_Create(&camera, 0.0f, (Vector2){0.0f, 0.0f}, 0.1f, 0.0f);

    // Create a texture to bind to the rectangle
    Texture georgia_texture;
    Texture_Create(&georgia_texture, "assets/textures/IMG_5191.JPG", true);

    // // Initialize Transformation Stack
    Transform_Init();

    // Render loop
    while (Window_IsOpen(window))
    {
        Time_Update();
        //Window_PrintFPS();

        // Update the camera (2D)
        Camera2D_Update(&window, &camera, Time_Delta(), false, false, false);

        Window_Clear(Colour_Violet);

        // Draw rectangle
        Transform_PushMatrix();

        Transform_Translate((Vector3){0,0,0});
        Transform_Rotate(Math_DegToRad(45.0f*Time_Total()), (Vector3){0,0,1});
        Transform_Scale((Vector3){10.0f, 10.0f, 0.0f});

        Shader_Enable(&tex_shader);
        Texture_Enable(&georgia_texture, 0);

        Shader_SetUniformMat4(&tex_shader, "uModel", Transform_ModelMatrix());
        Shader_SetUniformMat4(&tex_shader, "uView", Camera2D_ViewMatrix(&camera));
        Shader_SetUniformMat4(&tex_shader, "uProjection",
            Math_GetOrthoMatrix(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));


        Shader_SetUniform1i(&tex_shader, "uUseTexture", 1);
        Shader_SetUniform1i(&tex_shader, "uTexture", 0);

        Mesh_Draw(&rectangle);

        Texture_Disable();
        Shader_Disable();

        Transform_PopMatrix();

        Window_PollEvents();
        Window_SwapBuffers(window);
    }

    // Here we delete any meshes, shaders, textures, and the window
    Shader_Delete(&tex_shader);

    Texture_Delete(&georgia_texture);

    Arena_Free(&arena);

    Window_Delete();

    return 0;
}