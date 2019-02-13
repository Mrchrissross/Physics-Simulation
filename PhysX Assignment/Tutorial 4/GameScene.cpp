#include "GameScene.h"

namespace PhysicsEngine
{
	void GameScene::SetVisualisation()
	{
		px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);

		px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_HORIZONTAL, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_VERTICAL, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_BENDING, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_SHEARING, 1.0f);
	}

	void GameScene::Init()
	{
		SetVisualisation();

		GetMaterial()->setRestitution(0.0f);

		// Create Plane
		plane = new Plane();
		plane->SetColor(color_palette[3]);
		plane->SetTrigger(true);
		plane->SetName("Plane");
		AddActor(plane);

		// Create Ball
		ball = new Ball(this, new PxVec3(0, 3, 50)); // Orig pos: 0, 3, 50

		

		startingFloor = new Flooring(this, new PxVec3(0.0f, 0.0f, 45.0f), 0.0f, 2.0f, true);

		for (int i = 0; i < 10; i++)
		{
			flooring.push_back(new Flooring(this, new PxVec3(0.0f, 0.0f, i * -24.0f), 0.0f, 2.0f, false));
			flooring[i]->Floor->SetColor(PxVec3(255, 255, 255));
		}
		
		flooring.push_back(new Flooring(this, new PxVec3(0.0f, 1.5f, 38.5f), 0.0f, 0.5f, false));
		
		/*for (int i = 0; i < 3; i++)
		{
			trackRot.push_back(new TrackPieceRotated(this, new PxVec3(i * 12.0f + 10.0f, 10.0f, -18.0f), 0.0f, 2.0f));
		}

		flag = new FlagPole(this, new PxVec3(38.0f, 10.5f, -18.0f), 0.0f, 1.0f);*/

		miniWindmill = new MiniWindmill(this, new PxVec3(0.0f, 2.0f, 5.0f), false, 0.97f);


		box = new SpringBox(this, new PxVec3(0.0f, 0.5f, 17.0f), 0.0f, 1.0f);


		bridge = new Bridge(this, new PxVec3(0.0f, 0.0f, -9.0f), new PxVec3(0, 0, 0), 1.0f);

		//corner = new TrackPiece(this, new PxVec3(0.0f, 0.0f, -18.0f), 0.0f, 2.0f, true);

		ramp = new Ramp(this, new PxVec3(1.75f, 0.45f, 42.5f), 1.0f);

		cloth = new Cloth(PxTransform(PxVec3(4, 1.5f, 45.0f), PxQuat(0.0, -0.707, 0.0, 0.707)), PxVec2(1.0f, 8.0f), 40, 40);
		cloth->SetColor(color_palette[2]);
		((PxCloth*)cloth->GetPxActor())->setStretchConfig(PxClothFabricPhaseType::eSHEARING, PxClothStretchConfig(0.2f));
		AddActor(cloth);

		catapult = new Catapult(this, new PxVec3(0.0f, -1.0f, 30.0f), new PxVec3(0, 0, 0), 1.0f);
		button = new CatapultButton(this, new PxVec3(0.0f, 1.5f, 41.0f), catapult);

		//sand = new Sandpit(this, new PxVec3(25.0f, 10.45f, -18.0f), PxQuat(0.0f, 0.0f, 0.0f, 0.0f), 1.0f);

		my_callback = new MySimulationEventCallback(button);
		px_scene->setSimulationEventCallback(my_callback);
	}

	void GameScene::Update()
	{
		miniWindmill->Update();
		//corner->Update();
		button->Update();
		catapult->Update();
	}
}
