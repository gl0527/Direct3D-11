#include "DXUT.h"
#include "Control/ControlledEntity.h"
#include "Physics/PhysicsMathConversions.h"

using namespace Egg;
using namespace Egg::Control;
using namespace physx;

ControlledEntity::ControlledEntity(physx::PxScene* scene, Egg::Math::float3 position, Egg::Math::float4 orientation, Mesh::Multi::P multiMesh)
	:PhysicsEntity(scene, position, orientation, multiMesh)
{
}

ControlledEntity::~ControlledEntity()
{
 	bool mivan= true;
}

bool ControlledEntity::animate(float dt)
{
	if( luabind::type(controlState) == LUA_TTABLE)
	{
		if(luabind::type(controlState["script"]) == LUA_TFUNCTION)
			controlState["script"](shared_from_this(), controlState);
		if(luabind::type(controlState["killed"]) != LUA_TNIL)
			return false;
	}
	return true;
}

void ControlledEntity::onContact(ControlledEntity::P other)
{
	if( luabind::type(controlState) == LUA_TTABLE)
		if(luabind::type(controlState["onContact"]) == LUA_TFUNCTION)
			controlState["onContact"](shared_from_this(), controlState, other->getControlState());
}
