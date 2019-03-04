#pragma once
#include "BasicActors.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h> 
#include "ComplexActors.h"

namespace PhysicsEngine
{
	class MySimulationEventCallback : public PxSimulationEventCallback
	{
	public:
		bool trigger;
		Ball* ball;
		CatapultButton* button;
		ScoreButton* scoreButton;
		PxTriggerPair object;

		MySimulationEventCallback(Ball* _ball, CatapultButton* _button, ScoreButton* _scoreButton) : trigger(false), ball(_ball), button(_button), scoreButton(_scoreButton) {}

		///Method called when the contact with the trigger object is detected.
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count)
		{
			//you can read the trigger information here
			for (PxU32 i = 0; i < count; i++)
			{
				if (pairs[i].otherShape->getGeometryType() != PxGeometryType::eBOX)
				{
					//check if eNOTIFY_TOUCH_FOUND trigger (On Trigger Enter)
					if ((pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND) && !trigger)
					{
						object = pairs[i];
						OnTriggerEnter();

						cerr << "OnTriggerEnter: " << object.triggerActor->getName() << endl;
						trigger = true;
					}

					//check if eNOTIFY_TOUCH_LOST trigger (On Trigger Exit)
					if ((pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST) && trigger)
					{
						cerr << "OnTriggerExit: " << object.triggerActor->getName() << endl;
						trigger = false;
					}
				}
			}
		}

		// Add collisions here.
		void OnTriggerEnter()
		{
			//If the ball touches the button.
			if (CheckCollision("Button"))
				button->activated = true;

			//If the ball touches the score button.
			if (CheckCollision("ScoreButton"))
				scoreButton->activated = true;

			// If the ball touches the plane.
			if (CheckCollision("Plane"))
				ball->addScore = true;
		}

		bool CheckCollision(const char* _object)
		{
			bool collision = false;

			if (std::strcmp(object.triggerActor->getName(), _object) == 0)
				collision = true;

			return collision;
		}

		///Method called when the contact by the filter shader is detected.
		virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs){}
		virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {}
		virtual void onWake(PxActor **actors, PxU32 count) {}
		virtual void onSleep(PxActor **actors, PxU32 count) {}
	};

	class GameScene : public Scene
	{
	public:
		void SetVisualisation();
		virtual void Init();
		virtual void Update();
		virtual int GetScore();
		virtual int GetDistance();
		virtual int GetHeight();
		virtual float GetVelocity();
		virtual vector <int> GetDistanceBoard();
		virtual vector <int> GetHeightBoard();
		virtual vector <float> GetVelocityBoard();

		MySimulationEventCallback* my_callback;

		Plane* plane;
		Ball* ball;
		Ramp* ramp;
		Cloth* cloth;
		Catapult* catapult;
		CatapultButton* button;
		Flooring* startingFloor;
		vector <Flooring*> flooring;
		vector <MiniWindmill*> miniWindmills;
		WobblyPlatform* wobblyPlatform;
		vector <WreckingBall*> wreckingBalls;
		vector <GoalPost*> goalPosts;
		ScoreButton* scoreButton;
		vector<BouncyBall*> bouncyBalls;
	};
}
