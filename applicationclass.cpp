////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"
#include <math.h>

ApplicationClass::ApplicationClass()
{
    m_OpenGL = 0;
    m_LightShader = 0;
    m_Model = 0;
    m_Camera = 0;
    m_Light = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(Display* display, Window win, int screenWidth, int screenHeight)
{
    char textureFilename[128];
    char modelFilename[128];
    bool result;


    // Create and initialize the OpenGL object.
    m_OpenGL = new OpenGLClass;

    result = m_OpenGL->Initialize(display, win, screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH, VSYNC_ENABLED);
    if(!result)
    {
        cout << "Error: Could not initialize the OpenGL object." << endl;
        return false;
    }

    // Create and initialize the color shader object.
    m_LightShader = new LightShaderClass;

    result = m_LightShader->Initialize(m_OpenGL);
    if(!result)
    {
        cout << "Error: Could not initialize the light shader object." << endl;
        return false;
    }

    // Create and initialize the camera object.
    m_Camera = new CameraClass;

    m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
    m_Camera->Render();

    strcpy(textureFilename, "./walz.tga");
    strcpy(modelFilename, "./cube.txt");

    // Create and initialize the model object.
    m_Model = new ModelClass;

    result = m_Model->Initialize(m_OpenGL, textureFilename, modelFilename, true);
    if(!result)
    {
        cout << "Error: Could not initialize the model object." << endl;
        return false;
    }

    m_Light = new LightClass;
    m_Light->SetDiffuseColor(0.0, 1.0, 0.0, 1.0);
    // if I want a light that faces forward why would I not use -1? The camera direction vector is (0,0,-1)...
    m_Light->SetDirection(0.0, 0.0, 1.0);

    return true;
}


void ApplicationClass::Shutdown()
{
    // Release the texture shader object.
    if(m_LightShader)
    {
        m_LightShader->Shutdown();
        delete m_LightShader;
        m_LightShader = 0;
    }

    // Release Light
    if(m_Light)
    {
        delete m_Light;
        m_Light = 0;
    }

    // Release the model object.
    if(m_Model)
    {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = 0;
    }

    // Release the camera object.
    if(m_Camera)
    {
        delete m_Camera;
        m_Camera = 0;
    }

    // Release the OpenGL object.
    if(m_OpenGL)
    {
        m_OpenGL->Shutdown();
        delete m_OpenGL;
        m_OpenGL = 0;
    }

    return;
}


bool ApplicationClass::Frame(InputClass* Input)
{
    static float rotation = 360.0f;
    bool result;


    // Check if the escape key has been pressed, if so quit.
    if(Input->IsEscapePressed() == true)
    {
        return false;
    }

    // Update the rotation variable each frame.
    rotation -= 0.0174532925f * 1.0f;
    if(rotation <= 0.0f)
    {
        rotation += 360.0f;
    }

    // Render the graphics scene.
    result = Render(rotation);
    if(!result)
    {
        return false;
    }

    return true;
}


bool ApplicationClass::Render(float rotation)
{
    float worldMatrix[16], viewMatrix[16], projectionMatrix[16];
    float diffuseLightColor[4], lightDirection[3];
    bool result;


    // Clear the buffers to begin the scene.
    m_OpenGL->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);



    // Get the world, view, and projection matrices from the opengl and camera objects.
    m_OpenGL->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_OpenGL->GetProjectionMatrix(projectionMatrix);

    // Perform the rotation. This works because worldMatrix is a pointer to a matrix shared by us and OpenGL  
    // m_OpenGL->MatrixRotationY(worldMatrix, rotation);
    rotation = rotation * 10.0f;
    float transformMatrix[16] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, cos(rotation*(3.14f/180.0f)), -sin(rotation*(3.14f/180.0f)), 0.0,
        0.0, sin(rotation*(3.14f/180.0f)), cos(rotation*(3.14f/180.0f)), 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    float worldMatrixCopy[16] = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    std::copy(worldMatrix, worldMatrix + 16, worldMatrixCopy);
    
    m_OpenGL->MatrixMultiply(worldMatrix, transformMatrix, worldMatrixCopy);

    m_Light->GetDiffuseColor(diffuseLightColor);
    m_Light->GetDirection(lightDirection);

    // Set the color shader as the current shader program and set the matrices that it will use for rendering.
    result = m_LightShader->SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix, lightDirection, diffuseLightColor);
    if(!result)
    {
        return false;
    }

    // Set the texture for the model in the pixel shader.
    m_Model->SetTexture(0);

    // Render the model.
    m_Model->Render();

    // Present the rendered scene to the screen.
    m_OpenGL->EndScene();

    return true;
}
