using namespace std;

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Change working directory
#include <direct.h>
#include <Windows.h>

// Additional Header-Files:
#include "common.h"
#include "window.h"
#include "texture.h"
#include "objectLoader.h"
#include "animation.h"


// global functions
void info ();
int  main (int argc, char **argv);

// glut event handlers
void display  ();
void keyDown  (unsigned char key, int x, int y);
void skeyDown (int key, int x, int y);

void mouse(int button, int state, int x, int y);
void motion(int x, int y);

// Instance for top-level-window
extern Window *window;
