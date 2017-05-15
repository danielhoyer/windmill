#include "../include/main.h"
// route to fly
point3 p1(0.2f, -0.1f, 0.1f);
point3 p2(-0.2f, -0.1f, 0.1f);
point3 p3(-0.2f, 0.1f, -0.1f);
point3 p4(0.0f, 0.2f, -0.1f);
point3 p5(0.2f, 0.1f, -0.1f);
point3 p6(0.2f, -0.1f, 0.1f);
point3 p7(-0.2f, -0.1f, 0.0f);
vector<point3> route;

/*
	constructor --> set animated object
*/
Animation::Animation(ObjectLoader *object) : run(false), speed(-0.1f), animationStep(0), routeItem(0), animationRunning(false)
{
	this->object = object;
	route.push_back(p1);
	route.push_back(p2);
	route.push_back(p3);
	route.push_back(p4);
	route.push_back(p5);
	route.push_back(p6);
	route.push_back(p7);
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
		if(speed < -2.0f && animationStep == 0) // start flying
		{
			animationRunning = true;
			routeItem = 0;
			animationStep = route.size() * 1000;
			for (size_t i = 0; i < scene.size(); i++)
			{	
				scene[i]->Move(route[routeItem].x / 1000, route[routeItem].y / 1000);
				scene[i]->Zoom(route[routeItem].z / 1000);
				speed = -2.0f;
			}
			animationStep--;
			/*yPos += 0.01f;
			for (size_t i = 0; i < scene.size(); i++)
			{
				scene[i]->Move(0.0f, -0.01f);
				speed = -2.0f;
			}*/
		}else if (animationStep > 0)
		{
			if (animationStep % 1000 == 0)		// next route entry
			{
				routeItem++;
			}
			if (animationStep < 1000)
			{
				speed += 0.002f;					//slow down in last step
			}
			for (size_t i = 0; i < scene.size(); i++)
			{
				scene[i]->Move(route[routeItem].x / 1000, route[routeItem].y / 1000);
				scene[i]->Zoom(route[routeItem].z / 1000);
			}
			animationStep--;
			if (animationStep <= 0)		//reset windmill after animation
			{
				animationRunning = false;
				routeItem = 0;
				speed = -0.1f;
				run = !run;
			}
		}
		/*if(speed > -2.0f && yPos > 0.0f)  //landing
		{
			yPos -= 0.01f;
			for (size_t i = 0; i < scene.size(); i++)
			{
				scene[i]->Move(0.0f, +0.01f);
				speed = -2.0f;
			}
		}*/
		this->object->RotateX(speed);	// rotate wings
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

bool Animation::isAnimationRunning()
{
	return this->animationRunning;
}

//float Animation::getYPos()
//{
//	return this->yPos;
//}
//
//void Animation::resetWings()
//{
//	this->yPos = 0.0f;
//	this->speed = -0.1f;
//}