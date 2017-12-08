#pragma once
#include "Mesh/Multi.h"
#include "Cam/Base.h"
#include "Scene/RigidBody.h"
#include "Scene/RenderParameters.h"
#include "Scene/ControlParameters.h"
#include "Scene/ControlLogic.h"

namespace Egg {
	namespace Scene
	{
		GG_DECL(ControlState)

		GG_CLASS(Entity)
			Egg::Mesh::Multi::P mesh;
		Egg::Scene::RigidBody::P rigidBody;
		Egg::Scene::ControlStateP controlState;

	protected:
		Entity(Egg::Mesh::Multi::P mesh, Egg::Scene::RigidBody::P rigidBody);
	public:
		void setController(Egg::Scene::ControlLogic::P controlLogic);

		Egg::Scene::RigidBody::P getRigidBody() { return rigidBody; }

		void render(const Egg::Scene::RenderParameters& params);
		bool control(Egg::Scene::ControlParameters& params);
		void animate(float dt, float t);

	GG_ENDCLASS
}}
