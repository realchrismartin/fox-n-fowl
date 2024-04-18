#include "src/systems/Systems.hpp"
#include "src/content/SceneDefinitions.hpp"

int main()
{
	Systems::runGame(SceneDefinitions::get(SceneEnum::MAIN_MENU),"../img/sprite_sheet.png");

	return 0;
}
