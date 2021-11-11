#pragma once
#include "Types.h"
#include "ColorBuffer.h"
#include "Buffer.h"
#include <string>

class Scene;
class Camera;

class Tracer
{
public:
	void Trace(Buffer* buffer, Scene* scene, Camera* camera, const std::string& message);

public:
	int samples = 1;
	int depth = 50;
};
