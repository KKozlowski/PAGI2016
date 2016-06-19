#pragma once
#include "main.h"
#include "Timer.h"

class Ball
{
private:
	Object3DS *me;

	void updateSelf(float deltaTime)
	{
		me->transform.position = me->transform.position + velocity*deltaTime;
	}

	static Timer t;

	
public:
	Vector3 velocity;

	Ball(Object3DS *o)
	{
		me = o;
		balls.push_back(this);
	}

	TransformInfo *transform() { return &me->transform; }
	
	static vector<Ball *> balls;

	static void update()
	{
		t.update();
		for (Ball * b : balls)
		{
			b->updateSelf(t.get_delta_time());
		}
	}
};
