//Evil illumination
//EiRender.cpp
//
//Created by MonsterENT on 17/05/22.


#include"EiRender.h"
#include"ResManager.h"


EiRender::EiRender(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ComputeShader* shader, UINT NumOfSamples)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->shader = shader;
	this->NumOfSamples = NumOfSamples;

	deviceContext->CSSetShader(this->shader, 0, 0);

};


void EiRender::setResManager(ResManager* manager)
{
	this->manager = manager;
	this->deviceContext->CSSetShaderResources(Solt_Triangel_BufferSRV, 1, &manager->ResView4Triangel);
	this->deviceContext->CSSetShaderResources(Solt_GeneralInfor_BufferSRV, 1, &manager->ResView4GeneralInfo);
	
	//this->deviceContext->CSSetShaderResources(Solt_Sphere_BufferSRV, 1, &manager->ResView4Sphere);

	

}

ID3D11ShaderResourceView* EiRender::Render()
{

	for (int i = 0; i < this->NumOfSamples; i++)
	{
		this->manager->UpdateRandValue();
		
		deviceContext->CSSetShaderResources(Solt_Input_SRV, 1, &this->manager->ResView4Input->srv);
		deviceContext->CSSetShaderResources(Solt_Rand_BufferSRV, 1, &this->manager->ResView4RandValue);

		deviceContext->CSSetUnorderedAccessViews(Solt_Output_UAV, 1, &this->manager->ResView4Output->uav, 0);

		deviceContext->Dispatch((UINT)ceilf((Ei_width) / 16), (UINT)ceilf((Ei_height) / 16), 1); // 启动着色器

		//解除绑定
		ID3D11UnorderedAccessView *pNullUAV = NULL;
		ID3D11ShaderResourceView  *nullSRV = NULL;
		deviceContext->CSSetUnorderedAccessViews(0, 1, &pNullUAV, NULL);
		deviceContext->CSSetShaderResources(5, 1, &nullSRV);

		deviceContext->CSSetShaderResources(Solt_Input_SRV, 1, &this->manager->ResView4Output->srv);
		deviceContext->CSSetUnorderedAccessViews(Solt_Output_UAV, 1, &this->manager->ResView4Input->uav, 0);
		deviceContext->Dispatch((UINT)ceilf((Ei_width) / 16), (UINT)ceilf((Ei_height) / 16), 1); // 启动着色器


		deviceContext->CSSetUnorderedAccessViews(0, 1, &pNullUAV, NULL);
		deviceContext->CSSetShaderResources(5, 1, &nullSRV);

	}

	return this->manager->ResView4Output->srv;
}