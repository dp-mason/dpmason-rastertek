////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureclass.h"

//The class constructor will initialize the loaded boolean to false so that we know there has not been a texture loaded yet.

TextureClass::TextureClass()
{
    m_loaded = false;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}

// Initialize takes in the OpenGL pointer, the file name of the texture, and a boolean value indicating if the texture should wrap or clamp the colors at the edges. It then loads the Targa file by calling the LoadTarga function. The texture can now be used to render with.

bool TextureClass::Initialize(OpenGLClass* OpenGL, char* filename, bool wrap)
{
    bool result;


    // Load the texture from the file.
    result = LoadTarga32Bit(OpenGL, filename, wrap);
    if(!result)
    {
        return false;
    }

    // Set that the texture is loaded.
    m_loaded = true;

    return true;
}

// The Shutdown function releases the texture resource if it had been successfully loaded earlier.

void TextureClass::Shutdown()
{
    // If the texture was loaded then make sure to release it on shutdown.
    if(m_loaded)
    {
        glDeleteTextures(1, &m_textureID);
        m_loaded = false;
    }

    return;
}

// This is our Targa image loading function. Once again note that Targa images need the red and blue channel flipped before using. So here we will open the file, read it into an array, and then take that array data and load it into the m_targaData array in the correct order.

// Note also that we are purposely only dealing with 32-bit Targa files that have alpha channels. This function will reject Targas that are saved as 24-bit or any other format.

bool TextureClass::LoadTarga32Bit(OpenGLClass* OpenGL, char* filename, bool wrap)
{
    TargaHeader targaFileHeader;
    FILE* filePtr;
    int bpp, error, index, i, j;
    unsigned long count, imageSize;
    unsigned char* targaData;
    unsigned char* targaImage;

    // First read the 32-bit Targa file into a buffer.

    // Open the targa file for reading in binary.
    filePtr = fopen(filename, "rb");
    if(filePtr == NULL)
    {
        return false;
    }

    // Read in the file header.
    count = fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
    if(count != 1)
    {
        return false;
    }

    // Get the important information from the header.
    m_width = (int)targaFileHeader.width;
    m_height = (int)targaFileHeader.height;
    bpp = (int)targaFileHeader.bpp;

    // Check that it is 32 bit and not 24 bit.
    if(bpp != 32)
    {
        return false;
    }

    // Calculate the size of the 32 bit image data.
    imageSize = m_width * m_height * 4;

    // Allocate memory for the targa image data.
    targaImage = new unsigned char[imageSize];

    // Read in the targa image data.
    count = fread(targaImage, 1, imageSize, filePtr);
    if(count != imageSize)
    {
        return false;
    }

    // Close the file.
    error = fclose(filePtr);
    if(error != 0)
    {
        return false;
    }

    // Now use a second buffer to flip the image data from the first buffer in the correct order.

    // Allocate memory for the targa destination data.
    targaData = new unsigned char[imageSize];

    // Initialize the index into the targa destination data array.
    index = 0;

    // Now copy the targa image data into the targa destination array in the correct order since the targa format is not stored in the RGBA order.
    for(j=0; j<m_height; j++)
    {
        for(i=0; i<m_width; i++)
        {
            targaData[index + 0] = targaImage[index + 2];  // Red.
            targaData[index + 1] = targaImage[index + 1];  // Green.
            targaData[index + 2] = targaImage[index + 0];  // Blue
            targaData[index + 3] = targaImage[index + 3];  // Alpha

            // Increment the indexes into the targa data.
            index += 4;
        }
    }

    // Release the targa image data now that it was copied into the destination array.
    delete [] targaImage;
    targaImage = 0;

    // Now that the buffer contains the correct image data, we create an OpenGL texture object and copy the buffer into that texture object.

    // Set the active texture unit in which to store the data.
    OpenGL->glActiveTexture(GL_TEXTURE0 + 0);

    // Generate an ID for the texture.
    glGenTextures(1, &m_textureID);

    // Bind the texture as a 2D texture.
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Load the image data into the texture unit.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, targaData);

    // Once the texture has been loaded, we can set the wrap, filtering, and generate mipmaps for it.

    // Set the texture color to either wrap around or clamp to the edge.
    if(wrap)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    // Set the texture filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Generate mipmaps for the texture.
    OpenGL->glGenerateMipmap(GL_TEXTURE_2D);

    // Release the targa image data.
    delete [] targaData;
    targaData = 0;

    return true;
}

// The SetTexture function sets this texture as the current rendering texture in the specific texture unit. For this tutorial we only use one texture so it will be stored in texture unit zero.

void TextureClass::SetTexture(OpenGLClass* OpenGL, unsigned int textureUnit)
{
    if(m_loaded)
    {
        // Set the texture unit we are working with.
        OpenGL->glActiveTexture(GL_TEXTURE0 + textureUnit);

        // Bind the texture as a 2D texture.
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }

    return;
}


int TextureClass::GetWidth()
{
    return m_width;
}


int TextureClass::GetHeight()
{
    return m_height;
}
