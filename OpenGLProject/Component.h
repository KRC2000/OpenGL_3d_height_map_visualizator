#pragma once

#include "Id.h"

struct Component
{
	ecs::ComponentId id = ecs::ComponentId::EMPTY;
	virtual ~Component() {};
};