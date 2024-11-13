////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_


//////////////
// INCLUDES //
//////////////
#include <stdio.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "openglclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class TextureClass
{
private:

    // The image format we use is called Targa and has a unique header that we require this structure for.

    struct TargaHeader
    {
        unsigned char data1[12];
        unsigned short width;
        unsigned short height;
        unsigned char bpp;
        unsigned char data2;
    };

public:
    TextureClass();
    TextureClass(const TextureClass&);
    ~TextureClass();

    // The first two functions will load a texture from a given file name, and then unload that texture later when it is no longer needed.
    // The third function sets the texture as active for rendering. 
    // And finally, the last two functions provide the size of the texture to calling functions.

    bool Initialize(OpenGLClass*, char*, bool);
    void Shutdown();

    void SetTexture(OpenGLClass*, unsigned int);

    int GetWidth();
    int GetHeight();

private:

    // The LoadTarga functions loads a Targa image into an OpenGL texture.
    // If you were to use other formats such as .bmp, .dds, and so forth you would place the loading function here.

    bool LoadTarga32Bit(OpenGLClass*, char*, bool);

private:

    // The loaded boolean indicates if a texture has been loaded into this class object or not. 
    // The m_textureID is the ID number of the texture as OpenGL sees it. And then the width and height are the dimensions of the texture.

    unsigned int m_textureID;
    int m_width, m_height;
    bool m_loaded;
};

#endif
