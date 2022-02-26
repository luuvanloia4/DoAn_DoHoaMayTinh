#ifndef ResourcesManager_h
#define ResourcesManager_h

#include "RawTexture.h"
#include "RawModel/RawModel.h"
//#include "Sound.h"
#include "../debuging/Logger.h"

#include <string>
#include <unordered_map>

constexpr const char* ERROR_TEXTURE_PATH = "resources/textures/ErrorTexture.png";
constexpr const char* ERROR_MODEL3D_PATH = "resources/models/ErrorMesh/ErrorMesh.obj";

class ResourcesManager {
public:
    RawTexture& LoadTexture(std::string path);
    RawModel& LoadModel(std::string path);
    //Sound& LoadSound(std::string path);

private:
    std::unordered_map<std::string, RawTexture> m_Textures;
    std::unordered_map<std::string, RawModel> m_Models;
    //std::unordered_map<std::string, Sound> m_Sounds;
};

#endif 
