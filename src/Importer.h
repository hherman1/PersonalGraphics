#pragma once
#include "Graphics.h"
#include "Mesh.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <vector>
#include <memory>

namespace importer {


	std::shared_ptr<IndexedMeshes> loadModel(string path);
}

