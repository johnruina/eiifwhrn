
 
#include "Engine.h"

//OVERARCHING LIBRARIES
#include <iostream>
#include <vector>
#include <random>
#include <memory>
#include <type_traits>


//OPENGL LIBRARIES
#include<glad/glad.h>
#include<GLFW/glfw3.h>

//math libraries
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

//NON MANMADES
#include <ft2build.h>
#include FT_FREETYPE_H  

//MANMADE LIBRARIES
#include "common.h"
#include "Folder.h"
#include "Window.h"
#include "Physics.h"
#include "ResourceManager.h"
#include "font.h"
#include "Camera.h"
#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "FBO.h"
#include "texture.h"
#include "Material.h"
#include "Keyboard.h"
#include "Model.h"
#include "DirLight.h"
#include "SoundSystem.h"
#include "tFunctions.h"
#include "Debug.h"
#include "Cubemap.h"
#include "Mouse.h"
#include "QuadVertices.h"
#include "Gui.h"
#include "Particle.h"
#include "RenderSystem.h"
#include "Mesh.h"
#include "Sound.h"
#include "SoundSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Engine::Engine() {
    engine = this;
    glfwSetFramebufferSizeCallback(window->handle, framebuffer_size_callback);
    glfwSetCursorPosCallback(window->handle, mouse_callback);
    glfwSetScrollCallback(window->handle, scroll_callback);
    glfwSetKeyCallback(window->handle, key_callback);
    glfwSetMouseButtonCallback(window->handle, mouse_button_callback);

    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, window->width, window->height);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    //SYSTEM INTIALIZATION
    rendersystem = new RenderSystem();
    rendersystem->Initialize();

    //SHADERS
    Shader* SkyboxShader = new Shader("shaders/skybox.vert", "shaders/skybox.frag");
    Shader* ScreenShader = new Shader("shaders/ScreenShader.vert", "shaders/ScreenShader.frag");
    Shader* MeshShader = new Shader("shaders/default.vert", "shaders/default.frag");
    Shader* ImageBoxShader = new Shader("shaders/ImageBox.vert", "shaders/ImageBox.frag");
    Shader* BoxShader = new Shader("shaders/Box.vert", "shaders/Box.frag");
    Shader* Text2DShader = new Shader("shaders/2DText.vert", "shaders/2DText.frag");
    Shader* ParticleShader = new Shader("shaders/Particle.vert", "shaders/Particle.frag");

    rendersystem->BoxShader = BoxShader;
    rendersystem->MeshShader = MeshShader;
    rendersystem->ImageBoxShader = ImageBoxShader;
    rendersystem->Text2DShader = Text2DShader;
    rendersystem->ParticleShader = ParticleShader;
    rendersystem->SkyboxShader = SkyboxShader;
    rendersystem->ScreenShader = ScreenShader;
    //LIGHTING

    //MainDirLight.Initialize(glm::vec3(0.5f), glm::vec3(0.5f), glm::vec3(0.5f), glm::vec3(0.0f, -1.0f, 1.0f));

    camera = new Camera();
    physicsengine = new Physics();

    mainf = new Folder();

    //FONTS
    std::vector<Font> fonts;

    std::string arialdirectory = "assets/ft/arial.ttf";
    Font* arial = new Font(arialdirectory.c_str());

    //EVENTUALLY CONVERGE INTO ONE FOLDER

    //logic and shit

    unsigned long int frame = 0;
    unsigned long prev = 0;

    //STUFF

    ParticleEmitter* rain = new ParticleEmitter;
    rain->t.TranslateTo({ 0.0f,10.0f,0.0f });
    rain->t.RotateToQuaternion(glm::quat(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f)));
    rain->speed = 30.0f;
    rain->lifespan = 0.4f;
    rain->angularvelocity = { 0.0f,0.0f,glm::radians(10.0f) };
    rain->size = { 0.02f,0.2f,0.0f };
    rain->emitangle = { glm::radians(60.0f),glm::radians(360.0f) ,glm::radians(60.0f) };
    rain->facecamera = false;
    rain->color = { 1.0f,1.0f,1.0f,1.0f };
    rain->emitdirection = ParticleEmitter::EmitDirection::Perpendicular;
    rain->name = "rain";
    rain->AddToRenderSystem();
    mainf->AddChild(rain);
    
    Sound* music = new Sound();
    music->LoadSoundData(resourcemanager->LoadSoundData(L"fs.wav"));
    music->Update3DPosition(0.0f,0.0f,0.0f);
    music->PlayTrack();
    music->name = "music";
    mainf->AddChild(music);

    Box* crosshair = new Box();
    crosshair->Color = { 0.0f,0.0f,0.0f };
    crosshair->t2d.center = { 0.5f,0.5f };
    crosshair->t2d.position = { 0.5f,0.5f,0.0f,0.0f };
    crosshair->t2d.size = { 0.0f,0.0f,4.0f,4.0f };
    crosshair->AddToRenderSystem();

    ImageBox* testgui = new ImageBox();
    testgui->t2d.center = { 0.0f,1.0f };
    testgui->t2d.position = { 0.0f,1.0f,10.0f,-10.0f };
    testgui->t2d.size = { 0.0f,0.0f,744.0f / 2.0f,914.0f / 2.0f };
    testgui->Color = { 1.0f,1.0f,1.0f };
    testgui->Opacity = 0.8f;
    testgui->rounding = 0.1f;
    testgui->AddToRenderSystem();

    Texture* magic = new Texture("itsmagicbitch.jpg");
    testgui->tex = magic;

    Texture* snowflake = new Texture("assets/snowflake.png");
    rain->tex = snowflake;
    
    Model leiheng("bullet.obj");
    leiheng.t.TranslateTo({ 5.0f,10.0f,0.0f });
    leiheng.t.ScaleTo({ 1.0f,1.0f,1.0f });
    for (Mesh* mesh : leiheng.meshes) {
        auto newmesh = mesh->Clone();
        newmesh->t.TranslateBy({ 0.0f,10.0f,0.0f });
        mainf->AddChild(newmesh);
        newmesh->AddToRenderSystem();
    }
        
    Mesh* floor = CreateCubeMesh();
    floor->t.ScaleTo({ 10.0f,0.4f,10.0f });
    floor->t.TranslateTo({ 0.0f,1.0f,0.0f });
    floor->name = "floor";
    p* np = new p(floor);
    floor->AddToRenderSystem();
    np->velocity = false;
    mainf->AddChild(floor);
    physicsengine->AddObject(np);
    Mesh* cube = CreateCubeMesh();
    cube->t.ScaleTo({ 1.0f,1.0f,1.0f });
    cube->t.TranslateTo({ 5.0f,4.0f,6.5f });
    cube->name = "cube";
    cube->AddToRenderSystem();
    mainf->AddChild(cube);
}

void Engine::Initiate() {
    while (!glfwWindowShouldClose(window->handle))
    {
        ////////////////////////////////////START OF LOOP///////////////////////////////////
        frame++;

        float deltatime = 1.0f / float(fps);

        ////////////////////////////////////LOGIC////////////////////////////////////

        while (Mouse::Event buffer = mouse->Read()) {
            if (buffer.GetType() == Mouse::Event::Type::Move) {
                //std::cout << mouse.GetLastX() << ' ' << mouse.GetX() << ' ' << mouse.GetX() - mouse.GetLastX() << '\n';
                float xoffset = mouse->GetX() - mouse->GetLastX();
                float yoffset = -mouse->GetY() + mouse->GetLastY(); // reversed since y-coordinates go from bottom to top
                engine->camera->ProcessMouseMovement(xoffset, yoffset);
            }
            else if (buffer.GetType() == Mouse::Event::Type::RPress) {
                camera->lockedcursor = not camera->lockedcursor;
            }
            else if (buffer.GetType() == Mouse::Event::Type::LPress) {

            }
        }
        /*
        if (mouse->IsLeftDown()) {
            for (auto e : meshes->GetChildren()) {
                if (Mesh* mesh = static_cast<Mesh*>(e))
                    RayIntersectsMesh({ camera->t.GetTranslation(), camera->t.GetFrontVector() * 100.0f }, mesh);
            }
        }
        */
        while (Keyboard::Event buffer = keyboard->ReadKey()) {
            if (buffer.GetCode() == 'F' and buffer.IsPress()) {
                Model* newcube = new Model("cratelookingthing.obj");
                newcube->t.TranslateTo(camera->t.GetTranslation());
            }
            /*
            else  if (buffer.GetCode() == 'Z' and buffer.IsPress()) {
                for (auto e : meshes->GetChildren()) {
                    if (Mesh* mesh = static_cast<Mesh*>(e))
                        mesh->Slice(camera->t.GetTranslation(), LookAt(camera->t.GetFrontVector()));
                }
            }
            else  if (buffer.GetCode() == 'X' and buffer.IsPress()) {
                for (auto e : meshes->GetChildren()) {
                    if (Mesh* mesh = static_cast<Mesh*>(e))
                        mesh->Slice(camera->t.GetTranslation(), LookAt(camera->t.GetFrontVector()) * glm::quat({ 0.0f, 0.0f, glm::radians(90.0f) }));
                }
            }

            else  if (buffer.GetCode() == 'H' and buffer.IsPress()) {
                renderwireframe = not renderwireframe;
            }
                        */
            else  if (buffer.GetCode() == 'E' and buffer.IsPress()) {
                Mesh* nc = CreateCubeMesh();
                nc->t.ScaleTo(1.0f);
                nc->t.TranslateTo(camera->t.GetTranslation());
                nc->AddToRenderSystem();
                mainf->AddChild(nc);
                p* np = new p(nc);
                physicsengine->AddObject(np);
                
            }
        }


        if (keyboard->IsKeyDown(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window->handle, 1);
        }

        if (keyboard->IsKeyDown('W')) {
            camera->t.TranslateBy(camera->speed * camera->t.GetFrontVector());
        }
        if (keyboard->IsKeyDown('S')) {
            camera->t.TranslateBy(-(camera->speed * camera->t.GetFrontVector()));
        }
        if (keyboard->IsKeyDown('A')) {
            camera->t.TranslateBy(camera->speed * camera->t.GetRightVector());
        }
        if (keyboard->IsKeyDown('D')) {
            camera->t.TranslateBy(-(camera->speed * camera->t.GetRightVector()));
        }
        if (keyboard->IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
            camera->speed *= 1.005;
        }
        else {
            camera->speed = camera->originalspeed;
        }

        if (keyboard->IsKeyDown('R')) {

        }

        dynamic_cast<ParticleEmitter*>(mainf->GetFirstChildOfName("rain"))->Emit();
        dynamic_cast<ParticleEmitter*>(mainf->GetFirstChildOfName("rain"))->Emit();
        dynamic_cast<ParticleEmitter*>(mainf->GetFirstChildOfName("rain"))->Emit();
        dynamic_cast<ParticleEmitter*>(mainf->GetFirstChildOfName("rain"))->Emit();
        dynamic_cast<ParticleEmitter*>(mainf->GetFirstChildOfName("rain"))->Emit();
        dynamic_cast<ParticleEmitter*>(mainf->GetFirstChildOfName("rain"))->Emit();
        dynamic_cast<ParticleEmitter*>(mainf->GetFirstChildOfName("rain"))->Emit();
        dynamic_cast<ParticleEmitter*>(mainf->GetFirstChildOfName("rain"))->Step(deltatime);

        unsigned int onceeveryframes = 1;
        if (frame % onceeveryframes == 0) {

        }
        
        soundsystem->listener.Position.x = camera->t.GetTranslation().x;
        soundsystem->listener.Position.y = camera->t.GetTranslation().y;
        soundsystem->listener.Position.z = camera->t.GetTranslation().z;
        soundsystem->listener.OrientFront.x = -camera->t.GetFrontVector().x;
        soundsystem->listener.OrientFront.y = camera->t.GetFrontVector().y;
        soundsystem->listener.OrientFront.z = -camera->t.GetFrontVector().z;

        Sound* music = dynamic_cast<Sound*>(mainf->GetFirstChildOfName("music"));
        Mesh* cube = dynamic_cast<Mesh*>(mainf->GetFirstChildOfName("cube"));
        music->Update3DPosition(cube->t.GetTranslation().x, cube->t.GetTranslation().y, cube->t.GetTranslation().z);
        soundsystem->Recalculate(music->GetEmitter(), music->GetSourceVoice());
        physicsengine->Step(deltatime);

        ////////////////////////////////////RENDER SCENE////////////////////////////////////
        rendersystem->Render(*camera);
        ////////////////////////////////////END OF FRAME////////////////////////////////////
        glfwPollEvents();
    }
}

void framebuffer_size_callback(GLFWwindow* windowe, int w, int h)
{
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        keyboard->KeyDown(key);

    }
    else if (action == GLFW_RELEASE) {
        keyboard->KeyUp(key);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

void mouse_callback(GLFWwindow* windowe, double xposIn, double yposIn)
{
    mouse->OnMouseMove(static_cast<float>(xposIn), static_cast<float>(yposIn));
    if (true) {
        //MOVE IT!!!
        glfwSetInputMode(windowe, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(windowe, window->width / 2, window->height / 2);
        mouse->OnMouseMove(window->width / 2.0f, window->height / 2.0f);
    }
    else {
        glfwSetInputMode(windowe, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    {
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            if (action == GLFW_PRESS) mouse->RightDown();
            else mouse->RightUp();
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) mouse->LeftDown();
            else mouse->LeftUp();
        }
    }
}

