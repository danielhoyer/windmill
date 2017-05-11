/*
	Window class:
	copied from example project of Dr. Norbert Kohlmüller
*/
class Window
{
  private:
    // window parameters
    int  id;
    int  width;
    int  height;
    bool fullScreen;
	
	// mouse handler
	int xmouse, ymouse;
	bool lbutton, rbutton;

  public:
    Window (const string &title);

    // event handler
    void Display ();
	// keyboard events
    void KeyDown (unsigned char key, int x, int y);
    void KeyDown (int key, int x, int y);
	// move, zoom and rotate objects
	void Mouse(int button, int state, int x, int y);
	void Motion(int x, int y);
	// scale to window-size
	void Reshape(int width, int height);
	// timer for animation
	void Timer();
	
    // toggle fullscreen mode
    void Toggle ();

	//Render Windmill-objects:
	void RenderScene();
	//Render Background-Image:
	void RenderBackground();
};
