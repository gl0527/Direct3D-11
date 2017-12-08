#include "stdafx.h"
#include "Control/AvatarState.h"
#include "Control/AvatarLogic.h"
#include "Scene/Entity.h"
#include "Scene/ControlParameters.h"

using namespace Egg;
using namespace physx;
using namespace Egg::Control;

AvatarState::AvatarState(Egg::Scene::Entity::P entity, AvatarLogicP logic) :
	Scene::ControlState(entity), logic(logic), MaxForceMagnitude (100'000.0f), MaxTorqueMagnitude (50'000.0f)
{
	physicsRigidBody = std::dynamic_pointer_cast<Physics::PhysicsRigidBody>(entity->getRigidBody());
}

bool AvatarState::control(Egg::Scene::ControlParameters& params)
{
	auto actor = physicsRigidBody->getActor();
	auto pose = actor->getGlobalPose();

	PxVec3 force (0, 0, 0);
	PxVec3 torque (0, 0, 0);
	
	if (params.keyPressed[VK_NUMPAD8]) {
		PxVec3 F (30'000.0f, 0.0f, 0.0f);

		if ((force + F).magnitude () <= MaxForceMagnitude)
			force += F;
	}
	if (params.keyPressed[VK_NUMPAD6]) {
		PxVec3 M (0, 15'000, 0);

		if ((torque + M).magnitude () <= MaxTorqueMagnitude)
			torque += M;
	}
	if (params.keyPressed[VK_NUMPAD4]) {
		PxVec3 M (0, -15'000, 0);

		if ((torque + M).magnitude () >= -MaxTorqueMagnitude)
			torque += M;
	}
	if (params.keyPressed[VK_SPACE]) {
		//LABTODO: spawn rocket
		// ehhez a logic-ot kellene hasznalni
		
	}

	actor->addForce (pose.transform (force));
	actor->addTorque (pose.transform (torque));

	return false;
}
