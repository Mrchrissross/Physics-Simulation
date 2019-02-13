#include "VisualDebugger.h"
#include <vector>
#include "Extras\Camera.h"
#include "Extras\Renderer.h"
#include "Extras\HUD.h"
#include "GameScene.h"

namespace VisualDebugger
{
	using namespace physx;

	enum RenderMode
	{
		DEBUG,
		NORMAL,
		BOTH
	};

	enum HUDState
	{
		EMPTY = 0,
		HELP = 1,
		PAUSE = 2
	};

	//function declarations
	void KeyHold();
	void KeySpecial(int key, int x, int y);
	void KeyRelease(unsigned char key, int x, int y);
	void KeyPress(unsigned char key, int x, int y);

	void motionCallback(int x, int y);
	void mouseCallback(int button, int state, int x, int y);
	void exitCallback(void);

	void RenderScene();
	void ToggleRenderMode();
	void HUDInit();
	
	float camSpeed = 5.0f;
	bool enableCameraMotion;
	int size;

	///simulation objects
	Camera* camera;
	PhysicsEngine::GameScene* scene;
	PxReal delta_time = 1.f/60.f;
	RenderMode render_mode = NORMAL;
	const int MAX_KEYS = 256;
	bool key_state[MAX_KEYS];
	bool hud_show = true;
	HUD hud;

	//Init the debugger
	void Init(const char *window_name, int width, int height)
	{
		///Init PhysX
		PhysicsEngine::PxInit();
		scene = new PhysicsEngine::GameScene();
		scene->SceneInit();

		///Init renderer
		Renderer::BackgroundColor(PxVec3(150.f/255.f,150.f/255.f,150.f/255.f));
		Renderer::SetRenderDetail(40);
		Renderer::InitWindow(window_name, width, height);
		Renderer::Init();

		camera = new Camera(PxVec3(0.0f, 5.0f, 15.0f), PxVec3(0.f,-.1f,-1.f), 5.f);

		//initialise HUD
		HUDInit();

		///Assign callbacks
		//render
		glutDisplayFunc(RenderScene);

		//keyboard
		glutKeyboardFunc(KeyPress);
		glutSpecialFunc(KeySpecial);
		glutKeyboardUpFunc(KeyRelease);

		//mouse
		glutMouseFunc(mouseCallback);
		glutMotionFunc(motionCallback);

		//exit
		atexit(exitCallback);

		//init motion callback
		motionCallback(0,0);
	}

	void HUDInit()
	{
		//initialise HUD
		//add an empty screen
		hud.AddLine(EMPTY, "");
		//add a help screen
		hud.AddLine(HELP, "");
		hud.AddLine(HELP, "   Press F1 to pause simulation.");
		hud.AddLine(HELP, "");
		hud.AddLine(HELP, "");
		//add a pause screen
		hud.AddLine(PAUSE, "");
		hud.AddLine(PAUSE, "   Controls:");
		hud.AddLine(PAUSE, "");
		hud.AddLine(PAUSE, "   Ball Movement");
		hud.AddLine(PAUSE, "      W,S,A,D - forward,backward,left,right");
		hud.AddLine(PAUSE, "      R - Reset ball (adds score)");
		hud.AddLine(PAUSE, "");
		hud.AddLine(PAUSE, "   Camera Movement");
		hud.AddLine(PAUSE, "      F9 - Enable Freeview / Disable Freeview");
		hud.AddLine(PAUSE, "      W,S,A,D,E,Q - forward,backward,left,right,up,down");
		hud.AddLine(PAUSE, "      mouse + click - change orientation");
		hud.AddLine(PAUSE, "");
		hud.AddLine(PAUSE, "");
		hud.AddLine(PAUSE, "   Simulation Paused. Press F1 to continue.");
		hud.AddLine(PAUSE, "");
		//set font size for all screens
		hud.FontSize(0.018f);
		//set font color for all screens
		hud.Color(PxVec3(0.f, 0.f, 0.f));
	}

	//Start the main loop
	void Start()
	{ 
		glutMainLoop(); 
	}

	//Render the scene and perform a single simulation step
	void RenderScene()
	{
		if (scene->ball != nullptr && !enableCameraMotion)
			camera->setEye(scene->ball->GetPosition() - PxVec3(0.0f, -2.5f, -5.0f));

		//handle pressed keys
		KeyHold();

		//start rendering
		Renderer::Start(camera->getEye(), camera->getDir());

		if ((render_mode == DEBUG) || (render_mode == BOTH))
			Renderer::Render(scene->GetScene()->getRenderBuffer());

		if ((render_mode == NORMAL) || (render_mode == BOTH))
		{
			std::vector<PxActor*> actors = scene->GetAllActors();
			if (actors.size())
				Renderer::Render(&actors[0], (PxU32)actors.size());
		}

		int score = scene->GetScore();
		hud.AmendLine(HELP, "   Score: " + to_string(score));

		vector <int> scores = scene->GetScoreBoard();

		if (size != scores.size())
		{
			hud.AddLine(PAUSE, "");
			hud.AmendLine(PAUSE, "   Score " + to_string(scores.size()) + " was " + to_string(scores[scores.size() - 1]));
			size = scores.size();
		}

		//adjust the HUD state
		if (hud_show)
		{
			if (scene->isPaused())
				hud.ActiveScreen(PAUSE);
			else
				hud.ActiveScreen(HELP);
		}
		else
			hud.ActiveScreen(EMPTY);

		//render HUD
		hud.Render();

		//finish rendering
		Renderer::Finish();

		//perform a single simulation step
		scene->SceneUpdate(delta_time);
	}

	// This is used for when a key is pressed.
	void UserKeyPress(int key)
	{
		switch (toupper(key))
		{
		case 'R':
			scene->ball->addScore = true;
			break;
		default:
			break;
		}
	}

	// This is used when a key is released.
	void UserKeyRelease(int key)
	{
		switch (toupper(key))
		{
		case 'R': // Replace 
			break;
		default:
			break;
		}
	}

	void UserKeyHold(int key){}

	//handle camera control keys
	void CameraInput(int key){}

	//handle force control keys
	void ForceInput(int key)
	{
		if (!scene->GetSelectedActor())
			return;

		if(enableCameraMotion)
		{
			switch (toupper(key))
			{
			case 'W':
				camera->MoveForward(delta_time * 5);
				break;
			case 'S':
				camera->MoveBackward(delta_time * 5);
				break;
			case 'A':
				camera->MoveLeft(delta_time * 5);
				break;
			case 'D':
				camera->MoveRight(delta_time * 5);
				break;
			case 'E':
				camera->MoveUp(delta_time * 5);
				break;
			case 'Q':
				camera->MoveDown(delta_time * 5);
				break;
			default:
				break;
			}
		}
		else
		{
			switch (toupper(key))
			{
				case 'W':
					scene->ball->addForce(PxVec3(0, 0, -1));
					break;
				case 'S':
					scene->ball->addForce(PxVec3(0, 0, 1));
					break;
				case 'A':
					scene->ball->addForce(PxVec3(-1, 0, 0));
					break;
				case 'D':
					scene->ball->addForce(PxVec3(1, 0, 0));
					break;
					/*case 'Q':
					scene->ball->addForce(PxVec3(0, -1, 0));
					break;*/
				default:
					break;
			}
		}
	}

	///handle special keys
	void KeySpecial(int key, int x, int y)
	{
		//simulation control
		switch (key)
		{
		case GLUT_KEY_F1:
			//toggle scene pause
			scene->Pause(!scene->isPaused());
			break;
			//display control
		case GLUT_KEY_F5:
			//hud on/off
			hud_show = !hud_show;
			break;
		case GLUT_KEY_F6:
			//shadows on/off
			Renderer::ShowShadows(!Renderer::ShowShadows());
			break;
		case GLUT_KEY_F7:
			//toggle render mode
			ToggleRenderMode();
			break;
		case GLUT_KEY_F8:
			break;
			//simulation control
		case GLUT_KEY_F9:
			//enable camera motion
			enableCameraMotion = !enableCameraMotion;
			//reset camera view
			if(!enableCameraMotion) camera->Reset();
			break;
		case GLUT_KEY_F12:
			//resect scene
			scene->Reset();
			break;
		default:
			break;
		}
	}

	//handle single key presses
	void KeyPress(unsigned char key, int x, int y)
	{
		//do it only once
		if (key_state[key] == true)
			return;

		key_state[key] = true;

		//exit
		if (key == 27)
			exit(0);

		UserKeyPress(key);
	}

	//handle key release
	void KeyRelease(unsigned char key, int x, int y)
	{
		key_state[key] = false;
		UserKeyRelease(key);
	}

	//handle held keys
	void KeyHold()
	{
		for (int i = 0; i < MAX_KEYS; i++)
		{
			if (key_state[i]) // if key down
			{
				CameraInput(i);
				ForceInput(i);
				UserKeyHold(i);
			}
		}
	}

	///mouse handling
	int mMouseX = 0;
	int mMouseY = 0;

	void motionCallback(int x, int y)
	{
		int dx = mMouseX - x;
		int dy = mMouseY - y;

		if(enableCameraMotion)
		camera->Motion(dx, dy, delta_time);

		mMouseX = x;
		mMouseY = y;
	}

	void mouseCallback(int button, int state, int x, int y)
	{
		mMouseX = x;
		mMouseY = y;
	}

	void ToggleRenderMode()
	{
		if (render_mode == NORMAL)
			render_mode = DEBUG;
		else if (render_mode == DEBUG)
			render_mode = BOTH;
		else if (render_mode == BOTH)
			render_mode = NORMAL;
	}

	///exit callback
	void exitCallback(void)
	{
		delete camera;
		delete scene;
		PhysicsEngine::PxRelease();
	}
}

