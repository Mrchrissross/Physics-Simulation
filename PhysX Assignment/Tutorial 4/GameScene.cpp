#include "GameScene.h"

namespace PhysicsEngine
{
	
	static int score = 0;
	static vector <int> scores;
	static int height = 0;
	static vector <int> heights;
	static float velocity = 0;
	static vector <float> velocities;

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
		plane->SetColor(color_palette[7]);
		plane->SetTrigger(true);
		plane->SetName("Plane");
		AddActor(plane);

		// Create Ball
		ball = new Ball(this, new PxVec3(0, 3, 50)); // Orig pos: 0, 3, 50

		startingFloor = new Flooring(this, new PxVec3(0.0f, 0.0f, 48.0f), 0.0f, 3.0f, true, false, false);

		int miniwinds = 0;
		for (int i = 0; i < 100; i++)
		{
			if ((i % 6) == 0 && i != 0)
			{

				miniwinds++;
				flooring.push_back(new Flooring(this, new PxVec3(0.0f, 0.0f, i * -24.0f), 0.0f, 3.0f, false, false, true));

				if ((miniwinds % 2) == 0) 
					miniWindmills.push_back(new MiniWindmill(this, new PxVec3(0.0f, 2.0f, i * -24.0f), false, 0.97f));
				else
					miniWindmills.push_back(new MiniWindmill(this, new PxVec3(3.75f, 2.0f, i * -24.0f), true, 0.97f));

			}
			else
				flooring.push_back(new Flooring(this, new PxVec3(0.0f, 0.0f, i * -24.0f), 0.0f, 3.0f, false, true, false));
		}
		
		miniWindmills.push_back(new MiniWindmill(this, new PxVec3(0.0f, 2.0f, 5.0f), false, 0.97f));

		cloth = new Cloth(PxTransform(PxVec3(4, 1.7f, 45.0f), PxQuat(0.0, -0.707, 0.0, 0.707)), PxVec2(1.0f, 8.0f), 40, 40);
		cloth->SetColor(color_palette[2]);
		((PxCloth*)cloth->GetPxActor())->setStretchConfig(PxClothFabricPhaseType::eSHEARING, PxClothStretchConfig(0.2f));
		AddActor(cloth);

		ramp = new Ramp(this, new PxVec3(1.75f, 0.45f, 43.8f), 1.45f);
		wobblyPlatform = new WobblyPlatform(this, new PxVec3(0.0f, 1.2f, 39.0f), 0.0f, 1.0f);

		catapult = new Catapult(this, new PxVec3(0.0f, -1.75f, 33.5f), 1.0f);
		button = new CatapultButton(this, new PxVec3(0.0f, 0.5f, 48.0f), catapult);

		my_callback = new MySimulationEventCallback(ball, button);
		px_scene->setSimulationEventCallback(my_callback);
		
	}

	void GameScene::Update()
	{
		for (int i = 0; i < miniWindmills.size(); i++)
		{
			miniWindmills[i]->Update();
		}

		button->Update();
		catapult->Update();

		if ((-ball->GetPosition().z + 38) >= 0)
		{
			score = -ball->GetPosition().z + 38;

			// Determine Max Height
			if (height < ball->GetPosition().y)
				height = ball->GetPosition().y;
		
			if (-velocity > ball->GetBall()->GetVelocity().z)
				velocity = -ball->GetBall()->GetVelocity().z;
		}
		else
			score = 0;

		if (score > 5)
			button->activated = false;

		if (ball->addScore)
		{
			scores.push_back(score);
			heights.push_back(height);
			velocities.push_back(velocity);

			height   = 0;
			velocity = 0;

			ball->Reset();
			ball->addScore = false;
		}
	}

	#pragma region Scoring

		int GameScene::GetScore()
		{
			return score;
		}

		vector <int> GameScene::GetScoreBoard()
		{
			return scores;
		}

		int GameScene::GetHeight()
		{
			return height;
		}

		vector <int> GameScene::GetHeightBoard()
		{
			return heights;
		}
	
		float GameScene::GetVelocity()
		{
			return velocity;
		}

		vector <float> GameScene::GetVelocityBoard()
		{
			return velocities;
		}

	#pragma endregion

}
