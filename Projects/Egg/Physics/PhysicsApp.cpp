#include "stdafx.h"
#include "Physics/PhysicsApp.h"

using namespace Egg;
using namespace physx;


PxFilterFlags allContactsFilterShader(
        PxFilterObjectAttributes attributes0, PxFilterData filterData0,
        PxFilterObjectAttributes attributes1, PxFilterData filterData1,
        PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
        // generate contacts for all that were not filtered above
        pairFlags = PxPairFlag::eCONTACT_DEFAULT;
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

        return PxFilterFlag::eDEFAULT;
}

HRESULT Physics::PhysicsApp::createResources()
{
	__super::createResources();

	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;
	static PxSimulationFilterShader gDefaultFilterShader =
		PxDefaultSimulationFilterShader;

	foundation = PxCreateFoundation(PX_FOUNDATION_VERSION,
		gDefaultAllocatorCallback, gDefaultErrorCallback);

	PxPvd*  pvd = PxCreatePvd(*foundation);

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation,
		PxTolerancesScale(), true, pvd);

	PxInitExtensions(*physics, pvd);

	PxPvdTransport* transport =
		PxDefaultPvdSocketTransportCreate("localhost", 5425, 100);

	pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	if (!sceneDesc.cpuDispatcher)
	{
		cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = cpuDispatcher;
	}
	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = allContactsFilterShader;//gDefaultFilterShader;

	scene = physics->createScene(sceneDesc);
	
	return S_OK;
}

HRESULT Physics::PhysicsApp::releaseResources()
{
	HRESULT hr = __super::releaseResources();

	return hr;
}

void Physics::PhysicsApp::render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{	
	__super::render(context);
}

void Physics::PhysicsApp::animate(double dt, double t)
{

	timeRemainingOfTimestep -= dt;
	while(timeRemainingOfTimestep < 0)
	{
		timeRemainingOfTimestep += timestep;
		__super::animate(timestep, t);
		scene->simulate(timestep);
		scene->fetchResults(true);
	}
}

bool Physics::PhysicsApp::processMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::processMessage(hWnd, uMsg, wParam, lParam);
}
