#pragma once

#include <iostream>
#include <vector>
#include <typeinfo>

#include "Component.h"

class Entity
{
	std::vector<Component*> components;
	//std::vector<System*> systems;

public:
	template<class Ctype>
	Ctype* getFirstComponent()
	{
		for (Component* c : components)
		{
			if ( typeid(*c) == typeid(Ctype) ) return (static_cast<Ctype*>(c));
		}
		return nullptr;
	}

	void addComponent(Component& c)
	{
		components.push_back(&c);
	}
};