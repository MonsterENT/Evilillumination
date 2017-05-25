//Evil illumination
//Resource.h
//
//Created by MonsterENT on 17/05/22.


#ifndef Resource_h
#define Resource_h

#include"EiRender.h"
#include"EiAssimp.h"

XMFLOAT4 getFromVec(XMVECTOR vec);
XMFLOAT4 getFromFloat3(XMFLOAT3 float3);


enum Material
{
	MAT_Reflect = 0,
	MAT_Diffuse = 1,

};



class SRV_UAV_Resource
{
public:

	ID3D11Texture2D *tex;
	ID3D11ShaderResourceView* srv;
	ID3D11UnorderedAccessView* uav;


	SRV_UAV_Resource(ID3D11Device* device)
	{
		//Create tex
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Width = Ei_width;
		desc.Height = Ei_height;
		desc.MipLevels = 0;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;

		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

		device->CreateTexture2D(&desc, 0, &this->tex);


		//Create SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
		descSRV.Format = desc.Format;

		descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		descSRV.Texture2D.MostDetailedMip = 0;
		descSRV.Texture2D.MipLevels = 1;

		device->CreateShaderResourceView(tex, &descSRV, &this->srv);


		//Create UAV
		device->CreateUnorderedAccessView(tex, NULL, &this->uav);

	}
	~SRV_UAV_Resource()
	{
		if (this->tex)
			this->tex->Release();
		if (this->srv)
			this->srv->Release();
		if (this->uav)
			this->uav->Release();
	}

private:

};



class Triangel
{

public:

	typedef struct TriangelStruct
	{
		XMFLOAT4 posX;
		XMFLOAT4 posY;
		XMFLOAT4 posZ;
		XMFLOAT4 normal;
		XMFLOAT4 emit;
		XMFLOAT4 reflect;
		XMFLOAT4 materialTypeX_SmoothY;
	}TriangelStruct;



	Triangel(XMFLOAT4 posX,XMFLOAT4 posY,XMFLOAT4 posZ,XMFLOAT4 emit,XMFLOAT4 reflect,Material MaterialType,float Smoothness)
	{
		this->resource = new TriangelStruct;
		
		this->resource->posX = posX;
		this->resource->posY = posY;
		this->resource->posZ = posZ;

		this->resource->emit = emit;
		this->resource->reflect = reflect;
		this->resource->materialTypeX_SmoothY = XMFLOAT4(MaterialType, Smoothness, 0, 0);

		XMVECTOR x_z = XMVectorSet(posX.x - posZ.x, posX.y - posZ.y, posX.z - posZ.z, 0);
		XMVECTOR x_y = XMVectorSet(posX.x - posY.x, posX.y - posY.y, posX.z - posY.z, 0);

		this->resource->normal = getFromVec(XMVector3Cross(x_z, x_y));	
	}

	~Triangel()
	{
		delete this->resource;
	}



	TriangelStruct* resource;

private:
	
};





class Sphere
{
public:
	
	struct SphereStruct
	{
		XMFLOAT4 CenterPos;
		XMFLOAT4 Radius;
		XMFLOAT4 emit, reflect;
		XMFLOAT4 materialTypeX_SmoothY;
	};
	
	Sphere(XMFLOAT4 CenterPos,float Radius,XMFLOAT4 emit,XMFLOAT4 reflect,Material materialType,float Smoothness)
	{
		this->resource = new SphereStruct;

		this->resource->CenterPos = CenterPos;
		this->resource->Radius = XMFLOAT4(Radius, 0, 0, 0);

		this->resource->emit = emit;
		this->resource->reflect = reflect;
		this->resource->materialTypeX_SmoothY = XMFLOAT4(materialType, Smoothness, 0, 0);
	}

	~Sphere()
	{
		delete this->resource;
	}

	SphereStruct* resource;

private:

};





class General_info
{
public:

	struct infoStruct
	{
		XMFLOAT4 numTriangelX_numSamplesY;
		XMFLOAT4 PixelWidthX_HeightY;
	};

	General_info(int numOfSamples)
	{
		this->resouce = new infoStruct;
		this->resouce->numTriangelX_numSamplesY = XMFLOAT4(0, numOfSamples, 1, 0);
		this->resouce->PixelWidthX_HeightY = XMFLOAT4(Ei_width, Ei_height, 0, 0);

	}
	~General_info()
	{
		delete this->resouce;
	}


	infoStruct* resouce;

private:

};



#define m1 0x100000000LL
#define c1 0xB16
#define a1 0x5DEECE66DLL


static unsigned long long seed = 1;


class RandValue
{
public:

	struct RandStruct
	{
		float data[10];
	};

	RandValue(UINT Samples)
	{
		this->Samples = Samples;
		this->buffer = NULL;
	}
	~RandValue()
	{
		if (this->resource)
		{
			delete[] this->resource;
		}
		if (this->buffer)
			delete this->buffer;
	}

	UINT Samples;

	RandStruct resource[Ei_width][Ei_height];

	ID3D11Buffer* buffer;

	double drand(void)
	{
		seed = (a1 * seed + c1) & 0xFFFFFFFFFFFFLL;
		unsigned int x = seed >> 16;
		return ((double)x / (double)m1);
	}
private:

};



class EiMesh
{
public:
	vector<Triangel*> TriangelLine;


	EiMesh(string fileName, XMFLOAT4 emit, XMFLOAT4 reflect, Material materialType, float Smoothness,XMFLOAT3 mul,XMFLOAT3 offset)
	{
		this->Loader = new EiAssimp(fileName,mul,offset);

		for (int i = 0; i < this->Loader->IndexLine.size(); i+=3)
		{
			XMFLOAT4 posX = getFromFloat3(Loader->VertexLine[Loader->IndexLine[i]]);
			XMFLOAT4 posY = getFromFloat3(Loader->VertexLine[Loader->IndexLine[i + 1]]);
			XMFLOAT4 posZ = getFromFloat3(Loader->VertexLine[Loader->IndexLine[i + 2]]);
			

			Triangel* TriangelObj = new Triangel(posX,posY,posZ,emit,reflect,materialType,Smoothness);
			TriangelObj->resource->normal = getFromFloat3(Loader->normalLine[i]);

			TriangelLine.push_back(TriangelObj);
		}
	}

	~EiMesh()
	{
		if (this->Loader)
			delete this->Loader;
	}
	


private:
	XMFLOAT4 getNormal(XMFLOAT3 normal1, XMFLOAT3 normal2, XMFLOAT3 normal3)
	{
		XMFLOAT3 mixedNormal = XMFLOAT3((normal1.x + normal2.x + normal3.x)/3.0, (normal1.y + normal2.y + normal3.y)/3.0, (normal1.z + normal2.z + normal3.z)/3.0);
		return getFromFloat3(mixedNormal);
	}
	EiAssimp* Loader;
};



#endif /* Resource_h */