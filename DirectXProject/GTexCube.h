#ifndef GTXCUBE_H

#define GTXCUBE_H

#include "d3dUtil.h"
#include <string>

const int NUM_VERTS = 24;
const int NUM_INDICES = 36;

class GTexCube
{
private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;
	D3DXMATRIX world;

	std::string texName;

	IDirect3DVertexBuffer9* vBuffer;
	IDirect3DIndexBuffer9* iBuffer;
	IDirect3DTexture9* texture;
	D3DMATERIAL9 material;
	
	D3DXMATRIX GetWorld();

public:
	GTexCube(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scal, const char* fileName);
	~GTexCube();

	void Initialize(IDirect3DDevice9* device);
	void Update(float dt);
	void Draw(IDirect3DDevice9* device);
	void Release();
};


#endif