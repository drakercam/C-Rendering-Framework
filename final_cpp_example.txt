#include <iostream>
#include "master.h"

int main(void)
{
    Window window = Window_Init(1280, 720, 60.0f, "c framework");
    if (!Window_Generate(window))
    {
        Window_Delete();
        return -1;
    }

    // Enable depth testing
    Window_EnableDepthTesting();

    // Create shader
    Shader shader;
    Shader_Init(shader, "shaders/vertex.glsl", "shaders/fragment.glsl");

    // Create a triangle
    Mesh triangle;
    Mesh_LoadTriangle(triangle);
    Mesh_Generate(triangle);

    // Create a rectangle
    Mesh rectangle;
    Mesh_LoadRectangle(rectangle);
    Mesh_Generate(rectangle);

    // Create a circle
    Mesh circle;
    Mesh_LoadCircle(circle, 1.0f, 48);
    Mesh_Generate(circle);

    // Create a cube
    Mesh cube;
    Mesh_LoadCube(cube);
    Mesh_Generate(cube);

    // Create a sphere
    Mesh sphere;
    Mesh_LoadSphere(sphere, 1.0f, 24, 48);
    Mesh_Generate(sphere);

    // Load a model from the assets folder
    Mesh car;
    Mesh_LoadModel(car, "assets/covered_car_4k.obj");
    Mesh_Generate(car);

    // Load another model from the assets folder
    Mesh boom_box;
    Mesh_LoadModel(boom_box, "assets/boombox_4k.obj");
    Mesh_Generate(boom_box);

    // Create the camera
    Camera3D camera;
    Camera3D_Init(camera, 5.0f, 90.0f, {0.0f, 0.0f, 0.0f});

    // Create a texture to bind to the rectangle
    Texture georgia_texture;
    Texture_Init(georgia_texture, "assets/textures/IMG_5191.JPG");

    Vector3 light_pos_world = {1.0f, 3.0f, 2.0f};
    Vector3 light_color = {1.0f, 0.95f, 0.8f};

    // Render loop
    while (Window_IsOpen(window))
    {
        Time_Update();

        // Update the camera
        Camera3D_Update(window, camera, Time_Delta());

        Window_Clear(Colour::White);

        // Drawing the rectangle
        Transform_PushMatrix();

            // Perform transformations here
            Transform_Translate({-5.0f,0.0f,0.0f});
            Transform_Rotate(Math_DegToRad(45.0f)*Time_Total(), {0,1,0});

            Shader_Enable(shader);
            Texture_Enable(georgia_texture, 0);

            // model, view, projection
            Shader_SetUniformMat4(shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(shader, "uView",       Camera3D_ViewMatrix(camera));
            Shader_SetUniformMat4(shader, "uProjection", Math_ProjectionMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform3f(shader, "lightPos", light_pos_world);
            Shader_SetUniform3f(shader, "lightColor", light_color);
            Shader_SetUniform3f(shader, "viewPos", camera.position);
            Shader_SetUniform1i(shader, "uUseTexture", 1);
            Shader_SetUniform1i(shader, "uTexture", 0);
            Shader_SetUniform4f(shader, "uColor", Colour::White);
            Mesh_Draw(rectangle);

            Texture_Disable();
            Shader_Disable();

        Transform_PopMatrix();

        // Drawing the triangle
        Transform_PushMatrix();

            Transform_Translate({5.0f,0.0f,0.0f});
            Transform_Rotate(Math_DegToRad(30.0f)*Time_Total(), {1,1,0});

            Shader_Enable(shader);
            Texture_Enable(georgia_texture, 0);

            // model, view, projection
            Shader_SetUniformMat4(shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(shader, "uView",       Camera3D_ViewMatrix(camera));
            Shader_SetUniformMat4(shader, "uProjection", Math_ProjectionMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform3f(shader, "lightPos", light_pos_world);
            Shader_SetUniform3f(shader, "lightColor", light_color);
            Shader_SetUniform3f(shader, "viewPos", camera.position);
            Shader_SetUniform1i(shader, "uUseTexture", 1);
            Shader_SetUniform1i(shader, "uTexture", 0);
            Shader_SetUniform4f(shader, "uColor", Colour::White);
            Mesh_Draw(triangle);

            Texture_Disable();
            Shader_Disable();

        Transform_PopMatrix();

        // Drawing the circle
        Transform_PushMatrix();

            Transform_Translate({2.0f,0.0f,7.0f});
            Transform_Rotate(Math_DegToRad(50.0f)*Time_Total(), {0,1,1});

            Shader_Enable(shader);
            Texture_Enable(georgia_texture, 0);

            // model, view, projection
            Shader_SetUniformMat4(shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(shader, "uView",       Camera3D_ViewMatrix(camera));
            Shader_SetUniformMat4(shader, "uProjection", Math_ProjectionMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform3f(shader, "lightPos", light_pos_world);
            Shader_SetUniform3f(shader, "lightColor", light_color);
            Shader_SetUniform3f(shader, "viewPos", camera.position);
            Shader_SetUniform1i(shader, "uUseTexture", 1);
            Shader_SetUniform1i(shader, "uTexture", 0);
            Shader_SetUniform4f(shader, "uColor", Colour::White);
            Mesh_Draw(circle);

            Texture_Disable();
            Shader_Disable();

        Transform_PopMatrix();

        // Drawing the cube
        Transform_PushMatrix();

            Transform_Translate({0.0f,0.0f,-10.0f});
            Transform_Rotate(Math_DegToRad(25.0f)*Time_Total(), {1,1,1});

            Shader_Enable(shader);
            Texture_Enable(georgia_texture, 0);

            // model, view, projection
            Shader_SetUniformMat4(shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(shader, "uView",       Camera3D_ViewMatrix(camera));
            Shader_SetUniformMat4(shader, "uProjection", Math_ProjectionMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform3f(shader, "lightPos", light_pos_world);
            Shader_SetUniform3f(shader, "lightColor", light_color);
            Shader_SetUniform3f(shader, "viewPos", camera.position);
            Shader_SetUniform1i(shader, "uUseTexture", 1);
            Shader_SetUniform1i(shader, "uTexture", 0);
            Shader_SetUniform4f(shader, "uColor", Colour::White);
            Mesh_Draw(cube);

            Texture_Disable();
            Shader_Disable();

        Transform_PopMatrix();

        // Drawing the sphere
        Transform_PushMatrix();

            Transform_Translate({0.0f,0.0f,10.0f});

            Shader_Enable(shader);
            Texture_Enable(georgia_texture, 0);

            // model, view, projection
            Shader_SetUniformMat4(shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(shader, "uView",       Camera3D_ViewMatrix(camera));
            Shader_SetUniformMat4(shader, "uProjection", Math_ProjectionMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform3f(shader, "lightPos", light_pos_world);
            Shader_SetUniform3f(shader, "lightColor", light_color);
            Shader_SetUniform3f(shader, "viewPos", camera.position);
            Shader_SetUniform1i(shader, "uUseTexture", 1);
            Shader_SetUniform1i(shader, "uTexture", 0);
            Shader_SetUniform4f(shader, "uColor", Colour::White);
            Mesh_Draw(sphere);

            Texture_Disable();
            Shader_Disable();

        Transform_PopMatrix();

        // Draw the car
        Transform_PushMatrix();

            // Perform transformations
            Transform_Translate({-20.0f, 0.0f, 10.0f});

            Shader_Enable(shader);

            // model, view, projection
            Shader_SetUniformMat4(shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(shader, "uView",       Camera3D_ViewMatrix(camera));
            Shader_SetUniformMat4(shader, "uProjection", Math_ProjectionMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform3f(shader, "lightPos", light_pos_world);
            Shader_SetUniform3f(shader, "lightColor", light_color);
            Shader_SetUniform3f(shader, "viewPos", camera.position);

            if (!car.textures.empty())
            {   
                Shader_SetUniform1i(shader, "uUseTexture", 1);
                Shader_SetUniform1i(shader, "uTexture", 0);

                Texture_Enable(car.textures[0], 0);
            }
            else
            {
                Shader_SetUniform1i(shader, "uUseTexture", 0);
                Shader_SetUniform4f(shader, "uColor", Colour::Chocolate);
            }

            Mesh_Draw(car);

            Texture_Disable();
            Shader_Disable();

        Transform_PopMatrix();

        // Draw the boom box
        Transform_PushMatrix();

            // Perform transformations
            Transform_Translate({20.0f, 0.0f, 10.0f});
            Transform_Scale({5.0f,5.0f,5.0f});

            Shader_Enable(shader);

            // model, view, projection

            Shader_SetUniformMat4(shader, "uModel",      Transform_ModelMatrix());
            Shader_SetUniformMat4(shader, "uView",       Camera3D_ViewMatrix(camera));
            Shader_SetUniformMat4(shader, "uProjection", Math_ProjectionMatrix(window.fov, window.aspect, 0.1f, 100.0f));

            Shader_SetUniform3f(shader, "lightPos", light_pos_world);
            Shader_SetUniform3f(shader, "lightColor", light_color);
            Shader_SetUniform3f(shader, "viewPos", camera.position);

            if (!boom_box.textures.empty())
            {  
                Shader_SetUniform1i(shader, "uUseTexture", 1);
                Shader_SetUniform1i(shader, "uTexture", 0);

                Texture_Enable(boom_box.textures[0], 0);
            }
            else
            {
                Shader_SetUniform1i(shader, "uUseTexture", 0);
                Shader_SetUniform4f(shader, "uColor", Colour::Brick);
            }

            Mesh_Draw(boom_box);

            Texture_Disable();
            Shader_Disable();

        Transform_PopMatrix();

        Window_PollEvents();
        Window_SwapBuffers(window);
    }

    // Here we delete any meshes, shaders, textures, and the window
    Shader_Delete(shader);

    Mesh_Delete(triangle);
    Mesh_Delete(rectangle);
    Mesh_Delete(cube);
    Mesh_Delete(sphere);
    Mesh_Delete(circle);
    Mesh_Delete(car);
    Mesh_Delete(boom_box);

    Texture_Delete(georgia_texture);

    Window_Delete();

    return 0;
}