#include "stdafx.h"
#include "Game.h"
#include "Scene/Entity.h"
#include "Physics/Model.h"
#include "Physics/PhysicsRigidBody.h"
#include "Control/AvatarLogic.h"
#include "ThrowOnFail.h"

#include "DirectXTex.h"
#include "UtfConverter.h"
#include "Scene/DDSTextureLoader.h"


using namespace Egg::Math;

Game::Game(Microsoft::WRL::ComPtr<ID3D11Device> device) : Egg::Physics::PhysicsApp(device)
{
}

Game::~Game(void)
{
}

HRESULT Game::createResources() {
	using namespace Microsoft::WRL;
	using namespace Egg;
	using namespace physx;

	Physics::PhysicsApp::createResources();

	////////////////////////////////////
	// CREATE BACKGROUND
	////////////////////////////////////

	Microsoft::WRL::ComPtr<ID3D11Resource> envTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> envSrv;

	std::wstring envfile = Egg::UtfConverter::utf8to16 (App::getSystemEnvironment ().resolveMediaPath ("Sunset.dds"));
	Egg::ThrowOnFail ("Could not create cube map texture and/or SRV.", __FILE__, __LINE__) ^
		DirectX::CreateDDSTextureFromFile (device.Get (),
			envfile.c_str (),
			envTexture.GetAddressOf (), envSrv.GetAddressOf ());

	Egg::Mesh::Geometry::P backgroundGeometry = Egg::Mesh::Indexed::createQuad (device);

	ComPtr<ID3DBlob> backgroundVertexShaderByteCode = loadShaderCode ("vsQuad.cso");
	Egg::Mesh::Shader::P backgroundVertexShader = Egg::Mesh::Shader::create ("vsQuad.cso", device, backgroundVertexShaderByteCode);

	ComPtr<ID3DBlob> backgroundPixelShaderByteCode = loadShaderCode ("psBackground.cso");
	Egg::Mesh::Shader::P backgroundPixelShader = Egg::Mesh::Shader::create ("psBackground.cso", device, backgroundPixelShaderByteCode);

	Egg::Mesh::Material::P backgroundMaterial = Egg::Mesh::Material::create ();
	backgroundMaterial->setShader (Egg::Mesh::ShaderStageFlag::Vertex, backgroundVertexShader);
	backgroundMaterial->setShader (Egg::Mesh::ShaderStageFlag::Pixel, backgroundPixelShader);
	backgroundMaterial->setShaderResource ("envTexture", envSrv);

	ComPtr<ID3D11InputLayout> backgroundInputLayout = inputBinder->getCompatibleInputLayout (backgroundVertexShaderByteCode, backgroundGeometry);
	backgroundMesh = Egg::Mesh::Shaded::create (backgroundGeometry, backgroundMaterial, backgroundInputLayout);

	////////////////////////////////////
	// CREATE GROUND
	////////////////////////////////////

	auto groundMesh = loadMultiMesh ("ground.x", 0, "ground");
	auto groundMaterial = groundMesh->getSubmesh (0)->getShaded (renderParameters.mien)->getMaterial ();

	const char* groundVSFile = "vsTrafo.cso";
	const char* groundPSFile = "psIdle.cso";

	ComPtr<ID3DBlob> groundVSBC = loadShaderCode (groundVSFile);
	Egg::Mesh::Shader::P groundVS = Egg::Mesh::Shader::create (groundVSFile, device, groundVSBC);

	ComPtr<ID3DBlob> groundPSBC = loadShaderCode (groundPSFile);
	Egg::Mesh::Shader::P groundPS = Egg::Mesh::Shader::create (groundPSFile, device, groundPSBC);

	ComPtr<ID3D11SamplerState> groundSampler;
	CD3D11_SAMPLER_DESC groundSamplerDesc = CD3D11_SAMPLER_DESC (CD3D11_DEFAULT ());
	groundSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	groundSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	Egg::ThrowOnFail ("Could not create SS.", __FILE__, __LINE__) ^
		device->CreateSamplerState (&groundSamplerDesc, groundSampler.GetAddressOf ());

	groundMaterial->setCb ("perFrame", renderParameters.perFrameConstantBuffer);
	groundMaterial->setShaderResource ("envTexture", envSrv);
	groundMaterial->setSamplerState ("ss", groundSampler);

	auto groundModel = Physics::Model::create();
	groundModel->rigidBodyFlags = PxRigidBodyFlag::eKINEMATIC;
	PxMaterial* pxGroundMaterial = physics->createMaterial (0.5, 0.5, 0.9);
	groundModel->addShape(physics->createShape(PxBoxGeometry (500.0f, 3.5f, 500.0f), *pxGroundMaterial));
	auto groundRigidBody = Physics::PhysicsRigidBody::create (scene, groundModel, float3 (0, 0, 0), float4 (0, 0, 0, 1));
	auto groundEntity = Scene::Entity::create (groundMesh, groundRigidBody);
	entities.push_back (groundEntity);

	////////////////////////////////////
	// LOAD ROCKET MESH
	////////////////////////////////////

	auto rocketMesh = loadMultiMesh("rocket.obj", 0, "rocket");
	auto rocketModel = Physics::Model::create();
	PxMaterial* pxMaterial = physics->createMaterial (0.5, 0.5, 0.5);
	rocketModel->addShape(physics->createShape(PxCapsuleGeometry(1, 1), *pxMaterial));

	////////////////////////////////////
	// LOAD POD MESH
	////////////////////////////////////

	auto podMesh = loadMultiMesh("geopod.x", 0, "pod");
	
	////////////////////////////////////
	// SET THE WINDOWS OF THE POD MESH
	////////////////////////////////////

	auto podGlass = podMesh->getSubmesh (1);
	auto podGlassShaded = podGlass->getShaded (renderParameters.mien);
	auto podGlassMaterial = podGlassShaded->getMaterial ();

	const char* podGlassVSFile = "vsTrafo.cso";
	const char* podGlassPSFile = "psGlass.cso";

	ComPtr<ID3DBlob> podGlassVSBC = loadShaderCode (podGlassVSFile);
	Egg::Mesh::Shader::P podGlassVS = Egg::Mesh::Shader::create (podGlassVSFile, device, podGlassVSBC);

	ComPtr<ID3DBlob> podGlassPSBC = loadShaderCode (podGlassPSFile);
	Egg::Mesh::Shader::P podGlassPS = Egg::Mesh::Shader::create (podGlassPSFile, device, podGlassPSBC);

	podGlassMaterial->setShader (Egg::Mesh::ShaderStageFlag::Vertex, podGlassVS);
	podGlassMaterial->setShader (Egg::Mesh::ShaderStageFlag::Pixel, podGlassPS);

	podGlassMaterial->setCb ("perFrame", renderParameters.perFrameConstantBuffer);
	podGlassMaterial->setShaderResource("envTexture", envSrv);

	ComPtr<ID3D11SamplerState> ss;
	CD3D11_SAMPLER_DESC ssDesc = CD3D11_SAMPLER_DESC (CD3D11_DEFAULT ());
	Egg::ThrowOnFail ("Could not create SS.", __FILE__, __LINE__) ^
		device->CreateSamplerState (&ssDesc, ss.GetAddressOf ());

	podGlassMaterial->setSamplerState ("linearSampler", ss);

	////////////////////////////////////
	// SET THE BODY OF THE POD MESH
	////////////////////////////////////

	auto podBody = podMesh->getSubmesh (0);
	auto podBodyShaded = podBody->getShaded (renderParameters.mien);
	auto podBodyMaterial = podBodyShaded->getMaterial ();

	const char* podBodyVSFile = "vsTrafo.cso";
	const char* podBodyPSFile = "psIdle.cso";

	ComPtr<ID3DBlob> podGBodyVSBC = loadShaderCode (podBodyVSFile);
	Egg::Mesh::Shader::P podBodyVS = Egg::Mesh::Shader::create (podBodyVSFile, device, podGBodyVSBC);

	ComPtr<ID3DBlob> podBodyPSBC = loadShaderCode (podBodyPSFile);
	Egg::Mesh::Shader::P podBodyPS = Egg::Mesh::Shader::create (podBodyPSFile, device, podBodyPSBC);

	podBodyMaterial->setShader (Egg::Mesh::ShaderStageFlag::Vertex, podBodyVS);
	podBodyMaterial->setShader (Egg::Mesh::ShaderStageFlag::Pixel, podBodyPS);

	podBodyMaterial->setCb ("perFrame", renderParameters.perFrameConstantBuffer);
	podBodyMaterial->setShaderResource ("envTexture", envSrv);
	podBodyMaterial->setSamplerState ("ss", groundSampler);
	
	/*D3D11_BUFFER_DESC matcapBufferDesc;
	matcapBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matcapBufferDesc.ByteWidth = sizeof (Egg::Math::float4x4);
	matcapBufferDesc.CPUAccessFlags = 0;
	matcapBufferDesc.MiscFlags = 0;
	matcapBufferDesc.StructureByteStride = 0;
	matcapBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	Egg::ThrowOnFail ("Failed to create per frame constant buffer.", __FILE__, __LINE__) ^
		device->CreateBuffer (&matcapBufferDesc, nullptr, matcapBuffer.GetAddressOf ());
	
	podBodyMaterial->setCb ("matCap", matcapBuffer);
	podBodyMaterial->setShaderResource ("envTexture", envSrv);
	podBodyMaterial->setSamplerState ("ss", groundSampler);*/


	////////////////////////////////////
	// SET THE PHYSICS OF THE POD MESH
	////////////////////////////////////

	auto podModel = Physics::Model::create();
	podModel->addShape(physics->createShape(PxCapsuleGeometry(3, 5), *pxMaterial));
	auto box = physics->createShape(PxBoxGeometry(PxVec3(4, 1, 4)), *pxMaterial);
	box->setLocalPose(PxTransform(PxVec3(4.5, -1, 0), PxQuat(0.79, PxVec3(0, 1, 0))));
	podModel->addShape(box);

	box = physics->createShape(PxBoxGeometry(PxVec3(4, 1, 4)), *pxMaterial);
	box->setLocalPose(PxTransform(PxVec3(-5, -1, -2), PxQuat(0.79, PxVec3(0, 1, 0))));
	podModel->addShape(box);

	box = physics->createShape(PxBoxGeometry(PxVec3(4, 1, 4)), *pxMaterial);
	box->setLocalPose(PxTransform(PxVec3(-5, -1, 2), PxQuat(0.79, PxVec3(0, 1, 0))));
	podModel->addShape(box);

	box = physics->createShape(PxBoxGeometry(PxVec3(4, 1, 4)), *pxMaterial);
	box->setLocalPose(PxTransform(PxVec3(-7, 2, 0), PxQuat(1.098, PxVec3(0.146, -0.353, -0.353))));
	podModel->addShape(box);
	podModel->actorFlags = PxActorFlag::eDISABLE_GRAVITY;

	auto podRigidBody = Physics::PhysicsRigidBody::create(scene, podModel, float3(0, 10, 0), float4(0, 0, 0, 1));
	podRigidBody->getActor ()->setLinearDamping (0.3f);
	podRigidBody->getActor ()->setAngularDamping (0.3f);

	auto podEntity = Scene::Entity::create(podMesh, podRigidBody);
	podEntity->setController(Control::AvatarLogic::create(scene, rocketMesh, rocketModel));
	entities.push_back(podEntity);

	////////////////////////////////////
	// BALL
	////////////////////////////////////

	auto ballMesh = loadMultiMesh ("ball.obj", 0, "ball");
	auto ballMaterial = ballMesh->getSubmesh (0)->getShaded (renderParameters.mien)->getMaterial ();

	const char* ballVSFile = "vsTrafo.cso";
	const char* ballPSFile = "psMatcap.cso";

	ComPtr<ID3DBlob> ballVSBC = loadShaderCode (ballVSFile);
	Egg::Mesh::Shader::P ballVS = Egg::Mesh::Shader::create (ballVSFile, device, ballVSBC);

	ComPtr<ID3DBlob> ballPSBC = loadShaderCode (ballPSFile);
	Egg::Mesh::Shader::P ballPS = Egg::Mesh::Shader::create (ballPSFile, device, ballPSBC);

	ballMaterial->setShader (Egg::Mesh::ShaderStageFlag::Vertex, ballVS);
	ballMaterial->setShader (Egg::Mesh::ShaderStageFlag::Pixel, ballPS);

	auto ballTexSRV = loadSrv ("matcap_steel.jpg");

	D3D11_BUFFER_DESC matcapBufferDesc;
	matcapBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matcapBufferDesc.ByteWidth = sizeof (Egg::Math::float4x4);
	matcapBufferDesc.CPUAccessFlags = 0;
	matcapBufferDesc.MiscFlags = 0;
	matcapBufferDesc.StructureByteStride = 0;
	matcapBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	Egg::ThrowOnFail ("Failed to create per frame constant buffer.", __FILE__, __LINE__) ^
		device->CreateBuffer (&matcapBufferDesc, nullptr, matcapBuffer.GetAddressOf ());
	
	ballMaterial->setCb ("matCap", matcapBuffer);
	ballMaterial->setShaderResource ("kd", ballTexSRV);
	ballMaterial->setSamplerState ("ss", groundSampler);

	auto ballModel = Physics::Model::create ();
	PxMaterial* pxBallMaterial = physics->createMaterial (0.1f, 0.1f, 0.2f);
	ballModel->addShape(physics->createShape(PxSphereGeometry (3), *pxBallMaterial));
	auto ballRigidBody = Physics::PhysicsRigidBody::create (scene, ballModel, float3 (20.0f, 20.0f, -12.0f), float4 (0,0,0,1));
	
	auto ballEntity = Scene::Entity::create (ballMesh, ballRigidBody);
	entities.push_back (ballEntity);

	return S_OK;
}

HRESULT Game::releaseResources()
{
	return Egg::Physics::PhysicsApp::releaseResources();
}

void Game::render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
{
	context->OMSetRenderTargets(1, defaultRenderTargetView.GetAddressOf(), defaultDepthStencilView.Get());
	float clearColor[4] = { 0.9f, 0.7f, 0.1f, 0.0f };
	context->ClearRenderTargetView(defaultRenderTargetView.Get(), clearColor);
	context->ClearDepthStencilView(defaultDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);

	float4x4 viewMat = cameras.at(currentCamera)->getViewMatrix ();
	context->UpdateSubresource (matcapBuffer.Get (), 0, nullptr, &viewMat, 0, 0);

	Egg::Physics::PhysicsApp::render(context);

	backgroundMesh->draw(context);
}

void Game::animate(double dt, double t)
{
	Egg::Physics::PhysicsApp::animate(dt, t);
}

bool Game::processMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Egg::Physics::PhysicsApp::processMessage(hWnd, uMsg, wParam, lParam);
}
