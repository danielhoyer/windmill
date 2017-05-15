/*
	Animation:
	holds an animated object, a state to start/stop animation 
	and a state to change the animation's speed
*/

class Animation
{
  private:
	// animated Object (= windmill-wings)
	ObjectLoader *object;
	// state
    bool run;
	// actual animation speed
	float speed;
	//// store y-movement while flying
	//float yPos;

	//float xPos, zPos;
	bool animationRunning;
	int animationStep, routeItem;



  public:
    Animation (ObjectLoader *object);
    ~Animation ();

	// start/stop animation --> change state
    void StartStop () {run = !run;}
	// one animation step
	void Step  (vector<ObjectLoader*> scene);
	// change actual animation speed
	void changeSpeed(float accelerateValue);
	// check if flying windmill animation is actually running
	bool isAnimationRunning();
	//// returns flight height
	//float getYPos();
	//// set yPos = 0
	//void resetWings();
};
