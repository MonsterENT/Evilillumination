//Evil illumination
//Resource.cpp
//
//Created by MonsterENT on 17/05/22.



#include"Resource.h"



XMFLOAT4 getFromVec(XMVECTOR vec)
{
	return XMFLOAT4(XMVectorGetX(vec), XMVectorGetY(vec), XMVectorGetZ(vec), 0);

}


XMFLOAT4 getFromFloat3(XMFLOAT3 float3)
{
	return XMFLOAT4(float3.x, float3.y, float3.z, 1);
}