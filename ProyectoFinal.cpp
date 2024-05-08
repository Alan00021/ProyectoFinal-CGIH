//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION
#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib,"irrKlang.lib")

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <iostream>

#include <glew.h>
#include <glfw3.h>
#include <random>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"
#include <chrono>


//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
Camera camera2;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture cristal;
Texture expendTexture;

//Sol
float movSol;
float movSolOffset;
int contSol;
float green;


//Objetos general
Model ciudadela;
Model bancaparque01;
Model barreraAccesoVehicular;
Model boteBasura01;
Model boteBasura02Amarillo;
Model boteBasura02Azul;
Model boteBasura02Cafe;
Model boteBasura02Verde;
Model boteBasura02;
Model cerca01;
Model cerca02;
Model cerca03;
Model cerca04;
Model cerca05;
Model cerca06;
Model cerca07;
Model cerca08;
Model lamparaPared;
Model mesa01;
Model mesa02;
Model mesaRestaurante;
Model posteLuz;
Model sombrilla;
Model topeEstacionamiento01;
Model topeEstacionamiento02;
Model topeEstacionamiento03;
Model museum;
Model focoM;

//Universo AC
Model arbolAC;
Model arbustoAC;
Model cedroAC;
Model fuenteAC;
Model mariposaAC;
Model mariquitaAC;
Model pelotaAC;
Model puenteAC;
Model entradaAC;
Model restauranteAC;
Model taquillaAC;
Model casaAC;
Model Canelita;

//Universo LT
Model arbol01LT;
Model arbol02LT;
Model arbol03LT;
Model arbol04LT;
Model arbusto01LT;
Model arbusto02LT;
Model arbusto03LT;
Model arbusto04LT;
Model arbusto05LT;
Model aveLT;
Model aveLTPata1;
Model aveLTPata2;
Model marvinLT;
Model marvinLTPI;
Model marvinLTPD;
Model naveLT;
Model estatuaMarvin;

//movimiento naveLT

float rotNx;
float rotNxOffset;
float movNx;
float movNxOffset;
float movNy;
float movNyOffset;
float movNz;
float movNzOffset;

bool avanza;
bool avanzax;
bool avanzay;
bool avanzaz;

//movimiento aveLT

float contAve;
float rotAve;
float rotAveP;
float rotAveOffset;
float rotAveP1x;
float rotAvexP1Offset;
float rotAveP2x;
float rotAvexP2Offset;

float movAvex;
float movAvexOffset;
float movAvey;
float movAveyOffset;

bool avanzaAve;

//movimiento MarvinLT

float rotMarvin;
float rotMarvinyOffset;
float rotMarvinyP;

float movMarvinx;
float movMarvinxOffset;
float movMarviny;
float movMarvinyOffset;
float movMarvinz;
float movMarvinzOffset;

bool avanzaMarvinx;
bool avanzaMarviny;
bool avanzaMarvinz;

float rotMarvinP1x;
float rotMarvinP1xOffset;
float rotMarvinP2x;
float rotMarvinP2xOffset;
int contMarvin;

//Movimiento naveGG
bool detenido;
bool step1;
bool step2;
bool step3;
bool step4;
bool step5;
bool step6;
bool step7;
bool step8;
float pos_x;
float pos_y;
float h_angleX;
float h_angleY;

//Movimiento Gema GG
bool detenidoG;
bool step1G;
float pos_xG;
float pos_yG;
float h_angleXG;
float h_angleYG;

//Movimiento Mariposa
float rotMarip;
float rotM;
float rotMaripOffset;
float movMarip;
float movMaripOffset;

bool avanzaM;

//Universo GG
Model Nave;
Model plantaLuz;
Model conejo;
Model capGemIzq;
Model capGemDer;
Model baseGem;
Model gem;
Model groot;

//Universo 31M


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
// luz direccional
//DirectionalLight mainLight1;						//
DirectionalLight mainLight2;						//
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 1, 2,
		3, 4, 5
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,

		-10.0f, 0.0f, -10.0f,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		-1.0f, -1.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		-1.0f, -1.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		-1.0f, -1.0f, -1.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		-1.0f, -1.0f, -1.0f,


	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 160, 12);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearMaquina()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.0f,   0.0f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,   0.5f,	0.6f,	0.0f,		0.0f,	0.0f,	-1.0f,	//1			CAra principal 
		0.5f,  0.5f,   0.5f,	0.6f,	1.0f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		   T
		0.5f, -0.5f,  0.5f,	    0.9f,    0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.95f,   0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.95f,   0.1f,		-1.0f,	0.0f,	0.0f,			//
		0.5f,  0.5f,  0.5f,	    0.95f,   0.1f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.9f,    0.0f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.95f,   0.0f,		0.0f,	0.0f,	1.0f,			//
		0.5f,  0.5f, -0.5f,		0.95f,   0.1f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.95f,   0.1f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		   T
		-0.5f, -0.5f,  -0.5f,	0.9f,    0.0f,		1.0f,	0.0f,	0.0f,	//Inferior Izq
		-0.5f, -0.5f,  0.5f,	0.95f,   0.0f,		1.0f,	0.0f,	0.0f,	//Inferior Der 		
		-0.5f,  0.5f,  0.5f,	0.95f,   0.1f,		1.0f,	0.0f,	0.0f,	//Superior Der 
		-0.5f,  0.5f,  -0.5f,	0.95f,   0.1f,		1.0f,	0.0f,	0.0f,	//Superior Izq 

		// bottom
		//x		y		z		S		  T
		-0.5f, -0.5f,  0.5f,	0.9f,    0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.95f,   0.0f,		0.0f,	1.0f,	0.0f,			//
		 0.5f,  -0.5f,  -0.5f,	0.95f,   0.1f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.95f,   0.1f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,	 0.5f,	  0.9f,    0.0f,	0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,	 0.5f,	  0.95f,   0.0f,	0.0f,	-1.0f,	0.0f,			///
		  0.5f, 0.5f,	-0.5f,	  0.95f,   0.1f,	0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,	-0.5f,	  0.95f,   0.1f,	0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();


	// start the sound engine with default parameters
	ISoundEngine* engine = createIrrKlangDevice();
	if (!engine)
		return 0; // error starting up the engine

	// play some sound stream, looped
	engine->play2D("ophelia.mp3", true);



	CreateObjects();
	CrearMaquina();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	//Avatar
	camera2 = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);


	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso1.tga");
	pisoTexture.LoadTextureA();
	cristal = Texture("Textures/cristal.tga");
	cristal.LoadTextureA();

	//objetos generales
	ciudadela = Model();
	ciudadela.LoadModel("Models/ciudadela-01.obj");
	restauranteAC = Model();
	restauranteAC.LoadModel("Models/restauranteP.obj");
	taquillaAC = Model();
	taquillaAC.LoadModel("Models/taquilla-AC.obj");
	bancaparque01 = Model();
	bancaparque01.LoadModel("Models/banca-parque-01.obj");
	barreraAccesoVehicular = Model();
	barreraAccesoVehicular.LoadModel("Models/barrera-accesoVehicular-01.obj");
	boteBasura01 = Model();
	boteBasura01.LoadModel("Models/bote-basura-01.obj");
	boteBasura02Amarillo = Model();
	boteBasura02Amarillo.LoadModel("Models/bote-basura-02-amarillo.obj");
	boteBasura02Azul = Model();////
	boteBasura02Azul.LoadModel("Models/bote-basura-02-azul.obj");
	boteBasura02Cafe = Model();
	boteBasura02Cafe.LoadModel("Models/bote-basura-02-cafe.obj");
	boteBasura02Verde = Model();
	boteBasura02Verde.LoadModel("Models/bote-basura-02-verde.obj");
	boteBasura02 = Model();
	boteBasura02.LoadModel("Models/bote-basura-02.obj");
	cerca01 = Model();
	cerca01.LoadModel("Models/cerca-01.obj");
	cerca02 = Model();
	cerca02.LoadModel("Models/cerca-02.obj");
	cerca03 = Model();
	cerca03.LoadModel("Models/cerca-03.obj");
	cerca04 = Model();
	cerca04.LoadModel("Models/cerca-04.obj");
	cerca05 = Model();
	cerca05.LoadModel("Models/cerca-05.obj");
	cerca06 = Model();
	cerca06.LoadModel("Models/cerca-06.obj");
	cerca07 = Model();
	cerca07.LoadModel("Models/cerca-07.obj");
	cerca08 = Model();
	cerca08.LoadModel("Models/cerca-08.obj");
	lamparaPared = Model();
	lamparaPared.LoadModel("Models/lampara-pared-01.obj");
	mesa01 = Model();
	mesa01.LoadModel("Models/mesa-01.obj");
	mesa02 = Model();
	mesa02.LoadModel("Models/mesa-02.obj");
	mesaRestaurante = Model();
	mesaRestaurante.LoadModel("Models/mesa-restaurante-01.obj");
	posteLuz = Model();
	posteLuz.LoadModel("Models/poste-luz-01.obj");
	sombrilla = Model();
	sombrilla.LoadModel("Models/sombrilla-01.obj");
	topeEstacionamiento01 = Model();
	topeEstacionamiento01.LoadModel("Models/tope-estacionamiento-01.obj");
	topeEstacionamiento02 = Model();
	topeEstacionamiento02.LoadModel("Models/tope-estacionamiento-02.obj");
	topeEstacionamiento03 = Model();
	topeEstacionamiento03.LoadModel("Models/tope-estacionamiento-03.obj");
	museum = Model();
	museum.LoadModel("Models/Museum.obj");
	focoM = Model();
	focoM.LoadModel("Models/focoMuseo.obj");
	expendTexture = Texture("Textures/expendText.tga");		//Modificación por vertices
	expendTexture.LoadTextureA();

	//universo AC
	arbolAC = Model();
	arbolAC.LoadModel("Models/AC/arbol.obj");
	arbustoAC = Model();
	arbustoAC.LoadModel("Models/AC/arbusto.obj");
	cedroAC = Model();
	cedroAC.LoadModel("Models/AC/cedro.obj");
	fuenteAC = Model();
	fuenteAC.LoadModel("Models/AC/fuente.obj");
	mariposaAC = Model();
	mariposaAC.LoadModel("Models/AC/mariposa.obj");
	mariquitaAC = Model();
	mariquitaAC.LoadModel("Models/AC/mariquita.obj");
	pelotaAC = Model();
	pelotaAC.LoadModel("Models/AC/pelota.obj");
	puenteAC = Model();
	puenteAC.LoadModel("Models/AC/puente.obj");
	entradaAC = Model();
	entradaAC.LoadModel("Models/AC/entrada.obj");
	casaAC = Model();
	casaAC.LoadModel("Models/AC/casa.obj");
	Canelita = Model();
	Canelita.LoadModel("Models/Ac/canelita.obj");


	//universo LT
	arbol01LT = Model();
	arbol01LT.LoadModel("Models/LT/arbol01LT.obj");
	arbol02LT = Model();
	arbol02LT.LoadModel("Models/LT/arbol02LT.obj");
	arbol03LT = Model();
	arbol03LT.LoadModel("Models/LT/arbol03LT.obj");
	arbol04LT = Model();
	arbol04LT.LoadModel("Models/LT/arbol04LT.obj");
	arbusto01LT = Model();
	arbusto01LT.LoadModel("Models/LT/arbusto01LT.obj");
	arbusto02LT = Model();
	arbusto02LT.LoadModel("Models/LT/arbusto02LT.obj");
	arbusto03LT = Model();
	arbusto03LT.LoadModel("Models/LT/arbusto03LT.obj");
	arbusto04LT = Model();
	arbusto04LT.LoadModel("Models/LT/arbusto04LT.obj");
	arbusto05LT = Model();
	arbusto05LT.LoadModel("Models/LT/arbusto05LT.obj");
	aveLT = Model();
	aveLT.LoadModel("Models/LT/ave02-LT.obj");
	aveLTPata1 = Model();
	aveLTPata1.LoadModel("Models/LT/ave02-pata1-LT.obj");
	aveLTPata2 = Model();
	aveLTPata2.LoadModel("Models/LT/ave02-pata2-LT.obj");
	marvinLT = Model();
	marvinLT.LoadModel("Models/LT/marvinLT.obj");
	marvinLTPI = Model();
	marvinLTPI.LoadModel("Models/LT/marvinLT-piernaI.obj");
	marvinLTPD = Model();
	marvinLTPD.LoadModel("Models/LT/marvinLT-piernaD.obj");
	naveLT = Model();
	naveLT.LoadModel("Models/LT/naveLT.obj");
	estatuaMarvin = Model();
	estatuaMarvin.LoadModel("Models/LT/estatua-Marvin.obj");

	//Universo GG
	Nave = Model();
	Nave.LoadModel("Models/GG/naveGuardians.obj");
	plantaLuz = Model();
	plantaLuz.LoadModel("Models/GG/plantaLuz.obj");
	conejo = Model();
	conejo.LoadModel("Models/GG/rabit.obj");
	capGemIzq = Model();
	capGemIzq.LoadModel("Models/GG/CapaGemaIzq.obj");
	capGemDer = Model();
	capGemDer.LoadModel("Models/GG/CapaGemaDer.obj");
	baseGem = Model();
	baseGem.LoadModel("Models/GG/BaseGema.obj");
	gem = Model();
	gem.LoadModel("Models/GG/gema.obj");
	groot = Model();
	groot.LoadModel("Models/GG/Groot.obj");

	//Universo 31M

	std::vector<std::string> skyboxFaces;

	skyboxFaces.push_back("Textures/Skybox/skyrender0001.tga");
	skyboxFaces.push_back("Textures/Skybox/skyrender0004.tga");
	skyboxFaces.push_back("Textures/Skybox/skyrender0006.tga");//piso
	skyboxFaces.push_back("Textures/Skybox/skyrender0003.tga");//cielo
	skyboxFaces.push_back("Textures/Skybox/skyrender0005.tga");
	skyboxFaces.push_back("Textures/Skybox/skyrender0002.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	/*
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.7f, 0.7f,
		0.0f, 0.0f, -1.0f);
	 */
	 /*mainLight2 = DirectionalLight(1.0f, .0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);*/

	//DirectionalLight mainLigth1(1.0f, 1.0f, 1.0f,
	//	0.3f, 0.3f,
	//	0.0f, 0.0f, -1.0f);

	//DirectionalLight mainLight2 (1.0f, .0f, 1.0f,
	//	0.3f, 0.3f,
	//	0.0f, .4f, 1.0f); 


	/*mainLight = DirectionalLight(1.0f, 0.0f, 0.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);*/
		//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de primer luz puntual Planta GG
	pointLights[0] = PointLight(0.99f, 0.5f, 0.0f,//0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,			//Cambia el tamaño de la luz
		100.0f, 5.0f, -70.0f,		//Cambia la posición
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//Declaración luz restaurante
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,//0.0f, 0.0f, 1.0f,
		3.0f, 3.0f,			//Cambia el tamaño de la luz
		410.0f, 27.0f, 130.0f,		//Cambia la posición
		0.3f, 0.2f, 0.01f);
	pointLightCount++;

	//Declaración luz museo
	pointLights[2] = PointLight(0.99f, 1.0f, 1.0f,//0.0f, 0.0f, 1.0f,
		10.0f, 10.0f,			//Cambia el tamaño de la luz
		65.0f, 20.0f, -140.0f,		//Cambia la posición
		0.3f, 0.24, 0.4f);
	pointLightCount++;



	unsigned int spotLightCount = 0;

	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//Luz gema
	spotLights[1] = SpotLight(0.6f, .0f, 1.0f,
		1.0f, 2.0f,
		0.0f, 6.5f, 1.0f,		//Priemr valor mueve en x		y mueve el tamaño del radio
		0.0f, 8.0f, 0.0f,		//Es la dirección hacía donde queremos que s emeuva
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;

	//Luz nave Marvin azul
	spotLights[2] = SpotLight(0.0f, 1.0f, 1.0f,
		1.0f, 2.0f,
		0.0f, -1.0f, 0.0f,		//Priemr valor mueve en x		y mueve el tamaño del radio
		0.0f, -5.0f, 0.0f,		//Es la dirección hacía donde queremos que s emeuva
		1.0f, 0.0f, 0.0f,
		40.0f);
	spotLightCount++;


	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	//VARIABLES DE ANIMACIONES

	//ANIMACION DE LA NAVE

	rotNx = 0.0f;
	rotNxOffset = 2.0f;
	movNx = 900.0f;
	movNxOffset = 1.0;
	movNy = 100.0f;
	movNyOffset = 1.0;
	movNz = 0.0f;
	movNzOffset = 1.0;

	avanza = true;
	avanzax = false;
	avanzay = true;
	avanzaz = true;

	//ANIMACION DE LA AVE

	rotAve = 270.0f;
	rotAveP = 0.0f;
	rotAveOffset = 1.0f;
	rotAveP1x = 0.0f;
	rotAvexP1Offset = 3.0f;
	rotAveP2x = 0.0f;
	rotAvexP2Offset = 3.0f;

	movAvex = 100.0f;
	movAvexOffset = 1.0;
	movAvey = 50.0f;
	movAveyOffset = 0.1;

	avanzaAve = true;
	contAve = 0;

	//ANIMACION DE MARVIN

	rotMarvin = 0.0f;
	rotMarvinyOffset = 1.0f;
	rotMarvinyP = 0.0f;

	movMarvinx = 0.0f;
	movMarvinxOffset = 0.5f;
	movMarviny = 0.0f;
	movMarvinyOffset = 0.5f;
	movMarvinz = 0.0f;
	movMarvinzOffset = 0.5f;

	avanzaMarvinx = true;
	avanzaMarviny = false;
	avanzaMarvinz = true;

	rotMarvinP1x = 0.0f;
	rotMarvinP1xOffset = 3.0f;
	rotMarvinP2x = 0.0f;
	rotMarvinP2xOffset = 3.0f;
	contMarvin = 0;

	// Animación de nave GG
	/// NAVE
	detenido = true;
	step1 = false;
	step2 = false;
	step3 = false;
	step4 = false;
	step5 = false;
	step6 = false;
	step7 = false;
	step8 = false;
	pos_x = 0.0f;
	pos_y = 0.0f;
	h_angleX = -90.0f;
	h_angleY = 0.0f;

	/////////////// GEMA
	detenidoG = true;
	step1G = false;
	pos_xG = 0.0f;
	pos_yG = 0.0f;
	h_angleXG = 0.0f;
	h_angleYG = 0.0f;

	//MARIPOSA

	rotMarip = 0.0f;
	rotM = 270.0f;;
	rotMaripOffset = 1.0f;
	movMarip = 1.0f;
	movMaripOffset = 0.5;

	avanzaM = true;

	/////LUZ SOL
	movSol = 1000.0f;
	movSolOffset = 1.0f;
	contSol = 0;
	green = 0.0f;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime para estandarizar el tiempo trascurrido entre ciclos de reloj y las variables
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//movimiento del sol
		if (contSol == 0) {
			if (movSol <= 1000.0f && movSol >= -1000.0f) {
				movSol -= movSolOffset * deltaTime;
			}
			else {
				movSol = 1000.0f;
				//contSol = 1;
			}
		}

		if (movSol >= 1000.0f) {
			green = 0.0f;
		}

		if (movSol <= -500 && movSol >= -1000) {
			green -= (deltaTime * 0.002);
		}

		if (movSol >= 500 && movSol <= 1000) {
			green += (deltaTime * 0.002);
		}


		//printf("%f         %f           %f\n", movSol, green, deltaTime);


		//luz direccional, sólo 1 y siempre debe de existir
		mainLight = DirectionalLight(green, green, 1.0f,
			(green / 2) + 0.2f, (green / 2) + 0.2f,
			movSol, -500.0f, 0.0f);


		//ANIMACIONES

		/////////////////////////////////////////////////////////////

		//ANIMACION DE MARVIN

		if (avanzax == false && avanzay == false) {
			if (avanzaMarviny == false && movMarviny >= -43.0) {
				movMarviny -= movMarvinyOffset + deltaTime;
			}
			else {
				avanzaMarviny = true;
			}
		}

		if (avanzax == false && avanzay == false && avanzaMarvinx == true) {
			if (movMarvinx <= 10.0f && movMarvinx >= 0.0f && movMarvinz <= 0.0f && movMarvinz >= -130.0f)		//1
			{
				movMarvinz -= movMarvinzOffset * deltaTime;
			}
			else {
				if (movMarvinx <= 915.0f && movMarvinx >= 0.0f && movMarvinz <= -130.0f && movMarvinz >= -135.0f && rotMarvin >= 90.0f)		//2
				{
					movMarvinx += movMarvinxOffset * deltaTime;
				}
				else {
					if (movMarvinx <= 920.0f && movMarvinx >= 910.0f && movMarvinz <= 200.0f && movMarvinz >= -135.0f && rotMarvin >= 180.0f)		//3
					{
						movMarvinz += movMarvinzOffset * deltaTime;
					}
					else {
						if (movMarvinx <= 920.0f && movMarvinx >= 750.0f && movMarvinz <= 210.0f && movMarvinz >= 200.0f && rotMarvin >= 270.0f)		//4
						{
							movMarvinx -= movMarvinxOffset * deltaTime;
						}
						else {
							if (movMarvinx >= 740.0f && movMarvinx <= 750.0f && movMarvinz <= 360.0f && movMarvinz >= 200.0f && rotMarvin <= 180.0f)		//4
							{
								movMarvinz += movMarvinzOffset * deltaTime;
							}
						}
					}
				}
			}
		}

		if (movMarvinx <= 915.0f && movMarvinx >= 0.0f && movMarvinz <= -130.0f && movMarvinz >= -135.0f && rotMarvin >= 0.0f && rotMarvin <= 90.0f)		//2
		{
			rotMarvin += rotMarvinyOffset * deltaTime;
		}
		else {
			if (movMarvinx <= 920.0f && movMarvinx >= 910.0f && movMarvinz <= 200.0f && movMarvinz >= -135.0f && rotMarvin >= 90.0f && rotMarvin <= 180.0f)		//3
			{
				rotMarvin += rotMarvinyOffset * deltaTime;
			}
			else {
				if (movMarvinx <= 920.0f && movMarvinx >= 750.0f && movMarvinz <= 210.0f && movMarvinz >= 200.0f && rotMarvin >= 180.0f && rotMarvin <= 270.0f)		//4
				{
					rotMarvin += rotMarvinyOffset * deltaTime;
				}
				else {
					if (movMarvinx >= 740.0f && movMarvinx <= 750.0f && movMarvinz <= 360.0f && movMarvinz >= 200.0f && rotMarvin >= 180.0f && rotMarvin <= 280.0f)		//4
					{
						rotMarvin -= rotMarvinyOffset * deltaTime;
					}
				}
			}
		}

		if (contMarvin == 0) {

			if (rotMarvinP1x <= 30.0f && rotMarvinP1x >= -30.0f && rotMarvinP2x <= 30.0f && rotMarvinP2x >= -30.0f) {
				rotMarvinP1x -= rotMarvinP1xOffset * deltaTime;
				rotMarvinP2x += rotMarvinP2xOffset * deltaTime;
			}
			else {
				rotMarvinP1x = -30.0f;
				rotMarvinP2x = 30.0f;
				contMarvin = 1;
			}
		}

		if (contMarvin == 1) {

			if (rotMarvinP1x <= 30.0f && rotMarvinP1x >= -30.0f && rotMarvinP2x <= 30.0f && rotMarvinP2x >= -30.0f) {
				rotMarvinP1x += rotMarvinP1xOffset * deltaTime;
				rotMarvinP2x -= rotMarvinP2xOffset * deltaTime;
			}
			else {
				rotMarvinP1x = 30.0f;
				rotMarvinP2x = -30.0f;
				contMarvin = 0;
			}
		}

		if (movMarvinx >= 740.0f && movMarvinx <= 750.0f && movMarvinz >= 360.0f && movMarvinz <= 370.0f)
		{
			contMarvin = 2;
		}

		/////////////////////////////////////////////////////////////
		//ANIMACION DE LA NAVE DE MARVIN

		if (deltaTime <= 1.5)
		{
			avanzax = true;
			avanzaz = true;
		}

		if (avanzax && avanzaz)
		{
			if (movNx <= 900.0f && movNx >= 0.0f && movNz == 0.0f)		//1
			{
				movNx -= movNxOffset * deltaTime;
				rotNx += rotNxOffset * deltaTime;
			}
			else
			{
				if (movNx <= 0.0f && movNz <= 400.0f && movNz >= 0.0f && movNx >= -10.0f)		//2
				{
					movNz += movNzOffset * deltaTime;
					rotNx += rotNxOffset * deltaTime;
				}
				else
				{
					if (movNx <= 0.0f && movNx >= -270.0f && movNz >= 400.0f && movNz <= 410.0f)		//3
					{
						movNx -= movNxOffset * deltaTime;
						rotNx += rotNxOffset * deltaTime;
					}
					else
					{
						if (movNz >= 250.0f && movNz <= 410.0f && movNx <= -270.0f && movNx >= -280.0f)		//4
						{
							movNz -= movNzOffset * deltaTime;
							rotNx += rotNxOffset * deltaTime;
						}
						else
						{
							if (movNx <= -270.0f && movNx >= -590.0f && movNz >= 240.0f && movNz <= 250.0f)		//5
							{
								movNx -= movNxOffset * deltaTime;
								rotNx += rotNxOffset * deltaTime;
							}
							else
							{
								if (movNz >= 160.0f && movNx <= -590.0f && movNx >= -630.0f && movNz <= 250.0f)					//6
								{
									movNz -= movNzOffset * deltaTime;
									rotNx += rotNxOffset * deltaTime;
								}
								else
								{
									if (movNz <= 160.0f && movNz >= 150.0f && movNy <= 100.0f && movNy >= 10.0 && movNx <= -590.0f && movNx >= -610.0f)		//7
									{
										movNy -= movNyOffset * deltaTime;
										rotNx += rotNxOffset * deltaTime;
									}
									else
									{
										avanzax = false;
										avanzay = false;
									}
								}
							}
						}
					}

				}

			}
		}
		/////////////////////////////////////////////////////////////

		//ANIMACION DE LA AVE

		if (avanzaAve == true)
		{
			if (movAvex >= -350.0f)		//1
			{
				rotAveP = 0;
				movAvex -= movAvexOffset * deltaTime;
			}
			else
			{
				if (movAvex <= -350.0f && rotAveP <= 180) {
					rotAve += rotAveOffset * deltaTime;
					rotAveP += rotAveOffset * deltaTime;
				}
				else
				{

					avanzaAve = false;
				}
			}
		}
		else
		{
			if (movAvex <= 100.0f)		//1
			{
				rotAveP = 0;
				movAvex += movAvexOffset * deltaTime;
			}
			else
			{
				if (movAvex >= 100.0f && rotAveP <= 180) {
					rotAve += rotAveOffset * deltaTime;
					rotAveP += rotAveOffset * deltaTime;
				}
				else
				{
					avanzaAve = true;
				}
			}
		}

		if (contAve == 0) {
			if (rotAveP1x <= 30.0f && rotAveP1x >= -30.0f && rotAveP2x <= 30.0f && rotAveP2x >= -30.0f) {
				rotAveP1x -= rotAvexP1Offset * deltaTime;
				rotAveP2x += rotAvexP2Offset * deltaTime;
			}
			else {
				rotAveP1x = -30.0f;
				rotAveP2x = 30.0f;
				contAve = 1;
			}
		}

		if (contAve == 1) {
			if (rotAveP1x <= 30.0f && rotAveP1x >= -30.0f && rotAveP2x <= 30.0f && rotAveP2x >= -30.0f) {
				rotAveP1x += rotAvexP1Offset * deltaTime;
				rotAveP2x -= rotAvexP2Offset * deltaTime;
			}
			else {
				rotAveP1x = 30.0f;
				rotAveP2x = -30.0f;
				contAve = 0;
			}
		}

		//////////////////////////////////////////////////

			//////////////////////////////////////////////////

	//////////////////Animación Nave Groot //////////////////////////////////

		if (mainWindow.PIniciarNave())
		{
			detenido = false;
			step1 = true;

			mainWindow.IniciaNave();
		}

		if (step1)
		{
			pos_x += 0.4 * deltaTime;


			if (pos_x > 30.0f)
			{
				pos_x = 30.0f;
				step1 = false;
				step2 = true;
			}
		}

		if (step2)
		{
			pos_y += 0.1 * deltaTime;
			pos_x += 0.1 * deltaTime;
			h_angleY += 1.0 * deltaTime;

			if (h_angleY > 20.0f)
			{
				h_angleY = 20.0f;
			}
			if (pos_y > 20.0f)
			{
				pos_y = 20.0f;
				h_angleY = 0.0;
				step2 = false;
				step3 = true;

			}
		}

		if (step3)
		{
			pos_x += 0.4 * deltaTime;

			if (pos_x > 400.0f)
			{
				pos_x = 400.0f;
				step3 = false;
				step4 = true;
			}
		}

		if (step4)
		{
			h_angleX += 2.0f * deltaTime;

			if (h_angleX > 90.0f)
			{
				h_angleX = 90.0f;
				step4 = false;
				step5 = true;
			}
		}

		if (step5)
		{
			pos_x -= 0.4 * deltaTime;

			if (pos_x < 30.0f)
			{
				pos_x = 30.0f;
				step5 = false;
				step6 = true;
			}
		}
		if (step6)
		{
			h_angleX -= 2.0f * deltaTime;

			if (h_angleX < 90.0f)
			{
				h_angleX = 90.0f;
				step6 = false;
				step7 = true;
			}
		}
		if (step7)
		{
			pos_y -= 0.02 * deltaTime;
			pos_x -= 0.04 * deltaTime;
			h_angleY -= 1.0 * deltaTime;


			if (h_angleY < -15.0f)
			{
				h_angleY = -15.0f;
			}

			if (pos_y < 0.0f)
			{
				h_angleY = 0.0f;
				pos_y = 0.0f;
				step7 = false;
				step8 = true;
			}
		}
		if (step8)
		{
			pos_x -= 0.04 * deltaTime;

			if (pos_x < -30.0f)
			{
				pos_x = -30.0f;
				h_angleY = 0.0f;
				pos_y = 0.0f;
				step8 = false;
				detenido = true;
			}
		}
		////////////////////Fin animación nave groot /////////////////////////////


		//////////////////Animación Gema  //////////////////////////////////
		if (mainWindow.PIniciargemGG())
		{
			detenidoG = false;
			step1G = true;

			mainWindow.IniciagemGG();
		}
		if (step1G)
		{
			pos_yG += 0.02 * deltaTime;
			pos_xG += 0.02 * deltaTime;
			h_angleXG += 2.0 * deltaTime;
			h_angleYG += 1.0 * deltaTime;

			if (h_angleXG > 360.0f)
			{
				h_angleXG = 360.0f;
			}

			if (h_angleYG > 130.0f)
			{
				h_angleYG = 130.0f;
			}
			if (pos_yG > 1.0f)
			{
				pos_yG = 1.0f;
				h_angleXG = 0.0;
				h_angleXG = 130.0;
				step1G = false;
				detenidoG = true;

			}
		}
		//////////////////////////////////////////////////

		//////////////////////////////////////////////////
		//ANIMACIÓN MARIPOSA
		if (avanzaM == true)
		{
			if (movMarip >= -15.0f)
			{
				rotMarip = 0;
				movMarip -= movMaripOffset * deltaTime;
			}
			else
			{
				if (movMarip <= -15.0f && rotMarip <= 180) {
					rotM += rotAveOffset * deltaTime;
					rotMarip += rotMaripOffset * deltaTime;
				}
				else
				{
					avanzaM = false;
				}
			}
		}
		else
		{
			if (movMarip <= 5.0f)
			{
				rotMarip = 0;
				movMarip += movMaripOffset * deltaTime;
			}
			else
			{
				if (movMarip >= 5.0f && rotMarip <= 180) {
					rotM += rotAveOffset * deltaTime;
					rotMarip += rotMaripOffset * deltaTime;
				}
				else
				{
					avanzaM = true;
				}
			}
		}

		//////////////////////////////////////////////////

		
		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Verficiar para camra
		//camera2.keyControl(mainWindow.getsKeys(), deltaTime);
		//camera2.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		
		//shaderList[0].SetDirectionalLight(&mainLight);
		//auto start = std::chrono::steady_clock::now();

		// Esperamos hasta que hayan pasado dos segundos
		//while (true) {
		//	auto end = std::chrono::steady_clock::now();
		//	auto duracion = std::chrono::duration_cast<std::chrono::seconds>(end - start);
		//	
		//	if (duracion.count() >= 2) {
		//		shaderList[0].SetDirectionalLight(&mainLight1);
		//		return true;
		//	}
		//	if (duracion.count() <= 2) {
		//		shaderList[0].SetDirectionalLight(&mainLight2);
		//	}

		//}
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 1.0f, 25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//OBJETOS GENERALES

		//Maquina expendedora
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(130.0f, 8.0f, 140.0f));
		model = glm::scale(model, glm::vec3(10.9f, 18.0f, 10.9f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		expendTexture.UseTexture();
		meshList[4]->RenderMesh();

			//CIUDADELA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(390.0f, -1.0f, -165.0));
		model = glm::scale(model, glm::vec3(2.0f, 4.25f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ciudadela.RenderModel();

		//ESTACIONAMIENTO

		// empezando del tope que se encuentra mas al fondo del escenario

			// tope de estacionamiento 02 - 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-450.0f, -1.0f, 45.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topeEstacionamiento01.RenderModel();

		// tope de estacionamiento 02 - 02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-372.0f, -1.0f, 45.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topeEstacionamiento01.RenderModel();

		// tope de estacionamiento 02 - 03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-299.0f, -1.0f, 45.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topeEstacionamiento01.RenderModel();

		// tope de estacionamiento 02 - 04
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-228.0f, -1.0f, 45.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topeEstacionamiento01.RenderModel();

		// tope de estacionamiento 02 - 05
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-156.0f, -1.0f, 45.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topeEstacionamiento01.RenderModel();

		// tope de estacionamiento 02 - 06
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, -1.0f, 45.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topeEstacionamiento01.RenderModel();

		// tope de estacionamiento 03 - 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, -1.0f, 120.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		topeEstacionamiento03.RenderModel();

		//BARRERA DE ACCESO VEHICULAR 01 - 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-97.0f, -1.0f, 215.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		barreraAccesoVehicular.RenderModel();

		// empezando de la cerca que se encuentra mas al fondo del escenario

		// cerca 05 - 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-390.0f, -1.0f, 30.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerca05.RenderModel();

		// cerca 05 - 02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-390.0f, -1.0f, 210.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerca05.RenderModel();

		// cerca 05 - 03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -1.0f, 30.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerca05.RenderModel();

		// cerca 06 - 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-67.0f, -1.0f, 30.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerca06.RenderModel();

		// cerca 06 - 02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-37.0f, -1.0f, 59.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerca06.RenderModel();

		// cerca 06 - 03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-37.0f, -1.0f, 180.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerca06.RenderModel();

		// cerca 07 - 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-276.0f, -1.0f, 210.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerca07.RenderModel();

		// bote DE Basura 02 - 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-276.0f, -1.0f, 215.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		boteBasura02.RenderModel();

		// bote DE Basura 02 - 02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -1.0f, 180.0f));
		model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		boteBasura02.RenderModel();

		// DELIMITANTES DE LA ZONA

		//a lado de la entrada

			// cerca 07 - 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(194.0f, -1.0f, 205.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerca08.RenderModel();

		// cerca 07 - 02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.0f, -1.0f, 89.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerca08.RenderModel();

		// cerca 07 - 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(106.0f, -1.0f, 89.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerca02.RenderModel();

		// cerca 07 - 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(117.0f, -1.0f, 98.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.25f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cerca02.RenderModel();

		//extra

		// bote DE Basura 02 - 03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(130.0f, -1.0f, 59.0f));
		model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		boteBasura02.RenderModel();

		// bote DE Basura 02 - 04
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-276.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		boteBasura02.RenderModel();

		// bote DE Basura 02 - 05
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(400.0f, 0.0f, 92.0f));
		model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		boteBasura02.RenderModel();

		//UNIVERSO LT

			//PERSONAJE MARVIN

			//NAVELT-01
		model = glm::mat4(1.0);
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(movNx, movNy, movNz));
		spotLights[2].SetPos(glm::vec3(movNx / 2, movNy / 2, movNz / 2));
		model = glm::rotate(model, rotNx * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		naveLT.RenderModel();

		//Marvin
		int xM = 0, yM = 0;

		if (fmod(rotNx, 360) != 0) {
			xM = rotNx / 360;
			xM = floor(xM);
			yM = rotNx - (xM * 360);
			if (xM >= 90) {
				yM = yM - 90;
			}
			else {
				yM = 90 - yM;
			}

		}

		model = glm::rotate(model, (yM)*toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(movMarvinz, movMarviny + 40.0f, movMarvinx));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, rotMarvin * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		marvinLT.RenderModel();


		model = glm::rotate(model, rotMarvinP1x * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		marvinLTPD.RenderModel();

		model = modelaux;
		model = glm::rotate(model, rotMarvinP2x * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		marvinLTPI.RenderModel();

		//AVELT-01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movAvex, 0.8f, -5.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, rotAve * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aveLT.RenderModel();

		model = glm::rotate(model, rotAveP1x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aveLTPata1.RenderModel();

		model = modelaux;
		model = glm::rotate(model, rotAveP2x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aveLTPata2.RenderModel();

		//alrededor del estacionamiento

		//BANCA PARQUE 01 - 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, -1.0f, 59.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bancaparque01.RenderModel();

		//BANCA PARQUE 01 - 02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.0f, -1.0f, 59.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bancaparque01.RenderModel();

		//BANCA PARQUE 01 - 03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(65.0f, -1.0f, 25.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bancaparque01.RenderModel();

		//BANCA PARQUE 01 - 04
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, -1.0f, 25.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bancaparque01.RenderModel();

		//BANCA PARQUE 01 - 05
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bancaparque01.RenderModel();

		//BANCA PARQUE 01 - 06
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-84.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bancaparque01.RenderModel();

		//BANCA PARQUE 01 - 07
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-372.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bancaparque01.RenderModel();

		//BANCA PARQUE 01 - 08
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-468.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bancaparque01.RenderModel();

		//FAUNA LT DETRAS DEL MUSEO

		//ARBOL03LT-01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, -1.0f, -225.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol03LT.RenderModel();

		//ARBOL03LT-02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(68.0f, -1.0f, -225.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol03LT.RenderModel();

		//ARBOL03LT-03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.0f, -1.0f, -225.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol03LT.RenderModel();

		//ARBOL03LT-04
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(125.0f, -1.0f, -225.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol03LT.RenderModel();

		//ARBOL03LT-05
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -1.0f, -225.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol03LT.RenderModel();

		//ARBOL04LT-01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f, -1.0f, -217.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol04LT.RenderModel();

		//ARBOL04LT-02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(85.0f, -1.0f, -217.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol04LT.RenderModel();

		//ARBOL04LT-03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(115.0f, -1.0f, -217.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol04LT.RenderModel();

		//ARBOL04LT-04
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, -1.0f, -217.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol04LT.RenderModel();

		//ESTATUA MARVIN museo

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(73.8f, 10.8f, -144.0f));
		model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		estatuaMarvin.RenderModel();

		// PARTE DEL RESTAURANTE

		//MESA 02 - 01

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(340.0f, 0.0f, 75.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa02.RenderModel();

		//SOMBRILLA 01 - 01

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(340.0f, 0.0f, 75.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sombrilla.RenderModel();

		//MESA 02 - 02

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(340.0f, 0.0f, 130.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa02.RenderModel();

		//SOMBRILLA 01 - 02

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(340.0f, 0.0f, 130.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sombrilla.RenderModel();

		//MESA 02 - 03

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(340.0f, 0.0f, 185.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa02.RenderModel();

		//SOMBRILLA 01 - 03

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(340.0f, 0.0f, 185.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sombrilla.RenderModel();

		//MESA 02 - 04

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(300.0f, 0.0f, 157.5f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa02.RenderModel();

		//SOMBRILLA 01 - 04

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(300.0f, 0.0f, 157.5f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sombrilla.RenderModel();

		//MESA 02 - 05

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(300.0f, 0.0f, 102.5f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesa02.RenderModel();

		//SOMBRILLA 01 - 05

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(300.0f, 0.0f, 102.5f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sombrilla.RenderModel();

		// LUMINARIA DE TODO EL ESCENARIO

		// poste de luz 01 - 01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		posteLuz.RenderModel();

		// poste de luz 01 - 02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-135.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		posteLuz.RenderModel();

		// poste de luz 01 - 03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-230.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		posteLuz.RenderModel();

		// poste de luz 01 - 04
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-325.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		posteLuz.RenderModel();

		// poste de luz 01 - 05
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-420.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		posteLuz.RenderModel();

		// poste de luz 01 - 06
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(325.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		posteLuz.RenderModel();

		//ARBUSTO05LT-01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, -1.0f, -95.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto05LT.RenderModel();

		//ARBUSTO05LT-02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -1.0f, -95.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, 270 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto05LT.RenderModel();

		//ARBUSTO01LT-03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto05LT.RenderModel();

		//ARBUSTO05LT-04
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(115.0f, -1.0f, 65.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto05LT.RenderModel();

		//ARBUSTO01LT-05
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, -1.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto05LT.RenderModel();

		/////

		//ARBOL02LT-01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(405.0f, -1.0f, 235.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol02LT.RenderModel();

		//ARBOL02LT-02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(375.0f, -1.0f, 235.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol02LT.RenderModel();

		//ARBOL02LT-03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(345.0f, -1.0f, 235.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol02LT.RenderModel();

		//ARBOL02LT-04
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(315.0f, -1.0f, 235.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol02LT.RenderModel();

		//ARBOL02LT-05
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(285.0f, -1.0f, 235.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol02LT.RenderModel();

		//ARBOL02LT-06
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(435.0f, -1.0f, 235.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol02LT.RenderModel();

		////

		//ARBOL04LT-05
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(390.0f, -1.0f, 235.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol04LT.RenderModel();

		//ARBOL04LT-06
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(360.0f, -1.0f, 235.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol04LT.RenderModel();

		//ARBOL04LT-07
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(330.0f, -1.0f, 235.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol04LT.RenderModel();

		//ARBOL04LT-08
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(300.0f, -1.0f, 235.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol04LT.RenderModel();

		//ARBOL04LT-09
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(420.0f, -1.0f, 235.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol04LT.RenderModel();

		//ARBOL04LT-10
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(450.0f, -1.0f, 235.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol04LT.RenderModel();

		////

		//ARBUSTO01LT-06
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(330.0f, -1.0f, 215.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto05LT.RenderModel();

		//ARBUSTO01LT-07
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(420.0f, -1.0f, 215.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto05LT.RenderModel();

		//ARBUSTO01LT-08
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(375.0f, -1.0f, 220.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto05LT.RenderModel();

		//ARBUSTO01LT-09
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(292.0f, -1.0f, 220.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto05LT.RenderModel();

		//// AVES

		//AVELT-02
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(110.0f, 0.8f, -60.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, (rotAveP2x + 30) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aveLT.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(110.0f, 0.8f, -60.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aveLTPata1.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(110.0f, 0.8f, -60.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aveLTPata2.RenderModel();

		//AVELT-03
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(360.0f, 0.8f, 200.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, -30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, (rotAveP2x + 30) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aveLT.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(360.0f, 0.8f, 200.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, -30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aveLTPata1.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(360.0f, 0.8f, 200.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, -30 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aveLTPata2.RenderModel();

		//AVELT-04
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-160.0f, 0.8f, 15.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, (rotAveP2x + 30) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aveLT.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-160.0f, 0.8f, 15.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aveLTPata1.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-160.0f, 0.8f, 15.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aveLTPata2.RenderModel();




		//UNIVERSO GG

			//Nave
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-450.0 + pos_x, 4.7 + pos_y, 165.0f));
		model = glm::rotate(model, glm::radians(h_angleX), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(h_angleY), glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));		//
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nave.RenderModel();
		Material_brillante = Material(1.0f, 256);

		//PlantaLuz1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-450.0f, -1.0f, -70.0f));	//
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plantaLuz.RenderModel();

		//PlantaLuz2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -1.0f, -70.0f));	//
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plantaLuz.RenderModel();

		//PlantaLuz3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -1.0f, -70.0f));	//
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plantaLuz.RenderModel();

		//PlantaLuz4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(400.0f, -1.0f, 194.0f));	//
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plantaLuz.RenderModel();

		//PlantaLuz5
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(400.0f, -1.0f, 66.0f));	//
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plantaLuz.RenderModel();

		//Fauna
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.65f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		conejo.RenderModel();

		//CapaGemaIzq
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(95.0f, 12.0f, -143.5f));
		model = glm::translate(model, glm::vec3(96.0f - pos_xG, 11.3f + pos_yG, -143.6f));
		model = glm::rotate(model, glm::radians(h_angleXG), glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		capGemIzq.RenderModel();

		//CapaGemaDer
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(97.6f, 12.0f, -143.5f));
		model = glm::translate(model, glm::vec3(96.6f + pos_xG, 11.3f + pos_yG, -143.5f));
		model = glm::rotate(model, glm::radians(h_angleXG), glm::vec3(-1, 0, 0));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		capGemDer.RenderModel();

		//Base Gema
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(96.6f, 9.8f, -143.5f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		baseGem.RenderModel();

		//Gema
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(96.5f, 12.0f, -143.5f));
		model = glm::translate(model, glm::vec3(96.5f, 11.3f + pos_yG, -143.5f));
		spotLights[1].SetPos(glm::vec3(96.7f, 10.0f, -142.5f));
		model = glm::rotate(model, glm::radians(h_angleYG), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		gem.RenderModel();

		//Groot
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f, -0.6f, 27.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		groot.RenderModel();

		//UNIVERSO 31M


		//UNIVERSO AC
		
		//CANELITA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(300.0f, 1.4f, 70.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Canelita.RenderModel();

		//ENTRADA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(158.0f, 6.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.3f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		entradaAC.RenderModel();

		//TAQUILLA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(130.0f, -1.0f, 120.0f));
		model = glm::scale(model, glm::vec3(0.30f, 0.30f, 0.30f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		taquillaAC.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(140.0f, 11.2f, 119.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.6f, 1.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		cristal.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		glDisable(GL_BLEND);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(133.0f, 12.5f, 130.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.56f, 0.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		cristal.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		glDisable(GL_BLEND);

		//PUENTE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(247.0f, 3.0f, -18.0));
		model = glm::scale(model, glm::vec3(0.4f, 0.5f, 0.7f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puenteAC.RenderModel();

		//FUENTE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(45.0f, -1.0f, 59.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		fuenteAC.RenderModel();

		//RESTAURANTE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(400.0f, 18.0f, 130.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		restauranteAC.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(388.5f, 19.0f, 134.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.7f, 2.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		cristal.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		glDisable(GL_BLEND);

		//ARBUSTO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(480.0f, -1.0f, 21.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbustoAC.RenderModel();

		//ARBUSTO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(453.0f, -1.0f, 21.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbustoAC.RenderModel();

		//ARBUSTO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(426.0f, -1.0f, 21.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbustoAC.RenderModel();

		//ARBUSTO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(399.0f, -1.0f, 21.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbustoAC.RenderModel();

		//ARBUSTO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(372.0f, -1.0f, 21.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbustoAC.RenderModel();

		//ARBUSTO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(345.0f, -1.0f, 21.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbustoAC.RenderModel();

		//ÁRBOL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(470.0f, -1.0f, 50.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbolAC.RenderModel();

		//ÁRBOL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(470.0f, -1.0f, 90.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbolAC.RenderModel();

		//ÁRBOL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(470.0f, -1.0f, 130.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbolAC.RenderModel();

		//ÁRBOL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(470.0f, -1.0f, 170.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbolAC.RenderModel();

		//ÁRBOL
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(470.0f, -1.0f, 210.0f));
		model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbolAC.RenderModel();

		//PELOTA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(299.0f, 1.7f, 50.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 70 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pelotaAC.RenderModel();

		//PELOTA restaurante
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(360.0f, 1.7f, 177.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 70 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pelotaAC.RenderModel();

		//MARIQUITA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(290.0f, 0.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mariquitaAC.RenderModel();

		//MARIPOSA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(345.0f, 20.0f, 21.0f));
		model = glm::translate(model, glm::vec3(movMarip, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, rotMarip * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mariposaAC.RenderModel();

		//MARIPOSA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(365.0f, 15.0f, 51.0f));
		model = glm::translate(model, glm::vec3(movMarip, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, rotMarip * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -49 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mariposaAC.RenderModel();

		//MARIPOSA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(245.0f, 22.0f, 41.0f));
		model = glm::translate(model, glm::vec3(movMarip, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, rotMarip * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -62 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mariposaAC.RenderModel();

		//MARIPOSA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(189.0f, 17.0f, 142.0f));
		model = glm::translate(model, glm::vec3(movMarip, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, rotMarip * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mariposaAC.RenderModel();

		//MARIPOSA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(425.0f, 9.0f, 176.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -49 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mariposaAC.RenderModel();

		//MARIPOSA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(367.0f, 5.0f, 194.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, 60 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -62 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mariposaAC.RenderModel();

		//ESTATUA CASA CANELITA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.4f, 9.6f, -144.0f));
		model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.06f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		casaAC.RenderModel();


		//OBJETOS GENERALES

			//FOCO MUSEO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(65.0f, 23.0f, -140.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		focoM.RenderModel();

		//MUSEO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, -2.6f, -140.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		museum.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(63.0f, 13.0f, -127.0f));
		model = glm::scale(model, glm::vec3(4.45f, 1.38f, 12.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 0.8f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		cristal.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}





//CARGA DE MODELOS

//GENERALES

/*
//BANCA PARQUE 01 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
bancaparque01.RenderModel();

//BOTE DE BASURA 01 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
boteBasura01.RenderModel();


// bote DE Basura Amarillo 01 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
boteBasura02Amarillo.RenderModel();

// bote de Basura Azul 01 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
boteBasura02Azul.RenderModel();

// bote de Basura Cafe 01 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
boteBasura02Cafe.RenderModel();

// bote de Basura verde 01 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
boteBasura02Verde.RenderModel();

// cerca 01 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
cerca01.RenderModel();

// cerca 02 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
cerca02.RenderModel();

// cerca 03 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
cerca03.RenderModel();

// cerca 04 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
cerca04.RenderModel();

// lampara de pared 01 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
lamparaPared.RenderModel();

// mesa 01 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
mesa01.RenderModel();

// mesa 02 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
mesa02.RenderModel();

// mesa restaurante 01 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
mesaRestaurante.RenderModel();

// poste de luz 01 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
posteLuz.RenderModel();

// sombrilla 01 - 01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
sombrilla.RenderModel();
*/

//UNIVERSO LT

/*

//ARBOL01LT-01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
arbol01LT.RenderModel();

//ARBOL02LT-01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(30.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
arbol02LT.RenderModel();

//ARBOL03LT-01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(60.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
arbol03LT.RenderModel();

//ARBOL04LT-01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(90.0f, -1.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
arbol04LT.RenderModel();

//ARBUSTO01LT-01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 30.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
arbusto01LT.RenderModel();

//ARBUSTO02LT-01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(30.0f, -1.0f, 30.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
arbusto02LT.RenderModel();

//ARBUSTO03LT-01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(60.0f, -1.0f, 30.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
arbusto03LT.RenderModel();

//ARBUSTO04LT-01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(90.0f, -1.0f, 30.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
arbusto04LT.RenderModel();

//ARBUSTO05LT-01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(120.0f, -1.0f, 30.0f));
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
arbusto05LT.RenderModel();

//AVELT-01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.0f, 60.0f));
model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
aveLT.RenderModel();

//MARVINLT-01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(90.0f, 3.0f, 60.0f));
model = glm::scale(model, glm::vec3(0.075f, 0.075f, 0.075f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
marvinLT.RenderModel();

//NAVELT-01
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(60.0f, -1.0f, 60.0f));
model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
naveLT.RenderModel();
*/