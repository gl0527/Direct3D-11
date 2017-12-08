#pragma once
#include "Scene/Entity.h"
#include "Scene/ControlState.h"
#include "Physics/PhysicsRigidBody.h"

namespace Egg { namespace Control
{
	GG_DECL(AvatarLogic)

	GG_SUBCLASS(AvatarState, Egg::Scene::ControlState)
		AvatarLogicP logic;
		Egg::Physics::PhysicsRigidBody::P physicsRigidBody;

		const float MaxForceMagnitude;
		const float MaxTorqueMagnitude;
	protected:
		AvatarState(Egg::Scene::Entity::P entity, AvatarLogicP logic);
	public:
		bool control(Egg::Scene::ControlParameters& params);
	GG_ENDCLASS

}}
