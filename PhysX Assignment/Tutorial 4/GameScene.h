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
		virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs)
		{
			cerr << "Contact found between " << pairHeader.actors[0]->getName() << " and " << pairHeader.actors[1]->getName() << endl;
				
			//check all pairs
			for (PxU32 i = 0; i < nbPairs; i++)
			{
				switch (pairs[i].shapes[0]->getSimulationFilterData().word0)
				{
				case BALL:
					cerr << "Ball..." << endl;
					break;
				case GOALPOST:
					cerr << "GoalPost Hit!" << endl;
					break;
				}
			}
		}

		virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {}
		virtual void onWake(PxActor **actors, PxU32 count) {}
		virtual void onSleep(PxActor **actors, PxU32 count) {}
	};

	//A simple filter shader based on PxDefaultSimulationFilterShader - without group filtering
	static PxFilterFlags CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
		PxFilterObjectAttributes attributes1, PxFilterData filterData1,
		PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
	{
		// let triggers through
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlags();
		}

		//enable continous collision detection
		pairFlags = PxPairFlag::eSOLVE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT;


		//customise collision filtering here
		//e.g.

		// trigger the contact callback for pairs (A,B) where 
		// the filtermask of A contains the ID of B and vice versa.
		if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		{
			//trigger onContact callback for this pair of objects
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
			//pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
		}

		return PxFilterFlags();
	};

	class GameScene : public Scene
	{
	public:
		GameScene() : Scene(CustomFilterShader) {};

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
