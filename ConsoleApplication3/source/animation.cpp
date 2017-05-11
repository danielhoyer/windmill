#include "../include/main.h"

/*
	constructor --> set animated object
*/
Animation::Animation(ObjectLoader *object) : run(false), speed(-0.1f), yPos(0.0f)
{
	this->object = object;
}

/*
	deconstructor
*/
Animation::~Animation()
{

}

/*
	one animation step --> rotate windmill-windgs around x-axis
	==> wings can only rotate into one directory (from where wind
	comes an turns them) --> uses objectsLoader's RotateX-method
*/
void Animation::Step(vector<ObjectLoader*> scene)
{
	if (run && speed < 0) 
	{
		if(speed < -2.0f)
		{
			yPos += 0.01f;
			for (size_t i = 0; i < scene.size(); i++)
			{
				scene[i]->Move(0.0f, -0.01f);
				speed = -2.0f;
			}
		}
		if(speed > -2.0f && yPos > 0.0f)
		{
			yPos -= 0.01f;
			for (size_t i = 0; i < scene.size(); i++)
			{
				scene[i]->Move(0.0f, +0.01f);
				speed = -2.0f;
			}
		}
		this->object->RotateX(speed);
	}
}

/*
	speed of rotation can be changed --> accelerate or decelerate
	==> wings can only rotate into one directory (from where wind
	comes an turns them) --> speed can't get greater than 0
*/
void Animation::changeSpeed(float accelerateValue)
{
	if (speed - accelerateValue > 0)
	{
		speed = 0;
		return;
	}
	speed -= accelerateValue;
}

float Animation::getYPos()
{
	return this->yPos;
}

void Animation::resetWings()
{
	this->yPos = 0.0f;
	this->speed = -0.1f;
}