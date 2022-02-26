#ifndef IShaderProperty_h
#define IShaderProperty_h

#include "ShaderProgram.h"

class IShaderProperty {
public:
    IShaderProperty() = default;
    IShaderProperty(const IShaderProperty&) = delete;
    IShaderProperty& operator=(const IShaderProperty&) = delete;
    IShaderProperty(IShaderProperty&&) = delete;
    IShaderProperty& operator=(IShaderProperty&&) = delete;
    ~IShaderProperty() = default;

    virtual void SetProperty(const ShaderProgram& shader) const = 0;
};

#endif
