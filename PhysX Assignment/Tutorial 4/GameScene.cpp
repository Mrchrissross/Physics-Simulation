#include "GameScene.h"

namespace PhysicsEngine
{
	static int distance = 0;
	static vector <int> distances;
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

		plane->SetupFiltering(PLANE, BALL);

		// Create Ball
		ball = new Ball(this, new PxVec3(0, 3, 50)); // Orig pos: 0, 3, 50

		startingFloor = new Flooring(this, new PxVec3(0.0f, 0.0f, 48.0f), new PxVec3(6.0f, 0.5, 9.0f), color_palette[6], true, false, false);

		float spacing = -18.0f;
		for (int i = 0; i < 14; i++)
		{
			flooring.push_back(new Flooring(this, new PxVec3(-12.0f, 0.0f, i * spacing), new PxVec3(6.0f, 0.5, 9.0f), color_palette[5], false, true, false));
			flooring.push_back(new Flooring(this, new PxVec3(0.0f, 0.0f, i * spacing), new PxVec3(6.0f, 0.5, 9.0f), color_palette[5], false, true, false));
			flooring.push_back(new Flooring(this, new PxVec3(12.0f, 0.0f, i * spacing), new PxVec3(6.0f, 0.5, 9.0f), color_palette[5], false, true, false));
		}

		spacing = 10.0;

		for (int i = 1; i < (3 + 1); i++)
		{
			flooring.push_back(new Flooring(this, new PxVec3(0.0f, 0.005f, flooring[spacing * i]->Floor->GetPosition().z), new PxVec3(6.0f, 0.55, 6.0f), color_palette[10], false, true, false));
			flooring.push_back(new Flooring(this, new PxVec3(0.0f, 0.5f, flooring[spacing * i]->Floor->GetPosition().z), new PxVec3(3.0f, 0.25, 3.0f), color_palette[9], false, false, true));
			flooring.push_back(new Flooring(this, new PxVec3(0.0f, 0.01f, flooring[spacing * i]->Floor->GetPosition().z), new PxVec3(18.0f, 0.5, 0.75f), color_palette[10], false, true, false));
			flooring.push_back(new Flooring(this, new PxVec3(0.0f, 0.7f, flooring[spacing * i]->Floor->GetPosition().z), new PxVec3(0.5f, 0.1f, 0.75f), color_palette[10], false, true, false));
		}
		
		miniWindmills.push_back(new MiniWindmill(this, new PxVec3(-4.0f, 2.2f, 5.0f), false, 0.97f));
		miniWindmills.push_back(new MiniWindmill(this, new PxVec3(5.0f, 2.2f, -40.0f), true, 0.97f));
		miniWindmills.push_back(new MiniWindmill(this, new PxVec3(-2.0f, 2.2f, -75.0f), false, 0.97f));
		miniWindmills.push_back(new MiniWindmill(this, new PxVec3(8.0f, 2.2f, -120.0f), true, 0.97f));
		miniWindmills.push_back(new MiniWindmill(this, new PxVec3(0.0f, 2.2f, -155.0f), false, 0.97f));

		for (int i = 0; i < 6; i++)
		{
			wobblyPlatform = new WobblyPlatform(this, new PxVec3(-5.0f + (i * 2), 1.2f, 39.0f), 0.0f, 1.0f);
		}

		wreckingBalls.push_back(new WreckingBall(this, new PxVec3(2.5f, 17.0f, 25.0f), 7.25));
		wreckingBalls.push_back(new WreckingBall(this, new PxVec3(-2.5f, 17.0f, 20.0f), 7, true));
		wreckingBalls.push_back(new WreckingBall(this, new PxVec3(0.0f, 25.0f, -55.0f), 7.05));
		wreckingBalls.push_back(new WreckingBall(this, new PxVec3(0.0f, 40.0f, -55.0f), 7.5));
		wreckingBalls.push_back(new WreckingBall(this, new PxVec3(0.0f, 25.0f, -135.0f), 7.15, true));
		wreckingBalls.push_back(new WreckingBall(this, new PxVec3(0.0f, 40.0f, -135.0f), 7.5, true));

		ramp = new Ramp(this, new PxVec3(1.75f, 0.45f, 43.8f), 1.45f);
		
		cloth = new Cloth(PxTransform(PxVec3(4, 1.7f, 45.0f), PxQuat(0.0, -0.707, 0.0, 0.707)), PxVec2(1.0f, 8.0f), 40, 40);
		cloth->SetColor(color_palette[2]);
		((PxCloth*)cloth->GetPxActor())->setStretchConfig(PxClothFabricPhaseType::eSHEARING, PxClothStretchConfig(0.2f));
		AddActor(cloth);

		catapult = new Catapult(this, new PxVec3(0.0f, -1.77f, 33.9f), 1.0f);
		button = new CatapultButton(this, new PxVec3(0.0f, 0.5f, 48.0f), catapult, 2.3f);

		goalPosts.push_back(new GoalPost(this, new PxVec3(0.0f, 0.0f, -90.0f)));
		scoreButton = new ScoreButton(this, new PxVec3(0.0f, 9.8f, -90.0f));
		goalPosts.push_back(new GoalPost(this, new PxVec3(0.0f, 0.0f, -160.0f)));

		bouncyBalls.push_back(new BouncyBall(this, new PxVec3(-10.0f, 10.0f, -20.0f), 2.0f, 1.975f));
		bouncyBalls.push_back(new BouncyBall(this, new PxVec3(10.0f, 20.0f, -70.0f), 2.0f, 1.975f));
		bouncyBalls.push_back(new BouncyBall(this, new PxVec3(-15.0f, 15.0f, -130.0f), 2.0f, 1.975f));

		my_callback = new MySimulationEventCallback(ball, button, scoreButton);
		px_scene->setSimulationEventCallback(my_callback);
		
	}

	void GameScene::Update()
	{
		#pragma region Updates
			for (int i = 0; i < miniWindmills.size(); i++)
				miniWindmills[i]->Update();

			for (int i = 0; i < wreckingBalls.size(); i++)
				wreckingBalls[i]->Update();

			button->Update();
			catapult->Update();
			scoreButton->Update(ball);

		#pragma endregion

		// Updating the UI elements - speed, height, and distance.
			if ((-ball->GetPosition().z + 38) >= 0)
			{
				distance = -ball->GetPosition().z + 38;

				// Determine Max Height
				if (height < ball->GetPosition().y)
					height = ball->GetPosition().y;
		
				if (-velocity > ball->GetBall()->GetVelocity().z)
					velocity = -ball->GetBall()->GetVelocity().z;
			}
			else
				distance = 0;

		// Stop the catapult from turning after a certain point
			if (distance > 5)
				button->activated = false;

		// Reset sequence - when the player has either clicked reset or come in contact with the plane.
			if (ball->addScore)
			{
				distances.push_back(distance);
				heights.push_back(height);
				velocities.push_back(velocity);

				height   = 0;
				velocity = 0;

				ball->Reset();
				scoreButton->Reset();

				for (int i = 0; i < goalPosts.size(); i++)
					goalPosts[i]->Reset();
				
				for (int i = 0; i < bouncyBalls.size(); i++)
					bouncyBalls[i]->Reset();

				ball->addScore = false;
			}

		#pragma region InvalidScenarios

			if (ball->invalidScenario1)
			{
				int numberOfWB = 5;

				for (int i = 0; i < numberOfWB; i++)
				{
					wreckingBalls.push_back(new WreckingBall(this, new PxVec3(-15.0f, 17.0f, -155.0f + (15.0 * i)), 7.25));
			
					for (int y = 0; y < numberOfWB; y++)
					{
						wreckingBalls.push_back(new WreckingBall(this, new PxVec3(-25.0f + (15.0 * y), 17.0f, -155.0f + (15.0 * i)), 7.25));
					}
				}

				ball->invalidScenario1 = false;
			}

			if (ball->invalidScenario2)
			{
				int numberOfWB = 7;

				for (int i = 0; i < numberOfWB; i++)
				{
					wreckingBalls.push_back(new WreckingBall(this, new PxVec3(-15.0f, 17.0f, -155.0f + (15.0 * i)), 7.25));

					for (int y = 0; y < numberOfWB; y++)
					{
						wreckingBalls.push_back(new WreckingBall(this, new PxVec3(-35.0f + (15.0 * y), 17.0f, -155.0f + (15.0 * i)), 7.25));
					}
				}

				ball->invalidScenario2 = false;
			}

			if (ball->invalidScenario3)
			{
				int numberOfWB = 10;

				for (int i = 0; i < numberOfWB; i++)
				{
					wreckingBalls.push_back(new WreckingBall(this, new PxVec3(-15.0f, 17.0f, -155.0f + (15.0 * i)), 7.25));

					for (int y = 0; y < numberOfWB; y++)
					{
						wreckingBalls.push_back(new WreckingBall(this, new PxVec3(-75.0f + (15.0 * y), 17.0f, -155.0f + (15.0 * i)), 7.25));
					}
				}

				ball->invalidScenario3 = false;
			}

		#pragma endregion
	}

	#pragma region Scoring

		int GameScene::GetScore()
		{
			return ball->score;
		}

		int GameScene::GetDistance()
		{
			return distance;
		}

		vector <int> GameScene::GetDistanceBoard()
		{
			return distances;
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
