#include "stdafx.h"
#include "Control/AvatarLogic.h"
#include "Control/AvatarState.h"

using namespace Egg;
using namespace Egg::Control;
using namespace physx;

Egg::Scene::ControlStateP AvatarLogic::createState(Egg::Scene::EntityP entity) {
	return AvatarState::create(entity, getShared());
}
