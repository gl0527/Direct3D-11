#pragma once
#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"
#include "Scene/ManagerApp.h"
#include "Physics/PhysicsRigidBody.h"

namespace Egg { namespace Physics
{
	
	GG_SUBCLASS(PhysicsApp, Egg::Scene::ManagerApp)
	protected:
		physx::PxFoundation* foundation;
		physx::PxPhysics* physics;
		physx::PxCpuDispatcher* cpuDispatcher;
		physx::PxScene* scene;

		double timeRemainingOfTimestep;
		double timestep;

	public:
		PhysicsApp(Microsoft::WRL::ComPtr<ID3D11Device> device) : Egg::Scene::ManagerApp(device) {
			timestep = 0.02;
			timeRemainingOfTimestep = 0.0;
		}

		virtual HRESULT createResources();
		virtual HRESULT releaseResources();

		virtual void animate(double dt, double t);
		virtual bool processMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	GG_ENDCLASS

}}