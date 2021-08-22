#pragma once

#include "MeshComponent.h"
#include "TransformComponent.h"
#include "SizeComponent.h"

class ObjectBP
{
public:
	virtual ~ObjectBP() {}
	ecs::MeshComponent mesh_c;
	ecs::TransformComponent transform_c;
	ecs::SizeComponent size_c;

};