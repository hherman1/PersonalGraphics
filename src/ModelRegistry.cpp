#include "ModelRegistry.h"


using namespace std; 


namespace basicgraphics {
	namespace model_registry {
		map<string, shared_ptr<Model>> registry;

		shared_ptr<Model> fetch(string key) {
			return registry.at(key); 
		}
		shared_ptr<Model> fetchFile(string filename) {
			try {
				return fetch(filename);
			}
			catch (out_of_range e) {
				return loadFile(filename);
			}
		}
		shared_ptr<Model> loadFile(string filename) {
			shared_ptr<Model> m = make_shared<Model>(filename, 1.0, vec4(1, 1, 1, 1));
			registry.insert(pair<string,shared_ptr<Model>>(filename, m));
			return m;

		}

	}
	
}
