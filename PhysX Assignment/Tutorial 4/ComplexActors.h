#pragma once
#include "BasicActors.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h> 

namespace PhysicsEngine
{
	static const PxVec3 color_palette[] = 
	{
		PxVec3(	41.0f / 255.0f,		35.0f / 255.0f,		56.0f / 255.0f),
		PxVec3(	194.0f / 255.0f,	30.0f / 255.0f,		86.0f / 255.0f),
		PxVec3(	193.0f / 255.0f,	38.0f / 255.0f,		78.0f / 255.0f),
		PxVec3(	255.0f / 255.0f,	207.0f / 255.0f,	164.0f / 255.0f),
		PxVec3(	255.0f / 255.0f,	0.0f / 255.0f,		0.0f / 255.0f),
		PxVec3(	11.0f / 255.0f,		102.0f / 255.0f,	35.0f / 255.0f),
		PxVec3(	82.0f / 255.0f,		57.0f / 255.0f,		87.0f / 255.0f),
		PxVec3(	76.0f / 255.0f,		187.0f / 255.0f,	23.0f / 255.0f),
		PxVec3(	130.0f / 255.0f,	247.0f / 255.0f,	114.0f / 255.0f),
		PxVec3(	0.0f / 255.0f,		0.0f / 255.0f,		0.0f / 255.0f),
		PxVec3(	255.0f / 255.0f,	255.0f / 255.0f,	255.0f / 255.0f)
	};

	class Ball
	{

	public:
		Sphere* ball;
		PxReal force = 20;
		PxTransform originalPosition;
		PxMaterial* material;

		bool addScore;

		Capsule* capRight;
		Capsule* capLeft;

		Ball(Scene* scene, PxVec3* position)
		{
			originalPosition = PxTransform(PxVec3(position->x, position->y, position->z));

			ball = new Sphere(PxTransform(PxVec3(position->x, position->y, position->z)), 0.3f, 15.0f);
			material = scene->GetScene()->getPhysics().createMaterial(0.0f, 10.0f, 0.8f);
			ball->SetMaterial(material);
			ball->SetName("Ball");
			scene->AddActor(ball);

			/*float capSize = 0.15f;
			
			capRight = new Capsule(PxTransform(PxVec3(position->x, position->y, position->z)), PxVec2(capSize, capSize), 15.0f);
			RevoluteJoint* joint = new RevoluteJoint(ball, PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxPi / 2, PxVec3(0.0f, 1.0f, 0.0f))), capRight, PxTransform(PxVec3(0.25f, 0.0f, 0.0f)));
			scene->AddActor(capRight);
			
			capLeft = new Capsule(PxTransform(PxVec3(position->x, position->y, position->z)), PxVec2(capSize, capSize), 15.0f);
			joint = new RevoluteJoint(ball, PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxPi / 2, PxVec3(0.0f, 1.0f, 0.0f))), capLeft, PxTransform(PxVec3(-0.25f, 0.0f, 0.0f)));
			scene->AddActor(capLeft);*/
		}

		Sphere* GetBall()
		{
			return ball;
		}

		PxVec3 GetPosition()
		{
			return ((PxRigidDynamic*)ball->GetPxActor())->getGlobalPose().p;
		}

		void Reset() 
		{
			((PxRigidDynamic*)ball->GetPxActor())->setGlobalPose(originalPosition);
			//((PxRigidDynamic*)capRight->GetPxActor())->setGlobalPose(originalPosition);
			//((PxRigidDynamic*)capLeft->GetPxActor())->setGlobalPose(originalPosition);
			ball->GetPxActor()->isRigidDynamic()->setLinearVelocity(PxVec3(0, 0, 0));
			//capRight->GetPxActor()->isRigidDynamic()->setLinearVelocity(PxVec3(0, 0, 0));
			//capLeft->GetPxActor()->isRigidDynamic()->setLinearVelocity(PxVec3(0, 0, 0));
		}

		void addForce(PxVec3 _force)
		{
			ball->GetRigidBody()->addForce(_force * force);
		}
	
	};

	class MiniWindmill
	{
	public:
		Box* base;
		Box* blade1;
		Box* blade2;

		RevoluteJoint* joint;

		MiniWindmill(Scene* scene, PxVec3* position, bool side, float scale)
		{
			if (side)
				base = new Box(PxTransform(PxVec3(position->x - 1.9, position->y, position->z), PxQuat(40, PxVec3(0.0, 0.0, 1.0))), PxVec3(scale / 2, scale * 3, scale / 2));
			else
				base = new Box(PxTransform(PxVec3(position->x - 1.9, position->y, position->z), PxQuat(-40, PxVec3(0.0, 0.0, 1.0))), PxVec3(scale / 2, scale * 3, scale / 2));

			base->SetKinematic(true);
			base->SetColor(color_palette[0]);

			blade1 = new Box(PxTransform(PxVec3(position->x, position->y, position->z)), PxVec3(scale / 7, scale / 3, scale * 3));
			blade1->SetColor(color_palette[10]);

			joint = new RevoluteJoint(base, PxTransform(PxVec3(0.0f, -2.5f, 0.0f), PxQuat(PxPi / 2, PxVec3(0.0f, 1.0f, 0.0f))), blade1, PxTransform(PxVec3(1.0f, 0.0f, 0.0f)));
			joint->DriveVelocity(3.0f);

			scene->AddActor(blade1);
			
			blade2 = new Box(PxTransform(PxVec3(position->x, position->y + 3, position->z + 1)), PxVec3(scale / 7, scale * 3, scale / 3));
			blade2->SetColor(color_palette[10]);

			joint = new RevoluteJoint(base, PxTransform(PxVec3(0.0f, -2.5f, 0.0f), PxQuat(PxPi / 2, PxVec3(0.0f, 1.0f, 0.0f))), blade2, PxTransform(PxVec3(1.0f, 0.0f, 0.0f)));

			scene->AddActor(base);
			scene->AddActor(blade2);
		}

		void Update()
		{
			blade2->SetRotation(blade1->GetRotation());
		}
	};

	class Flooring
	{
	public:
		Box* Floor;
		Box* Left;
		Box* Right;
		Box* End;
		PxMaterial* material;

		Flooring(Scene* scene, PxVec3* position, float rotation, float scale, bool startingFloor, bool slow, bool bouncy)
		{
			Floor = new Box(PxTransform(PxVec3(position->x, position->y + 0.3f, position->z), PxQuat(rotation, PxVec3(0.0f, 1.0f, 0.0f))), PxVec3(scale * 2, scale / 7, scale * 3));
			Floor->SetKinematic(true);
			Floor->SetColor(color_palette[6]);
			
			if (slow)
			{
				material = scene->GetScene()->getPhysics().createMaterial(0.0f, 20.0f, 0.0f);
				Floor->SetColor(color_palette[10]);
			}

			if (bouncy)
			{
				material = scene->GetScene()->getPhysics().createMaterial(0.0f, 0.0f, 1.5f);
				Floor->SetColor(color_palette[9]);
			}

			Floor->SetMaterial(material);

			scene->AddActor(Floor);

			if (startingFloor)
			{
				Left = new Box(PxTransform(PxVec3(position->x - (scale * 2) - 0.2f, position->y + 0.8f, position->z), PxQuat(rotation, PxVec3(0.0f, 1.0f, 0.0f))), PxVec3(scale / 10, scale / 3, scale * 3));
				Left->SetColor(color_palette[0]);
				Left->SetKinematic(true);

				Right = new Box(PxTransform(PxVec3(position->x + (scale * 2) + 0.2f, position->y + 0.8f, position->z), PxQuat(rotation, PxVec3(0.0f, 1.0f, 0.0f))), PxVec3(scale / 10, scale / 3, scale * 3));
				Right->SetColor(color_palette[0]);
				Right->SetKinematic(true);

				End = new Box(PxTransform(PxVec3(position->x, position->y + 0.8f, position->z + (scale * 3) + 0.2f), PxQuat(rotation, PxVec3(0.0f, 1.0f, 0.0f))), PxVec3(scale * 2, scale / 3, scale / 10));
				End->SetColor(color_palette[0]);
				End->SetKinematic(true);

				scene->AddActor(Left);
				scene->AddActor(Right);
				scene->AddActor(End);
			}
		}
	};

	class WreckingBall
	{
	public:
		float speed = 1.0f;
		PxReal ballMass = 20.0f;

		vector<Box*> boxes;
		vector<RevoluteJoint*> joints;

		WreckingBall(Scene* scene, PxVec3* position, int links)
		{
			for (int i = 0; i < links; i++)
			{
				boxes.push_back(new Box(PxTransform(PxVec3(position->x, position->y - i, position->z))));

				if (i != 0)
				{
					boxes[i]->SetColor(color_palette[10]);

					if (i == links - 1)
					{
						joints.push_back(new RevoluteJoint(boxes[i - 1], PxTransform(PxVec3(0.0f, -2.0f, 0.0f)), boxes[i], PxTransform(PxVec3(0.0f, 0.0f, 0.0f))));
						boxes[i]->GetShape()->setGeometry(PxBoxGeometry(1.0f, 1.0f, 1.0f));
						boxes[i]->SetMass(ballMass);
					}
					else
						joints.push_back(new RevoluteJoint(boxes[i - 1], PxTransform(PxVec3(0.0f, -1.5f, 0.0f), PxQuat(PxPi / 2, PxVec3(0.0f, 1.0f, 0.0f))), boxes[i], PxTransform(PxVec3(0.0f, 0.0f, 0.0f))));
				}
				else
				{
					boxes[i]->SetColor(color_palette[9]);
					boxes[i]->SetKinematic(true);
				}

				scene->AddActor(boxes[i]);
			}
			
			joints[0]->DriveVelocity(speed);
		}

		void Update()
		{
			cout << boxes[1]->GetRotation().z << endl;

			if(boxes[1]->GetRotation().z > 0.50f)
				joints[0]->DriveVelocity(speed);
			else if(boxes[1]->GetRotation().z < -0.50f)
				joints[0]->DriveVelocity(-speed);
		}
	};

	class WobblyPlatform
	{
	public:
		Box* box;
		Box* platform;
		vector<DistanceJoint*> joints;

		float distance = -0.15f;
		float damping = 1.0f;
		float stiffness = 100.0f;

		WobblyPlatform(Scene* scene, PxVec3* position, float rotation, float scale)
		{
			box = new Box(PxTransform(PxVec3(position->x, position->y + 0.5f, position->z)), PxVec3(scale / 1.0f, scale / 3.5f, scale / 1.0f));
			box->SetKinematic(true);

			platform = new Box(PxTransform(PxVec3(position->x, position->y + 0.5f + 0.7f, position->z)), PxVec3(scale / 1.0f, scale / 10.0f, scale / 1.0f));

			joints.push_back(new DistanceJoint(box, PxTransform(PxVec3(1.0f, distance, 1.0f)), platform, PxTransform(PxVec3(1, -1, 1))));
			joints.push_back(new DistanceJoint(box, PxTransform(PxVec3(1.0f, distance, -1.0f)), platform, PxTransform(PxVec3(1, -1, -1))));
			joints.push_back(new DistanceJoint(box, PxTransform(PxVec3(-1.0f, distance, 1.0f)), platform, PxTransform(PxVec3(-1, -1, 1))));
			joints.push_back(new DistanceJoint(box, PxTransform(PxVec3(-1.0f, distance, -1.0f)), platform, PxTransform(PxVec3(-1, -1, -1))));

			for (DistanceJoint* joint : joints)
			{
				joint->Damping(damping);
				joint->Stiffness(stiffness);
			}

			platform->SetColor(color_palette[6]);

			scene->AddActor(box);
			scene->AddActor(platform);
		}
	};

	class Ramp
	{
	public:
		Box* ramp;

		Ramp(Scene* scene, PxVec3* position, float scale)
		{
			ramp = new Box(PxTransform(position->x - 1.76f, position->y - 0.31f, position->z, PxQuat(0, PxVec3(0.0, 0.0, 1.0))), PxVec3(scale * 4, scale / 2, scale * 3));

			ramp->SetRotation(PxQuat(35, PxVec3(1.0, 0.0, 0.0)));

			ramp->SetKinematic(true);
			ramp->SetColor(color_palette[6]);

			scene->AddActor(ramp);
		}
	};

	class Catapult
	{
	public:
		Box* base;
		Box* body;

		PxReal force = 5;
		RevoluteJoint* joint;
		PxVec3* originalPos;
		PxQuat originalRot;

		bool shoot;

		Catapult(Scene* scene, PxVec3* position, float scale)
		{
			originalPos = new PxVec3(position->x - 1.5f, position->y, position->z);

			base = new Box(PxTransform(PxVec3(originalPos->x, originalPos->y, originalPos->z)), PxVec3(scale / 2, scale*4, scale / 2));
			base->SetKinematic(false);
			base->GetPxActor()->setActorFlags(PxActorFlag::eDISABLE_GRAVITY);
			base->SetColor(color_palette[2]);

			body = new Box(PxTransform(PxVec3(originalPos->x, originalPos->y, originalPos->z)), PxVec3(1.0f, scale / 10, scale * 4));
			body->GetPxActor()->setActorFlags(PxActorFlag::eDISABLE_GRAVITY);

			joint = new RevoluteJoint(base, PxTransform(PxVec3(1.6f, 4.0f, 0.0f)), body, PxTransform(PxVec3(0.0f, 0.0f, 0.0f)));

			scene->AddActor(base);
			scene->AddActor(body);

			originalRot = base->GetRotation();
		}

		void Update()
		{

			if (shoot)
			{
				base->SetKinematic(true);
				joint->DriveVelocity(-10.0f);
			}
			else
			{
				base->SetKinematic(false);
				Reset();
			}

		}

		void Reset()
		{
			joint->DriveVelocity(0.0f);
			base->SetRotation(originalRot);
			body->SetRotation(PxQuat(-PxPi, PxVec3(1.0f, 0.0f, 0.0f)));
			((PxRigidDynamic*)base->GetPxActor())->setGlobalPose(PxTransform(PxVec3(originalPos->x, originalPos->y, originalPos->z)));
		}

		void Move(PxVec3 _force)
		{
			base->GetRigidBody()->setLinearVelocity(_force * force);
			originalPos = new PxVec3(base->GetPosition().x, originalPos->y, originalPos->z);
		}
	};

	class CatapultButton
	{
	public:
		Sphere* Button;

		bool activated;

		Catapult* catapult;

		CatapultButton(Scene* scene, PxVec3* position, Catapult* lift)
		{
			catapult = lift;

			Button = new Sphere(PxTransform(PxVec3(position->x, position->y, position->z)), 0.4f, 1.0f);
			Button->SetKinematic(true);
			Button->SetColor(color_palette[4]);
			Button->SetName("Button");
			Button->SetTrigger(true);
			scene->AddActor(Button);
		}

		void Update()
		{
			SetPosition();

			if (activated)
			{
				Button->SetColor(color_palette[3]);
				catapult->shoot = true;
			}
			else
			{
				Button->SetColor(color_palette[4]);
				catapult->shoot = false;
			}
		}

		void SetPosition()
		{
			((PxRigidDynamic*)Button->GetPxActor())->setGlobalPose(PxTransform(PxVec3(catapult->originalPos->x + 1.6f, catapult->originalPos->y + 3.8f, catapult->originalPos->z + 3.0f)));
		}
	};

}
