#include "src/libraries/SceneLibrary.hpp"
#include "src/libraries/GameEntityLibrary.hpp"

#include "src/scenes/SceneConfig.hpp"
#include "src/scenes/Scene.hpp"

#include "src/entities/EntityInstanceConfig.hpp"

#include "src/components/config/TextConfig.hpp"

#include "src/components/TransformComponent.hpp"
#include "src/components/InputComponent.hpp"
#include "src/components/TriggerComponent.hpp"

#include "src/systems/EventRelay.hpp"
#include "src/systems/EventTypes.hpp"
#include "src/util/Logger.hpp"

namespace Scenes
{
	static const SceneConfig MAIN_MENU = SceneLibrary::initSceneConfig(SceneEnum::MAIN_MENU);
	static const SceneConfig LEVEL_1 = SceneLibrary::initSceneConfig(SceneEnum::LEVEL_1);
}

/// @brief Get a statically allocated scene configuration. This will be called from Scene:: when a new scene is requested.
/// @brief It will always return a reference to the single statically allocated config for the scene enum specified.
/// @param scene 
/// @return 
const SceneConfig& SceneLibrary::get(SceneEnum scene)
{
	switch (scene)
	{
		case(SceneEnum::LEVEL_1):
			return Scenes::LEVEL_1;
		case(SceneEnum::MAIN_MENU):
		default:
			return Scenes::MAIN_MENU;
	}
}

/// @brief Build a scene config for the scene. This should not be called directly except when statically initializing scenes.
/// @brief Define your scene's contents here!
/// @param scene 
/// @return 
const SceneConfig SceneLibrary::initSceneConfig(SceneEnum scene)
{
	SceneConfig config;

	switch(scene)
	{
	case(SceneEnum::LEVEL_1):
	{
		//Player
		auto player = config.addEntity(GameEntityLibrary::get(GameEntityEnum::PLAYER));
		player.addInitFn([](int entityUID, auto& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,5.f,0.f });
		});

		//Floor
		auto floor = config.addEntity(GameEntityLibrary::get(GameEntityEnum::FLOOR));
		floor.addInitFn([](int entityUID, Scene& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,-2.f,0.f });
		});

		//Bush
		auto bush = config.addEntity(GameEntityLibrary::get(GameEntityEnum::BUSH));
		bush.addInitFn([](int entityUID, Scene& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 10.f,0.f,5.f });
		});

		//Mushroom
		auto mushroom1 = config.addEntity(GameEntityLibrary::get(GameEntityEnum::MUSHROOM));
		mushroom1.addInitFn([](int entityUID, Scene& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 10.f,0.f,5.f });

		});

		//Another mushroom
		auto mushroom2 = config.addEntity(GameEntityLibrary::get(GameEntityEnum::MUSHROOM));
		mushroom2.addInitFn([](int entityUID, Scene& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,0.f,10.f });
		});

		auto emitter = config.addEntity(GameEntityLibrary::get(GameEntityEnum::BLOCK_EMITTER));
		emitter.addInitFn([](int entityUID, Scene& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,2.f,0.f });
		});

		break;
	}

	case(SceneEnum::MAIN_MENU):
	{
		//Title
		auto title = config.addEntity(GameEntityLibrary::get(GameEntityEnum::BUTTON));
		title.addInitFn([](int entityUID, Scene& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({0.f,.5f,0.f });
		});

		//Start button
		auto startButton = config.addEntity(GameEntityLibrary::get(GameEntityEnum::BUTTON));
		startButton.addInitFn([](int entityUID, Scene& scene)
		{
				//NB: right now this button loads its text twice since default buttons start with text. Maybe we dont want that?
			TextConfig config;
			config.textToDisplay = "click to begin";
			config.centered = true;
			config.animated = true;
			config.margin = { .05f,.05f };
			config.fontSize = 5;
			scene.loadText(config, entityUID);

			scene.getComponent<TransformComponent>(entityUID).setTranslation({0.f,-.3f,0.f });

			scene.addComponent<InputComponent>(entityUID);

			scene.addComponent<TriggerComponent>(entityUID);
			TriggerComponent& triggerComponent = scene.getComponent<TriggerComponent>(entityUID);

			Trigger trigger;
			trigger.setUpdateCondition([](Scene& scene, int entityUID, float lifetime, float elapsedTime)
			{
				if (!scene.hasComponent<InputComponent>(entityUID))
				{
					return false;
				}

				//TODO: later make sure this click is actually on the button
				return scene.getComponent<InputComponent>(entityUID).getActiveInputs().contains(UserInputActionsEnum::LEFT_CLICKING);
			});
			trigger.setAction([](Scene& scene, int entityUID)
			{
				if (!scene.hasComponent<InputComponent>(entityUID))
				{
					return false;
				}
				
				std::optional<glm::vec2> coords = scene.getComponent<InputComponent>(entityUID).getInputActionWindowCoordinates(UserInputActionsEnum::LEFT_CLICKING);

				if (coords.has_value())
				{
					std::cout << "Clicked on the start button in a manner of speaking, coords were " << coords.value().x << " " << coords.value().y << std::endl;
				}

				scene.changeScene(SceneLibrary::get(SceneEnum::LEVEL_1));
			});

			triggerComponent.addTrigger(trigger);
		});

		//Floor
		auto floor = config.addEntity(GameEntityLibrary::get(GameEntityEnum::FLOOR));
		floor.addInitFn([](int entityUID, Scene& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setScale({ 1000.f,1000.f,1.f });
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,-2.f,-2.f });
			scene.getComponent<TransformComponent>(entityUID).setRotation({ 45.f,0.f,0.f });
		});

		break;
	}
	case(SceneEnum::NONE):
	default:
		break;
	}

	return config;
}

