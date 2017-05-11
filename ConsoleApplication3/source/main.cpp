#include "../include/main.h"

// Instance for top-level-window
Window *window;

// print help
void info ()
{
  string msg [] = {"r, fullscreen",
                   "left mouse button, rotate windmill",
				   "right mouse button, zoom windmill",
				   "left and right mouse button, move windmill",
				   "arrow keys, move windmill(without interpolation)",
				   "a, start/stop animation",
				   "f, enable/disable fog",
				   "+, accelerate windmill wings",
				   "-, decelerate windmill wings",
	               "ESC, Quit", 
	               "F1, help"};

  cout << "OpenGL-Version: " << glGetString (GL_VERSION) << endl;
  cout << "Key usage: " << endl;
  for (size_t i = 0; i < sizeof (msg) / sizeof (string); i++)
  {
    size_t pos = msg [i].find (',');
    cout << left << setw (6) << (msg [i].substr (0, pos) + ':') << msg [i].substr (pos+1) << endl;
  }
}

// glut event handler
void display ()
{
  window->Display ();
}

void keyDown (unsigned char key, int x, int y)
{
  window->KeyDown (key, x, y);
}

void skeyDown (int key, int x, int y)
{
  window->KeyDown (key, x, y);
}

void mouse (int button, int state, int x, int y)
{
  window->Mouse (button, state, x, y);
}

void motion (int x, int y)
{
  window->Motion (x, y);
}

void reshape(int width, int height)
{
	window->Reshape(width, height);
}

void timer(int value)
{
	window->Timer();
	glutTimerFunc(TIMER_MSECS, timer, 0);
}

// main-function
int main (int argc, char **argv)
{
  try
  {
    // init glut & create top-level window
    glutInit (&argc, argv);
    window = new Window ("Windmill - Adam Bickel, Daniel Hoyer, Manuel Horn");

    // register event-handler
    glutDisplayFunc  (display);
    glutKeyboardFunc (keyDown);
    glutSpecialFunc  (skeyDown);
	
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);				//Skalierung passt nicht, wenn aktiviert!!! Evtl Kamera in andere Richtung bewegen!!!
	glutTimerFunc(TIMER_MSECS, timer, 0);

    // start main loop
    glutMainLoop ();
  }
  catch (string s)
  {
    cout << "error: " << s << endl;
    cin.ignore ();
  }
  return 0;
}