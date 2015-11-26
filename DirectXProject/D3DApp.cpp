#include "D3DApp.h"
#include <sstream>

namespace
{
	//Create global poitner to D3DApp
	D3DApp* g_pd3dApp;
}

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Forward the messages
	return g_pd3dApp->MsgProc(hwnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance)
{
	m_hAppInstance = hInstance;
	m_hAppWindow = NULL;
	m_uiClientWidth = 800;
	m_uiClientHeight = 600;
	m_sApptitle = "DiretX APPLICATION  ";
	m_WindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	m_bHandleDeviceLost = false;
	m_bDeviceLost = false;
	g_pd3dApp = this;
}

D3DApp::~D3DApp()
{
	//Release objects from memory
}

int D3DApp::Run()
{
	//Counts per second
	__int64 countsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	float secPerCount = 1.0f / countsPerSec;

	//Initial previous time
	__int64 prevTime = 0;
	QueryPerformanceCounter ((LARGE_INTEGER*)&prevTime);

	//Main message Loop
	MSG msg = {0};
	while(WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Capture current Count
			__int64 curTime = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
			///Calculate deltaTime
			float deltaTime = (curTime - prevTime) * secPerCount;

			//Calculate FPS
			CalculateFPS(deltaTime);

			//Update
			Update(deltaTime);
			
			//Render
			Render();

			//Set prev to current after frame ends
			prevTime = curTime;
		}
	}

	Shutdown();

	return static_cast<int>(msg.wParam);
}

bool D3DApp::Init()
{
	//Check window creation
	if(!InitWindow())
		return false;

	//Check Direct3D initialization
	if(!InitDirect3D())
		return false;
	
	return true;
}

bool D3DApp::InitWindow()
{
	//Create WNDCLASS
	WNDCLASSEX	wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbClsExtra = 0;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hAppInstance;
	wcex.lpfnWndProc = MainWindowProc;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "D3DAPPWNDCLASS";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//Register that class
	if(!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Failed to register Class", NULL, NULL);
		return false;
	}

	//Cache requester width and height
	RECT r = {0, 0, m_uiClientWidth, m_uiClientHeight};
	AdjustWindowRect(&r, m_WindowStyle, false);
	int width = r.right - r.left;
	int height = r.bottom - r.top;

	//Create the window
	m_hAppWindow = CreateWindow("D3DAPPWNDCLASS", m_sApptitle.c_str(), m_WindowStyle,
								GetSystemMetrics(SM_CXSCREEN)/2 - width/2,
								GetSystemMetrics(SM_CYSCREEN)/2 - height/2,
								width, height, NULL, NULL, m_hAppInstance, NULL);

	//Check if window was created
	if(!m_hAppWindow)
	{
		MessageBox(NULL, "Failed to create window", NULL, NULL);
		return false;
	}

	//Show the window
	ShowWindow(m_hAppWindow, SW_SHOW);

	return true;
}


void D3DApp::Shutdown()
{
	//Release direct3D form memory
	d3d::Release(m_pDevice3D);
	d3d::Release(m_pDirect3D);
}

bool D3DApp::InitDirect3D()
{
	//Get interface to Driect3D
	m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(!m_pDirect3D)
	{
		MessageBox(NULL, "Failed to create direct3D com object", NULL, NULL);
		return false;
	}

	//Check device capabiities
	D3DCAPS9 m_d3dDevCaps;
	m_pDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_d3dDevCaps);

	int vp;
	if(m_d3dDevCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//Fill out the present parameters
	ZeroMemory(&m_d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	m_d3dpp.BackBufferWidth = m_uiClientWidth;
	m_d3dpp.BackBufferHeight = m_uiClientHeight;
	m_d3dpp.Windowed = true;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	m_d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_d3dpp.MultiSampleQuality = 0;
	m_d3dpp.hDeviceWindow = m_hAppWindow;
	m_d3dpp.Flags = 0;
	m_d3dpp.EnableAutoDepthStencil = true;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	//Create the device
	m_pDirect3D->CreateDevice(	D3DADAPTER_DEFAULT,
								D3DDEVTYPE_HAL,
								m_hAppWindow,
								vp,
								&m_d3dpp,
								&m_pDevice3D);

	if(!m_pDevice3D)
	{
		MessageBox(NULL, "Failed to create device", NULL, NULL);
		return false;
	}

	D3DVIEWPORT9 viewport;
	ZeroMemory(&viewport, sizeof(D3DVIEWPORT9));
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = m_uiClientWidth;
	viewport.Height = m_uiClientHeight;
	viewport.MinZ = 0;
	viewport.MaxZ = 1;

	//set viewport
	m_pDevice3D->SetViewport(&viewport);

	return true;
}

void D3DApp::CalculateFPS(float dt)
{
	static int framCnt;
	static float elapsedTime;

	//Incrememt Framecount
	framCnt++;
	//Increment elapsed Time
	elapsedTime += dt;

	if(elapsedTime >= 1.0f)
	{
		m_FPS = (float)framCnt;

		//Output FPS to window title
		std::stringstream ss;
		ss << m_sApptitle.c_str() << "FPS:   " << m_FPS;
		SetWindowText(m_hAppWindow, ss.str().c_str());

		//Reset elapsed time and framecount
		framCnt = 0;
		elapsedTime = 0;
	}
}


LRESULT D3DApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_F1:
			m_bHandleDeviceLost = !m_bHandleDeviceLost;
			return 0;
		}
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}