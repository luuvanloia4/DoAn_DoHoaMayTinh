#ifndef IDrawable_h
#define IDrawable_h

#include "ShaderProgram.h"
#include "Material.h"

class IDrawable {
public:
    IDrawable() = default;
    virtual ~IDrawable() = default;

    virtual void Draw(const ShaderProgram &shader) = 0;
};

#endif