#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 2000.0f  //DEFAULT 800.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BACK_ACCELERATION 2000.0f //DEFAULT 800.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:

	PhysVehicle3D* vehicle;
	float turn;
	float f_acceleration;
	float l_acceleration;
	float u_acceleration;
};