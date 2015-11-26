#include <Windows.h>
#include "D3DApp.h"
#include "GTexCube.h"

const DWORD d3dVertex::VertexPositionColor::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;								//POSITION COLOR
const DWORD d3dVertex::VertexPositionTexture::FVF = D3DFVF_XYZ | D3DFVF_TEX1;								//POSITION TEXTURE
const DWORD d3dVertex::VertexPositionNormalTexture::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;			//POSITION NORMALS U,V Texture

class TestApp : public D3DApp
{
public:
	//Constructor
	TestApp(HINSTANCE hInstance);
	//Destructor
	~TestApp();

	//Methods
	bool Init() override;
	void Update(float dt) override;
	void Render() override;

	void InvalidateDeviceObjects();
	void RestoreDeviceObjects();

	GTexCube* cube;

};


IDirect3DVertexBuffer9* VB;
IDirect3DIndexBuffer9*	IB;
D3DXMATRIX World;

TestApp::TestApp(HINSTANCE hInstance) : D3DApp(hInstance)
{
	cube = new GTexCube(D3DXVECTOR3(0.0f, 0.0f, 5.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						D3DXVECTOR3(1.0f, 1.0f, 1.0f),
						"dirt.png");
}

TestApp::~TestApp()
{
}

bool TestApp::Init()
{
	if(!D3DApp::Init())
	{
		return false;
	}
	
	//Initialize everything
	RestoreDeviceObjects();
	
	return true;
}

void TestApp::InvalidateDeviceObjects()
{
	cube->Release();
	d3d::Release(IB);
	d3d::Release(VB);
}

void TestApp::RestoreDeviceObjects()
{

	cube->Initialize(m_pDevice3D);

	D3DXMATRIX view;
	D3DXMATRIX proj;


	//Set view
	D3DXVECTOR3 position = D3DXVECTOR3 (0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&view, &position, &target, &up);
	m_pDevice3D->SetTransform(D3DTS_VIEW, &view);

	//Set Projection
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI/ 4, static_cast<float>(m_uiClientWidth)/m_uiClientHeight, 1.0f, 1000.0f);

	m_pDevice3D->SetTransform(D3DTS_PROJECTION, &proj);

	m_pDevice3D->SetRenderState(D3DRS_LIGHTING, true);

	D3DLIGHT9 light;
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Position = D3DXVECTOR3(-2.0f, 0.0f, 5.0f);
	light.Direction = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	D3DXCOLOR c = d3dColors::White;
	light.Diffuse = c;
	light.Ambient = c * 0.4f;
	light.Specular = c * 0.6f;

	m_pDevice3D->SetLight(0, &light);
	m_pDevice3D->LightEnable(0, true);
}


void TestApp::Update(float dt)
{
	cube->Update(dt);
}

void TestApp::Render()
{
	//
	HRESULT hr;

	if(m_bHandleDeviceLost)
	{
		if(m_bDeviceLost)
		{

			//Sleep the thread
			Sleep(100); //Free cpu

			if(FAILED(hr = m_pDevice3D->TestCooperativeLevel()))
			{
				if(hr == D3DERR_DEVICELOST)
					return;

				if(hr == D3DERR_DEVICENOTRESET)
				{
					//Invalidate device Objects
					InvalidateDeviceObjects();

					//Reset the device
					hr = m_pDevice3D->Reset(&m_d3dpp);

					if(FAILED(hr))
					{
						MessageBox(NULL, "Failed to reset device", NULL, NULL);
						return;
					}

					//Restore device Objects
					RestoreDeviceObjects();
				}
				return;
			}
		}
	}

	m_bDeviceLost = false;



	m_pDevice3D->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, d3dColors::Black, 1.0f, 0);

	m_pDevice3D->BeginScene();

	/////////////////  RENDER ////////////////////////////////////

	cube->Draw(m_pDevice3D);

	//////////////////////////////////////////////////////////////


	m_pDevice3D->EndScene();

	hr = m_pDevice3D->Present(0, 0, 0, 0);

	if(hr = D3DERR_DEVICELOST)
		m_bDeviceLost = true;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	TestApp* tApp = new TestApp(hInstance);

	if(!tApp->Init())
		return 1;

	return tApp->Run();
}

