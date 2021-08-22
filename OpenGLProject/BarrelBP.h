#pragma once

#include "ObjectBP.h"

#include "SphereComponent.h"


class BarrelBP: public ObjectBP
{
public:
	ecs::SphereComponent sphere_c;

};