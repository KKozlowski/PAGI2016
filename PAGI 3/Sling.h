#pragma once
#include "Timer.h"
#include "Ball.h"

class Sling
{
private:
	Timer t;
	Object3DS *me;
	Ball *ball;
	Object3DS *leftEnd;
	Object3DS *rightEnd;

public:
	static Sling *main;

	Sling(Object3DS *me, Ball *ball, Object3DS* L, Object3DS * R)
	{
		this->me = me;
		this->ball = ball;
		leftEnd = L;
		rightEnd = R;
	}

	void update()
	{
		t.update();
	}

	void release()
	{
		Vector3 velocity = get_center() - ball->transform()->position;
		printSTR(velocity.to_string());
		ball->velocity = velocity;
	}

	Vector3 get_center()
	{
		Vector3 center = (leftEnd->transform.position + rightEnd->transform.position) / 2;
		center = center + me->transform.position;
		return center;
	}

	void take_ball()
	{
		printSTR("RETURN");
		ball->velocity = Vector3::zero;
		ball->transform()->position = get_center();
	}
};
