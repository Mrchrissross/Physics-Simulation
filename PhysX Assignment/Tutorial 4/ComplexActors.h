#pragma once
#include "BasicActors.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h> 

#include "PhysicsEngine.h"
#include "Extras/Renderer.h"

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
		bool invalidScenario1;
		bool invalidScenario2;
		bool invalidScenario3;

		Capsule* capRight;
		Capsule* capLeft;

		Ball(Scene* scene, PxVec3* position)
		{
			originalPosition = PxTransform(PxVec3(position->x, position->y, position->z));

			ball = new Sphere(PxTransform(PxVec3(position->x, position->y, position->z)), 0.3f, 15.0f);
			material = scene->GetScene()->getPhysics().createMaterial(0.0f, 10.0f, 0.8f);
			ball->SetMaterial(material);
			ball->SetMass(0.94798750009838f); // Rugby Ball Weight = 0.94798750009838 pounds
			ball->SetName("Ball");
			scene->AddActor(ball);

			float capSize = 0.15f;
			
			capRight = new Capsule(PxTransform(PxVec3(position->x, position->y, position->z)), PxVec2(capSize, capSize), 15.0f);
			RevoluteJoint* joint = new RevoluteJoint(ball, PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxPi / 2, PxVec3(0.0f, 1.0f, 0.0f))), capRight, PxTransform(PxVec3(0.25f, 0.0f, 0.0f)));
			scene->AddActor(capRight);

			capLeft = new Capsule(PxTransform(PxVec3(position->x, position->y, position->z)), PxVec2(capSize, capSize), 15.0f);
			joint = new RevoluteJoint(ball, PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxPi / 2, PxVec3(0.0f, 1.0f, 0.0f))), capLeft, PxTransform(PxVec3(-0.25f, 0.0f, 0.0f)));
			scene->AddActor(capLeft);
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
			((PxRigidDynamic*)capRight->GetPxActor())->setGlobalPose(originalPosition);
			((PxRigidDynamic*)capLeft->GetPxActor())->setGlobalPose(originalPosition);
			ball->GetPxActor()->isRigidDynamic()->setLinearVelocity(PxVec3(0, 0, 0));
			capRight->GetPxActor()->isRigidDynamic()->setLinearVelocity(PxVec3(0, 0, 0));
			capLeft->GetPxActor()->isRigidDynamic()->setLinearVelocity(PxVec3(0, 0, 0));
		}

		void addForce(PxVec3 _force)
		{
			ball->GetRigidBody()->addForce(_force * force);
		}
	
	};

	class BouncyBall
	{

	public:
		Sphere* ball;
		PxMaterial* material;
		PxVec3* originalPosition;

		BouncyBall(Scene* scene, PxVec3* position, float size, float bounciness)
		{
			originalPosition = position;

			ball = new Sphere(PxTransform(PxVec3(position->x, position->y, position->z)), size, 15.0f);
			material = scene->GetScene()->getPhysics().createMaterial(0.0f, 10.0f, bounciness);
			ball->SetMaterial(material);
			ball->SetColor(color_palette[7]);
			ball->SetMass(1.0f); // Rugby Ball Weight = 0.94798750009838 pounds
			scene->AddActor(ball);
		}

		void Reset()
		{
			ball->SetPosition(PxVec3(originalPosition->x, originalPosition->y, originalPosition->z));
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

		Flooring(Scene* scene, PxVec3* position, PxVec3* scale, PxVec3 color, bool startingFloor, bool slow, bool bouncy)
		{
			Floor = new Box(PxTransform(PxVec3(position->x, position->y + 0.3f, position->z), PxQuat(0.0f, PxVec3(0.0f, 1.0f, 0.0f))), PxVec3(scale->x, scale->y, scale->z));
			Floor->SetKinematic(true);
			Floor->SetColor(color);
			
			if (slow)
			{
				material = scene->GetScene()->getPhysics().createMaterial(0.0f, 100.0f, 0.0f);
				Floor->SetColor(color);
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
				Left = new Box(PxTransform(PxVec3(position->x - (scale->x) - 0.2f, position->y + 0.8f, position->z), PxQuat(0.0f, PxVec3(0.0f, 1.0f, 0.0f))), PxVec3(0.3f, 1.0f, 9.0f));
				Left->SetColor(color_palette[0]);
				Left->SetKinematic(true);

				Right = new Box(PxTransform(PxVec3(position->x + (scale->x) + 0.2f, position->y + 0.8f, position->z), PxQuat(0.0f, PxVec3(0.0f, 1.0f, 0.0f))), PxVec3(0.3f, 1.0f, 9.0f));
				Right->SetColor(color_palette[0]);
				Right->SetKinematic(true);

				End = new Box(PxTransform(PxVec3(position->x, position->y + 0.8f, position->z + (scale->z) + 0.2f), PxQuat(0.0f, PxVec3(0.0f, 1.0f, 0.0f))), PxVec3(6.0f, 1.0f, 0.3f));
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
		float speed = 0.975f;
		PxReal ballMass = 15.0f;
		float linkDistance = 1.5f;

		vector<Box*> boxes;
		vector<RevoluteJoint*> joints;

		WreckingBall(Scene* scene, PxVec3* position, int links, bool reverse = false, float _speed = 0.975f)
		{
			speed = _speed;

			for (int i = 0; i < links; i++)
			{
				boxes.push_back(new Box(PxTransform(PxVec3(position->x, position->y - (1.5f * i), position->z))));

				if (i != 0)
				{
					boxes[i]->SetColor(color_palette[10]);

					if (i == links - 1)
					{
						joints.push_back(new RevoluteJoint(boxes[i - 1], PxTransform(PxVec3(0.0f, -linkDistance - 0.5f, 0.0f)), boxes[i], PxTransform(PxVec3(0.0f, 0.0f, 0.0f))));
						boxes[i]->GetShape()->setGeometry(PxBoxGeometry(1.0f, 1.0f, 1.0f));
						boxes[i]->SetMass(ballMass);
					}
					else
					{
						if((i % 2) == 0 && i != 2)
							joints.push_back(new RevoluteJoint(boxes[i - 1], PxTransform(PxVec3(0.0f, -linkDistance, 0.0f)), boxes[i], PxTransform(PxVec3(0.0f, 0.0f, 0.0f))));
						else
							joints.push_back(new RevoluteJoint(boxes[i - 1], PxTransform(PxVec3(0.0f, -linkDistance, 0.0f), PxQuat(PxPi / 2, PxVec3(0.0f,  1.0f, 0.0f))), boxes[i], PxTransform(PxVec3(0.0f, 0.0f, 0.0f))));
					}
				}
				else
				{
					boxes[i]->SetColor(color_palette[9]);
					boxes[i]->SetKinematic(true);
				}

				scene->AddActor(boxes[i]);
			}

			if(reverse)
				joints[0]->DriveVelocity(-speed);
			else
				joints[0]->DriveVelocity(speed);

		}

		void Update()
		{
			if (boxes[1]->GetRotation().z > 0.50f)
				joints[0]->DriveVelocity(speed);
			else if (boxes[1]->GetRotation().z < -0.50f)
				joints[0]->DriveVelocity(-speed);
		}
	};

	class GoalPost
	{
	public:
		Box * barBox;
		vector<Box*> boxes;
		PxMaterial* material;

		PxVec3 barPos;
		PxQuat barRot;
		vector<PxVec3> positions;
		vector<PxQuat> rotations;

		GoalPost(Scene* scene, PxVec3* position, int boxNumber = 18)
		{
			int bar = (boxNumber / 6) * 5;
			material = scene->GetScene()->getPhysics().createMaterial(0.0f, 10.0f, 0.0f);
			int y = 0;

			for (int i = 0; i < boxNumber; i++)
			{
				y++;

				if ((i % 2) == 0)
					continue;

				if (bar == (i + 1))
					bar = i;

				if (i == bar)
				{
					barBox = new Box(PxTransform(PxVec3(position->x, position->y + (y * 0.52f), position->z)), PxVec3(3.0f, 0.5f, 0.5f));
					barBox->SetColor(color_palette[10]);
					scene->AddActor(barBox);
					bar = 100000000;
					i--;
					continue;
				}

				boxes.push_back(new Box(PxTransform(PxVec3(position->x - 2.5f, position->y + (y * 0.55f), position->z))));
				boxes.push_back(new Box(PxTransform(PxVec3(position->x + 2.5f, position->y + (y * 0.55f), position->z))));
				
				if ((i % 3) == 0)
				{
					boxes[i - 1]->SetColor(color_palette[10]);
					boxes[i]->SetColor(color_palette[10]);
				}
				else
				{
					boxes[i - 1]->SetColor(color_palette[9]);
					boxes[i]->SetColor(color_palette[9]);
				}

				boxes[i - 1]->SetMaterial(material);
				boxes[i]->SetMaterial(material);

				scene->AddActor(boxes[i - 1]);
				scene->AddActor(boxes[i]);
			}
				
			for (int i = 0; i < boxes.size(); i++)
			{
				positions.push_back(boxes[i]->GetPosition());
				rotations.push_back(boxes[i]->GetRotation());
			}

			barPos = barBox->GetPosition();
			barRot = barBox->GetRotation();
		}

		void Reset()
		{
			for (int i = 0; i < boxes.size(); i++)
			{
				boxes[i]->GetRigidBody()->setLinearVelocity(PxVec3(0.0f, 0.0f, 0.0f));
				boxes[i]->GetRigidBody()->setAngularVelocity(PxVec3(0.0f, 0.0f, 0.0f));
				boxes[i]->SetPosition(PxVec3(positions[i].x, positions[i].y, positions[i].z));
				boxes[i]->SetRotation(rotations[i]);
			}

			barBox->GetRigidBody()->setLinearVelocity(PxVec3(0.0f, 0.0f, 0.0f));
			barBox->GetRigidBody()->setAngularVelocity(PxVec3(0.0f, 0.0f, 0.0f));
			barBox->SetPosition(PxVec3(barPos.x, barPos.y, barPos.z));
			barBox->SetRotation(barRot);
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
		PxQuat originalRot2;
		bool reset;

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
			originalRot2 = body->GetRotation();
		}

		void Update()
		{

			if (shoot)
			{
				base->SetKinematic(true);
				joint->DriveVelocity(-10.0f);
				reset = false;
			}
			else
			{
				Reset();
				base->SetKinematic(false);
			}
		}

		void Reset()
		{
			joint->DriveVelocity(0.0f);
			base->SetPosition(PxVec3(originalPos->x, originalPos->y, originalPos->z));
			base->SetRotation(originalRot);
			body->SetRotation(originalRot2);
			body->GetRigidBody()->setLinearVelocity(PxVec3(0.0f, 0.0f, 0.0f));
			body->GetRigidBody()->setAngularVelocity(PxVec3(0.0f, 0.0f, 0.0f));
			reset = true;
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
		float buttonPosition = 2.7f;
		bool activated;

		Catapult* catapult;

		CatapultButton(Scene* scene, PxVec3* position, Catapult* _catapult, float _buttonPosition = 2.7f)
		{
			buttonPosition = _buttonPosition;

			catapult = _catapult;

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
			((PxRigidDynamic*)Button->GetPxActor())->setGlobalPose(PxTransform(PxVec3(catapult->originalPos->x + 1.6f, catapult->originalPos->y + 3.8f, catapult->originalPos->z + buttonPosition)));
		}
	};

	// Custom Sphere
	class CustomSphere : public DynamicActor
	{
	public:
		CustomSphere(const PxTransform & pose, PxVec3 dimensions, PxReal density) : DynamicActor(pose)
		{
			CreateShape(PxBoxGeometry(dimensions), density);
		}

		void Render()
		{
			//Get the position of this object
			PxTransform pose = ((PxRigidBody*)GetPxActor())->getGlobalPose();
			PxMat44 shapePose(pose);
			//move the opengl matrix to match the physx object
			glPushMatrix();
			glMultMatrixf((float*)&shapePose);

			//currently, renderer is at center of mass of object, use standard opengl
			//glutSolidCone(3.0, 2.0, 6.0, 3.0);
			glutSolidSphere(1.5, 6, 6);
			//glutWireSphere(1.5, 6, 6);
			//pop opengl matrix back to before
			glPopMatrix();
		}
	};
}
