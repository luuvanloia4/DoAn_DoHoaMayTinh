#ifndef AudioManager_h
#define AudioManager_h

#include "../debuging/Logger.h"

#pragma warning(push, 0)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <AL/alut.h>
#pragma warning(pop)

class AudioManager {
public:
    void Initialize();

    void ListenerPosition(float x, float y, float z);
    void ListenerVelocity(float x, float y, float z);
    void ListenerGain(float gain);
    void ListenerOrientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z);

private:

};

#endif
