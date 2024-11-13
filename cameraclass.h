// We have examined how to code GLSL shaders, how to setup vertex and index buffers, and how to invoke the GLSL shaders to draw those buffers using the ColorShaderClass. The one thing we are missing however is the view point to draw them from. For this we will require a camera class to let OpenGL know from where and also how we are viewing the scene. The camera class will keep track of where the camera is and its current rotation. It will use the position and rotation information to generate a view matrix which will be passed into the GLSL shaders for rendering.

////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
private:
    struct VectorType
    {
        float x, y, z;
    };

public:
    CameraClass();
    CameraClass(const CameraClass&);
    ~CameraClass();

    // The CameraClass header is quite simple with just six functions that will be mostly used.
    // The SetPosition and SetRotation functions will be used to set the position and rotation of the camera object.
    // The GetPosition and GetRotation functions will be used to return the position and rotation of the camera object. 
    // Render will be used to create the view matrix based on the position and rotation of the camera.
    // And finally, GetViewMatrix will be used to retrieve the view matrix from the camera object so that the shaders can use it for rendering.

    void SetPosition(float, float, float);
    void SetRotation(float, float, float);

    void GetPosition(float*);
    void GetRotation(float*);

    void Render();
    void GetViewMatrix(float*);

private:
    void MatrixRotationYawPitchRoll(float*, float, float, float);
    void TransformCoord(VectorType&, float*);
    void BuildViewMatrix(float*, VectorType, VectorType, VectorType);

private:
    float m_positionX, m_positionY, m_positionZ;
    float m_rotationX, m_rotationY, m_rotationZ;
    float m_viewMatrix[16];
};

#endif
