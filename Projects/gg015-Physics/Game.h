#pragma once
#include "Physics/PhysicsApp.h"

GG_SUBCLASS(Game, Egg::Physics::PhysicsApp)
	Egg::Mesh::Shaded::P backgroundMesh;
	Microsoft::WRL::ComPtr<ID3D11Buffer> matcapBuffer;
public:
	Game(Microsoft::WRL::ComPtr<ID3D11Device> device);
	~Game(void);

	HRESULT createResources();
	HRESULT releaseResources();

	void render(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	void animate(double dt, double t);
	bool processMessage(HWND hWnd,
		UINT uMsg, WPARAM wParam, LPARAM lParam);

GG_ENDCLASS
