//Evil illumination
//ResManager.h
//
//Created by MonsterENT on 17/05/22.




#ifndef ResManager_H
#define ResManager_H

#include"EiRender.h"
#include<vector>
#include"Resource.h"
#include<string>




using std::vector;
using std::string;


#define Solt_Input_SRV 5
#define Solt_Output_UAV 0

#define Solt_Rand_BufferSRV 6
#define Solt_Sphere_BufferSRV 1
#define Solt_Triangel_BufferSRV 3

#define Solt_GeneralInfor_BufferSRV 0



class ResManager
{
public:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ResManager(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
	
		this->device = device;
		this->deviceContext = deviceContext;
		this->ResView4Input = new SRV_UAV_Resource(device);
		this->ResView4Output = new SRV_UAV_Resource(device);

		this->ResView4RandValue = NULL;
		this->ResView4GeneralInfo = NULL;

		this->ResView4Sphere = NULL;
		this->ResView4Triangel = NULL;
	}
	~ResManager()
	{
		if (this->ResView4Input)
			delete this->ResView4Input;

		if (this->ResView4Output)
			delete this->ResView4Output;

		if (this->ResView4RandValue)
			delete this->ResView4RandValue;

		if (this->ResView4GeneralInfo)
			delete this->ResView4GeneralInfo;
		
		if (this->ResView4Sphere)
			delete this->ResView4Sphere;

		if (this->ResView4Triangel)
			delete this->ResView4Triangel;

		if (this->ResView4Input)
		{

		}

		if (this->ResView4Output)
		{
		
		}

	}

	ID3D11ShaderResourceView* Create_Buffer_SRV(UINT Stride, UINT Num, void* Data, ID3D11Buffer** buffer = NULL);

	void UpdateRandValue();

	void addResource(General_info* res);
	void addResource(Sphere* res);
	void addResource(Triangel* res);
	void addResource(RandValue* res);
	void addResource(EiMesh* res);
	void buildResource();

	SRV_UAV_Resource* ResView4Input;
	SRV_UAV_Resource* ResView4Output;

	ID3D11ShaderResourceView* ResView4RandValue;
	ID3D11ShaderResourceView* ResView4GeneralInfo;

	ID3D11ShaderResourceView*	ResView4Sphere;
	ID3D11ShaderResourceView*	ResView4Triangel;

private:
	RandValue* RandRes;
	General_info* InfoRes;
	vector<Triangel::TriangelStruct> TriangelResLine;
};



#endif //ResManager_H