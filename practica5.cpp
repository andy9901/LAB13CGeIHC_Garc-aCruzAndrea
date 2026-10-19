/*
Practica 5: Optimizacion y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <string>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshListColor;
std::vector<MeshModel*> meshListModel;
std::vector<Shader> shaderList;

Camera camera;
//Lista de Modelos a importar
Model Rover_M;
Model Holo_Esq[8];
Model Sat_Cuerpo, Sat_PanelIzq, Sat_PanelDer, Sat_Antena;

//Lista de Skybox a crear
Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";


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
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	MeshModel* obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel* obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel* obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);
}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	//Cargar modelos
	Rover_M = Model();
	Rover_M.LoadModel("Models/RoverModificado.obj");

	for (int i = 0; i < 8; i++)
	{
		Holo_Esq[i] = Model();
		Holo_Esq[i].LoadModel("Models/Holo_Esq" + std::to_string(i + 1) + ".obj");
	}

	Sat_Cuerpo = Model();   Sat_Cuerpo.LoadModel("Models/Sat_Cuerpo.obj");
	Sat_PanelIzq = Model(); Sat_PanelIzq.LoadModel("Models/Sat_PanelIzq.obj");
	Sat_PanelDer = Model(); Sat_PanelDer.LoadModel("Models/Sat_PanelDer.obj");
	Sat_Antena = Model();   Sat_Antena.LoadModel("Models/Sat_Antena.obj");
	

	//Crear Skybox con sus 6 texturas
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	// Colores de las 8 esquinas del Holocron
	glm::vec3 coloresEsq[8] = {
		glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(0.6f, 0.3f, 0.9f)
	};

	// Manda la matriz y el color al shader y dibuja el modelo
	auto dibuja = [&](Model& m, const glm::mat4& mat, const glm::vec3& col)
		{
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(mat));
			glUniform3fv(uniformColor, 1, glm::value_ptr(col));
			m.RenderModel();
		};

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// ---------------- PISO ----------------
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshListModel[2]->RenderMeshModel();

		// ---------------- ROVER ----------------
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -1.5f));
		dibuja(Rover_M, model, color);
		modelaux = model;
		// (aqui van las llantas y el brazo del Rover de la Practica 4)

		// ---------------- HOLOCRON ----------------
		glm::mat4 holoAux = glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, -1.0f, -1.5f));

		glm::vec3 dirEsq[8] = {
			glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(-1.0f,  1.0f, -1.0f),
			glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(-1.0f,  1.0f,  1.0f),
			glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f),
			glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(-1.0f, -1.0f,  1.0f)
		};

		for (int i = 0; i < 8; i++)
		{
			model = holoAux;
			model = glm::rotate(model, glm::radians(mainWindow.getRotEsq(i)), glm::normalize(dirEsq[i]));
			dibuja(Holo_Esq[i], model, coloresEsq[i]);
		}

		//SATELITE
		// ---------------- SATELITE ----------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-15.0f + mainWindow.getSatX(),
			6.0f + mainWindow.getSatY(),
			-1.5f + mainWindow.getSatZ()));
		model = glm::scale(model, glm::vec3(0.5f));
		glm::mat4 satAux = model;
		dibuja(Sat_Cuerpo, model, glm::vec3(0.7f, 0.7f, 0.7f));

		// Puntos de articulacion (coordenadas del modelo, ya convertidas a OpenGL)
		glm::vec3 unionPanelIzq(-0.2864f, -0.0508f, -0.8760f);
		glm::vec3 unionPanelDer(0.2908f, -0.0542f, -0.8758f);
		glm::vec3 unionAntena(0.2353f, -0.0882f, 2.6986f);
		// Panel izquierdo: se inclina sobre su bisagra
		model = satAux;
		model = glm::translate(model, unionPanelIzq);
		model = glm::rotate(model, glm::radians(mainWindow.getRotPanelIzq()), glm::vec3(1, 0, 0));
		model = glm::translate(model, -unionPanelIzq);
		dibuja(Sat_PanelIzq, model, glm::vec3(0.1f, 0.2f, 0.8f));

		// Panel derecho
		model = satAux;
		model = glm::translate(model, unionPanelDer);
		model = glm::rotate(model, glm::radians(mainWindow.getRotPanelDer()), glm::vec3(1, 0, 0));
		model = glm::translate(model, -unionPanelDer);
		dibuja(Sat_PanelDer, model, glm::vec3(0.1f, 0.2f, 0.8f));

		// Antena: barre de lado a lado sobre su base
		model = satAux;
		model = glm::translate(model, unionAntena);
		model = glm::rotate(model, glm::radians(mainWindow.getRotAntena()), glm::vec3(0, 1, 0));
		model = glm::translate(model, -unionAntena);
		dibuja(Sat_Antena, model, glm::vec3(0.9f, 0.9f, 0.2f));
		

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}