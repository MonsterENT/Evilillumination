//Evil illumination
//ResManager.cpp
//
//Created by MonsterENT on 17/05/22.


#include"ResManager.h"
#include"Resource.h"


ID3D11ShaderResourceView* ResManager::Create_Buffer_SRV(UINT Stride, UINT Num, void* Data, ID3D11Buffer** buffer)
{

	ID3D11Buffer* pbuffer = NULL;

	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = Stride*Num;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.StructureByteStride = Stride;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;


	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = Data;



	HRESULT R1 = device->CreateBuffer(&desc, &data, &pbuffer);
	if (FAILED(R1)) MessageBox(0, L"r1", 0, 0);


	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;

	srv_desc.Format = DXGI_FORMAT_UNKNOWN;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srv_desc.Buffer.FirstElement = 0;
	srv_desc.Buffer.NumElements = Num;
	// srv_desc.Buffer.ElementWidth = sizeof(test);


	ID3D11ShaderResourceView* srv = NULL;
	HRESULT R2 = device->CreateShaderResourceView(pbuffer, &srv_desc, &srv);
	if (FAILED(R2)) MessageBox(0, L"r2", 0, 0);

	if (buffer)
		*buffer = pbuffer;
	return srv;


}

void ResManager::UpdateRandValue()
{

		for (int i = 0; i < Ei_width; i++)
			for (int j = 0; j < Ei_height; j++)
			{
				for (int k = 0; k < 10; k++)
					this->RandRes->resource[i][j].data[k] = this->RandRes->drand();
			}

		D3D11_MAPPED_SUBRESOURCE Pmap;
		HRESULT back = deviceContext->Map(this->RandRes->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Pmap);
		if (back == E_INVALIDARG) MessageBox(0, L"map", 0, 0);


		memcpy(Pmap.pData, this->RandRes->resource, Ei_width* Ei_height *sizeof(RandValue::RandStruct));

		deviceContext->Unmap(this->RandRes->buffer, 0);


}

void ResManager::addResource(General_info* res)
{
	this->InfoRes = res;
}

void ResManager::addResource(Sphere* res)
{
	/*this->SphereResLine.push_back(res->resource);*/
}

void ResManager::addResource(Triangel* res)
{
	this->TriangelResLine.push_back(*res->resource);
}


void ResManager::addResource(RandValue* res)
{
	this->RandRes = res;
	if (this->ResView4RandValue)
		delete this->ResView4RandValue;
	this->ResView4RandValue = Create_Buffer_SRV(sizeof(RandValue::RandStruct), Ei_width * Ei_height, res->resource, &res->buffer);
}

void ResManager::addResource(EiMesh* res)
{
	for (int i = 0; i < res->TriangelLine.size(); i++)
	{
		this->addResource(res->TriangelLine[i]);
	}
}

void ResManager::buildResource()
{
	if (!this->TriangelResLine.empty())
	{
		if (this->ResView4Triangel)
			delete this->ResView4Triangel;
		this->ResView4Triangel = Create_Buffer_SRV(sizeof(Triangel::TriangelStruct), this->TriangelResLine.size(), &this->TriangelResLine[0], 0);
	}



	if (this->ResView4GeneralInfo)
		delete this->ResView4GeneralInfo;
	this->InfoRes->resouce->numTriangelX_numSamplesY.x = this->TriangelResLine.size();
	this->ResView4GeneralInfo = Create_Buffer_SRV(sizeof(General_info::infoStruct), 1, this->InfoRes->resouce, 0);
	/*if (!this->SphereResLine.empty())
	{
		if (this->ResView4Sphere)
			delete this->ResView4Sphere;
		this->ResView4Sphere = Create_Buffer_SRV(sizeof(Sphere::SphereStruct), this->SphereResLine.size(), &this->SphereResLine[0], 0);
	}*/
}