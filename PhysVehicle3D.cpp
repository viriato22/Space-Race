#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, const VehicleInfo& info) : PhysBody3D(body), body(body), info(info), currentSpeedKmHour(0)
{
	forward = right = up = { 0,0,0 };
	body->setDamping(0,0.5);
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Update(bool render)
{
	float v_matrix[16];
	this->GetTransform(v_matrix);

	forward = { v_matrix[8], v_matrix[9], v_matrix[10] };
	forward.normalize();

	right = forward.cross({ 0,1,0 });
	right.normalize();

	up = forward.cross({ -1,0,0 });
	up.normalize();

	currentSpeedKmHour = btScalar(3.6) * body->getLinearVelocity().length();

	if (forward.dot(body->getLinearVelocity()) < btScalar(0.))
	{
		currentSpeedKmHour *= btScalar(-1.);
	}
	

	if (render)
	{
		Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
		body->getWorldTransform().getOpenGLMatrix(&chassis.transform);
		btQuaternion q = body->getWorldTransform().getRotation();
		btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
		offset = offset.rotate(q.getAxis(), q.getAngle());

		chassis.transform.M[12] += offset.getX();
		chassis.transform.M[13] += offset.getY();
		chassis.transform.M[14] += offset.getZ();


		chassis.Render();
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::applyForwardImpulse(float force)
{
	btVector3 velocity = body->getLinearVelocity();
	btVector3 projection = (velocity.dot(forward) / forward.dot(forward)) * forward;

	if (force != 0)
	{
		body->applyCentralForce(forward * (force*info.mass / (fabs(currentSpeedKmHour) + 1)));
		if (projection.length() < 5.0f)
			body->applyCentralForce(forward * (force* 2 *info.mass / (fabs(currentSpeedKmHour) + 1)));
	}

	else
		body->setLinearVelocity(velocity - projection / 50);
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::applyLateralImpulse(float force)
{
	btVector3 velocity = body->getLinearVelocity();
	btVector3 projection = (velocity.dot(right) / right.dot(right)) * right;

	if (force != 0)
	{
		body->applyCentralForce(right * (force*info.mass / (fabs(currentSpeedKmHour) + 1)));
		if (projection.length() < 5.0f)
			body->applyCentralForce(forward * (force * 2 * info.mass / (fabs(currentSpeedKmHour) + 1)));
	}
	else
		body->setLinearVelocity(velocity - projection /10);

}

// ----------------------------------------------------------------------------
void PhysVehicle3D::applyUpwardImpulse(float force)
{
	btVector3 velocity = body->getLinearVelocity();
	btVector3 projection = (velocity.dot(up) / up.dot(up)) * up;

	if (force != 0)
	{
		body->applyCentralForce(up * (-force*info.mass / (fabs(currentSpeedKmHour) + 1)));
		if (projection.length() < 5.0f)
			body->applyCentralForce(forward * (force * 2 * info.mass / (fabs(currentSpeedKmHour) + 1)));
	}
	else
		body->setLinearVelocity(velocity - projection / 10);

}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	//TBD
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return currentSpeedKmHour;
}