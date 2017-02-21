#include "Importer.h"
#include <iostream>

using namespace std;


Importer::Importer()
{
}


Importer::~Importer()
{
}
void loadModel(string path)
{
	// Read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	// Retrieve the directory path of the filepath
	//this->directory = path.substr(0, path.find_last_of('/'));

	//// Process ASSIMP's root node recursively
	//this->processNode(scene->mRootNode, scene);
}
