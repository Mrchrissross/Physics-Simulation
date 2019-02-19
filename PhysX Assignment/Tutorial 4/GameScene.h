#pragma once
#include "BasicActors.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h> 
#include "ComplexActors.h"

namespace PhysicsEngine
{
	static const PxVec3 colors_palette[] = 
	{ 
		PxVec3(	146.f / 255.f, 
				19.f / 255.f, 
				75.f / 255.f), 

		PxVec3(	183.f / 255.f, 
				71.f / 255.f,
				98.f / 255.f),

		PxVec3(	95.f / 255.f,
				5.f / 255.f,
				200.f / 255.f),

		PxVec3(	5.f / 255.f,
				10.f / 255.f,
				255.f / 255.f),

		PxVec3(	174.f / 255.f,
				255.f / 255.f,
				111.f / 255.f) 
	};

	class MySimulationEventCallback : public PxSimulationEventCallback
	{
	public:
		//an example variable that will be checked in the main simulation loop
		bool trigger;
		Ball* ball;
		CatapultButton* button;

		MySimulationEventCallback(Ball* _ball, CatapultButton* _button) : trigger(false), ball(_ball), button(_button) {}

		///Method called when the contact with the trigger object is detected.
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count)
		{
			//you can read the trigger information here
			for (PxU32 i = 0; i < count; i++)
			{
				//filter out contact with the planes
				if (pairs[i].otherShape->getGeometryType() != PxGeometryType::ePLANE)
				{

					if (pairs[i].otherShape->getGeometryType() != PxGeometryType::eBOX)
					{

						//check if eNOTIFY_TOUCH_FOUND trigger
						if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
						{
							cerr << pairs[i].triggerActor->getName() << endl;
							
							//If the ball touches the button.
							if (std::strcmp(pairs[i].triggerActor->getName(), "Button") == 0)
								button->activated = true;

							// If the ball touches the plane.
							else
								ball->addScore = true;
						
							cerr << "onTrigger::eNOTIFY_TOUCH_FOUND"<< endl;
							trigger = true;
						}
						//check if eNOTIFY_TOUCH_LOST trigger
						if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST)
						{
							cerr << "onTrigger::eNOTIFY_TOUCH_LOST" << endl;
						}
						
					}

				}
			}
		}

		///Method called when the contact by the filter shader is detected.
		virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs)
		{
			cerr << "Contact found between " << pairHeader.actors[0]->getName() << " " << pairHeader.actors[1]->getName() << endl;

			//check all pairs
			for (PxU32 i = 0; i < nbPairs; i++)
			{
				//check eNOTIFY_TOUCH_FOUND
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					cerr << "onContact::eNOTIFY_TOUCH_FOUND" << endl;
				}
				//check eNOTIFY_TOUCH_LOST
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_LOST)
				{
					cerr << "onContact::eNOTIFY_TOUCH_LOST" << endl;
				}
			}
		}

		virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {}
		virtual void onWake(PxActor **actors, PxU32 count) {}
		virtual void onSleep(PxActor **actors, PxU32 count) {}
	};

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

		pairFlags = PxPairFlag::eCONTACT_DEFAULT;

		// trigger the contact callback for pairs (A,B) where 
		// the filtermask of A contains the ID of B and vice versa.
		if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		{
			//trigger onContact callback for this pair of objects
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
		}

		return PxFilterFlags();
	};

	class GameScene : public Scene
	{
	public:
		GameScene() {};
		~GameScene() {};

		void SetVisualisation();
		virtual void Init();
		virtual void Update();
		virtual int GetScore();
		virtual int GetHeight();
		virtual float GetVelocity();
		virtual vector <int> GetScoreBoard();
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
	};
}
