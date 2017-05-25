//Evil illumination
//EiAssimp.h
//
//Create by MonsterENT on 17/05/24.


#ifndef EiAssimp_H
#define EiAssimp_H
#include"EiRender.h"

#include <Importer.hpp>      // 导入器在该头文件中定义
#include <scene.h>           // 读取到的模型数据都放在scene中
#include <postprocess.h>     // 该头文件中包含后处理的标志位定义


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