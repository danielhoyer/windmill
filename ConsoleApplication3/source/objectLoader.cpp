#include "../include/main.h"

/*
	set objectLoader to initial position (0,0,-1) and rotate it -45°
	after that load object from .obj-file and load the texture found in the .mtl-file
*/
ObjectLoader::ObjectLoader(const string &path, const string &filename, point3 translate, point3 scale) 
	: hasVertices(true), hasNormals(false), hasTexCoords(false), hasPolygons(false), pos(0.0f, 0.0f, -1.0f), rotate(0.0f, -45.0f, 0.0f), fog(false)
{
	loadObjFile(path, filename, translate, scale);
	pic = new Texture();
	string imagePath = "Maps/" + this->actMtl.map_diffuse;
	textureIdDiffuse = pic->loadTexture(imagePath.c_str());
	imagePath = "Maps/" + this->actMtl.map_ambient;
	textureIdAmbient = pic->loadTexture(imagePath.c_str());
}

ObjectLoader::~ObjectLoader()
{
}

void ObjectLoader::render()
{
	glLoadIdentity();
	glPushMatrix();

	/*
		move object if any movemnet-method was invoke
	*/
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(rotate.y, 0, 1, 0);
	glRotatef(rotate.x, 1, 0, 0);
	glRotatef(rotate.z, 0, 0, 1);
	
	/*
		default-color
	*/
	glColor3f (1.0f, 1.0f, 1.0f);
	
	/*  calclate offsets in Vertex Buffer Object
		vertices-offsets:
	*/
	void *offsetVertex = (void *) 0;
	void *offsetNormal = (void *) (sizeof(point3));
	void *offsetTexCoord = (void *) (2* sizeof(point3));
//	void *offsetColor = (void *) (2 * sizeof(point3) + sizeof(point2));				//offset to material.color

	GLint stride = sizeof (vertex);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	/*
		Enable Object-Buffers
	*/
	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_NORMAL_ARRAY);
//	glEnableClientState (GL_COLOR_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY);

	glVertexPointer (3, GL_FLOAT, stride, offsetVertex);
	glNormalPointer (GL_FLOAT, stride, offsetNormal);
	glTexCoordPointer (3, GL_FLOAT, stride, offsetTexCoord);
//	glColorPointer (3, GL_FLOAT, stride, offsetColor);

	//Load materials:
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (float *) &this->actMtl.ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (float *) &this->actMtl.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (float *) &this->actMtl.specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, (float *) &this->actMtl.emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, (float *) &this->actMtl.shininess);

	/*
		Fog:
	*/
	if (fog)
	{
		glEnable(GL_FOG);
		glFogf(GL_FOG_DENSITY, 0.5f);
	}
	else
	{
		glDisable(GL_FOG);
	}
	
	glBindTexture(GL_TEXTURE_2D, this->textureIdDiffuse);
	glBindTexture(GL_TEXTURE_2D, this->textureIdAmbient);
	
	glShadeModel(GL_SMOOTH);
	/*
		if object has polygons, render as GL_QUAD
		else render as GL_TRIANGLE
	*/
	if(hasPolygons)
	{
		glDrawArrays (GL_QUADS, 0, numberOfElements);
	}
	else
	{
		glDrawArrays (GL_TRIANGLES, 0, numberOfElements);
	}

	glDisableClientState (GL_VERTEX_ARRAY);
	glDisableClientState (GL_NORMAL_ARRAY);
	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
//	glDisableClientState (GL_COLOR_ARRAY);
	glPopMatrix();
}

/*
	Move object in x- and y-axis
	Also move camera and lightning
*/
void ObjectLoader::Move(float dx, float dy)
{
		pos.x += dx;
		pos.y -= dy;
}

/*
	Zoom object in z-axis
	Also move camera and lightning
*/
void ObjectLoader::Zoom(float d)
{
		pos.x += d * pos.x;
		pos.y += d * pos.y;
		pos.z -= d * pos.z;
}

/*
	Rotate object in x-axis
	Also move camera and lightning
*/
void ObjectLoader::RotateX(float dx)
{
	rotate.x += dx;
}

/*
	Rotate object in y-axis
	Also move camera and lightning
*/
void ObjectLoader::RotateY(float dy)
{
	rotate.y += dy;
}

/*
	de-/activate fog
*/
void ObjectLoader::setFog(bool onOff) 
{
	this->fog = onOff;
}

/*
	get actual fog-state
*/
bool ObjectLoader::getFog()
{
	return this->fog;
}

point3 ObjectLoader::getPosition()
{
	return this->pos;
}

point3 ObjectLoader::getRotation()
{
	return this->rotate;
}

//private Methods:
/*
	Optional: 
		Change into working-directory that conatains .obj-file
	- Open .obj-file and load it into a stream-object
	- After that stream-object is iterated linewise
	- First string in line is copied into a new string as lineType
	- For each line type the data in line is copied into structs to store them temporary
	- At the end the createVBO-method is called
*/
bool ObjectLoader::loadObjFile(const string &path, const string &filename, point3 translate, point3 scale)
{
	//Optional: Change to .obj-file path:
	if(path.length() != 0)
	{
		_chdir(path.c_str());
		char currentPath[4096];
		_getcwd(currentPath, sizeof currentPath);
		cout << "Changed into directory: " << currentPath <<endl;
	}

	/*
		read file line-wise from:
		http://namespace-cpp.de/std/doku.php/howto/text_by_line
	*/
	//Open file:
	ifstream file(filename); 
	if(!file)
	{
		cout << "Error:\tFile \"" << filename << "\" not found!!!" << endl;
		return false;
	}

	//Read file line-wise:
	cout << "Reading file \"" << filename << "\" line-wise" << endl;
	string line;

	while (getline(file, line))
    {
		string mtlFilename;

		if(line.size() <= 1)	//Line-Break etc.
		{
			continue;
		}

		stringstream stream(line);
		string lineType;
		stream >> lineType;
		if(lineType == "mtllib")
		{
			stream >> mtlFilename;
			readMtlFile(mtlFilename);
		}
		else if(lineType == "v") //Vertex
		{
			point3 newVertex;
			stream >> newVertex.x >> newVertex.y >> newVertex.z;
			vertices.push_back(newVertex);
		}
		else if(lineType == "vn") //Normal
		{
			point3 newNormal;
			stream >> newNormal.x >> newNormal.y >> newNormal.z;
			normals.push_back(newNormal);
		}
		else if(lineType == "vt") //TexCoord
		{
			point2 newTexCoord;
			stream >> newTexCoord.x >> newTexCoord.y;
			texCoords.push_back(newTexCoord);
		}
		else if(lineType == "usemtl")	//Material
		{
			string mtlName;
			stream >> mtlName;
			for (size_t i = 0; i < mtls.size(); i++)
			{
				if(mtlName == mtls.at(i).mtlName)
				{
					actMtl = mtls.at(i);		// set actual material for this object
					break;
				}
			}
		}
		else if (lineType == "f")  //Face
		{
			string  pStr1, pStr2, pStr3, pStr4;
			stream >> pStr1 >> pStr2 >> pStr3 >> pStr4;		//part-String faces
			int slashesInLine = countSlashesInLine(line);	//slashesInLine/2 --> gives form of the face --> 3 = triangle, 4 = square, and so on

			vector<string> validate;						//Validate hasVertices, hasNormals and hasTexCoords first time 
			splitString(pStr1, &validate);
			validateHasNormalsAndTexCoords(&validate);
			
			vector<string> partStrings;						//[V1,Vn1,Vt1,V2,Vn2,Vt2,...]
			if(slashesInLine/2 == 1)						//Triangle without normals
			{
				hasNormals = false;							//rewrite hasNormals if only 2 slahes
				hasPolygons = false;						//to render triangles

				splitString(pStr1, &partStrings);
				splitString(pStr2, &partStrings);
				splitString(pStr3, &partStrings);

				triangleFace newTriangleFace;
				newTriangleFace.v1 = stoi(partStrings.at(0)) - 1;
				newTriangleFace.t1 = stoi(partStrings.at(1)) - 1;
				newTriangleFace.v2 = stoi(partStrings.at(2)) - 1;
				newTriangleFace.t2 = stoi(partStrings.at(3)) - 1;
				newTriangleFace.v3 = stoi(partStrings.at(4)) - 1;
				newTriangleFace.t3 = stoi(partStrings.at(5)) - 1;
				
				newTriangleFace.mtl = actMtl;				//notice material belonging to this triangle
				
				triangleFaces.push_back(newTriangleFace);
			}
			if(slashesInLine/2 == 2)						//Square without normals
			{
				hasNormals = false;							//rewrite hasNormals if only 2 slahes
				hasPolygons = true;							//to render polygons

				splitString(pStr1, &partStrings);
				splitString(pStr2, &partStrings);
				splitString(pStr3, &partStrings);
				splitString(pStr4, &partStrings);

				squareFace newSquareFace;
				newSquareFace.v1 = stoi(partStrings.at(0)) - 1;
				newSquareFace.t1 = stoi(partStrings.at(1)) - 1;
				newSquareFace.v2 = stoi(partStrings.at(2)) - 1;
				newSquareFace.t2 = stoi(partStrings.at(3)) - 1;
				newSquareFace.v3 = stoi(partStrings.at(4)) - 1;
				newSquareFace.t3 = stoi(partStrings.at(5)) - 1;
				newSquareFace.v4 = stoi(partStrings.at(6)) - 1;
				newSquareFace.t4 = stoi(partStrings.at(7)) - 1;

				newSquareFace.mtl = actMtl;					//notice material belonging to this square
				
				squareFaces.push_back(newSquareFace);
			}
			if(slashesInLine/2 == 3)						//Triangle
			{
				hasPolygons = false;

				splitString(pStr1, &partStrings);
				splitString(pStr2, &partStrings);
				splitString(pStr3, &partStrings);
				
				triangleFace newTriangleFace;
				newTriangleFace.v1 = stoi(partStrings.at(0)) - 1;
				newTriangleFace.t1 = stoi(partStrings.at(1)) - 1;
				newTriangleFace.n1 = stoi(partStrings.at(2)) - 1;
				newTriangleFace.v2 = stoi(partStrings.at(3)) - 1;
				newTriangleFace.t2 = stoi(partStrings.at(4)) - 1;
				newTriangleFace.n2 = stoi(partStrings.at(5)) - 1;
				newTriangleFace.v3 = stoi(partStrings.at(6)) - 1;
				newTriangleFace.t3 = stoi(partStrings.at(7)) - 1;
				newTriangleFace.n3 = stoi(partStrings.at(8)) - 1;

				newTriangleFace.mtl = actMtl;				//notice material belonging to this triangle

				triangleFaces.push_back(newTriangleFace);
			}
			else if(slashesInLine/2 == 4)					//Square
			{
				hasPolygons = true;

				splitString(pStr1, &partStrings);
				splitString(pStr2, &partStrings);
				splitString(pStr3, &partStrings);
				splitString(pStr4, &partStrings);

				squareFace newSquareFace;
				newSquareFace.v1 = stoi(partStrings.at(0)) - 1;
				newSquareFace.t1 = stoi(partStrings.at(1)) - 1;
				newSquareFace.n1 = stoi(partStrings.at(2)) - 1;
				newSquareFace.v2 = stoi(partStrings.at(3)) - 1;
				newSquareFace.t2 = stoi(partStrings.at(4)) - 1;
				newSquareFace.n2 = stoi(partStrings.at(5)) - 1;
				newSquareFace.v3 = stoi(partStrings.at(6)) - 1;
				newSquareFace.t3 = stoi(partStrings.at(7)) - 1;
				newSquareFace.n3 = stoi(partStrings.at(8)) - 1;
				newSquareFace.v4 = stoi(partStrings.at(9)) - 1;
				newSquareFace.t4 = stoi(partStrings.at(10)) - 1;
				newSquareFace.n4 = stoi(partStrings.at(11)) - 1;

				newSquareFace.mtl = actMtl;					//notice material belonging to this square

				squareFaces.push_back(newSquareFace);
			}
		}
		else  //ignore anything else!!!
		{
			continue;
		}
	}
	createVbo(translate, scale);
	return true;
}

/*
	Translate and Scale were copied from the sample project of Norbert Kohlmüller
	(Email-Attachment from 18.04.2017)
*/
void ObjectLoader::createVbo(point3 translate, point3 scale)
{
	cout << "Create Vertex-Buffer-Object" << endl;
	cout << "hasVertices =\t" << hasVertices << endl 
		<< "hasTexCoords =\t" << hasTexCoords << endl
		<< "hasNormals =\t"<< hasNormals << endl;
	/*
		Calculate number of triangles and quads to create an array with the size
		for needed triangles and quads
	*/
	int numberOfTriangleVertices = (int) (3 * triangleFaces.size());
	int numberOfTriangleFaces    = (int) triangleFaces.size();
	int numberOfSquareVertices = (int) (4 * squareFaces.size());
	int numberOfSquareFaces    = (int) squareFaces.size();
	vertex *vboData = new vertex[numberOfTriangleVertices + numberOfSquareVertices];

	/*
		iterate through triangles and write them into VBO
	*/
	for (int i = 0; i < numberOfTriangleFaces; i++)	//Triangle
	{
		int v1 = triangleFaces[i].v1;
		int v2 = triangleFaces[i].v2;
		int v3 = triangleFaces[i].v3;
		int n1 = triangleFaces[i].n1;
		int n2 = triangleFaces[i].n2;
		int n3 = triangleFaces[i].n3;
		int t1 = triangleFaces[i].t1;
		int t2 = triangleFaces[i].t2;
		int t3 = triangleFaces[i].t3;

		if (hasVertices)
		{
			vboData[3*i].coord.x = (vertices[v1].x - translate.x) * scale.x;
			vboData[3*i].coord.y = (vertices[v1].y - translate.y) * scale.y;
			vboData[3*i].coord.z = (vertices[v1].z - translate.z) * scale.z;
			vboData[3*i+1].coord.x = (vertices[v2].x - translate.x) * scale.x;
			vboData[3*i+1].coord.y = (vertices[v2].y - translate.y) * scale.y;
			vboData[3*i+1].coord.z = (vertices[v2].z - translate.z) * scale.z;
			vboData[3*i+2].coord.x = (vertices[v3].x - translate.x) * scale.x;
			vboData[3*i+2].coord.y = (vertices[v3].y - translate.y) * scale.y;
			vboData[3*i+2].coord.z = (vertices[v3].z - translate.z) * scale.z;
		}
		if (hasNormals)
		{
			vboData[3*i].normal = normals[n1];
			vboData[3*i+1].normal = normals[n2];
			vboData[3*i+2].normal = normals[n3];
		}
		if (hasTexCoords)
		{
			vboData[3*i].texcoord = texCoords[t1];
			vboData[3*i+1].texcoord = texCoords[t2];
			vboData[3*i+2].texcoord = texCoords[t3];
		}
		// write material into VBO
		vboData[3*i].mtl = triangleFaces[i].mtl;
		vboData[3*i+1].mtl = triangleFaces[i].mtl;
		vboData[3*i+2].mtl = triangleFaces[i].mtl;
	}

	/*
		iterate through quads and write them into VBO
	*/
	for (int i = 0; i < numberOfSquareFaces; i++)	//Square
	{
		int v1 = squareFaces[i].v1;
		int v2 = squareFaces[i].v2;
		int v3 = squareFaces[i].v3;
		int v4 = squareFaces[i].v4;
		int n1 = squareFaces[i].n1;
		int n2 = squareFaces[i].n2;
		int n3 = squareFaces[i].n3;
		int n4 = squareFaces[i].n4;
		int t1 = squareFaces[i].t1;
		int t2 = squareFaces[i].t2;
		int t3 = squareFaces[i].t3;
		int t4 = squareFaces[i].t4;

		if (hasVertices)
		{
			vboData[3*i].coord.x = (vertices[v1].x - translate.x) * scale.x;
			vboData[3*i].coord.y = (vertices[v1].y - translate.y) * scale.y;
			vboData[3*i].coord.z = (vertices[v1].z - translate.z) * scale.z;
			vboData[3*i+1].coord.x = (vertices[v2].x - translate.x) * scale.x;
			vboData[3*i+1].coord.y = (vertices[v2].y - translate.y) * scale.y;
			vboData[3*i+1].coord.z = (vertices[v2].z - translate.z) * scale.z;
			vboData[3*i+2].coord.x = (vertices[v3].x - translate.x) * scale.x;
			vboData[3*i+2].coord.y = (vertices[v3].y - translate.y) * scale.y;
			vboData[3*i+2].coord.z = (vertices[v3].z - translate.z) * scale.z;
			vboData[3*i+3].coord.x = (vertices[v4].x - translate.x) * scale.x;
			vboData[3*i+3].coord.y = (vertices[v4].y - translate.y) * scale.y;
			vboData[3*i+3].coord.z = (vertices[v4].z - translate.z) * scale.z;
		}
		if (hasNormals)
		{
			vboData[3*i].normal = normals[n1];
			vboData[3*i+1].normal = normals[n2];
			vboData[3*i+2].normal = normals[n3];
			vboData[3*i+3].normal = normals[n4];
		}
		if (hasTexCoords)
		{
			vboData[3*i].texcoord = texCoords[t1];
			vboData[3*i+1].texcoord = texCoords[t2];
			vboData[3*i+2].texcoord = texCoords[t3];
			vboData[3*i+3].texcoord = texCoords[t4];
		}
		// write material into VBO
		vboData[3*i].mtl = triangleFaces[i].mtl;
		vboData[3*i+1].mtl = triangleFaces[i].mtl;
		vboData[3*i+2].mtl = triangleFaces[i].mtl;
		vboData[3*i+3].mtl = triangleFaces[i].mtl;
	}
	if(!hasNormals)
	{
		getNormals(vboData, numberOfTriangleVertices, numberOfTriangleFaces);
		hasNormals = true;
	}

	// Print number of elements
	numberOfElements = numberOfTriangleVertices + numberOfSquareVertices;
	cout << numberOfElements << "Elements loaded into VBO" << endl;
	
	//Create VBO
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, numberOfElements * sizeof(vertex), vboData, GL_STATIC_DRAW);
}

/*
	returns number of slashes in a line
	--> necessary to decide if a line with lineType 'f' is 
		a triangle or quad 
*/
int ObjectLoader::countSlashesInLine(string line)
{
	int counter = 0;
	for(int i = line.length(); i > 0; i--)
	{
		if(line.at(i-1) == '/')
		{
			counter++;
		}
	}
	return counter;
}

/*
	validate if the object has vertices, texture Coords and normals
*/
void ObjectLoader::validateHasNormalsAndTexCoords(vector<string> *partStrings)
{
	partStrings[0].empty() ? hasVertices = false : hasVertices = true;
	partStrings[1].empty() ? hasTexCoords = false : hasTexCoords = true;
	partStrings[2].empty() ? hasNormals = false : hasNormals = true;
}

/*
	split a string in line with lineType 'f'
	split string if character '/' was read
*/
void ObjectLoader::splitString(string stringToSplit, vector<string> *partStrings)
{
	istringstream sts(stringToSplit);
	string token;
	/*
		How to seperate stringstreams idea from:
		http://stackoverflow.com/questions/11719538/how-to-use-stringstream-to-separate-comma-separated-strings 
	*/
	while(getline(sts, token, '/'))
	{
		if(token == "")
		{
			partStrings->push_back("-1");
		}
		else
		{
			partStrings->push_back(token);
		}
	}
}

/*
	Read all material-files and store them in a material struct:
	- Open .mtl-file and load it into a stream-object
	- After that stream-object is iterated linewise
	- First string in line is copied into a new string as lineType
	- For each line type the data in line is copied into a material struct to store them temporary
	- While reading newMaterial all data was read
		--> the material-structs are caopied into a vector of materials
*/
void ObjectLoader::readMtlFile(string mtlFilename)
{
	/*
		read file line-wise from:
		http://namespace-cpp.de/std/doku.php/howto/text_by_line
	*/

	//Open file:
	ifstream file(mtlFilename); 
	if(!file)
	{
		cout << "Error:\tFile \"" << mtlFilename << "\" not found!!!" << endl;
		return;
	}

	//Read file line-wise:
	cout << "Reading file \"" << mtlFilename << "\" line-wise" << endl;

	string line;
	bool isFirstMtl = true;

	material newMtl;
	while (getline(file, line))
	{
		
		if(line.size() <= 1)	//Line-Break etc.
		{
			continue;
		}

		stringstream stream(line);
		string lineType;
		stream >> lineType;

		if(lineType == "newmtl")
		{
			if(isFirstMtl)
			{
				isFirstMtl = false;
			}
			else
			{
				newMtl.color.a = newMtl.ambient.a + newMtl.diffuse.a + newMtl.specular.a;
				newMtl.color.b = newMtl.ambient.b + newMtl.diffuse.b + newMtl.specular.b;
				newMtl.color.g = newMtl.ambient.g + newMtl.diffuse.g + newMtl.specular.g;
				newMtl.color.r = newMtl.ambient.r + newMtl.diffuse.r + newMtl.specular.r;
				mtls.push_back(newMtl);
			}
			stream >> newMtl.mtlName;
		}else if(lineType == "Ns")
		{
			stream >> newMtl.shininess;
		}else if(lineType == "Ka")
		{
			stream >> newMtl.ambient.r >> newMtl.ambient.g >> newMtl.ambient.b >> newMtl.ambient.a;
		}
		else if(lineType == "Kd")
		{
			stream >> newMtl.diffuse.r >> newMtl.diffuse.g >> newMtl.diffuse.b >> newMtl.diffuse.a;
		}
		else if(lineType == "Ks")
		{
			stream >> newMtl.specular.r >> newMtl.specular.g >> newMtl.specular.b >> newMtl.specular.a;
		}
		else if(lineType == "Ke")
		{
			stream >> newMtl.emission.r >> newMtl.emission.g >> newMtl.emission.b >> newMtl.emission.a;
		}
		else if(lineType == "map_Ka")
		{
			stream >> newMtl.map_ambient;
		}
		else if(lineType == "map_Kd")
		{
			stream >> newMtl.map_diffuse;
		}
		else
		{
			continue;
		}
	}
	mtls.push_back(newMtl);	//last material
}

void ObjectLoader::getNormals(vertex *vertices, int numVertices, int numFaces)
{
	point3 *faceNormals = new point3[numFaces];

	for (int i = 0; i < numFaces; i++)
	{
		point3 v1 = vertices[3*i+1].coord - vertices[3*i].coord;
		point3 v2 = vertices[3*i+2].coord - vertices[3*i+1].coord;
		faceNormals[i] = cross (v1, v2);
	}
	for (int i = 0; i < numFaces; i++)
	{
		vertices[3*i].normal = vertices[3*i].normal + faceNormals[i];
		vertices[3*i+1].normal = vertices[3*i+1].normal + faceNormals[i];
		vertices[3*i+2].normal = vertices[3*i+2].normal + faceNormals[i];
	}
	for (int i = 0; i < numVertices; i++)
	{
		vertices[i].normal = normalize(vertices[i].normal);
	}
}

