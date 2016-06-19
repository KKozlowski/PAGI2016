#pragma once
#include "main.h"
#include "Timer.h"

class Ball
{
private:
	Object3DS *me;

	void updateSelf(float deltaTime)
	{
		if (duringThrow) me->transform.position.z = me->transform.position.z - minusZ;

		me->transform.position = me->transform.position + velocity*deltaTime;

		if (duringThrow) {
			timePassed += deltaTime;
			minusZ = -G*0.5f*timePassed*timePassed;
			me->transform.position.z = me->transform.position.z + minusZ;
		}
	}

	static Timer t;

	float timePassed = 0;
	bool duringThrow = false;
	float minusZ = 0;

	
public:
	Vector3 velocity;

	Ball(Object3DS *o)
	{
		me = o;
		balls.push_back(this);
	}

	TransformInfo *transform() { return &me->transform; }
	
	static vector<Ball *> balls;

	static float G;

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
