//Evil illumination
//EiRender.h
//
//Created by MonsterENT on 17/05/22.


#ifndef EiRender_H
#define EiRender_H

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>


#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DX11.lib") 


#define Ei_width 1600
#define Ei_height 1200


class ResManager;

class EiRender
{
public:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11ComputeShader* shader;
	UINT NumOfSamples;

	EiRender(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ComputeShader* shader,UINT NumOfSamples);

	~EiRender()
	{
	}


	void setResManager(ResManager* manager);

	ID3D11ShaderResourceView* Render();

private:
	ResManager* manager;

};

#endif //EiRender_H