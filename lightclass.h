// Now we will look at the new light class which is very simple. Its purpose is only to maintain the direction and color of lights.

////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_


////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
class LightClass
{
public:
    LightClass();
    LightClass(const LightClass&);
    ~LightClass();

    void SetDiffuseColor(float, float, float, float);
    void SetDirection(float, float, float);

    void GetDiffuseColor(float*);
    void GetDirection(float*);

private:
    float m_diffuseColor[4];
    float m_direction[3];
};

#endif


