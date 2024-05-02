//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <iostream>
#include <chrono>

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

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture plisa;
Texture piedra;
Texture bloques;
Texture agua;
Texture mariquita;
Texture cmariquita;
Texture arbusto;
Texture mariposa;
Texture copaCed;
Texture cedro;
Texture copaArb;
Texture arbol;
Texture pelota;
Texture tejado;
Texture teja;
Texture blanco;
Texture pared;
Texture colores;
Texture cuadro;
Texture pisorst;
Texture tb;
Texture camp;
Texture negro;
Texture mad;
Texture madera;
Texture cristal;


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

//Universo AC
Model arbolm;
Model arbustom;
Model cedrom;
Model fuentem;
Model mariposam;
Model mariquitam;
Model pelotam;
Model puentem;
Model entrada;
Model restaurante;

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
Model marvinLT;
Model naveLT;

//Universo GG

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

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);


	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso1.tga");
	pisoTexture.LoadTextureA();
	plisa = Texture("Textures/plisa.tga");
	plisa.LoadTextureA();
	piedra = Texture("Textures/piedra.tga");
	piedra.LoadTextureA();
	bloques = Texture("Textures/bloques.tga");
	bloques.LoadTextureA();
	agua = Texture("Textures/agua.tga");
	agua.LoadTextureA();
	mariquita = Texture("Textures/mariquita.tga");
	mariquita.LoadTextureA();
	cmariquita = Texture("Textures/cmariquita.tga");
	cmariquita.LoadTextureA();
	mariposa = Texture("Textures/mariposa.tga");
	mariposa.LoadTextureA();
	arbusto = Texture("Textures/arbusto.tga");
	arbusto.LoadTextureA();
	copaCed = Texture("Textures/copaCed.tga");
	copaCed.LoadTextureA();
	copaArb = Texture("Textures/copaArb.tga");
	copaArb.LoadTextureA();
	cedro = Texture("Textures/cedro.tga");
	cedro.LoadTextureA();
	arbol = Texture("Textures/arbol.tga");
	arbol.LoadTextureA();
	pelota = Texture("Textures/pelota.tga");
	pelota.LoadTextureA();
	teja = Texture("Textures/teja.tga");
	teja.LoadTextureA();
	tejado = Texture("Textures/tejado.tga");
	tejado.LoadTextureA();
	blanco = Texture("Textures/blanco.tga");
	blanco.LoadTextureA();
	colores = Texture("Textures/colores.tga");
	colores.LoadTextureA();
	pared = Texture("Textures/pared.tga");
	pared.LoadTextureA();
	cuadro = Texture("Textures/cuadro.tga");
	cuadro.LoadTextureA();
	tb = Texture("Textures/tb.tga");
	tb.LoadTextureA();
	camp = Texture("Textures/camp.tga");
	camp.LoadTextureA();
	pisorst = Texture("Textures/pisorst.tga");
	pisorst.LoadTextureA();
	negro = Texture("Textures/negro.tga");
	negro.LoadTextureA();
	mad = Texture("Textures/mad.tga");
	mad.LoadTextureA();
	madera = Texture("Textures/madera.tga");
	madera.LoadTextureA();
	cristal = Texture("Textures/cristal.tga");
	cristal.LoadTextureA();
	
	//objetos generales
	ciudadela = Model();
	ciudadela.LoadModel("Models/ciudadela-01.obj");
	restaurante = Model();
	restaurante.LoadModel("Models/restaurante.obj");
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
	topeEstacionamiento02= Model();
	topeEstacionamiento02.LoadModel("Models/tope-estacionamiento-02.obj");
	topeEstacionamiento03 = Model();
	topeEstacionamiento03.LoadModel("Models/tope-estacionamiento-03.obj");
	museum = Model();
	museum.LoadModel("Models/Museum.obj");

	//universo AC
	arbolm = Model();
	arbolm.LoadModel("Models/AC/arbol.obj");
	arbustom = Model();
	arbustom.LoadModel("Models/AC/arbusto.obj");
	cedrom = Model();
	cedrom.LoadModel("Models/AC/cedro.obj");
	fuentem = Model();
	fuentem.LoadModel("Models/AC/fuente.obj");
	mariposam = Model();
	mariposam.LoadModel("Models/AC/mariposa.obj");
	mariquitam = Model();
	mariquitam.LoadModel("Models/AC/mariquita.obj");
	pelotam = Model();
	pelotam.LoadModel("Models/AC/pelota.obj");
	puentem = Model();
	puentem.LoadModel("Models/AC/puente.obj");
	entrada = Model();
	entrada.LoadModel("Models/AC/entrada.obj");
	
	
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
	aveLT.LoadModel("Models/LT/aveLT.obj");
	marvinLT = Model();
	marvinLT.LoadModel("Models/LT/marvinLT.obj");
	naveLT = Model();
	naveLT.LoadModel("Models/LT/naveLT.obj");

	//Universo GG

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


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		-3.6f, 2.6f, 8.0f,
		0.3f, 0.2f, 0.1f);
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


	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime para estandarizar el tiempo trascurrido entre ciclos de reloj y las variables
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//anidación de más de 5 casos es animación compleja
		/* Así ni el auto ni las llantas se mueven
		if (movCoche < 30.0f)
		{
			movCoche -= movOffset * deltaTime;
			rotllanta += rotllantaOffset * deltaTime;
			//printf("avanza%f \n ",movCoche);
		}

		*/

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

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

		//CIUDADELA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(400.0f, -1.0f, -160.0));
		model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ciudadela.RenderModel();

		//Museo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, -1.9f, -200.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		museum.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(82.0f, 9.0f, -184.0f));
		model = glm::scale(model, glm::vec3(3.2f, 0.92f, 12.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 0.5f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		cristal.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		glDisable(GL_BLEND);

		
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

			// bote DE Basura 02 - 01
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-30.0f, -1.0f, 180.0f));
			model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			boteBasura02.RenderModel();

		//UNIVERSO LT
		
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
		

		//UNIVERSO GG

		//UNIVERSO 31M


		//UNIVERSO AC

		//ENTRADA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(158.0f, 1.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		entrada.RenderModel();

		//PUENTE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(247.0f, 3.0f, -18.0));
		model = glm::scale(model, glm::vec3(0.4f, 0.5f, 0.7f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		puentem.RenderModel();
		/*
		//FUENTE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, -90.0));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		fuentem.RenderModel();
		*/
		//RESTAURANTE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(400.0f, 0.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		restaurante.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(388.0f, 0.0f, 100.0f));
		model = glm::scale(model, glm::vec3(4.2f, 3.7f, 3.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
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