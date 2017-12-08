#pragma once
#include "Scene/ControlLogic.h"
#include "Mesh/Multi.h"
#include "Physics/Model.h"
#include "PxPhysicsAPI.h"

namespace Egg { namespace Control
{
	GG_SUBCLASS(AvatarLogic, Egg::Scene::ControlLogic)
		friend class AvatarState;
		physx::PxScene* scene;
		Egg::Mesh::Multi::P projectileMesh;
		Egg::Physics::Model::P projectileModel;
	protected:
		AvatarLogic(physx::PxScene* scene, Egg::Mesh::Multi::P projectileMesh, Egg::Physics::Model::P projectileModel)
			:Egg::Scene::ControlLogic(),
			scene(scene),
			projectileMesh(projectileMesh),
			projectileModel(projectileModel)
		{}
	public:
		Egg::Scene::ControlStateP createState(Egg::Scene::EntityP entity);


	GG_ENDCLASS

}}
