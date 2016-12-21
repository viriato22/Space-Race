#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = f_acceleration = l_acceleration = u_acceleration = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(3, 1, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 20000.0f;
	car.linear_dampening = 0.5f;

	time.Start();

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 50, 0);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = f_acceleration = l_acceleration = u_acceleration = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		f_acceleration += MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		f_acceleration -= BACK_ACCELERATION;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		l_acceleration -= MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		l_acceleration += MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
	{
		u_acceleration += MAX_ACCELERATION;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
	{
		u_acceleration -= MAX_ACCELERATION;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		vehicle->Push(0, 5000, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		vehicle->SetPos(0, 2, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		float* matrix = new float [30];
		
		vehicle->GetTransform(matrix);
		vehicle->SetPos(matrix[0], matrix[1], matrix[2] + 20);
		delete[] matrix;
	}

	//DANI-> BARREL ROLL
	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_REPEAT) {
		vehicle->body->applyImpulse(btVector3(0, 70, 0), btVector3(10, 0, 0));
	}

	vehicle->applyForwardImpulse(f_acceleration);
	vehicle->applyLateralImpulse(l_acceleration);
	vehicle->applyUpwardImpulse(u_acceleration);
	vehicle->Turn(turn);

	char title[80];
	sprintf_s(title, "%.1f Km/h, Time: %.1f / Record:%.1f ", vehicle->GetKmh(), (float)time.Read() / 1000.0f, record);
	App->window->SetTitle(title);
	float trans[16];
	vehicle->GetTransform(trans);
	vec3 pos(trans[12], trans[13], trans[14]);
	vec3 dir(trans[8], -0.5f, trans[10]);
	App->camera->LookAt(pos);
	pos -= dir * 10;
	App->camera->Position = pos;

	return UPDATE_CONTINUE;
}

