/*
	common.h:
		copied from example project of Dr. Norbert Kohlmüller
	Adjusted:
		- Deleted some structs
		- Added structs for quad-faces
		- Added structs for material
		- Added material to faces
*/

#define PI 3.141592654f
#define PATH_RESOURCES  string ("Resources/")

// timer
#define TIMER_MSECS 1
#define SECS_PER_DAY (24 * 60 * 60)
#define SECS(H,M)    (((H) * 60 + (M)) * 60)
#define START_DUSK   SECS(17,30)
#define END_DUSK     SECS(18,30)
#define START_DAWN   SECS(5,30)
#define END_DAWN     SECS(6,30)

// init window position & size
#define INIT_X 10
#define INIT_Y 10
#define INIT_WIDTH  1300
#define INIT_HEIGHT  1000

// structures for points, colors & material
struct point2
{
  float x, y;
  point2 (float x = 0, float y = 0) {this->x = x; this->y = y;}
};

struct point3
{
  float x, y, z;
  point3 (float x = 0, float y = 0, float z = 0) {this->x = x; this->y = y; this->z = z;}
  float length () const {return (float) sqrt (x * x + y * y + z * z);}
};

inline point3 operator+ (const point3 &pt1, const point3 &pt2)
{
  return point3 (pt1.x + pt2.x, pt1.y + pt2.y, pt1.z + pt2.z);
}

inline point3 operator- (const point3 &pt1, const point3 &pt2)
{
  return point3 (pt1.x - pt2.x, pt1.y - pt2.y, pt1.z - pt2.z);
}

inline point3 normalize (const point3 &pt) 
{
  float len = pt.length ();
  return (len == 0) ?  point3 () : point3 (pt.x / len, pt.y / len, pt.z / len);
}

inline point3 cross (const point3 &pt1, const point3 &pt2)
{
  point3 p (pt1.y * pt2.z - pt2.y * pt1.z, pt1.z * pt2.x - pt2.z * pt1.x, pt1.x * pt2.y - pt2.x * pt1.y);
  return p;
}

struct color4
{
  float r, g, b, a;
  color4 (float r = 0, float g = 0, float b = 0, float a = 1) {this->r = r; this->g = g; this->b = b; this->a = a;}
};

struct material
{
	color4 color, ambient, diffuse, specular, emission;
	float shininess;
	string mtlName, map_ambient, map_diffuse;
};

// structures for vertices, triangles and quads
struct vertex
{
  point3 coord;
  point3 normal;
  point2 texcoord;
  material mtl;
};

struct triangleFace
{
  int v1, v2, v3;
  int n1, n2, n3;
  int t1, t2, t3;
  material mtl;
};

struct squareFace
{
  int v1, v2, v3, v4;
  int n1, n2, n3, n4;
  int t1, t2, t3, t4;
  material mtl;
};
