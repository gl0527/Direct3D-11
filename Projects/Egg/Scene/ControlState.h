#pragma once
#include "Scene/Entity.h"
#include "Scene/ControlParameters.h"

namespace Egg { namespace Scene
{
	GG_CLASS(ControlState)
	protected:
		Entity::P entity;
		ControlState(Entity::P entity) :entity(entity) {}
	public:
		virtual bool control(ControlParameters& params) = 0;

	GG_ENDCLASS

}}
