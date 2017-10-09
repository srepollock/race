#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#endif
#include <string>
#include <iostream>
#include <thread>
#include "RenderEngine.h"
#include "PhysicsEngine.h"
#include "AIEngine.h"
#include "InputEngine.h"
#include "SoundEngine.h"
#include "FileEngine.h"
class Engine {
public:
    void start();
    void update();
    void stop();
    Engine();
    ~Engine();
    std::thread *_engineThread_p;
private:
	FileEngine *_fileEngine_p;
    RenderEngine *_renderEngine_p;
    PhysicsEngine *_physicsEngine_p;
    AIEngine *_aiEngine_p;
    InputEngine *_inputEngine_p;
    SoundEngine *_soundEngine_p;	
    std::thread *_physicsThread_p;
    std::thread *_aiThread_p;
    bool _running = false;
    void loop();
};
