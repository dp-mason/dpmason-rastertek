////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <fstream>
#include "textureclass.h"
using namespace std;



///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "openglclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class Name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:

    struct VertexType
    {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz; // Vertex Normal Vector
    };
    struct ModelType
    {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
    };

public:
    ModelClass();
    ModelClass(const ModelClass&);
    ~ModelClass();

    // 2nd arg is texture file path, 3rd indicates whether texture should wrap or clamp
    bool Initialize(OpenGLClass*, char*, char*, bool);
    void Shutdown();
    void Render();

    void SetTexture(unsigned int);

private:
    bool InitializeBuffers();
    void ShutdownBuffers();
    void RenderBuffers();

    bool LoadTexture(char*, bool);
    void ReleaseTexture();

    bool LoadModel(char*);
    void ReleaseModel();

private:
    OpenGLClass* m_OpenGLPtr;
    int m_vertexCount, m_indexCount;
    unsigned int m_vertexArrayId, m_vertexBufferId, m_indexBufferId;

    TextureClass* m_Texture;
    ModelType* m_model;
};

#endif
