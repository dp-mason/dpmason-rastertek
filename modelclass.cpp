////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
    m_OpenGLPtr = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(OpenGLClass* OpenGL, char* textureFilename, char* modelFilename, bool wrap)
{
    bool result;


    // Store a pointer to the OpenGL object.
    m_OpenGLPtr = OpenGL;

    // Load the model.
    result = LoadModel(modelFilename);
    if(!result)
    {
        return false;
    }

    // Initialize the vertex and index buffer that hold the geometry for the triangle.
    result = InitializeBuffers();
    if(!result)
    {
        return false;
    }

    // Load the texture for this model.
    result = LoadTexture(textureFilename, wrap);
    if(!result)
    {
        return false;
    }

    

    

    return true;
}

void ModelClass::Shutdown()
{
    // Release the texture used for this model.
    ReleaseTexture();

    ReleaseModel();
    
    // Release the vertex and index buffers.
    ShutdownBuffers();

    // Release the pointer to the OpenGL object.
    m_OpenGLPtr = 0;

    m_Texture = 0;

    return;
}

void ModelClass::Render()
{
    // Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
    RenderBuffers();

    return;
}

bool ModelClass::InitializeBuffers()
{
    VertexType* vertices;
    unsigned int* indices;

    // Create the vertex array.
    vertices = new VertexType[m_vertexCount];

    // Create the index array.
    indices = new unsigned int[m_indexCount];

    // Load the vertex array and index array with data.
    for(int i=0; i<m_vertexCount; i++)
    {
        vertices[i].x  = m_model[i].x;
        vertices[i].y  = m_model[i].y;
        vertices[i].z  = m_model[i].z;
        vertices[i].tu = m_model[i].tu;
        vertices[i].tv = m_model[i].tv;
        vertices[i].nx = m_model[i].nx;
        vertices[i].ny = m_model[i].ny;
        vertices[i].nz = m_model[i].nz;

        indices[i] = i;
    }

    // Allocate an OpenGL vertex array object.
    m_OpenGLPtr->glGenVertexArrays(1, &m_vertexArrayId);

    // Bind the vertex array object to store all the buffers and vertex attributes we create here.
    m_OpenGLPtr->glBindVertexArray(m_vertexArrayId);

    // Generate an ID for the vertex buffer.
    m_OpenGLPtr->glGenBuffers(1, &m_vertexBufferId);

    // Bind the vertex buffer and load the vertex (position, uv coords, vertex normal) data into the vertex buffer.
    m_OpenGLPtr->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    m_OpenGLPtr->glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(VertexType), vertices, GL_STATIC_DRAW);

    // Enable the three vertex array attributes.
    m_OpenGLPtr->glEnableVertexAttribArray(0);  // Vertex position.
    m_OpenGLPtr->glEnableVertexAttribArray(1);  // Vertex uv coords.
    m_OpenGLPtr->glEnableVertexAttribArray(2);  // Vertex Normal.

    // Specify the location and format of the position portion of the vertex buffer.
    m_OpenGLPtr->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexType), 0);

    // Specify the location and format of the uv coords portion of the vertex buffer.
    m_OpenGLPtr->glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (3 * sizeof(float)));

    // Specify the location and format of the normal portion of the vertex buffer.
    m_OpenGLPtr->glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(VertexType), (unsigned char*)NULL + (5 * sizeof(float)));

    // Generate an ID for the index buffer.
    m_OpenGLPtr->glGenBuffers(1, &m_indexBufferId);

    // Bind the index buffer and load the index data into it.
    m_OpenGLPtr->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
    m_OpenGLPtr->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount* sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // After the vertex buffer and index buffer have been created you can delete the vertex and index arrays as they are no longer needed since the data was copied into the buffers via the glBufferData functions.

    // Now that the buffers have been loaded we can release the array data.
    delete [] vertices;
    vertices = 0;

    delete [] indices;
    indices = 0;

    return true;
}

// The ShutdownBuffers function releases the buffers, attributes, and vertex array object that were created in the InitializeBuffers function.

void ModelClass::ShutdownBuffers()
{
    // Release the vertex array object.
    m_OpenGLPtr->glBindVertexArray(0);
    m_OpenGLPtr->glDeleteVertexArrays(1, &m_vertexArrayId);

    // Release the vertex buffer.
    m_OpenGLPtr->glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_OpenGLPtr->glDeleteBuffers(1, &m_vertexBufferId);

    // Release the index buffer.
    m_OpenGLPtr->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    m_OpenGLPtr->glDeleteBuffers(1, &m_indexBufferId);

    return;
}

// RenderBuffers is called from the Render function.
// The purpose of this function is to set the vertex buffer and index buffer as active on the input assembler in the GPU by binding the OpenGL vertex array object.
// Once the GPU has an active vertex buffer it can then use the currently set shader to render that buffer.
// This function also defines how those buffers should be drawn such as triangles, lines, fans, and so forth.
// In this tutorial we set the vertex buffer and index buffer as active on the input assembler and tell the GPU that the buffers should be drawn as triangles using the glDrawElements OpenGL function.
// The glDrawElements function indicates also that we will be drawing using an index buffer.

void ModelClass::RenderBuffers()
{
    // Bind the vertex array object that stored all the information about the vertex and index buffers.
    m_OpenGLPtr->glBindVertexArray(m_vertexArrayId);

    // Render the vertex buffer using the index buffer.
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

    return;
}

// LoadTexture is a new private function that will create the texture object and then initialize it with the input file name provided.
// This function is called during initialization.

bool ModelClass::LoadTexture(char* textureFilename, bool wrap)
{
    bool result;

    
    // Create and initialize the texture object.
    m_Texture = new TextureClass;

    result = m_Texture->Initialize(m_OpenGLPtr, textureFilename, wrap);
    if(!result)
    {
        return false;
    }

    return true;
}

// The ReleaseTexture function will release the texture object that was created and loaded during the LoadTexture function.

void ModelClass::ReleaseTexture()
{
    // Release the texture object.
    if(m_Texture)
    {
        m_Texture->Shutdown();
        delete m_Texture;
        m_Texture = 0;
    }

    return;
}

void ModelClass::SetTexture(unsigned int textureUnit)
{
    // Set the texture for the model.
    m_Texture->SetTexture(m_OpenGLPtr, textureUnit);

    return;
}

bool ModelClass::LoadModel(char* filename)
{
    ifstream fin;
    char input;
    int i;


    // Open the model file.
    fin.open(filename);

    // If it could not open the file then exit.
    if(fin.fail())
    {
        return false;
    }

    // Read up to the value of vertex count.
    fin.get(input);
    while(input != ':')
    {
        fin.get(input);
    }

    // Read in the vertex count.
    fin >> m_vertexCount;

    // Set the number of indices to be the same as the vertex count.
    m_indexCount = m_vertexCount;

    // Create the model using the vertex count that was read in.
    m_model = new ModelType[m_vertexCount];

    // Read up to the beginning of the data.
    fin.get(input);
    while(input != ':')
    {
        fin.get(input);
    }
    fin.get(input);
    fin.get(input);

    // Read in the vertex data.
    for(i=0; i<m_vertexCount; i++)
    {
        fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
        fin >> m_model[i].tu >> m_model[i].tv;
        fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;

        // Invert the V coordinate to match the OpenGL texture coordinate system.
        m_model[i].tv = 1.0f - m_model[i].tv;
    }

    // Close the model file.
    fin.close();

    return true;
}

// The ReleaseModel function handles deleting the model data array.

void ModelClass::ReleaseModel()
{
    if(m_model)
    {
        delete [] m_model;
        m_model = 0;
    }

    return;
}
