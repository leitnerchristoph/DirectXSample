#include "GTexCube.h"

GTexCube::GTexCube(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scal, const char* fileName)
{
	position = pos;
	rotation = rot;
	scale = scal;
	texName = fileName;

	material.Ambient = (D3DXCOLOR)d3dColors::White;
	material.Diffuse = (D3DXCOLOR)d3dColors::White;
	material.Emissive = (D3DXCOLOR)d3dColors::Black;
	material.Specular = (D3DXCOLOR)d3dColors::White;
	material.Power = 5.0f;
}

GTexCube::~GTexCube()
{
}

void GTexCube::Initialize(IDirect3DDevice9* device)
{
	//create vertex Buffer
	
	d3dVertex::VertexPositionNormalTexture verts[NUM_VERTS] =
	{
		
		//(0,0)----------(1,0)
		//
		//
		//
		//
		//(0,1)----------(1,1)


		//Front face
		d3dVertex::VertexPositionNormalTexture(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),		//0
		d3dVertex::VertexPositionNormalTexture(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),		//1
		d3dVertex::VertexPositionNormalTexture(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),		//2
		d3dVertex::VertexPositionNormalTexture(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),		//3

		//Back face
		d3dVertex::VertexPositionNormalTexture(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),			//4
		d3dVertex::VertexPositionNormalTexture(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f , 1.0f, 1.0f, 0.0f),		//5
		d3dVertex::VertexPositionNormalTexture(1.0f, -1.0f, 1.0f, 0.0f, 0.0f , 1.0f, 0.0f, 1.0f),		//6
		d3dVertex::VertexPositionNormalTexture(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f , 1.0f, 1.0f, 1.0f),		//7

		//Left face
		d3dVertex::VertexPositionNormalTexture(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f),		//8
		d3dVertex::VertexPositionNormalTexture(-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),		//9
		d3dVertex::VertexPositionNormalTexture(-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),		//10
		d3dVertex::VertexPositionNormalTexture(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),		//11

		//Right face
		d3dVertex::VertexPositionNormalTexture(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),		//12
		d3dVertex::VertexPositionNormalTexture(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 01.0f, 0.0f),		//13
		d3dVertex::VertexPositionNormalTexture(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),		//14
		d3dVertex::VertexPositionNormalTexture(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 01.0f, 1.0f),		//15

		//Top face
		d3dVertex::VertexPositionNormalTexture(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),		//16
		d3dVertex::VertexPositionNormalTexture(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),			//17
		d3dVertex::VertexPositionNormalTexture(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),		//18
		d3dVertex::VertexPositionNormalTexture(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),		//19

		//Bottom face
		d3dVertex::VertexPositionNormalTexture(1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f),		//20
		d3dVertex::VertexPositionNormalTexture(-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),		//21
		d3dVertex::VertexPositionNormalTexture(1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),		//22
		d3dVertex::VertexPositionNormalTexture(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f),		//23	
	};

	device->CreateVertexBuffer(NUM_VERTS * sizeof(d3dVertex::VertexPositionNormalTexture), NULL, d3dVertex::VertexPositionNormalTexture::FVF, D3DPOOL_MANAGED, &vBuffer, NULL);

	VOID* pVerts;
	vBuffer->Lock(0, sizeof(verts), (void**)&pVerts, 0);
	memcpy_s(pVerts, sizeof(verts), verts, sizeof(verts));
	vBuffer->Unlock();

	//Create index buffer

	WORD indices[NUM_INDICES] = 
	{
		//Front
		0, 1, 2,
		2, 1, 3,

		//Back
		4, 5, 6,
		6, 5, 7,

		//Left
		8, 9, 10,
		10, 9, 11,

		//Right
		12, 13, 14,
		14, 13, 15,

		//Top
		16, 17, 18,
		18, 17, 19,

		//Bottom
		20, 21, 22,
		22, 21, 23
	};

	device->CreateIndexBuffer(NUM_INDICES * sizeof(WORD), D3DUSAGE_WRITEONLY,
								D3DFMT_INDEX16, D3DPOOL_MANAGED, &iBuffer, NULL);

	VOID* pIndices;
	iBuffer->Lock(0, sizeof(indices), (void**)&pIndices, 0);
	memcpy_s(pIndices, sizeof(indices), indices, sizeof(indices));
	iBuffer->Unlock();


	//Load texture
	D3DXCreateTextureFromFile(device, texName.c_str(), &texture);
	if(!texture)
	{
		MessageBox(NULL, "Failed to create Texture", NULL, NULL);
		return;
	}
}

void GTexCube::Update(float dt)
{
	rotation.y += dt;
	rotation.x += dt;
	rotation.z += dt;
}

void GTexCube::Draw(IDirect3DDevice9* device)
{
	device->SetTexture(0, texture);
	device->SetMaterial(&material);
	device->SetTransform(D3DTS_WORLD, &GetWorld());
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetStreamSource(0, vBuffer, 0, sizeof(d3dVertex::VertexPositionNormalTexture));
	device->SetIndices(iBuffer);
	device->SetFVF(d3dVertex::VertexPositionNormalTexture::FVF);
	
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, NUM_INDICES, 0, NUM_INDICES / 3);
}

void GTexCube::Release()
{
	d3d::Release(texture);
	d3d::Release(iBuffer);
	d3d::Release(vBuffer);
}

D3DXMATRIX GTexCube::GetWorld()
{
	D3DXMATRIX Scale, Rx, Ry, Rz, Translate;

	D3DXMatrixScaling(&Scale, scale.x, scale.y, scale.z);
	D3DXMatrixRotationX(&Rx, rotation.x);
	D3DXMatrixRotationY(&Ry, rotation.y);
	D3DXMatrixRotationZ(&Rz, rotation.z);
	D3DXMatrixTranslation(&Translate, position.x, position.y, position.z);


	world =  Scale * Rx * Ry * Rz * Translate;
	return world;
}