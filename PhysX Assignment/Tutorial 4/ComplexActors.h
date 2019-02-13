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
		PxReal gForceStrength = 20;
		bool addScore;

		Ball(Scene* scene, PxVec3* position)
		{
			ball = new Sphere(PxTransform(PxVec3(position->x, position->y, position->z)), 0.3f, 15.0f);
			ball->SetName("Ball");

			scene->AddActor(ball);
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
			((PxRigidDynamic*)ball->GetPxActor())->setGlobalPose(PxTransform(PxVec3(0, 3, 50)));
			ball->GetPxActor()->isRigidDynamic()->setLinearVelocity(PxVec3(0, 0, 0));
		}

		void addForce(PxVec3 force)
		{
			ball->GetRigidBody()->addForce(force * gForceStrength);
		}
	};

	class MiniWindmill
	{
	public:
		Box* base;
		Box* blade1;
		Box* blade2;

		RevoluteJoint* joint;
		RevoluteJoint* joint2;

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
			
			blade2 = new Box(PxTransform(PxVec3(position->x, position->y + 3, position->z + 1)), PxVec3(scale / 7, scale * 3, scale / 3));
			blade2->SetColor(color_palette[10]);

			joint = new RevoluteJoint(base, PxTransform(PxVec3(0.0f, -2.5f, 0.0f), PxQuat(PxPi / 2, PxVec3(0.0f, 1.0f, 0.0f))), blade1, PxTransform(PxVec3(1.0f, 0.0f, 0.0f)));
			joint2 = new RevoluteJoint(base, PxTransform(PxVec3(0.0f, -2.5f, 0.0f), PxQuat(PxPi / 2, PxVec3(0.0f, 1.0f, 0.0f))), blade2, PxTransform(PxVec3(1.0f, 0.0f, 0.0f)));
			
			joint->DriveVelocity(3.0f);

			scene->AddActor(base);
			scene->AddActor(blade1);
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
				material = scene->GetScene()->getPhysics().createMaterial(20.0f, 20.0f, 0.0f);
				Floor->SetColor(color_palette[10]);
			}

			if (bouncy)
			{
				material = scene->GetScene()->getPhysics().createMaterial(0.0f, 0.0f, 2.0f);
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

	class FlagPole
	{
	public:
		Box* Pole;
		Cloth* Flag;

		FlagPole(Scene* scene, PxVec3* position, float rotation, float scale)
		{
			Pole = new Box(PxTransform(PxVec3(position->x, position->y + 3, position->z)), PxVec3(scale / 10, scale * 3, scale / 10));
			Pole->SetKinematic(true);
			Pole->SetTrigger(true);
			Pole->SetName("Pole");

			Flag = new Cloth(PxTransform(PxVec3(position->x, position->y + 6, position->z), PxQuat(rotation, PxVec3(0.0, 1.0, 0.0))), PxVec2(1.5f, 2.f), 20, 20);
			Flag->SetColor(color_palette[1]);

			scene->AddActor(Flag);
			scene->AddActor(Pole);
		}

		~FlagPole() {};
	};

	class WobblyPlatform
	{
	public:
		Box* box;
		Box* side1;
		vector<DistanceJoint*> joints1;

		float springDist = 0.1f;
		float damping = 9.0f;
		float stiffness = 300.f;

		PxMaterial* bounceMat;

		WobblyPlatform(Scene* scene, PxVec3* position, float rotation, float scale)
		{
			bounceMat = scene->GetScene()->getPhysics().createMaterial(1.0f, 1.0f, 2.5f);

			box = new Box(PxTransform(PxVec3(position->x, position->y + 0.5f, position->z)), PxVec3(scale / 1.0f, scale / 3.5f, scale / 1.0f));
			box->SetKinematic(true);

			side1 = new Box(PxTransform(PxVec3(position->x, position->y + 0.5f + 0.7f, position->z)), PxVec3(scale / 1.0f, scale / 10.0f, scale / 1.0f));

			joints1.push_back(new DistanceJoint(box, PxTransform(PxVec3(1.0f, springDist, 1.0f)), side1, PxTransform(PxVec3(1, -1, 1))));
			joints1.push_back(new DistanceJoint(box, PxTransform(PxVec3(1.0f, springDist, -1.0f)), side1, PxTransform(PxVec3(1, -1, -1))));
			joints1.push_back(new DistanceJoint(box, PxTransform(PxVec3(-1.0f, springDist, 1.0f)), side1, PxTransform(PxVec3(-1, -1, 1))));
			joints1.push_back(new DistanceJoint(box, PxTransform(PxVec3(-1.0f, springDist, -1.0f)), side1, PxTransform(PxVec3(-1, -1, -1))));

			for (DistanceJoint* joint : joints1)
			{
				joint->Damping(2.0f);
				joint->Stiffness(15.0f);
			}

			side1->SetColor(color_palette[6]);

			scene->AddActor(box);
			scene->AddActor(side1);


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
		Box * base;
		Box* base2;
		Box* body;

		RevoluteJoint* joint;

		bool shoot = false;

		Catapult(Scene* scene, PxVec3* position, PxVec3* rotation, float scale)
		{
			base = new Box(PxTransform(PxVec3(position->x - 1.5f, position->y, position->z)), PxVec3(scale / 2, scale*4, scale / 2));
			base->SetKinematic(true);
			base->SetColor(color_palette[2]);

			body = new Box(PxTransform(PxVec3(position->x, position->y, position->z)), PxVec3(1.0f, scale / 10, scale * 4));

			joint = new RevoluteJoint(base, PxTransform(PxVec3(2.5f, 4.0f, 0.0f), PxQuat(PxPi / 2, PxVec3(1.0f, 0.0f, 0.0f))), body, PxTransform(PxVec3(1.0f, 0.0f, 0.0f)));
			body->SetRotation(PxQuat(0.0f, PxVec3(0.0f, 0.0f, 0.0f)));

			scene->AddActor(base);
			scene->AddActor(body);
		}

		void Update()
		{
			if (shoot)
				joint->DriveVelocity(-10.0f);
			else
				joint->DriveVelocity(0.0f);
		}
	};

	class CatapultButton
	{
	public:
		Sphere* Button;

		bool activated = false;

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
			if (activated)
			{
				Button->SetColor(color_palette[3]);
				catapult->shoot = true;
			}
		}
	};

}
