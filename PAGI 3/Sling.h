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

	void DrawLine()
	{
		if (ball->transform()->position.y < get_center().z) {
			glPushMatrix();


			Vector3 lPos = leftEnd->transform.self_position_without_rotation();
			lPos = lPos + me->transform.self_position_without_rotation() + Vector3(0, 0, 24);
			Vector3 rPos = rightEnd->transform.self_position_without_rotation();
			rPos = rPos + me->transform.self_position_without_rotation() + Vector3(0, 0, 24);
			Vector3 bPos = ball->getObject()->transform.self_position_without_rotation();

			/*glPushMatrix();
			glTranslatef(lPos.x, lPos.y, lPos.z);
			glBegin(GL_TRIANGLES);
			glColor3f(0.1, 0.2, 0.3);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 10);
			glVertex3f(10, 0, 0);
			glEnd();
			glPopMatrix();*/



			glPushMatrix();
			glColor3f(0.1, 0.2, 0.3);
			glTranslatef(lPos.x, lPos.y, lPos.z);
			glBegin(GL_TRIANGLES);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 3);
			glVertex3f(bPos.x - lPos.x - 1.8f, bPos.y - lPos.y - 3, bPos.z - lPos.z - 2);
			glVertex3f(bPos.x - lPos.x - 1.8f, bPos.y - lPos.y - 3, bPos.z - lPos.z - 2);
			glVertex3f(bPos.x - lPos.x - 1.8f, bPos.y - lPos.y - 3, bPos.z - lPos.z);
			glVertex3f(0, 0, 3);
			glEnd();

			glTranslatef(rPos.x - lPos.x, rPos.y - lPos.y, rPos.z - lPos.z);
			glBegin(GL_TRIANGLES);
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 3);
			glVertex3f(bPos.x - rPos.x + 1.8f, bPos.y - rPos.y - 3, bPos.z - rPos.z - 2);
			glVertex3f(bPos.x - rPos.x + 1.8f, bPos.y - rPos.y - 3, bPos.z - rPos.z - 2);
			glVertex3f(bPos.x - rPos.x + 1.8f, bPos.y - rPos.y - 3, bPos.z - rPos.z);
			glVertex3f(0, 0, 3);
			glEnd();
			

			glTranslatef(bPos.x - rPos.x, bPos.y - rPos.y, bPos.z - rPos.z);
			glBegin(GL_TRIANGLES);
			glVertex3f(-1.8f, -3, 0);
			glVertex3f(-1.8f, -3, -2);
			glVertex3f(1.8f, -3, -2);
			glVertex3f(1.8f, -3, -2);
			glVertex3f(1.8f, -3, 0);
			glVertex3f(-1.8f, -3, 0);
			glEnd();
			glPopMatrix();

			/*glPushMatrix();
			glTranslatef(bPos.x, bPos.y, bPos.z);
				glVertex3f(0, 5, 0);
				glVertex3f(0, 5, 0);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(lPos.x, lPos.y, lPos.z);
				glVertex3f(0, -5, 0);
			glPopMatrix();
			glVertex3f(0, 10, 0);*/


			glPopMatrix();
		}
	}
};
