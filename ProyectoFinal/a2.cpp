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

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;



//variables para animaci�n



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

//Objetos general
Model ciudadela;

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

//funci�n de calculo de normales por promedio de v�rtices 
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

	//objetos generales
	ciudadela = Model();
	ciudadela.LoadModel("Models/ciudadela-01.obj");

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


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
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

	//ANIMACION DE LA NAVE

	rotNx = 0.0f;
	rotNxOffset = 0.5f;
	movNx = 0.0f;
	movNxOffset = 1.0;
	movNy = 0.0f;
	movNyOffset = 1.0;
	movNz = 0.0f;
	movNzOffset = 1.0;

	avanza = true;
	avanzax = false;
	avanzay = true;


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime para estandarizar el tiempo trascurrido entre ciclos de reloj y las variables
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//anidaci�n de m�s de 5 casos es animaci�n compleja
		/* As� ni el auto ni las llantas se mueven
		if (movCoche < 30.0f)
		{
			movCoche -= movOffset * deltaTime;
			rotllanta += rotllantaOffset * deltaTime;
			//printf("avanza%f \n ",movCoche);
		}

		*/

		/////////////////////////////////////////////////////////////
		//ANIMACION NAVE
		printf("%f", deltaTime);
		if (deltaTime <= 1.5f)
		{
			avanzax = true;
			avanzay = true;
		}

		if (avanzax && avanzay)
		{
			//la nave avanza
			if (movNx > -300.0f && movNx <= 0.0f && movNy == 0.0f)
			{
				movNx -= movNxOffset * deltaTime;
			}
			else
			{
				avanzax = false;
				avanzay = false;
			}
		}

		if (!avanzax && !avanzay)
		{
			//la nave rota
			if (movNx <= -300.0f && movNx > -350.2f && movNy <= 0.0f && movNy >= -50.0f)
			{
				movNx -= movNxOffset * deltaTime;
				rotNx += rotNxOffset * deltaTime;
				movNy -= movNyOffset * deltaTime;
			}
			else
			{
				if (movNy <= -350.0f && movNy > -400.0f && movNx < -300.0f && movNx >= -351.0f) {
					movNx += movNxOffset * deltaTime;
					rotNx -= rotNxOffset * deltaTime;
					movNy -= movNyOffset * deltaTime;

				}
				else {
					if (movNy < -350.0f && movNy >= -400.2f && movNx >= 0.0001f && movNy < 50.0f) {
						movNx += movNxOffset * deltaTime;
						rotNx -= rotNxOffset * deltaTime;
						movNy += movNyOffset * deltaTime;
					}
					else {
						if (movNy < 0.0f && movNy >= -50.0f && movNx > 0.0f && movNy <= 50.0f) {
							movNx -= movNxOffset * deltaTime;
							rotNx -= rotNxOffset * deltaTime;
							movNy += movNyOffset * deltaTime;
						}
						else {
							avanzax = true;
							avanzay = false;

						}
					}

				}

			}
		}
		if (avanzax && !avanzay)
		{
			//la nave avanza 
			if (movNy > -350.0f && movNy <= -50.0f && movNx <= -350.0f)
			{
				movNy -= movNyOffset * deltaTime;
			}
			else
			{
				if (movNx >= -301.0f && movNx < 0.0f && movNy <= -400.0f) {
					movNx += movNxOffset * deltaTime;
				}
				else {
					if (movNy > -350.2f && movNy < -50.0f && movNx >= 50.0f) {
						movNy += movNxOffset * deltaTime;
					}
					else {
						if (movNx > -300.0f && movNx <= 0.2f && movNy >= 0.1f) {
							movNx -= movNxOffset * deltaTime;
						}
						else {
							avanzax = false;
							avanzay = false;
						}

					}

				}

			}
		}
		/////////////////////////////////////////////////////////////


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

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//informaci�n al shader de fuentes de iluminaci�n
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

		//FUENTE
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, -90.0));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		fuentem.RenderModel();

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
*/
		//NAVELT-01
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 50.0f, 60.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(movNx, 10.0f, movNy));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		naveLT.RenderModel();

		//UNIVERSO GG

		//UNIVERSO 31M

		/*
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
		*/

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
