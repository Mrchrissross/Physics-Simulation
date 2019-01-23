#include <iostream> //cout, cerr
#include <iomanip> //stream formatting
#include <windows.h> //delay function
#include <PxPhysicsAPI.h> //PhysX

using namespace std;
using namespace physx;

//PhysX objects
PxPhysics* physics;
PxFoundation* foundation;
debugger::comm::PvdConnection* vd_connection;

//simulation objects
PxScene* scene;
PxRigidDynamic* box;
PxRigidStatic* plane;

int simSteps;
float runTime;
int seconds;

///Initialise PhysX objects
bool PxInit()
{
	//default error and allocator callbacks
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	//Init PhysX
	//foundation
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

	if(!foundation)
		return false;

	//physics
	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());

	if(!physics)
		return false;

	//connect to an external visual debugger (if exists)
	vd_connection = PxVisualDebuggerExt::createConnection(physics->getPvdConnectionManager(), "localhost", 5425, 100, 
		PxVisualDebuggerExt::getAllConnectionFlags());

	//create a default scene
	PxSceneDesc sceneDesc(physics->getTolerancesScale());

	if(!sceneDesc.cpuDispatcher)
	{
		PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		sceneDesc.cpuDispatcher = mCpuDispatcher;
	}

	if(!sceneDesc.filterShader)
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	scene = physics->createScene(sceneDesc);

	if (!scene)
		return false;

	return true;
}

/// Release all allocated resources
void PxRelease()
{
	if (scene)
		scene->release();
	if (vd_connection)
		vd_connection->release();
	if (physics)
		physics->release();
	if (foundation)
		foundation->release();
}

PxShape* boxShape;

///Initialise the scene
void InitScene()
{
	//default gravity
	scene->setGravity(PxVec3(0.f, -9.81f, 0.f));

	//materials
	PxMaterial* default_material = physics->createMaterial(0.f, 0.f, 0.0f);   //static friction, dynamic friction, restitution

	//create a static plane (XZ)
	plane = PxCreatePlane(*physics, PxPlane(PxVec3(0.f, 1.f, 0.f), 0.f), *default_material);
	scene->addActor(*plane);

	//create a dynamic actor and place it 10 m above the ground
	box = physics->createRigidDynamic(PxTransform(PxVec3(0.f, 0.5f, 0.f)));
	//create a box shape of 1m x 1m x 1m size (values are provided in halves)
	boxShape = box->createShape(PxBoxGeometry(.5f, .5f, .5f), *default_material);
	//update the mass of the box
	PxRigidBodyExt::updateMassAndInertia(*box, 1.f); //density of 1kg/m^3
	scene->addActor(*box);

	//create a dynamic actor and place it 10 m above the ground
	box = physics->createRigidDynamic(PxTransform(PxVec3(0.f, 5.f, 0.f)));
	//create a box shape of 1m x 1m x 1m size (values are provided in halves)
	boxShape = box->createShape(PxBoxGeometry(.5f, .5f, .5f), *default_material);
	//update the mass of the box
	PxRigidBodyExt::updateMassAndInertia(*box, 1.f); //density of 1kg/m^3
	scene->addActor(*box);

	box->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
}

/// Perform a single simulation step
void Update(PxReal delta_time)
{
	scene->simulate(delta_time);
	scene->fetchResults(true);

	simSteps++;
	runTime = runTime + delta_time;

	if (runTime > 60)
	{

		cout << endl;
		cout << "----------------------------------------------------------------------" << endl;
		cout << endl;

		////'visualise' position and velocity of the box
		PxVec3 pos = box->getGlobalPose().p;
		PxVec3 vel = box->getLinearVelocity();
		cout << setiosflags(ios::fixed) << setprecision(2) << "x=" << pos.x <<
			", y=" << pos.y << ", z=" << pos.z << ",  ";
		cout << setiosflags(ios::fixed) << setprecision(2) << "vx=" << vel.x <<
			", vy=" << vel.y << ", vz=" << vel.z << endl;

		float mass = box->getMass();

		cout << endl;
		cout << "Box Mass: " << mass << endl;

		seconds++;
		cout << "Seconds passed: " << seconds << endl;
		runTime = 0;

		if (pos.x >= 10)
			box->setLinearVelocity(PxVec3(0,0,0));
	}

	if (seconds == 2)
	{
		//box->setGlobalPose(PxTransform(PxVec3(10.f, 0.f, 0.f)));
		//box->addForce(PxVec3(0.001f, 0.0f, 0.0f));
	}
}

/// The main function
int main()
{
	//initialise PhysX	
	if (!PxInit())
	{
		cerr << "Could not initialise PhysX." << endl;
		return 0;
	}

	//initialise the scene
	InitScene();

	//set the simulation step to 1/60th of a second
	PxReal delta_time = 1.0f/60.0f;

	//simulate until the 'Esc' is pressed
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		//perform a single simulation step
		Update(delta_time);
		
		////introduce 100ms delay for easier visual analysis of the results
		//Sleep(1000);
	}

	//Release all resources
	PxRelease();

	return 0;
}
