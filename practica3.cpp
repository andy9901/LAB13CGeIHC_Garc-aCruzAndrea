//práctica 3: Modelado Geométrico y Cámara Sintética.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList; 
//vector<Mesh*>piramideCaras;
//***Se agregara un vector para las mallas de color
//vector<MeshColor*> meshColorList;
//***
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
//static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks
//CREACION DE FIGURAS 

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
	//**X--Y--Z**
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* piramidet = new Mesh();
	piramidet->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(piramidet);

}
//***NUEVA FUNCIÓN Piramide con color por cara (mismas coordenadas que Pirm.Cuadran., sin idices, repitiendo vértices por cara para que cada triangulo possea su propio color sólido)
/*void CrearPiramideColor() {
	GLfloat piramideColor_Vertices[] = {
		//FRONTAL RED
		0.5f, -0.5f, 0.5f,	1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,	1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
		//IZQ. GREEN
		-0.5f, -0.5f, 0.5f,	0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,	0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
		//DETRAS YELLOW
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,
		//DER. MAGENTA
		0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,	1.0f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f,	1.0f, 0.0f, 1.0f,
		//Base triángulo 1 BLUE
		0.5f, -0.5f, 0.5f,	0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,	0.0f, 0.0f, 1.0f,
		//Base triangulo 2 BLUE
		0.5f, -0.5f, 0.5f,	0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,	0.0f, 0.0f, 1.0f,
	};
	MeshColor* piramideColor = new MeshColor();
	piramideColor->CreateMeshColor(piramideColor_Vertices, 18);//18 son el # de vertices reales
	meshColorList.push_back(piramideColor);
}*/
//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,//frontal
		3,2,4,//izquierda
		2,1,4,//trasera
		1,0,4,//derecha
		0,1,2,//abajo1
		0,2,3//abajo2

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramidec = new Mesh();
	piramidec->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramidec);
}
/*
Similar a piramide cuadrangular separada en 5 mallas

void CrearPiramideCaras()
{
	unsigned int idxTri[] = { 0,1,2 };
	GLfloat frontal[] = {
		0.5f,-0.5f, 0.5f,
	   -0.5f,-0.5f, 0.5f,
		0.0f, 0.5f, 0.0f
	};
	Mesh* caraFrontal = new Mesh();
	caraFrontal->CreateMesh(frontal, idxTri, 9, 3);
	piramideCaras.push_back(caraFrontal); // índice 0 - ROJO

	GLfloat izquierda[] = {
	   -0.5f,-0.5f, 0.5f,
	   -0.5f,-0.5f,-0.5f,
		0.0f, 0.5f, 0.0f
	};
	Mesh* caraIzquierda = new Mesh();
	caraIzquierda->CreateMesh(izquierda, idxTri, 9, 3);
	piramideCaras.push_back(caraIzquierda); // índice 1 - VERDE

	GLfloat trasera[] = {
	   -0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.0f, 0.5f, 0.0f
	};
	Mesh* caraTrasera = new Mesh();
	caraTrasera->CreateMesh(trasera, idxTri, 9, 3);
	piramideCaras.push_back(caraTrasera); // índice 2 - AMARILLO

	GLfloat derecha[] = {
		0.5f,-0.5f,-0.5f,
		0.5f,-0.5f, 0.5f,
		0.0f, 0.5f, 0.0f
	};
	Mesh* caraDerecha = new Mesh();
	caraDerecha->CreateMesh(derecha, idxTri, 9, 3);
	piramideCaras.push_back(caraDerecha); // índice 3 - MAGENTA

	unsigned int idxBase[] = { 0,1,2, 0,2,3 };
	GLfloat base[] = {
		0.5f,-0.5f, 0.5f,
		0.5f,-0.5f,-0.5f,
	   -0.5f,-0.5f,-0.5f,
	   -0.5f,-0.5f, 0.5f
	};
	Mesh* caraBase = new Mesh();
	caraBase->CreateMesh(base, idxBase, 12, 6);
	piramideCaras.push_back(caraBase); // índice 4 - AZUL
}*/
/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
/*void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0)*dt);
			z = R * sin((0)*dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh *cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}*/
///*******************NUEVA FUNCION DE CILINDRO PARA COHETE
void CrearCilindro(int res, float R) {
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	// Pared lateral: un punto abajo y uno arriba por segmento
	for (int n = 0; n <= res; n++) {
		float ang = n * (2 * PI / res);
		float x = R * cos(ang);
		float z = R * sin(ang);
		vertices.push_back(x); vertices.push_back(-0.5f); vertices.push_back(z);
		vertices.push_back(x); vertices.push_back(0.5f);  vertices.push_back(z);
	}
	for (int n = 0; n < res; n++) {
		unsigned int b0 = n * 2, t0 = n * 2 + 1;
		unsigned int b1 = (n + 1) * 2, t1 = (n + 1) * 2 + 1;
		indices.push_back(b0); indices.push_back(t0); indices.push_back(b1);
		indices.push_back(t0); indices.push_back(t1); indices.push_back(b1);
	}

	// Tapa inferior
	unsigned int centroInf = vertices.size() / 3;
	vertices.push_back(0.0f); vertices.push_back(-0.5f); vertices.push_back(0.0f);
	unsigned int inicioInf = vertices.size() / 3;
	for (int n = 0; n <= res; n++) {
		float ang = n * (2 * PI / res);
		vertices.push_back(R * cos(ang)); vertices.push_back(-0.5f); vertices.push_back(R * sin(ang));
	}
	for (int n = 0; n < res; n++) {
		indices.push_back(centroInf);
		indices.push_back(inicioInf + n + 1);
		indices.push_back(inicioInf + n);
	}

	// Tapa superior
	unsigned int centroSup = vertices.size() / 3;
	vertices.push_back(0.0f); vertices.push_back(0.5f); vertices.push_back(0.0f);
	unsigned int inicioSup = vertices.size() / 3;
	for (int n = 0; n <= res; n++) {
		float ang = n * (2 * PI / res);
		vertices.push_back(R * cos(ang)); vertices.push_back(0.5f); vertices.push_back(R * sin(ang));
	}
	for (int n = 0; n < res; n++) {
		indices.push_back(centroSup);
		indices.push_back(inicioSup + n);
		indices.push_back(inicioSup + n + 1);
	}

	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}
//función para crear un cono
/* void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);
	
	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res+2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh *cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}*/

/// ****************************************NUEVA FUNCION CONO DEL COHETE
void CrearCono(int res, float R) {
	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	unsigned int apice = 0;
	vertices.push_back(0.0f); vertices.push_back(0.5f); vertices.push_back(0.0f);

	unsigned int inicioBase = vertices.size() / 3;
	for (int n = 0; n <= res; n++) {
		float ang = n * (2 * PI / res);
		vertices.push_back(R * cos(ang)); vertices.push_back(-0.5f); vertices.push_back(R * sin(ang));
	}
	for (int n = 0; n < res; n++) {
		indices.push_back(apice);
		indices.push_back(inicioBase + n);
		indices.push_back(inicioBase + n + 1);
	}

	// Tapa de la base (para que no quede hueca)
	unsigned int centroBase = vertices.size() / 3;
	vertices.push_back(0.0f); vertices.push_back(-0.5f); vertices.push_back(0.0f);
	unsigned int inicioBase2 = vertices.size() / 3;
	for (int n = 0; n <= res; n++) {
		float ang = n * (2 * PI / res);
		vertices.push_back(R * cos(ang)); vertices.push_back(-0.5f); vertices.push_back(R * sin(ang));
	}
	for (int n = 0; n < res; n++) {
		indices.push_back(centroBase);
		indices.push_back(inicioBase2 + n + 1);
		indices.push_back(inicioBase2 + n);
	}

	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cono);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	//Shader* shader2 = new Shader();
	//Se corrijio fshader por fshadercolor
	//shader2->CreateFromFiles(vShaderColor, fShader);
	//shaderList.push_back(*shader2);
}
//PARA LOS PRISMAS
struct PrismaCuadrangular {
	float lado;
	float alto;
	float y;
	glm::vec3 color;
};
void DibujarInstancia(Mesh* mesh, glm::mat4 model, glm::vec3 color, GLuint uniformModel, GLuint uniformColor)
{
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(uniformColor, 1, glm::value_ptr(color));
	mesh->RenderMesh();
}
//NUEVA FUNCION COHETE
void DibujarCohete(glm::mat4 model, GLuint uniformModel, GLuint uniformColor)
{
	// --- CUERPO (cilindro alargado) ---
	glm::mat4 cuerpo = model;
	cuerpo = glm::scale(cuerpo, glm::vec3(0.5f, 2.5f, 0.5f));
	DibujarInstancia(meshList[2], cuerpo, glm::vec3(0.8f, 0.8f, 0.8f), uniformModel, uniformColor); // gris

	// --- PUNTA (cono) ---
	glm::mat4 punta = model;
	punta = glm::translate(punta, glm::vec3(0.0f, 1.45f, 0.0f)); // antes 1.65f
	punta = glm::scale(punta, glm::vec3(0.25f, 0.4f, 0.25f));
	DibujarInstancia(meshList[3], punta, glm::vec3(1.0f, 0.0f, 0.0f), uniformModel, uniformColor);

	// --- VENTANILLA (esfera) ---
	glm::mat4 ventanilla = model;
	ventanilla = glm::translate(ventanilla, glm::vec3(0.0f, 0.4f, 0.55f)); // antes 0.26f
	ventanilla = glm::scale(ventanilla, glm::vec3(0.15f, 0.15f, 0.05f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(ventanilla));
	glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(0.3f, 0.6f, 1.0f)));
	sp.render();

	// --- ALETAS (4 cubos escalados, alrededor de la base) ---
	for (int i = 0; i < 4; i++)
	{
		glm::mat4 aleta = model;
		aleta = glm::rotate(aleta, glm::radians(90.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
		aleta = glm::translate(aleta, glm::vec3(0.6f, -1.0f, 0.0f));
		aleta = glm::scale(aleta, glm::vec3(0.3f, 0.6f, 0.05f));
		DibujarInstancia(meshList[0], aleta, glm::vec3(1.0f, 0.0f, 0.0f), uniformModel, uniformColor); // rojo
	}
}
// 
//NUEVA FUNCION
/*void DibujarPiramideColor(glm::mat4 model, GLuint uniformModel, GLuint uniformColor)
{
	glm::vec3 colores[5] = {
		glm::vec3(1.0f, 0.0f, 0.0f), //FRENTE ROJO
		glm::vec3(0.0f, 1.0f, 0.0f), //IZQ. VERDE
		glm::vec3(1.0f, 1.0f, 0.0f), //TRASERA AMARILLO
		glm::vec3(1.0f, 0.0f, 1.0f), //DER. MAGENTA
		glm::vec3(0.0f, 0.0f, 1.0f), //BASE AZUL
	};
	for (int i = 0; i < 5; i++)
		DibujarInstancia(piramideCaras[i], model, colores[i], uniformModel, uniformColor);
}
// 
//Nueva función
void DibujarConjuntoCaras(glm::mat4 baseOrientada, GLuint uniformModel, GLuint uniformColor) {
	float superficie = 0.5f; //cara cubo
	//++++++++++Cuadrado azul++++++++++++++++++++++
	float ladoAzul = 0.75f, altoAzul = 0.07f;//Alto es para grosor
	for (int signo = -1; signo <= 1;signo += 2)
	{
		float y = signo * (superficie + altoAzul / 2.0f);
		glm::mat4 modelPieza = baseOrientada;
		modelPieza = glm::translate(modelPieza, glm::vec3(0.0f, y, 0.0f));
		modelPieza = glm::rotate(modelPieza, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelPieza = glm::scale(modelPieza, glm::vec3(ladoAzul, altoAzul, ladoAzul));
		DibujarInstancia(meshList[0], modelPieza, glm::vec3(0.0f, 0.0f, 1.0f), uniformModel, uniformColor);
	}
	//++++++++Cuadrado olivo+++++++++++++++++++
	float ladoOlivo = 0.55f, altoOlivo = 0.04f;
	for (int signo = -1; signo <= 1; signo += 2) {
		float y = signo * (superficie + altoAzul + altoOlivo / 2.0f);
		glm::mat4 modelPieza = baseOrientada;
		modelPieza = glm::translate(modelPieza, glm::vec3(0.0f, y, 0.0f));
		modelPieza = glm::rotate(modelPieza, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelPieza = glm::scale(modelPieza, glm::vec3(ladoOlivo, altoOlivo, ladoOlivo));
		DibujarInstancia(meshList[0], modelPieza, glm::vec3(0.5f, 0.5f, 0.0f), uniformModel, uniformColor);
	}
}
void DibujarPiramidesVertices(glm::mat4 model, GLuint uniformModel, GLuint uniformColor) {
	//8 COMBINACIONES DE SIGNO, QUE SON LAS ESUQINAS DEL CUBO
	float signos[8][3] = {
		{1, 1,1}, {1,1,-1}, {1,-1,1}, {1,-1,-1},
		{-1,1,1}, {-1,1,-1}, {-1,-1,1}, {-1,-1,-1}
	};


	glm::vec3 colores[4] = {
		glm::vec3(1.0f, 1.0f, 0.0f),//YELLOW
		glm::vec3(1.0f, 0.0f, 0.0f),//RED
		glm::vec3(0.0f, 1.0f, 0.0f),//GREEN
		glm::vec3(0.6f, 0.0f, 0.8f),//PURPLE
	};
	float radioVertice = 0.63f;//Acercar o alejar piezas del rombo azul(bajrlo)
	float baseVertice = 0.70f;//Para que la pieza se más grande y tape el ollo(subirlo)
	float altoVertice = 0.56f;
	for (int v = 0;v < 8; v++)
	{
		float sx = signos[v][0], sy = signos[v][1], sz = signos[v][2];
		int idx;
		if (sy > 0 && sx < 0) idx = 0;
		else if (sy > 0 && sx > 0) idx = 1;
		else if (sy < 0 && sx>0) idx = 2;
		else idx = 3;
		//Calculo la rot. necesaria para eje y local, (donde apunta el pico de la pir.) mire hacia esta esquina 
		float a = asin(sz / sqrt(3.0f));
		float b = atan2(-sx, sy);
		glm::mat4 modelPieza = model;
		modelPieza = glm::rotate(modelPieza, b, glm::vec3(0.0f, 0.0f, 1.0f));
		modelPieza = glm::rotate(modelPieza, a, glm::vec3(1.0f, 0.0f, 0.0f));
		modelPieza = glm::translate(modelPieza, glm::vec3(0.0f, radioVertice, 0.0f));
		modelPieza = glm::scale(modelPieza, glm::vec3(baseVertice, altoVertice, baseVertice));
		*/
		//DibujarInstancia(meshList[4], modelPieza, colores[v % 4], uniformModel, uniformColor);
	//}
//}
int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	//CrearPiramideCaras();
	//CrearPiramideColor();//RECUERDA INDICE 0 EM MESHCOLORLIST
	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(5, 1.0f);//índice 2 en MeshList
	CrearCono(25, 2.0f);//índice 3 en MeshList
	CrearPiramideCuadrangular();//índice 4 en MeshList
	CreateShaders();

	
	

	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);
	
	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f); //inicializar Color para enviar a variable Uniform;

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader(); //ACTIVO SHADER
		uniformModel = shaderList[0].getModelLocation();//OBTENGO UBICACIONES VALIDAS
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();
	

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));

		DibujarCohete(model, uniformModel, uniformColor);

		//PRUEBA PIRAMIDE COLOR POR CARA
		/*glm::mat4 modelPiramideColor = glm::mat4(1.0f);
		modelPiramideColor = glm::translate(modelPiramideColor, glm::vec3(2.0f, 0.0f, -4.0f));
		modelPiramideColor = glm::rotate(modelPiramideColor, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		
		DibujarPiramideColor(modelPiramideColor, uniformModel, uniformColor);
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiramideColor));
		// *********************+*/
		//Para irlos formando los prismas
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		
		/*DibujarConjuntoCaras(model, uniformModel, uniformColor);
		glm::mat4 modelX = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		DibujarConjuntoCaras(modelX, uniformModel, uniformColor);

		glm::mat4 modelZ = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		DibujarConjuntoCaras(modelZ, uniformModel, uniformColor);

		DibujarPiramidesVertices(model, uniformModel, uniformColor);*/

		//model = glm::mat4(1.0);
		//Traslación inicial para posicionar en -Z a los objetos
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//otras transformaciones para el objeto
		//model = glm::scale(model, glm::vec3(0.5f,0.5f,0.5f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		
		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		//color = glm::vec3(1.0f, 0.0f, 1.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		//meshList[0]->RenderMesh(); //dibuja cubo, pirámide triangular y pirámide base cuadrangular
		//meshList[3]->RenderMeshGeometry(); //dibuja las figuras geométricas cilindro, cono
		//TENER EN CUENTA sp.render(); //dibuja esfera

		/*
		//ejercicio: Instanciar primitivas geométricas para recrear las figuras 2 y 3 de la práctica pasada en 3D,
		//se requiere que exista piso
		*/

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}

	
		