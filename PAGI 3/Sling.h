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
	bool duringThrow = false;
	float forceScale = 16.0f;

	Vector3 previousBallFinalPosition;

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
		if (duringThrow)
		{
			Vector3 velocity = get_center() - ball->transform()->position;
			
			ball->add_velocity(velocity*t.get_delta_time()*forceScale);
			if (ball->transform()->position.y > 0)
			{
				duringThrow = false;
				ball->start_throw_simulation();
			}
				
		}

		//printSTR(ball->transform()->position.to_string());
	}

	void release()
	{
		if (ball->transform()->position.y <= 0)
		{
			previousBallFinalPosition = ball->transform()->position;
			Vector3 velocity = get_center() - ball->transform()->position;
			printSTR(velocity.to_string());
			ball->add_velocity(velocity);
			
			duringThrow = true;
		}
		
	}

	Vector3 get_center()
	{
		Vector3 center = (leftEnd->transform.position + rightEnd->transform.position) / 2;
		center = center + me->transform.position;
		return center;
	}

	void take_ball(bool toPreviousPosition = true)
	{
		duringThrow = false;
		ball->stop_throw_simulation();
		printSTR("RETURN");
		ball->velocity = Vector3::zero;
		ball->getObject()->hidden = false;
		if (toPreviousPosition)
			ball->transform()->position = previousBallFinalPosition;
		else
			ball->transform()->position = get_center();

	}
};
