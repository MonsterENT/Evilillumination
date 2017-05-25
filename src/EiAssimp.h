//Evil illumination
//EiAssimp.h
//
//Create by MonsterENT on 17/05/24.


#ifndef EiAssimp_H
#define EiAssimp_H
#include"EiRender.h"

#include <Importer.hpp>      // �������ڸ�ͷ�ļ��ж���
#include <scene.h>           // ��ȡ����ģ�����ݶ�����scene��
#include <postprocess.h>     // ��ͷ�ļ��а�������ı�־λ����


#include<vector>
#include<string>
using std::vector;
using std::string;

class EiAssimp
{
public:
	EiAssimp(string fileName, XMFLOAT3 mul, XMFLOAT3 offset)
	{
		loadFile(fileName,mul,offset);
	}
	~EiAssimp()
	{

	}

	vector<XMFLOAT3> VertexLine;
	vector<XMFLOAT3> normalLine;
	vector<DWORD> IndexLine;

private:
	void loadFile(string fileName, XMFLOAT3 mul, XMFLOAT3 offset);
};


#endif //EiAssimp_H