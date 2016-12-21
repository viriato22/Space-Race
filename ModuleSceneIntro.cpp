#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#include <stdlib.h>
#include <time.h>

#define RADIUS 44

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	srand(time(NULL));

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	vec3 pos = { 0, 50, 100 };
	CreateTorus(pos);

	pos = { 0, 60, 500 };
	CreateTorus(pos);

	pos = { 20, 80, 1000 };
	CreateTorus(pos);

	pos = { -10, 100, 1500 };
	CreateTorus(pos);

	pos = { 40, 50, 2000 };
	CreateTorus(pos);

	pos = { 50, 90, 2500 };
	CreateTorus(pos);

	pos = { 30, 70, 3000 };
	CreateTorus(pos);

	//AsteroidField();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	/*Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();*/

	for (p2List_item<Cylinder>* aux = Cylinders.getFirst(); aux != nullptr; aux = aux->next) {
		aux->data.Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateTorus(vec3 pos) {
	/*Cylinder torus[12];

	for (int aux = 0; aux < 12; aux++) {
		torus[aux].color = Red;
		torus[aux].radius = 5;
		torus[aux].height = 20;
		//TEMP WAY
		switch(aux){
		case 0: // NORTH
			torus[aux].SetPos(pos.x, pos.y + RADIUS, pos.z);
			break;
		case 1: // NORTH NORTH-EAST
			torus[aux].SetPos(pos.x - RADIUS + 22, pos.y + RADIUS - 6, pos.z);
			torus[aux].SetRotation(30, { 0, 0, 1 });
			break;
		case 2: // EAST NORTH-EAST
			torus[aux].SetPos(pos.x - RADIUS + 6, pos.y + RADIUS - 22, pos.z);
			torus[aux].SetRotation(60, { 0, 0, 1 });
			break;
		case 3: // EAST
			torus[aux].SetPos(pos.x - RADIUS, pos.y, pos.z);
			torus[aux].SetRotation(90, { 0, 0, 1 });
			break;
		case 4: // EAST SOUTH-EAST
			torus[aux].SetPos(pos.x - RADIUS + 6, pos.y - RADIUS + 22, pos.z);
			torus[aux].SetRotation(60, { 0, 0, -1 });
			break;
		case 5: // SOUTH SOUTH-EAST
			torus[aux].SetPos(pos.x - RADIUS + 22, pos.y - RADIUS + 6, pos.z);
			torus[aux].SetRotation(30, { 0, 0, -1 });
			break;
		case 6: // SOUTH
			torus[aux].SetPos(pos.x, pos.y - RADIUS, pos.z);
			break;
		case 7: // SOUTH SOUTH-WEST
			torus[aux].SetPos(pos.x + RADIUS - 22, pos.y - RADIUS + 6, pos.z);
			torus[aux].SetRotation(30, { 0, 0, 1 });
			break;
		case 8: // WEST SOUTH-WEST
			torus[aux].SetPos(pos.x + RADIUS - 6, pos.y - RADIUS + 22, pos.z);
			torus[aux].SetRotation(60, { 0, 0, 1 });
			break;
		case 9: // WEST
			torus[aux].SetPos(pos.x + RADIUS, pos.y, pos.z);
			torus[aux].SetRotation(90, { 0, 0, 1 });
			break;
		case 10: // WEST NORTH-WEST
			torus[aux].SetPos(pos.x + RADIUS - 6, pos.y + RADIUS - 22, pos.z);
			torus[aux].SetRotation(60, { 0, 0, -1 });
			break;
		case 11: // NORTH NORTH-WEST
			torus[aux].SetPos(pos.x + RADIUS - 22, pos.y + RADIUS - 6, pos.z);
			torus[aux].SetRotation(30, { 0, 0, -1 });
			break;
		default:
			torus[aux].SetPos(pos.x + 20*aux, pos.y, pos.z);
		}
		App->physics->AddBody(torus[aux], 100000);
		Cylinders.add(torus[aux]);
	}*/
	btVector3 position(pos.x, pos.y, pos.z);
	App->physics->AddTorus(position, 5, 20, 30);
}

void ModuleSceneIntro::AsteroidField() {
	Sphere spheres[20];
	int radius;

	for (int aux = 0; aux < 20; aux++) {
		radius = rand() % 40 + 20;
		spheres[aux].radius = radius;

		switch (aux) {
		case 0:
			spheres[aux].SetPos(6, 70, 200);
			break;
		case 1:
			spheres[aux].SetPos(-20, -60, 300);
			break;
		}

		App->physics->AddBody(spheres[aux], 100000);
		Asteroids.add(spheres[aux]);
	}
}