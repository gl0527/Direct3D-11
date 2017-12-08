#pragma once
#include "Physics/PhysicsEntity.h"
extern "C"
{
    #include "lua.h"
    #include "lualib.h"
}
#include "luabind/luabind.hpp"
#include "luabind/adopt_policy.hpp"


namespace Egg { namespace Control
{
	class ControlledEntity : public Egg::Physics::PhysicsEntity
	{
	protected:
		luabind::object controlState;

		/// Private constructor. Initializes the model transformation to indentity.
		/// @param shadedMesh the ShadedMesh which the Entity uses for displaying itself.
		ControlledEntity(physx::PxScene* scene, Egg::Math::float3 position, Egg::Math::float4 orientation, Mesh::Multi::P multiMesh);
	public:
		/// Local shorthand for shared pointer type.
		typedef boost::shared_ptr<ControlledEntity> P;
		/// Local shorthand for weak pointer type.
		typedef boost::weak_ptr<ControlledEntity> W;

		/// Factory method for instantiation. Initializes the model transformation to indentity.
		/// @param shadedMesh the ShadedMesh which the Entity uses for displaying itself.
		/// @return shared pointer referencing created instance
		static ControlledEntity::P create(physx::PxScene* scene, Egg::Math::float3 position, Egg::Math::float4 orientation, Mesh::Multi::P multiMesh)
		{
			return ControlledEntity::P(new ControlledEntity(scene, position, orientation, multiMesh));
		}

		virtual ~ControlledEntity();

		/// Returns the model matrix. To be used for rendering, and positioning of light sources and cameras attached to the entity.

		void setControlState(luabind::object controlState)
		{
			this->controlState = controlState;
		}

		void releaseControlState()
		{
			this->controlState = luabind::object();
		}

		luabind::object getControlState()
		{
			return this->controlState;
		}

		virtual bool animate(float dt);

		virtual void onContact(ControlledEntity::P other);
	};

}}
