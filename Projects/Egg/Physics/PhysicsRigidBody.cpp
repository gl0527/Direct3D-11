#include "stdafx.h"
#include "Physics/PhysicsRigidBody.h"
#include "Physics/PhysicsMathConversions.h"

using namespace Egg;
using namespace Egg::Physics;
using namespace physx;

void toRadiansAndUnitAxis(PxQuat q, PxReal& angle, PxVec3& axis)
{
	const PxReal quatEpsilon = (PxReal(1.0e-8f));
	const PxReal s2 = q.x*q.x + q.y*q.y + q.z*q.z;
	if (s2<quatEpsilon*quatEpsilon)  // can't extract a sensible axis
	{
		angle = 0;
		axis = PxVec3(1, 0, 0);
	}
	else
	{
		const PxReal s = PxRecipSqrt(s2);
		axis = PxVec3(q.x, q.y, q.z) * s;
		angle = PxAbs(q.w)<quatEpsilon ? PxPi : PxAtan2(s2*s, q.w) * 2;
	}

}

PhysicsRigidBody::PhysicsRigidBody(
	physx::PxScene* scene,
	Model::P model,
	Egg::Math::float3 position,
	Egg::Math::float4 orientation)
{
	actor = scene->getPhysics().createRigidDynamic(
		PxTransform(~position, ~orientation));
	
	for (auto shape : model->shapes) {
		actor->attachShape(*shape);
	}

	actor->setActorFlags(model->actorFlags);
	actor->setRigidBodyFlags(model->rigidBodyFlags);
	PxRigidBodyExt::updateMassAndInertia(*actor, model->density);

	scene->addActor(*actor);

}

Math::float4x4 PhysicsRigidBody::getModelMatrix()
{
	using namespace Egg::Math;

	PxTransform m = actor->getGlobalPose();

	float angle;
	float3 axis;
	toRadiansAndUnitAxis(m.q, angle, ~axis);

	return float4x4::rotation(axis, angle) * float4x4::translation(~m.p);
}

Math::float4x4 PhysicsRigidBody::getModelMatrixInverse()
{
	using namespace Egg::Math;

	PxTransform m = actor->getGlobalPose();

	float angle;
	float3 axis;
	toRadiansAndUnitAxis(m.q, angle, ~axis);

	return float4x4::translation(~-m.p) * float4x4::rotation(axis, -angle);
}

Math::float3 PhysicsRigidBody::getPosition()
{
	using namespace Egg::Math;

	PxTransform m = actor->getGlobalPose();
	return ~m.p;
}

Math::float4 PhysicsRigidBody::getOrientation()
{
	using namespace Egg::Math;
	PxTransform m = actor->getGlobalPose();
	return ~m.q;
}

Math::float4x4 PhysicsRigidBody::getRotationMatrix()
{
	using namespace Egg::Math;
	PxTransform m = actor->getGlobalPose();

	float angle;
	float3 axis;
	toRadiansAndUnitAxis(m.q, angle, ~axis);

	return float4x4::rotation(axis, angle);
}

Math::float4x4 PhysicsRigidBody::getRotationMatrixInverse()
{
	using namespace Egg::Math;
	PxTransform m = actor->getGlobalPose();

	float angle;
	float3 axis;
	toRadiansAndUnitAxis(m.q, angle, ~axis);

	return float4x4::rotation(axis, -angle);
}

