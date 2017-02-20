
#include "BaseApp.h"
#include "Table.h"
#include "Ball.h"

namespace basicgraphics {

	class ExampleApp : public BaseApp {
		public:
			ExampleApp(int argc, char** argv, std::string windowName, int windowWidth, int windowHeight);
			~ExampleApp();

		private:
			std::unique_ptr<Box> _box;
			glm::vec2 mouse_pos;
			float _angle;

			std::unique_ptr<Ball> ball;

			void onRenderGraphics() override;
			void onUpdate(std::chrono::milliseconds m) override;
			void onEvent(std::shared_ptr<Event> event) override;
	};
}