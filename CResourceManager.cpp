#include "CResourceManager.h"

CResourceManager::CResourceManager()
{
	meshCount = 0;
}

int CResourceManager::LoadSkyBoxTexture(void)
{
	if (LoadedTextures["SkyBox"] != 0)
	{
		return LoadedTextures["SkyBox"];
	}
	else
	{
		CCubeTexture* SkyBoxTex = new CCubeTexture;
		SkyBoxTex->Load("TEXTURES/SkyBox/CloudyCrown_01_Midday/CloudyCrown_Midday_Back.png", "TEXTURES/SkyBox/CloudyCrown_01_Midday/CloudyCrown_Midday_Front.png",
			"TEXTURES/SkyBox/CloudyCrown_01_Midday/CloudyCrown_Midday_Left.png", "TEXTURES/SkyBox/CloudyCrown_01_Midday/CloudyCrown_Midday_Right.png",
			"TEXTURES/SkyBox/CloudyCrown_01_Midday/CloudyCrown_Midday_Down.png", "TEXTURES/SkyBox/CloudyCrown_01_Midday/CloudyCrown_Midday_Up.png");
		texCount++;
		LoadedTextures["SkyBox"] = texCount;
		textures.push_back(SkyBoxTex);
		return texCount;
	}
}

int CResourceManager::LoadSkyBoxMesh(void)
{
	CMesh *tempMesh = new CMesh;
	if (LoadedMeshes["meshes/box.obj"] != 0)
	{
		return LoadedMeshes["meshes/box.obj"];
	}
	else
	{
		if (tempMesh->LoadOBJ("meshes/box.obj") == -1)
		{
			return -1;
		}
		else
		{
			meshCount++;
			meshes.push_back(tempMesh);
			LoadedMeshes["meshes/box.obj"] = meshCount;
			return LoadedMeshes["meshes/box.obj"];
		}
	}
}

int CResourceManager::LoadMesh(std::string filename)
{
	CMesh *tempMesh = new CMesh;
	if(LoadedMeshes[filename]!=0)
	{
		return LoadedMeshes[filename];
	}
	else
	{
		if (tempMesh->LoadOBJ(filename.c_str()) == -1)
		{
			return -1;
		}
		else
		{
			meshCount++;
			meshes.push_back(tempMesh);
			LoadedMeshes[filename] = meshCount;
			return LoadedMeshes[filename];
		}
	}
}

int CResourceManager::LoadTexture(std::string filename)
{
	CTexture* temp=new CTexture;
	if (LoadedTextures[filename] != 0)
	{
		return LoadedTextures[filename];
	}
	else
	{
		temp->Load(filename.c_str());
		texCount++;
		LoadedTextures[filename] = texCount;
		textures.push_back(temp);
		return texCount;
	}
}

CTexture* CResourceManager::GetTexture(int index)
{
	if ((index > textures.size()) || (index-1 < 0))
		return nullptr;
	else
		return textures[index-1];
}

CMesh* CResourceManager::GetMesh(int index)
{
	if ((index> meshes.size()) || (index-1 < 0))
		return nullptr;
	else
	return meshes[index-1];
}