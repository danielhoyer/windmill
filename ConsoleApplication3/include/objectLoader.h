/*
	Class to load .obj-files:
	- constructor-invocation loads an .obj-file (optional in the given path)
		--> if there are material-infos in the .obj-file the .mtl-file of this
			object is loaded and stored in a material struct 
		--> After that a VBO is created out of the correct loaded vertices
	- render-invocation
		--> renders the VBO with the loaded material-information
	- move, zoom and rotate
		--> are for movements on the VBO
	- get & setFog
		--> are for fog activation
*/

class ObjectLoader
{
public:
	/* 
		- path (optional)-> to the .obj-file
		- filename		 -> filename.obj
		- translate		 -> offset for initial position	--> [copied from example project of Dr. Norbert Kohlmüller]
		- scale			 -> initial scale-factor		--> [copied from example project of Dr. Norbert Kohlmüller]
	*/
	ObjectLoader(const string &path, const string &filename, point3 translate, point3 scale);
	~ObjectLoader();

	// render VBO
	void render();

	// move VBO in x- and y-axis
	void Move(float dx, float dy);
	// zoom VBO in z-axis
	void Zoom(float dz);
	// rotate VBO around x-axis 
	void RotateX(float dx);
	// rotate VBO around y-axis
	void RotateY(float dy);
	
	// de-/activate fog
	void setFog(bool onOff);
	bool getFog();

	// position and rotation for animation
	point3 getPosition();
	point3 getRotation();

private:
	// read .obj-File storage:
	vector<point3> vertices;
	vector<point3> normals;
	vector<point2> texCoords;

	vector<triangleFace> triangleFaces;
	vector<squareFace> squareFaces;
	vector<material> mtls;
	material actMtl;
	GLuint textureIdDiffuse;
	GLuint textureIdAmbient;

	// states while reading .obj-files
	bool hasVertices;
	bool hasNormals;
	bool hasTexCoords;

	// state to render quads
	bool hasPolygons;

	// Vertex Buffer Object
	int numberOfElements;
	unsigned vbo;

	// Texture to load textureID for each ObjectLoader
	Texture *pic;

	// render loaded object --> actual position and rotation for movements
	point3 pos, rotate;

	// state for fog --> active or not
	bool fog;
	

	// private Methods:
	bool loadObjFile(const string &path, const string &filename, point3 translate, point3 scale);
	void createVbo(point3 translate, point3 scale);
	int countSlashesInLine(string line);
	void splitString(string stringToSplit, vector<string> *partStrings);
	void validateHasNormalsAndTexCoords(vector<string> *partStrings);
	void readMtlFile(string mtlFilename);
	// create normals
	void getNormals(vertex *vertices, int numVertices, int numFaces);
};
