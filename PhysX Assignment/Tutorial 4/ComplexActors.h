#pragma once
#include "BasicActors.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h> 

namespace PhysicsEngine
{
	static const PxVec3 color_palette[] = {
		PxVec3(41.f / 255.f, 35.f / 255.f, 56.f / 255.f),
		PxVec3(82.0f / 255.f, 57.0f / 255.f, 87.0f / 255.f),
		PxVec3(193.f / 255.f, 38.f / 255.f, 78.f / 255.f),
		PxVec3(255.f / 255.f, 207.f / 255.f, 164.f / 255.f),
		PxVec3(4.f / 255.f,117.f / 255.f,111.f / 255.f) };

	class Ball
	{
	public:
		Sphere* ball;
		PxReal gForceStrength = 20;

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
			blade1->SetColor(color_palette[1]);
			
			blade2 = new Box(PxTransform(PxVec3(position->x, position->y + 3, position->z + 1)), PxVec3(scale / 7, scale * 3, scale / 3));
			blade2->SetColor(color_palette[1]);

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

		Flooring(Scene* scene, PxVec3* position, float rotation, float scale, bool startingFloor)
		{
			Floor = new Box(PxTransform(PxVec3(position->x, position->y + 0.3f, position->z), PxQuat(rotation, PxVec3(0.0f, 1.0f, 0.0f))), PxVec3(scale * 2, scale / 7, scale * 3));
			Floor->SetColor(color_palette[1]);
			Floor->SetKinematic(true);

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

	class TrackPieceRotated
	{
	public:
		Box * Floor;
		Box* WallLeft;
		Box* WallRight;
		Box* WallTop;

		TrackPieceRotated(Scene* scene, PxVec3* position, float rotation, float scale, bool corner = 0)
		{
			Floor = new Box(PxTransform(PxVec3(position->x, position->y + 0.3f, position->z), PxQuat(0.0, 0.707, 0.0, 0.707)), PxVec3(scale * 2, scale / 10, scale * 3));
			Floor->SetColor(color_palette[0]);
			Floor->SetKinematic(true);

			WallLeft = new Box(PxTransform(PxVec3(position->x, position->y + 0.8f, position->z - (scale * 2) - 0.2f), PxQuat(0.0, 0.707, 0.0, 0.707)), PxVec3(scale / 10, scale / 3, scale * 3));
			WallLeft->SetColor(color_palette[1]);
			WallLeft->SetKinematic(true);

			WallRight = new Box(PxTransform(PxVec3(position->x, position->y + 0.8f, position->z + (scale * 2) + 0.2f), PxQuat(0.0, 0.707, 0.0, 0.707)), PxVec3(scale / 10, scale / 3, scale * 3));
			WallRight->SetColor(color_palette[1]);
			WallRight->SetKinematic(true);

			WallTop = new Box(PxTransform(PxVec3(position->x, position->y + 0.8f, position->z - (scale * 3) + 0.2f), PxQuat(0.0, 0.707, 0.0, 0.707)), PxVec3(scale * 2, scale / 3, scale / 10));
			WallTop->SetColor(color_palette[1]);

			if (corner == true)
			{
				scene->AddActor(Floor);
				scene->AddActor(WallTop);
				scene->AddActor(WallLeft);
			}
			else
			{
				scene->AddActor(Floor);
				scene->AddActor(WallLeft);
				scene->AddActor(WallRight);
			}
		}

		~TrackPieceRotated() {};
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

	class SpringBox
	{
	public:
		Box* box;
		Box* side1;
		vector<DistanceJoint*> joints1;

		Box* side2;
		vector<DistanceJoint*> joints2;

		Box* side3;
		vector<DistanceJoint*> joints3;

		Box* side4;
		vector<DistanceJoint*> joints4;

		Box* side5;
		vector<DistanceJoint*> joints5;

		float springDist = 0.0f;
		float damping = 9.0f;
		float stiffness = 150.f;

		PxMaterial* bounceMat;

		SpringBox(Scene* scene, PxVec3* position, float rotation, float scale)
		{
			bounceMat = scene->GetScene()->getPhysics().createMaterial(1.0f, 1.0f, 2.5f);

			box = new Box(PxTransform(PxVec3(position->x, position->y + 0.5f, position->z)), PxVec3(scale/1.5f, scale/1.5f, scale/1.5f));
			box->SetKinematic(true);

			//side1 = new Box(PxTransform(PxVec3(position->x, position->y + 0.5f + 0.7f, position->z)), PxVec3(scale / 2, scale / 10, scale / 2));

			//joints1.push_back(new DistanceJoint(box, PxTransform(PxVec3(1.0f, springDist, 1.0f)), side1, PxTransform(PxVec3(1, -1, 1))));
			//joints1.push_back(new DistanceJoint(box, PxTransform(PxVec3(1.0f, springDist, -1.0f)), side1, PxTransform(PxVec3(1, -1, -1))));
			//joints1.push_back(new DistanceJoint(box, PxTransform(PxVec3(-1.0f, springDist, 1.0f)), side1, PxTransform(PxVec3(-1, -1, 1))));
			//joints1.push_back(new DistanceJoint(box, PxTransform(PxVec3(-1.0f, springDist, -1.0f)), side1, PxTransform(PxVec3(-1, -1, -1))));

			//for (DistanceJoint* joint : joints1)
			//{
			//	joint->Damping(2.0f);
			//	joint->Stiffness(15.0f);
			//}

			side2 = new Box(PxTransform(PxVec3(position->x + 0.7f, position->y + 0.5f, position->z)), PxVec3(scale / 1.5f, scale / 10, scale / 2));
			side2->SetMaterial(bounceMat);

			joints2.push_back(new DistanceJoint(box, PxTransform(PxVec3(1.0f, 1.0f, -springDist)), side2, PxTransform(PxVec3(1, -1, 1))));
			joints2.push_back(new DistanceJoint(box, PxTransform(PxVec3(1.0f, -1.0f, -springDist)), side2, PxTransform(PxVec3(1, -1, -1))));
			joints2.push_back(new DistanceJoint(box, PxTransform(PxVec3(-1.0f, 1.0f, -springDist)), side2, PxTransform(PxVec3(-1, -1, 1))));
			joints2.push_back(new DistanceJoint(box, PxTransform(PxVec3(-1.0f, -1.0f, -springDist)), side2, PxTransform(PxVec3(-1, -1, -1))));

			for (DistanceJoint* joint : joints2)
			{
				joint->Damping(damping);
				joint->Stiffness(stiffness);
			}

			side3 = new Box(PxTransform(PxVec3(position->x - 0.7f, position->y + 0.5f, position->z)), PxVec3(scale / 1.5f, scale / 2, scale / 10));
			side3->SetMaterial(bounceMat);

			joints3.push_back(new DistanceJoint(box, PxTransform(PxVec3(1.0f, 1.0f, springDist)), side3, PxTransform(PxVec3(1, 1, -1))));
			joints3.push_back(new DistanceJoint(box, PxTransform(PxVec3(1.0f, -1.0f, springDist)), side3, PxTransform(PxVec3(1, -1, -1))));
			joints3.push_back(new DistanceJoint(box, PxTransform(PxVec3(-1.0f, 1.0f, springDist)), side3, PxTransform(PxVec3(-1, 1, -1))));
			joints3.push_back(new DistanceJoint(box, PxTransform(PxVec3(-1.0f, -1.0f, springDist)), side3, PxTransform(PxVec3(-1, -1, -1))));

			for (DistanceJoint* joint : joints3)
			{
				joint->Damping(damping);
				joint->Stiffness(stiffness);
			}

			side4 = new Box(PxTransform(PxVec3(position->x - 0.7f, position->y + 0.5f, position->z)), PxVec3(scale / 1.5f, scale / 2, scale / 10));
			side4->SetMaterial(bounceMat);

			joints4.push_back(new DistanceJoint(box, PxTransform(PxVec3(-springDist, 1.0f, 1.0f)), side4, PxTransform(PxVec3(1, 1, -1))));
			joints4.push_back(new DistanceJoint(box, PxTransform(PxVec3(-springDist, -1.0f, 1.0f)), side4, PxTransform(PxVec3(1, -1, -1))));
			joints4.push_back(new DistanceJoint(box, PxTransform(PxVec3(-springDist, 1.0f, -1.0f)), side4, PxTransform(PxVec3(-1, 1, -1))));
			joints4.push_back(new DistanceJoint(box, PxTransform(PxVec3(-springDist, -1.0f, -1.0f)), side4, PxTransform(PxVec3(-1, -1, -1))));

			for (DistanceJoint* joint : joints4)
			{
				joint->Damping(damping);
				joint->Stiffness(stiffness);
			}

			side5 = new Box(PxTransform(PxVec3(position->x + 0.7f, position->y + 0.5f, position->z)), PxVec3(scale / 1.5f, scale / 2, scale / 10));
			side5->SetMaterial(bounceMat);

			joints5.push_back(new DistanceJoint(box, PxTransform(PxVec3(springDist, 1.0f, 1.0f)), side5, PxTransform(PxVec3(-1, 1, -1))));
			joints5.push_back(new DistanceJoint(box, PxTransform(PxVec3(springDist, -1.0f, 1.0f)), side5, PxTransform(PxVec3(-1, -1, -1))));
			joints5.push_back(new DistanceJoint(box, PxTransform(PxVec3(springDist, 1.0f, -1.0f)), side5, PxTransform(PxVec3(1, 1, -1))));
			joints5.push_back(new DistanceJoint(box, PxTransform(PxVec3(springDist, -1.0f, -1.0f)), side5, PxTransform(PxVec3(1, -1, -1))));
			
			for (DistanceJoint* joint : joints5)
			{
				joint->Damping(damping);
				joint->Stiffness(stiffness);
			}

			side2->SetColor(color_palette[2]);
			side3->SetColor(color_palette[2]);
			side4->SetColor(color_palette[2]);
			side5->SetColor(color_palette[2]);

			scene->AddActor(box);
			scene->AddActor(side2);
			scene->AddActor(side3);
			scene->AddActor(side4);
			scene->AddActor(side5);


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
			ramp->SetColor(color_palette[1]);

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
			Button->SetColor(color_palette[1]);
			Button->SetName("Button");
			Button->SetTrigger(true);
			scene->AddActor(Button);
		}

		void Update()
		{
			if (activated)
			{
				Button->SetColor(color_palette[2]);
				catapult->shoot = true;
			}
		}
	};

	class Bridge
	{
	public:
		Box* floor;
		Box* ramp1;
		Box* ramp2;

		Bridge(Scene* scene, PxVec3* position, PxVec3* rotation, float scale)
		{
			floor = new Box(PxTransform(position->x, position->y, position->z), PxVec3(scale, scale / 2, scale * 3));
			floor->SetKinematic(true);
			floor->SetColor(color_palette[0]);

			ramp1 = new Box(PxTransform(position->x - 1.76f, position->y - 0.31f, position->z, PxQuat(0.0, 0.0, 0.174, 0.985)), PxVec3(scale, scale / 2, scale * 3));
			ramp1->SetKinematic(true);
			ramp1->SetColor(color_palette[1]);

			ramp2 = new Box(PxTransform(position->x + 1.76f, position->y - 0.31f, position->z, PxQuat(0.0, 0.0, -0.174, 0.985)), PxVec3(scale, scale / 2, scale * 3));
			ramp2->SetKinematic(true);
			ramp2->SetColor(color_palette[1]);

			scene->AddActor(floor);
			scene->AddActor(ramp1);
			scene->AddActor(ramp2);
		}
	};

	class Sandpit
	{
	public:
		Box* sand;
		PxMaterial* sandMat;

		Sandpit(Scene* scene, PxVec3* position, PxQuat rotation, float scale)
		{
			sandMat = scene->GetScene()->getPhysics().createMaterial(20.0f, 20.0f, 0.0f);
			sand = new Box(PxTransform(position->x, position->y, position->z), PxVec3(scale * 6, scale / 15, scale * 3));
			sand->SetMaterial(sandMat);
			sand->SetColor(color_palette[3]);
			sand->SetKinematic(true);

			scene->AddActor(sand);
		}
	};
}
