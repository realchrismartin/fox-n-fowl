#ifndef GAMEENTITYDEFINITIONS_HPP
#define GAMEENTITYDEFINITIONS_HPP

enum class GameEntityEnum
{
	TITLE_TEXT,
	BUTTON,
	PLAYER,
	FLOOR,
	SKYBOX,
	BUSH,
	MUSHROOM,
	TREE_1,
	TREE_2,
	LOG,
	FIRE,
	SMOKE,
	INVISIBLE_ENTITY
};

struct GameEntityConfig;

//To add a new game entity:
// 0) Update GameEntityEnum to add an ID for your game entity
// 1) Add a static GameEntityConfig variable for your enum to the GameEntities namespace in GameEntityDefinitions.cpp
// 2) Add a mapping in get() for your enum to the variable created in #1

namespace GameEntityDefinitions
{
	/// @brief Get a reference to the static game entity config for a given entity type
	/// @param gameEntity 
	/// @return 
	const GameEntityConfig& get(GameEntityEnum gameEntity);
}

#endif