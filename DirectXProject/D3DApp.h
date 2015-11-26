#include <Windows.h>
#include <string>

#include <d3d9.h>
#include <d3dx9.h>
#include "d3dUtil.h"

class D3DApp
{
public:
	//Constructor
	D3DApp(HINSTANCE hInstance);

	//Destructor
	virtual ~D3DApp();

	//Main application loop
	int Run();

	//Framework methods
	virtual bool Init();
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	void Shutdown();
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	//Attributes
	HWND			m_hAppWindow;
	HINSTANCE		m_hAppInstance;
	unsigned int	m_uiClientWidth;
	unsigned int	m_uiClientHeight;
	std::string		m_sApptitle;
	DWORD			m_WindowStyle;
	float			m_FPS;
	bool			m_bHandleDeviceLost;
	bool			m_bDeviceLost;

	//DirectX Attributes
	IDirect3D9*				m_pDirect3D;
	IDirect3DDevice9*		m_pDevice3D;
	D3DPRESENT_PARAMETERS	m_d3dpp;

protected:
	//Methods

	//Initialize app window
	bool InitWindow();

	//Initialize Direct3D
	bool InitDirect3D();

	void CalculateFPS(float dt);
};
