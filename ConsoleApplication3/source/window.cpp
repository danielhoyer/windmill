#include "../include/main.h"

// scene-objects:
vector<ObjectLoader *> scene;
// animation-object = windmill wings:
Animation *animatedWings;
// background picture:
Texture *background;
GLuint backgroundId;

/*
	Initialize window with default-values from common.h
	Set initial camera-position to z = 1
	Load all .obj-files and the background-image
*/

Window::Window (const string &title) : width(INIT_WIDTH), height(INIT_HEIGHT), fullScreen(false), lbutton(false), rbutton(false)
{ 
  // set defaults
  glutInitWindowPosition (INIT_X, INIT_Y);
  glutInitWindowSize  (INIT_WIDTH, INIT_HEIGHT);
  glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH);

  // create top-level window
  id = glutCreateWindow (title.c_str ());

  // use GLEW-Library for extension
  if (glewInit () != GLEW_OK) 
    throw string ("GLEW init failed");

  // set background color
  glClearColor (0.0f, 0.0f, 0.0f, 1);

  // enable Z-Buffer-Test
  glEnable (GL_DEPTH_TEST);

  // enable Backface-Culling
  glFrontFace(GL_CCW);													//anti-clockwise
  glEnable(GL_CULL_FACE);													

  // set alpha-blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // edge smoothing
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); 

  // Texture
  glEnable(GL_TEXTURE_2D);

  // Fog
  color4 fogColor(0.8f, 0.8f, 0.8f, 1);
  glFogfv(GL_FOG_COLOR, (float*) &fogColor);
  glFogi(GL_FOG_MODE, GL_EXP);

  // Lightning
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE , GL_SPECULAR);

  // Material
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);

  // Texture
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
  // to fullscreen
  Toggle();


  //load Windmill:
  scene.push_back(new ObjectLoader("Windmill", "windmill_03_haus_01.obj", point3(0, 475.0f, 120.0f), point3(0.0001f, 0.0001f, 0.0001f)));
  scene.push_back(new ObjectLoader("Windmill", "windmill_03_rad.obj", point3(0, 0, 0), point3(0.0001f, 0.0001f, 0.0001f)));
  scene.push_back(new ObjectLoader("Windmill", "windmill_03_haus_02.obj", point3(0, 475.0f, 120.0f), point3(0.0001f, 0.0001f, 0.0001f)));
  scene.push_back(new ObjectLoader("Windmill", "windmill_03_haus_03.obj", point3(0, 475.0f, 120.0f), point3(0.0001f, 0.0001f, 0.0001f)));
  scene.push_back(new ObjectLoader("Windmill", "windmill_03_haus_04.obj", point3(0, 475.0f, 120.0f), point3(0.0001f, 0.0001f, 0.0001f)));
  scene.push_back(new ObjectLoader("Windmill", "windmill_03_haus_05.obj", point3(0, 475.0f, 120.0f), point3(0.0001f, 0.0001f, 0.0001f)));
  scene.push_back(new ObjectLoader("Windmill", "windmill_03_haus_06.obj", point3(0, 475.0f, 120.0f), point3(0.0001f, 0.0001f, 0.0001f)));
 
  if(scene.size() > 1)
  {
	animatedWings = new Animation(scene[1]);	//Animation object = windmill_03_rad.obj
  }
  //meadow-727179_1920.bmp = https://pixabay.com/de/wiese-himmel-schaf-gras-l%C3%A4ndlich-727179/
  background = new Texture();
  backgroundId = background->loadTexture("Maps/meadow-727179_1920.bmp");
}

/*
	Callback-function to render all loaded objects
*/
void Window::Display ()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	Lightning:
	- Spotlight at camera position
	- Spotlight with a 100° light cone
	- Spotlight directory in -z
    */
	GLfloat pos[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60);
    GLfloat dir[] = { 0.0, 0.0, -1.0 };
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
	float color[4]={1.0, 1.0,1.0,  1.0};
	glLightfv(GL_LIGHT0,GL_SPECULAR, color);
	RenderBackground();
	RenderScene();
	glFlush ();
}

/*
	Callback-function for keyboard input
*/
void Window::KeyDown (unsigned char key, int x, int y)
{
  switch (key)
  {
    case 27:
      exit (0);
    case 'r':
      Toggle ();
      break;
	case 'a':
		if(scene.size() > 1)
		{
			if(animatedWings->getYPos() > 0.0f)
			{
				for (size_t i = 0; i < scene.size(); i++)
				{
					scene[i]->Move(0.0f, animatedWings->getYPos());
				}
				animatedWings->resetWings();
			}
			animatedWings->StartStop();
		}
		else
		{
			cout << "Error: No existing object to animate!!!" << endl;
		}
		break;
	case 'f':
		for (size_t i = 0; i < scene.size(); i++)
		{
			scene[i]->setFog(!scene[i]->getFog());
			scene[i]->render();
		}
		break;
	case '+':
		animatedWings->changeSpeed(0.1f);
		break;
	case '-':
		animatedWings->changeSpeed(-0.1f);
		break;
  }
  glutPostRedisplay ();
}

/*
	Callback-function for keyboard input
*/
void Window::KeyDown (int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_F1:
	  info();
	  return;
  //Move Windmill with arrow keys:
  case GLUT_KEY_DOWN:
	    for (size_t i = 0; i < scene.size(); i++)
		{
			scene[i]->Move(0.0f, 0.01f);
		}
	    break;
  case GLUT_KEY_UP:
	    for (size_t i = 0; i < scene.size(); i++)
		{
			scene[i]->Move(0.0f, -0.01f);
		}
	    break;
  case GLUT_KEY_LEFT:
	    for (size_t i = 0; i < scene.size(); i++)
		{
			scene[i]->Move(-0.01f, 0.0f);
		}
	    break;
  case GLUT_KEY_RIGHT:
	    for (size_t i = 0; i < scene.size(); i++)
		{
			scene[i]->Move(0.01f, 0.0f);
		}
	    break;
  }
}

/*
	Callback-function for fullscreen-mode
*/
void Window::Toggle ()
{
  static int left    = INIT_X;
  static int top     = INIT_Y;
  static int width   = INIT_WIDTH;
  static int height  = INIT_HEIGHT;
  static int xborder = glutGet (GLUT_WINDOW_X) - INIT_X;
  static int yborder = glutGet (GLUT_WINDOW_Y) - INIT_Y;

  fullScreen = !fullScreen;
  if (fullScreen)
  {
    left   = glutGet (GLUT_WINDOW_X) - xborder;
    top    = glutGet (GLUT_WINDOW_Y) - yborder;
    width  = glutGet (GLUT_WINDOW_WIDTH);
    height = glutGet (GLUT_WINDOW_HEIGHT);
    glutFullScreen ();
	glutReshapeWindow  (width, height);
  }
  else
  {
    glutPositionWindow (left,  top);
    glutReshapeWindow  (width, height);
  }
}

/*
	Callback-function to render all loaded object files
*/
void Window::RenderScene()
{
	glClear( /*GL_COLOR_BUFFER_BIT |*/ GL_DEPTH_BUFFER_BIT);			//Nicht um Hintergrundbild darzustellen!!!
	//int frameSize = 10;		//Frame 10px around the display
	// set projection/modelview matrix
	
	// render scene
	for (size_t i = 0; i < scene.size(); i++)
	{
		scene[i]->render();
	}
}

/*
	Callback-function to render background-image
*/
void Window::RenderBackground() 
{
	//Fog:
	if (scene[0]->getFog())
	{
		glEnable(GL_FOG);
		glFogf(GL_FOG_DENSITY, 0.5f);
	}
	else
	{
		glDisable(GL_FOG);
	}
	//Draw background-pic:
	glLoadIdentity();
	glPushMatrix();
	// Textur-Mapping im Immediate Mode
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, backgroundId);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex3f(-1.0, -0.85, -1.0);

	glTexCoord2f(1, 0);
	glVertex3f(1.0, -0.85, -1.0);

	glTexCoord2f(1, 1);
	glVertex3f(1.0, 0.5, -1.0);

	glTexCoord2f(0, 1);
	glVertex3f(-1.0, 0.5, -1.0);

	glEnd();
	/*glRasterPos3i(-2.0, -2.0, -5.0);
	glDrawPixels(background->getWidth(), background->getHeight(), GL_BGR, GL_UNSIGNED_BYTE, background->getData());*/
	glPopMatrix();
}

/*
	Callback-function to read mouse-position
*/
void Window::Mouse(int button, int state, int x, int y)
{
	// save mouse position
	if (button == GLUT_LEFT_BUTTON)  lbutton = (state == GLUT_DOWN);
	if (button == GLUT_RIGHT_BUTTON) rbutton = (state == GLUT_DOWN);
	xmouse = x;
	ymouse = y;
}

/*
	Callback-function for mouse input
*/
void Window::Motion(int x, int y)
{
	float speedMove = 0.001f;
	float speedZoom = 0.01f;
	float speedRotate = 0.5f;
	float dx = (float)(x - xmouse);
	float dy = (float)(y - ymouse);

	// move camera left/right & up/down
	if (lbutton && rbutton) {
		for (size_t i = 0; i < scene.size(); i++)
		{
			scene[i]->Move(dx * speedMove, dy * speedMove);
		}
	}

	// move camera forward/backward
	else if (rbutton) {
		for (size_t i = 0; i < scene.size(); i++)
		{
			scene[i]->Zoom(dy * speedZoom);
		}
	}

	// rotate camera
	else if (lbutton) {
		for (size_t i = 0; i < scene.size(); i++)
		{
			scene[i]->RotateY(speedRotate * dx);
		}
	}

	xmouse = x;
	ymouse = y;
	glutPostRedisplay();
}


void Window::Reshape(int width, int height)
{
	int frame = 25;
	glViewport(frame, frame, (GLsizei)width - 2*frame, (GLsizei)height - 2*frame);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)width /(GLfloat)height, 0.1f, 6.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}

/*
	Callback-fuction for animation
*/
void Window::Timer()
{
	if(animatedWings)
	{
		animatedWings->Step(scene);
	}
	glutPostRedisplay();
}
