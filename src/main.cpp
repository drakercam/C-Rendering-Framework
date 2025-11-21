#include <stdio.h>
#include <stdlib.h>
#include "framework_master.h"

int main(void)
{
    Window window;
    if (!Window_Create(&window, 1280, 720, 60.0f, "C_Framework")) return -1;

    // Enable depth testing
    Window_EnableDepthTest();

    // Create lighting shader
    Shader light_shader;
    Shader_Create(&light_shader, "shaders/vertex.glsl", "shaders/fragment.glsl");

    // Create texture shader
    Shader tex_shader;
    Shader_Create(&tex_shader, "shaders/texture_vertex.glsl", "shaders/texture_fragment.glsl");

    // Lets create an arena to handle the mesh data
    Arena allocator = Arena_Create(8096*8096); // 4MB

    // Create a triangle
    Mesh triangle;
    Mesh_CreateTriangle(&triangle, &allocator);
    Mesh_Upload(&triangle);

    // Create the sphere
    Mesh dome;
    Mesh_CreateDome(&dome, 1.0f, 24, 48, &allocator);
    Mesh_Upload(&dome);

    // Create the cube
    Mesh cube;
    Mesh_CreateCube(&cube, &allocator);
    Mesh_Upload(&cube);

    // Create a circle
    Mesh circle;
    Mesh_CreateCircle(&circle, 1.0f, 24, &allocator);
    Mesh_Upload(&circle);

    // Create a rectangle
    Mesh rectangle;
    Mesh_CreateRectangle(&rectangle, &allocator);
    Mesh_Upload(&rectangle);

    // // Create a boombox (model)
    Mesh boombox;
    Mesh_CreateModel(&boombox, "assets/boombox_4k.obj", &allocator);
    Mesh_Upload(&boombox);

    // Create the camera
    Camera3D camera;
    Camera3D_Create(&camera, 5.0f, 90.0f, (Vector3){0.0f, 0.0f, 0.0f});

    // Create a texture to bind to the rectangle
    Texture georgia_texture;
    Texture_Create(&georgia_texture, "assets/textures/IMG_5191.JPG", true);

    // Create a texture to bind to the sphere
    Texture ocean;
    Texture_Create(&ocean, "assets/textures/star_night_sky.jpg", false);

    Vector3 light_pos_world = {50.0f, 100.0f, 25.0f};
    Vector3 light_color = {1.0f, 0.95f, 0.8f};

    Transform_Init();

    // Render loop
    while (Window_IsOpen(window))
    {
        Time_Update();
        //Window_PrintFPS();

        // Update the camera
        Camera3D_Update(&window, &camera, Time_Delta());

        Window_Clear(Colour_Crimson);

        // // Draw the Dome which will represent the world itself
        Transform_PushMatrix();

            Transform_Translate(camera.position);
            Transform_Scale((Vector3){100.0f, 100.0f, 100.0f});

            Shader_Enable(&tex_shader);
            Texture_Enable(&ocean, 0);

            // model, view, projection
            Shader_SetUniformMat4(&tex_shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(&tex_shader, "uView",       Camera3D_ViewMatrix(&camera));
            Shader_SetUniformMat4(&tex_shader, "uProjection", Math_GetProjMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform1i(&tex_shader, "uUseTexture", 1);
            Shader_SetUniform1i(&tex_shader, "uTexture", 0);
            
            Mesh_Draw(&dome);

            Texture_Disable();
            Shader_Disable();

        Transform_PopMatrix();

        // Drawing the triangle
        Transform_PushMatrix();

            Transform_Translate((Vector3){0.0f,0.0f,-5.0f});
            Transform_Rotate(Math_DegToRad(30.0f)*Time_Total(), (Vector3){1,1,0});

            Shader_Enable(&light_shader);
            Texture_Enable(&georgia_texture, 0);

            // model, view, projection
            Shader_SetUniformMat4(&light_shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(&light_shader, "uView",       Camera3D_ViewMatrix(&camera));
            Shader_SetUniformMat4(&light_shader, "uProjection", Math_GetProjMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform3f(&light_shader, "lightPos", light_pos_world);
            Shader_SetUniform3f(&light_shader, "lightColor", light_color);
            Shader_SetUniform3f(&light_shader, "viewPos", camera.position);
            Shader_SetUniform1i(&light_shader, "uUseTexture", 1);
            Shader_SetUniform1i(&light_shader, "uTexture", 0);
            Shader_SetUniform4f(&light_shader, "uColor", Colour_White);
            Mesh_Draw(&triangle);

            Texture_Disable();
            Shader_Disable();

        Transform_PopMatrix();

        // Drawing the cube
        Transform_PushMatrix();

            Transform_Translate((Vector3){-10.0f,0.0f,0.0f});
            Transform_Rotate(Math_DegToRad(24.0f)*Time_Total(), (Vector3){1,1,1});

            Shader_Enable(&light_shader);
            Texture_Enable(&georgia_texture, 0);

            // model, view, projection
            Shader_SetUniformMat4(&light_shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(&light_shader, "uView",       Camera3D_ViewMatrix(&camera));
            Shader_SetUniformMat4(&light_shader, "uProjection", Math_GetProjMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform3f(&light_shader, "lightPos", light_pos_world);
            Shader_SetUniform3f(&light_shader, "lightColor", light_color);
            Shader_SetUniform3f(&light_shader, "viewPos", camera.position);
            Shader_SetUniform1i(&light_shader, "uUseTexture", 1);
            Shader_SetUniform1i(&light_shader, "uTexture", 0);
            Shader_SetUniform4f(&light_shader, "uColor", Colour_White);
            Mesh_Draw(&cube);

            Texture_Disable();
            Shader_Disable();

        Transform_PopMatrix();

        // Drawing the rectangle
        Transform_PushMatrix();

            Transform_Translate((Vector3){10.0f,0.0f,0.0f});
            Transform_Rotate(Math_DegToRad(24.0f)*Time_Total(), (Vector3){1,1,1});

            Shader_Enable(&light_shader);
            Texture_Enable(&georgia_texture, 0);

            // model, view, projection
            Shader_SetUniformMat4(&light_shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(&light_shader, "uView",       Camera3D_ViewMatrix(&camera));
            Shader_SetUniformMat4(&light_shader, "uProjection", Math_GetProjMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform3f(&light_shader, "lightPos", light_pos_world);
            Shader_SetUniform3f(&light_shader, "lightColor", light_color);
            Shader_SetUniform3f(&light_shader, "viewPos", camera.position);
            Shader_SetUniform1i(&light_shader, "uUseTexture", 1);
            Shader_SetUniform1i(&light_shader, "uTexture", 0);
            Shader_SetUniform4f(&light_shader, "uColor", Colour_White);
            Mesh_Draw(&rectangle);

            Texture_Disable();
            Shader_Disable();

        Transform_PopMatrix();

        // Drawing the circle
        Transform_PushMatrix();

            Transform_Translate((Vector3){0.0f,0.0f,-20.0f});
            Transform_Rotate(Math_DegToRad(24.0f)*Time_Total(), (Vector3){1,1,1});

            Shader_Enable(&light_shader);
            Texture_Enable(&georgia_texture, 0);

            // model, view, projection
            Shader_SetUniformMat4(&light_shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(&light_shader, "uView",       Camera3D_ViewMatrix(&camera));
            Shader_SetUniformMat4(&light_shader, "uProjection", Math_GetProjMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform3f(&light_shader, "lightPos", light_pos_world);
            Shader_SetUniform3f(&light_shader, "lightColor", light_color);
            Shader_SetUniform3f(&light_shader, "viewPos", camera.position);
            Shader_SetUniform1i(&light_shader, "uUseTexture", 1);
            Shader_SetUniform1i(&light_shader, "uTexture", 0);
            Shader_SetUniform4f(&light_shader, "uColor", Colour_White);
            Mesh_Draw(&circle);

            Texture_Disable();
            Shader_Disable();

        Transform_PopMatrix();

        // // Draw the boom box
        Transform_PushMatrix();

            // Perform transformations
            Transform_Translate((Vector3){20.0f, 0.0f, 10.0f});
            Transform_Scale((Vector3){5.0f,5.0f,5.0f});

            Shader_Enable(&light_shader);

            // model, view, projection
            Shader_SetUniformMat4(&light_shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(&light_shader, "uView",       Camera3D_ViewMatrix(&camera));
            Shader_SetUniformMat4(&light_shader, "uProjection", Math_GetProjMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform3f(&light_shader, "lightPos", light_pos_world);
            Shader_SetUniform3f(&light_shader, "lightColor", light_color);
            Shader_SetUniform3f(&light_shader, "viewPos", camera.position);

            if (boombox.textures.data)
            {  
                Shader_SetUniform1i(&light_shader, "uUseTexture", 1);
                Shader_SetUniform1i(&light_shader, "uTexture", 0);

                Texture_Enable(&DArray_Get_T(Texture, &boombox.textures, 0), 0);
            }
            else
            {
                Shader_SetUniform1i(&light_shader, "uUseTexture", 0);
                Shader_SetUniform4f(&light_shader, "uColor", Colour_Brick);
            }

            Mesh_Draw(&boombox);

            Texture_Disable();
            Shader_Disable();

        Transform_PopMatrix();

        Window_PollEvents();
        Window_SwapBuffers(window);
    }

    // Here we delete any meshes, shaders, textures, and the window
    Shader_Delete(&light_shader);
    Shader_Delete(&tex_shader);

    Arena_Free(&allocator);

    Texture_Delete(&georgia_texture);
    Texture_Delete(&ocean);

    Window_Delete();

    return 0;
}