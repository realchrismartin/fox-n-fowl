#include "extern/fox-engine/src/systems/Systems.hpp"
#include "src/libraries/SceneLibrary.hpp"

int main()
{
	Systems::runGame(SceneLibrary::get(SceneEnum::MAIN_MENU),"../img/sprite_sheet.png");

	return 0;
}
