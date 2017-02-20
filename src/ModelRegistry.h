#pragma once
#include "Model.h"
#include <map>
#include <string>

namespace basicgraphics {
	namespace model_registry {



		std::shared_ptr<Model> fetch(string key);
		std::shared_ptr<Model> fetchFile(string filename);

		//forces a reload
		std::shared_ptr<Model> loadFile(string filename);

	}

}
