#include "src/content/GameEntityDefinitions.hpp"

#include "src/entities/GameEntityConfig.hpp"
#include "src/components/ComponentPool.hpp"
#include "src/scenes/Scene.hpp"
#include "src/components/InputComponent.hpp"
#include "src/components/TransformComponent.hpp"
#include "src/components/TriggerComponent.hpp"
#include "src/components/config/ModelConfig.hpp"
#include "src/components/config/TextConfig.hpp"

//Static definitions for different types of entities go here!
namespace GameEntities 
{
	static const GameEntityConfig INVISIBLE_ENTITY = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			//Has an extremely small default model at scale 1,1
			ModelConfig model;
			model.keyframeFilePaths = { "../img/tiny_cube.obj" };
			scene.loadModel(model, entityUID);
		});

	static const GameEntityConfig TITLE_TEXT = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			TextConfig config;
			config.textToDisplay = "fox n fowl";
			config.charactersPerLine = 10;
			config.centered = true;
			scene.loadText(config, entityUID);
			config.margin = { .15f,.15f };
			config.fontSize = 15;
			scene.loadText(config, entityUID);
		});

	static const GameEntityConfig BUTTON = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			TextConfig config;
			config.textToDisplay = "Default Button Text";
			config.centered = true;
			config.animated = true;
			config.margin = { .05f,.05f };
			config.fontSize = 5;
			scene.loadText(config, entityUID);
		});

	static const GameEntityConfig PLAYER = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			scene.addComponent<InputComponent>(entityUID);

			ModelConfig model;
			model.spriteSize = { 32,32 };
			model.spriteOffsetOnTexture = { 0,1728 };
			model.keyframeFilePaths = { "../img/tree_1.obj" };

			scene.loadModel(model, entityUID);
			scene.getComponent<TransformComponent>(entityUID).setScale({ 2.f,2.f,2.f});
		});

	static const GameEntityConfig FLOOR = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			ModelConfig model;
			model.spriteSize = { 512,512 };
			model.spriteOffsetOnTexture = { 511,1988 };
			model.keyframeFilePaths = { "../img/cube.obj" };

			scene.loadModel(model, entityUID);
			scene.getComponent<TransformComponent>(entityUID).setScale({ 100.f,1.f,100.f });
		});
	static const GameEntityConfig BACKGROUND = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			ModelConfig model;
			model.spriteSize = { 10,10 };
			model.spriteOffsetOnTexture = { 36,1756 };
			model.keyframeFilePaths = { "../img/cube.obj" };

			scene.loadModel(model, entityUID);
			scene.getComponent<TransformComponent>(entityUID).setScale({ 100.f,100.f,1.f });
		});

	static const GameEntityConfig BUSH = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			ModelConfig model;
			model.spriteSize = { 228,228 };
			model.spriteOffsetOnTexture = { 0,1759 };
			model.keyframeFilePaths = { "../img/quoteunquote-bush.obj" };

			scene.loadModel(model, entityUID);
			scene.getComponent<TransformComponent>(entityUID).setScale({ 2.f,2.f,2.f});
		});
	static const GameEntityConfig TREE_1 = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			ModelConfig model;
			model.spriteSize = { 32,32 };
			model.spriteOffsetOnTexture = { 0,1728 };
			model.keyframeFilePaths = { "../img/tree_1.obj" };

			scene.loadModel(model, entityUID);
			scene.getComponent<TransformComponent>(entityUID).setScale({ 2.f,2.f,2.f});
		});
	static const GameEntityConfig TREE_2 = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			ModelConfig model;
			model.spriteSize = { 32,32 };
			model.spriteOffsetOnTexture = { 0,1728 };
			model.keyframeFilePaths = { "../img/tree_2.obj" };

			scene.loadModel(model, entityUID);
			scene.getComponent<TransformComponent>(entityUID).setScale({ 2.f,2.f,2.f});
		});
	static const GameEntityConfig LOG = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			ModelConfig model;
			model.spriteSize = { 32,32 };
			model.spriteOffsetOnTexture = { 0,1728 };
			model.keyframeFilePaths = { "../img/log.obj" };

			scene.loadModel(model, entityUID);
		});
	static const GameEntityConfig MUSHROOM = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			ModelConfig model;
			model.spriteSize = { 1024.f,1024.f };
			model.spriteOffsetOnTexture = { 1023.f,1476 };
			model.frameCount = 60;
			model.keyframeFilePaths = {
				"../img/mushroom/mushroom0.obj",
				"../img/mushroom/mushroom5.obj",
				"../img/mushroom/mushroom10.obj",
				"../img/mushroom/mushroom15.obj",
				"../img/mushroom/mushroom20.obj"
			};

			scene.loadModel(model, entityUID);
			scene.getComponent<TransformComponent>(entityUID).setScale({ .5f,.5f,.5f});
		});

	static const GameEntityConfig SMOKE = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			//The emitter itself
			ModelConfig model;
			model.spriteSize = { 10.f,10.f };
			model.spriteOffsetOnTexture = { 329.f,16.f };
			model.keyframeFilePaths = { "../img/cube.obj" };
			scene.loadModel(model, entityUID);

			//The emitted smoke
			model.spriteSize = { 10.f,10.f };
			model.spriteOffsetOnTexture = { 329.f,16.f };
			model.keyframeFilePaths = {
				"../img/cube.obj",
			};

			scene.getComponent<TransformComponent>(entityUID).setScale({ .01f,.01f,.01f});

			float visibleDuration = 5.f;

			//The stuff that get emitted
			for (int i = 0; i < 50; i++)
			{
				float startDelay = i * ((((rand() % 2) + 1) * .15f) + .15f);

				std::optional<int> id = scene.createEntity();

				if (!id.has_value())
				{
					continue;
				}
				
				scene.loadModel(model, id.value());
				scene.getComponent<TransformComponent>(id.value()).setScale({ 3.f,3.f,3.f });

				//Add this cube as a child of the emitter
				scene.addChild(entityUID, id.value());

				//Set this entity as inactive
				scene.setEntityActiveStatus(id.value(), false);

				scene.addComponent<TriggerComponent>(id.value());
				TriggerComponent& triggerComponent = scene.getComponent<TriggerComponent>(id.value());

				//Become visible trigger
				Trigger trigger;
				trigger.setUpdateCondition([startDelay](Scene& scene, int entityUID, float lifetime, float elapsedTime)
				{
					return lifetime > startDelay;
				});
				trigger.setAction([](Scene& scene, int entityUID)
				{
					scene.setEntityActiveStatus(entityUID,true);
				});

				triggerComponent.addTrigger(trigger);

				//Movement trigger
				Trigger moveTrigger;
				moveTrigger.setUpdateCondition([startDelay,visibleDuration](Scene& scene, int entityUID, float lifetime, float elapsedTime)
				{
					return lifetime > startDelay && lifetime < visibleDuration + startDelay;
				});
				moveTrigger.setAction([](Scene& scene, int entityUID)
				{
					if (!scene.hasComponent<TransformComponent>(entityUID))
					{
						return;
					}

					//Pick random directions upward
					
					float xDirection = (rand() % 2) > 0 ? 1.f : -1.f;
					float zDirection = (rand() % 2) > 0 ? 1.f : -1.f;

					constexpr float moveFactor = 1.5f;

					float xTranslation = xDirection * (float)(rand() % 2) * moveFactor;
					float yTranslation =  (float)(rand() % 2) * moveFactor;
					float zTranslation = xDirection * (float)(rand() % 2) * moveFactor;

					scene.getComponent<TransformComponent>(entityUID).addTranslation({ xTranslation,yTranslation,zTranslation });
				});

				triggerComponent.addTrigger(moveTrigger);

				//Reset trigger
				Trigger resetTrigger;
				resetTrigger.setUpdateCondition([startDelay,visibleDuration](Scene& scene, int entityUID, float lifetime, float elapsedTime)
				{
					return lifetime > visibleDuration + startDelay;
				});
				resetTrigger.setAction([](Scene& scene, int entityUID)
				{
					scene.setEntityActiveStatus(entityUID,false);

					if (!scene.hasComponent<TransformComponent>(entityUID))
					{
						return;
					}

					scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,0.f,0.f }); //Back to parent origin

					if (!scene.hasComponent<TriggerComponent>(entityUID))
					{
						//Weird...
						return;
					}

					scene.getComponent<TriggerComponent>(entityUID).resetLifetime();
				});

				triggerComponent.addTrigger(resetTrigger);
			}
		});

	static const GameEntityConfig FIRE = GameEntityConfig()
		.whenInit([](int entityUID, auto& scene)
		{
			//The emitter itself
			ModelConfig model;
			model.spriteSize = { 10.f,10.f };
			model.spriteOffsetOnTexture = { 329.f,16.f };
			model.keyframeFilePaths = { "../img/cube.obj" };
			scene.loadModel(model, entityUID);

			//The emitted fire
			model.spriteSize = { 2.f,2.f };
			model.spriteOffsetOnTexture = { 11,1758 };
			model.keyframeFilePaths = {
				"../img/cube.obj",
			};

			scene.getComponent<TransformComponent>(entityUID).setScale({ .01f,.01f,.01f});

			float visibleDuration = 3.f;

			//The stuff that get emitted
			for (int i = 0; i < 70; i++)
			{
				float startDelay = i * ((((rand() % 2) + 1) * .05f) + .15f);

				std::optional<int> id = scene.createEntity();

				if (!id.has_value())
				{
					continue;
				}

				scene.loadModel(model, id.value());
				scene.getComponent<TransformComponent>(id.value()).setScale({ 10.f,10.f,10.f });

				//Add this cube as a child of the emitter
				scene.addChild(entityUID, id.value());

				//Set this entity as inactive
				scene.setEntityActiveStatus(id.value(), false);

				scene.addComponent<TriggerComponent>(id.value());
				TriggerComponent& triggerComponent = scene.getComponent<TriggerComponent>(id.value());

				//Become visible trigger
				Trigger trigger;
				trigger.setUpdateCondition([startDelay](Scene& scene, int entityUID, float lifetime, float elapsedTime)
				{
					return lifetime > startDelay;
				});
				trigger.setAction([](Scene& scene, int entityUID)
				{
					scene.setEntityActiveStatus(entityUID,true);
				});

				triggerComponent.addTrigger(trigger);

				//Movement trigger
				Trigger moveTrigger;
				moveTrigger.setUpdateCondition([startDelay,visibleDuration](Scene& scene, int entityUID, float lifetime, float elapsedTime)
				{
					return lifetime > startDelay && lifetime < visibleDuration + startDelay;
				});
				moveTrigger.setAction([](Scene& scene, int entityUID)
				{
					if (!scene.hasComponent<TransformComponent>(entityUID))
					{
						return;
					}

					//Pick random directions upward
					
					float xDirection = (rand() % 2) > 0 ? 1.f : -1.f;
					float zDirection = (rand() % 2) > 0 ? 1.f : -1.f;

					constexpr float moveFactor = .7f;

					float xTranslation = xDirection * (float)(rand() % 2) * moveFactor;
					float yTranslation =  (float)(rand() % 2) * moveFactor;
					float zTranslation = xDirection * (float)(rand() % 2) * moveFactor;

					scene.getComponent<TransformComponent>(entityUID).addTranslation({ xTranslation,yTranslation,zTranslation });
				});

				triggerComponent.addTrigger(moveTrigger);

				//Reset trigger
				Trigger resetTrigger;
				resetTrigger.setUpdateCondition([startDelay,visibleDuration](Scene& scene, int entityUID, float lifetime, float elapsedTime)
				{
					return lifetime > visibleDuration + startDelay;
				});
				resetTrigger.setAction([](Scene& scene, int entityUID)
				{
					scene.setEntityActiveStatus(entityUID,false);

					if (!scene.hasComponent<TransformComponent>(entityUID))
					{
						return;
					}

					scene.getComponent<TransformComponent>(entityUID).setTranslation({ 0.f,0.f,0.f }); //Back to parent origin

					if (!scene.hasComponent<TriggerComponent>(entityUID))
					{
						//Weird...
						return;
					}

					scene.getComponent<TriggerComponent>(entityUID).resetLifetime();
				});

				triggerComponent.addTrigger(resetTrigger);
			}
		});
}

const GameEntityConfig& GameEntityDefinitions::get(GameEntityEnum gameEntity)
{
	switch (gameEntity)
	{
		case(GameEntityEnum::TITLE_TEXT):
			return GameEntities::TITLE_TEXT;
		case(GameEntityEnum::BUTTON):
			return GameEntities::BUTTON;
		case(GameEntityEnum::PLAYER):
			return GameEntities::PLAYER;
		case(GameEntityEnum::FLOOR):
			return GameEntities::FLOOR;
		case(GameEntityEnum::BUSH):
			return GameEntities::BUSH;
		case(GameEntityEnum::MUSHROOM):
			return GameEntities::MUSHROOM;
		case(GameEntityEnum::FIRE):
			return GameEntities::FIRE;
		case(GameEntityEnum::SMOKE):
			return GameEntities::SMOKE;
		case(GameEntityEnum::TREE_1):
			return GameEntities::TREE_1;
		case(GameEntityEnum::TREE_2):
			return GameEntities::TREE_2;
		case(GameEntityEnum::LOG):
			return GameEntities::LOG;
		case(GameEntityEnum::BACKGROUND):
			return GameEntities::BACKGROUND;
		case(GameEntityEnum::INVISIBLE_ENTITY):
		return GameEntities::INVISIBLE_ENTITY;
		default:
			return GameEntities::BUSH; //quote unqoute
	}
}
