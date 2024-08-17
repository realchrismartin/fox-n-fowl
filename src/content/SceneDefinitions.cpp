#include "src/content/SceneDefinitions.hpp"
#include "src/content/GameEntityDefinitions.hpp"

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
	static const SceneConfig MAIN_MENU = SceneDefinitions::initSceneConfig(SceneEnum::MAIN_MENU);
	static const SceneConfig LEVEL_1 = SceneDefinitions::initSceneConfig(SceneEnum::LEVEL_1);
}

/// @brief Get a statically allocated scene configuration. This will be called from Scene:: when a new scene is requested.
/// @brief It will always return a reference to the single statically allocated config for the scene enum specified.
/// @param scene 
/// @return 
const SceneConfig& SceneDefinitions::get(SceneEnum scene)
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
const SceneConfig SceneDefinitions::initSceneConfig(SceneEnum scene)
{
	SceneConfig config;

	switch(scene)
	{
	case(SceneEnum::LEVEL_1):
	{
		auto camera = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::FLOOR));
		camera.addInitFn([](int entityUID, auto& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setScale({.1f,.1f,.1f});
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,0.f,5.f });
			scene.setCameraEntity(entityUID);
			scene.addComponent<TriggerComponent>(entityUID);

			Trigger trigger;
			trigger.setUpdateCondition([](Scene& scene, int entityUID, float lifetime, float elapsedTime)
			{
				auto cameraTargetEntity = scene.getCameraTargetEntity();

				if(!cameraTargetEntity.has_value())
				{
					return false;
				}

				if(!scene.hasComponent<TransformComponent>(cameraTargetEntity.value()))
				{
					return false;
				}

				return true;
			});
			trigger.setAction([](Scene& scene, int entityUID)
			{
				auto cameraTargetEntity = scene.getCameraTargetEntity();

				if(!cameraTargetEntity.has_value())
				{
					return;
				}

				if(!scene.hasComponent<TransformComponent>(cameraTargetEntity.value()))
				{
					return;
				}

				//Use the transforms to determine how far apart the camera is from its target
				//NB: this assumes that neither the camera or the camera target are children in the scene graph - this should be fixed later
				const glm::mat4& cameraWorldMat = scene.getComponent<TransformComponent>(entityUID).getWorldMatrix();
				const glm::mat4& targetWorldMat = scene.getComponent<TransformComponent>(cameraTargetEntity.value()).getWorldMatrix();

				glm::vec4 cameraCenter = glm::vec4(1.0);
				glm::vec4 targetCenter = glm::vec4(1.0);

				cameraCenter = cameraWorldMat * cameraCenter;
				targetCenter = targetWorldMat * targetCenter;

				glm::vec3 translation = glm::vec3(0.f,0.f,0.f);

				//Only move if we're far from the target

				float x = std::abs(cameraCenter.x - targetCenter.x);
				float z = std::abs(cameraCenter.z - targetCenter.z);

				if(x > 7.f)
				{
					translation.x = cameraCenter.x > targetCenter.x ? -.1f : .1f;
				}

				if(z > 7.f)
				{
					translation.z = cameraCenter.z > targetCenter.z ? -.1f : .1f;
				}

				//TODO: use epsilon to check if floats are zeroed, this is bad practice 
				if(translation.x == 0.f && translation.z == 0.f)
				{
					return;
				}

				scene.getComponent<TransformComponent>(entityUID).addTranslation(translation);
			});

			scene.getComponent<TriggerComponent>(entityUID).addTrigger(trigger);
		});

		//Floor
		auto floor = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::FLOOR));
		floor.addInitFn([](int entityUID, Scene& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,-55.f,0.f });
		});

		auto skybox= config.addEntity(GameEntityDefinitions::get(GameEntityEnum::SKYBOX));
		skybox.addInitFn([](int entityUID, Scene& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,-8.f,-20.f });
		});

		auto player = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::PLAYER));
		player.addInitFn([](int entityUID, Scene& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 2.5f,-2.9f,-5.f });
			scene.setCameraTargetEntity(entityUID);
		});

		//Campfire
		auto campfire = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::INVISIBLE_ENTITY));
		campfire.addInitFn([](int entityUID, Scene& scene)
		{
			scene.getComponent<TransformComponent>(entityUID).setScale({2.f,2.f,2.f});
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 2.5f,-2.9f,-5.f });
		});

		auto log = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::LOG));
		log.addInitFn([campfire](int entityUID, Scene& scene)
		{
			scene.addChild(campfire.entityUID,entityUID);
			scene.getComponent<TransformComponent>(entityUID).setRotation({ 0.f,15.f,180.f});
			scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,-.3f,-.1f});
		});

		auto log2 = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::LOG));
		log2.addInitFn([log](int entityUID, Scene& scene)
		{
			scene.addChild(log.entityUID,entityUID);
			scene.getComponent<TransformComponent>(entityUID).setRotation({ 0.f,90.f,0.f});
		});

		auto fire = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::FIRE));
		fire.addInitFn([campfire](int entityUID, Scene& scene)
		{
			scene.addChild(campfire.entityUID,entityUID);
		});

		auto smoke = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::SMOKE));
		smoke.addInitFn([campfire](int entityUID, Scene& scene)
		{
			scene.addChild(campfire.entityUID,entityUID);
		});

		//Mushrooms
		float x = -20.f;

		for(size_t i=0; i < 5; i++)
		{
			x += (((rand() % 3) * .3f) + 2.5f);
			const float y = -3.f + (rand() % 2) * .1f; 
			const float z = -4.f + (rand() % 2) * .1f; 

			auto mushroom = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::MUSHROOM));
			mushroom.addInitFn([x,y,z](int entityUID, Scene& scene)
			{
				float scaleFactor = .5f + ((rand() % 2) * .15f);
				scene.getComponent<TransformComponent>(entityUID).setTranslation({x,y,z });
				scene.getComponent<TransformComponent>(entityUID).setScale({scaleFactor,scaleFactor,scaleFactor});
			});
		}

		//Trees
		x = -25.f;

		for(size_t i=0; i < 25; i++)
		{
			x += 3.5f + (rand() % 2) * .15f;
			float y = -4.f + (rand() % 2) * .5f; 

			auto tree = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::TREE_2));
			tree.addInitFn([x,y](int entityUID, Scene& scene)
			{
				float scaleFactor = 1.5f + ((rand() % 2) * .15f);
				scene.getComponent<TransformComponent>(entityUID).setTranslation({x,y,-10.f });
				scene.getComponent<TransformComponent>(entityUID).setScale({scaleFactor,scaleFactor,scaleFactor});
			});
		}

		x = -25.f;

		for(size_t i=0; i < 25; i++)
		{
			x += 3.f + (rand() % 2) * .15f;
			float y = -4.f + (rand() % 2) * .5f; 

			auto tree = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::TREE_2));
			tree.addInitFn([x,y](int entityUID, Scene& scene)
			{
				float scaleFactor = 1.25f + ((rand() % 2) * .15f);
				scene.getComponent<TransformComponent>(entityUID).setTranslation({x,y,-8.f });
				scene.getComponent<TransformComponent>(entityUID).setScale({scaleFactor,scaleFactor,scaleFactor});
			});
		}

		break;
	}

	case(SceneEnum::MAIN_MENU):
	{
		//Title
		auto title = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::BUTTON));
		title.addInitFn([](int entityUID, Scene& scene)
		{
			TextConfig config;
			config.textToDisplay = "fox n fowl";
			config.centered = true;
			config.margin = { .05f,.05f };
			config.fontSize = 20;
			scene.loadText(config, entityUID);
		});

		//Start button
		auto startButton = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::BUTTON));
		startButton.addInitFn([](int entityUID, Scene& scene)
		{
				//NB: right now this button loads its text twice since default buttons start with text. Maybe we dont want that?
			TextConfig config;
			config.textToDisplay = "click to begin";
			config.centered = true;
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

				scene.changeScene(SceneDefinitions::get(SceneEnum::LEVEL_1));
			});

			triggerComponent.addTrigger(trigger);
		});

		//Floor
		auto floor = config.addEntity(GameEntityDefinitions::get(GameEntityEnum::FLOOR));
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

