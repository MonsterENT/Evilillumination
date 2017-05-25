//Evil illumination
//EiAssimp.cpp
//
//Create by MonsterENT on 17/05/24.



#include"EiAssimp.h"




void EiAssimp::loadFile(string fileName, XMFLOAT3 mul, XMFLOAT3 offset)
{
	Assimp::Importer imp;

	const aiScene* pscene = imp.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

	//Sleep(1);
	if (!pscene)
	{
		MessageBox(0, L"Model unload", L"assimp error", MB_OK);
		return;
	}

	for (int i = 0; i < pscene->mNumMeshes; i++)
	{
		aiMesh* pmesh = pscene->mMeshes[i];

		for (int i2 = 0; i2 < pmesh->mNumVertices; i2++)
		{

			XMFLOAT3 Vertex = { (float)(pmesh->mVertices[i2].x * mul.x + offset.x), (float)(pmesh->mVertices[i2].y * mul.y + offset.y), (float)(pmesh->mVertices[i2].z * mul.z + offset.z) };
			XMFLOAT3 Normal = XMFLOAT3(pmesh->mNormals[i2].x, pmesh->mNormals[i2].y, pmesh->mNormals[i2].z);

			this->normalLine.push_back(Normal);
			this->VertexLine.push_back(Vertex);
		}

		for (int i3 = 0; i3 < pmesh->mNumFaces; i3++)
		{
			this->IndexLine.push_back(pmesh->mFaces[i3].mIndices[0]);
			this->IndexLine.push_back(pmesh->mFaces[i3].mIndices[1]);
			this->IndexLine.push_back(pmesh->mFaces[i3].mIndices[2]);
		}

	}
}