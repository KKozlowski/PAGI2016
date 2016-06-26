#pragma once
#include "main.h"
#include "Timer.h"
#include <cstdlib>
#include <vector>

using namespace std;

class Ball
{
private:
	Object3DS *me;

	vector<Ball *> colliding;

	bool wasColliding(Ball *b)
	{
		bool contains = false;
		for (Ball * bal : colliding)
		{
			if (bal == b)
			{
				contains = true;
				break;
			}
		}
		return contains;
	}

	void try_hit(Ball *b)
	{
		Vector3 distanceVec = b->me->transform.position - me->transform.position;
		if (distanceVec.Mag() < get_radius() + b->get_radius())
		{
			bool contains = wasColliding(b);
			if (contains)
				return;
			else
				colliding.push_back(b);

			//b->me->hidden = true;
			float velocityGiven = velocity.Dot(distanceVec);
			velocityGiven *= 0.01f;
			Vector3 vel = distanceVec/2;
			b->add_velocity(vel * (velocityGiven *radius/b->get_radius())  );
			this->add_velocity(vel*(-velocityGiven * b->get_radius()/radius));
				//printFLOAT(velocityGiven);
		} else
		{
			bool contains = wasColliding(b);
			if (contains)
				colliding.erase(find(colliding.begin(), colliding.end(), b));
		}
			
	}

	void updateSelf(float deltaTime)
	{
		float prevZ = me->transform.position.z;
		if (duringThrow) me->transform.position.z = me->transform.position.z - minusZ;

		me->transform.position = me->transform.position + velocity*deltaTime;

		if (duringThrow) {
			timePassed += deltaTime;
			minusZ = -G*0.5f*timePassed*timePassed;
			me->transform.position.z = me->transform.position.z + minusZ;
			float nowZ = me->transform.position.z;
			if (me->transform.position.z <= floorLevel + radius && prevZ > nowZ) {
				velocity.z = -velocity.z*bumpiness;
				stop_throw_simulation();
				start_throw_simulation();
			}
		}

		if (checksForCollisions)
		for (Ball *b : balls)
		{
			if (b!=this)
			{
				try_hit(b);
			}
		}
		
	}

	static Timer t;

	float timePassed = 0;
	bool duringThrow = false;
	float minusZ = 0;
	float radius = 1;
	
public:
	Vector3 velocity;
	bool checksForCollisions = false;
	Object3DS *getObject() { return me; }

	float get_radius() const
	{ return radius; }

	Ball(Object3DS *o)
	{
		me = o;
		balls.push_back(this);

		float maxZ = 0, minZ = 0;
		for (int i = 0; i < me->vertexCount; i++)
		{
			float z = me->vertices[i].z - me->transform.pivot.z;
			if (z > maxZ)
			{
				maxZ = z;
			}
			if (z < minZ)
				minZ = z;
		}
		radius = (maxZ - minZ) / 2;
		printFLOAT(radius);
	}

	TransformInfo *transform() { return &me->transform; }
	
	static vector<Ball *> balls;

	static float G;
	
	static float floorLevel;
	static float bumpiness;

	void add_velocity(Vector3 v)
	{
		velocity = velocity + v;
	}

	void start_throw_simulation()
	{
		printSTR("START SIMULATION");
		duringThrow = true;
		timePassed = 0;
		minusZ = 0;
	}

	void stop_throw_simulation()
	{
		duringThrow = false;
		timePassed = 0;
		minusZ = 0;
	}

	static void update()
	{
		t.update();
		for (Ball * b : balls)
		{
			b->updateSelf(t.get_delta_time());
		}
	}
};
