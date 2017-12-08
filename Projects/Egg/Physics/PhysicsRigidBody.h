#pragma once
#include "Scene/RigidBody.h"
#include "Physics/Model.h"

namespace Egg { namespace Physics
{
	GG_SUBCLASS(PhysicsRigidBody, Egg::Scene::RigidBody)
		physx::PxRigidDynamic* actor;
	protected:
		PhysicsRigidBody(physx::PxScene* scene,
			Egg::Physics::Model::P model,
			Egg::Math::float3 position,
			Egg::Math::float4 orientation);
	public:
		/// Returns the model matrix. To be used for rendering, and positioning of light sources and cameras attached to the entity.
		virtual Egg::Math::float4x4 getModelMatrix();

		/// Returns the inverse of the model matrix. To be used for rendering.
		virtual Egg::Math::float4x4 getModelMatrixInverse();


		/// Returns reference point in world space.
		virtual Egg::Math::float3 getPosition();

		/// Returns the orientation as a quaternion.
		virtual Egg::Math::float4 getOrientation();

		/// Returns the rotation matrix.
		virtual Egg::Math::float4x4 getRotationMatrix();

		/// Returns the inverse of the rotation matrix. To be used for the view transformation of attached cameras.
		virtual Egg::Math::float4x4 getRotationMatrixInverse();

		/// Updates time-varying entity properties. Returns whether the entity should be kept.
		/// @param dt time step in seconds
		virtual bool animate(float dt, float t) { return true; }

		physx::PxRigidDynamic* getActor() { return actor; }
	GG_ENDCLASS
}}


