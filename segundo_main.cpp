#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <time.h>
#include <stdlib.h>
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;
GLuint uniformColor;
//CAMBIO DE COLOR: DECLARACION DE VARIABLES
float CR = 1.0f;//ROJO
float CV = 0.0f;//VERDE
float CA = 0.0f;//AZUL
double TA = 0.0;//TIEMPO ATRAS
//LENGUAJE DE SHADER (SOMBRAS) GLSL
//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos,1.0); 			\n\
}";

//Fragment Shader
//RECIBIR COLOR Y DAR SALIDA DE COLORES
static const char* fShader = "						\n\
#version 330										\n\
out vec4 fragColor;					\n\
uniform vec4 uColor;\n\
void main()											\n\
{													\n\
fragColor = uColor; 			\n\
}";




//"						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0,1.0,0.0,1.0);	 			\n\
}";
//CAMBIO DE COLOR A AMARILLO LAS FIGURAS (color anterior --->)color = vec4(1.0f,0.0f,0.0f,1.0f);



void CrearLETRAS()
{
	GLfloat vertices[] = {
		//PARA LA LLETRA A 
		// IZQU.DIAGONAL
		-0.25f,-0.4f,0,   -0.20f,-0.4f,0,   -0.175f,0.1f,0,
		-0.20f,-0.4f,0,   -0.15f,0.1f,0,    -0.175f,0.1f,0,
		// DER. DIAGONAL
		-0.10f,-0.4f,0,   -0.05f,-0.4f,0,   -0.125f,0.1f,0,
		-0.05f,-0.4f,0,   -0.075f,0.1f,0,   -0.125f,0.1f,0,
		// PARA EL CENTRO
		-0.19f,-0.15f,0,  -0.11f,-0.15f,0,  -0.11f,-0.2f,0,
		-0.19f,-0.15f,0,  -0.11f,-0.2f,0,   -0.19f,-0.2f,0,
		//PARA ARRIBA

		//PARA LA LETRA G
		// BARRA SUPERIOR
		0.00f, 0.04f,0,   0.22f, 0.04f,0,   0.22f, 0.10f,0,
		0.00f, 0.04f,0,   0.22f, 0.10f,0,   0.00f, 0.10f,0,
		// BARRA IZQ
		0.00f,-0.40f,0,   0.06f,-0.40f,0,   0.06f, 0.10f,0,
		0.00f,-0.40f,0,   0.06f, 0.10f,0,   0.00f, 0.10f,0,
		// BARRA INF.
		0.00f,-0.40f,0,   0.22f,-0.40f,0,   0.22f,-0.34f,0,
		0.00f,-0.40f,0,   0.22f,-0.34f,0,   0.00f,-0.34f,0,
		// CURVITA: barra vertical corta (abajo-derecha)
		0.16f,-0.40f,0,   0.22f,-0.40f,0,   0.22f,-0.12f,0,
		0.16f,-0.40f,0,   0.22f,-0.12f,0,   0.16f,-0.12f,0,
		// CURVITA: barra horizontal que cierra hacia el centro
		0.11f,-0.18f,0,   0.22f,-0.18f,0,   0.22f,-0.12f,0,
		0.11f,-0.18f,0,   0.22f,-0.12f,0,   0.11f,-0.12f,0,
		//PARA LA LETRA C
		//BARRA SUPERIOR
		0.28f, 0.04f,0,   0.46f, 0.04f,0,   0.46f, 0.10f,0,
		0.28f, 0.04f,0,   0.46f, 0.10f,0,   0.28f, 0.10f,0,
		//BARRA IZQ
		0.28f,-0.40f,0,   0.34f,-0.40f,0,   0.34f, 0.10f,0,
		0.28f,-0.40f,0,   0.34f, 0.10f,0,   0.28f, 0.10f,0,
		//BARRA INFERIOR
		0.28f,-0.40f,0,   0.46f,-0.40f,0,   0.46f,-0.34f,0,
		0.28f,-0.40f,0,   0.46f,-0.34f,0,   0.28f,-0.34f,0,
	};
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es est�tico pues no se modificar�n los valores

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	glEnableVertexAttribArray(0);
	//agregar valores a v�rtices y luego declarar un nuevo vertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Funci�n para agregar los shaders a la tarjeta gr�fica

//the Program recibe los datos de theShader

{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el c�digo
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevenci�n de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el c�digo a la tarjeta gr�fica
}

void CompileShaders() {
	shader = glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gr�fica
	//verificaciones y prevenci�n de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	//****  LAS SIGUIENTES 4 L�NEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSI�N DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica 02", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	//Llamada a las funciones creadas antes del main
	CrearLETRAS();
	CompileShaders();
	uniformColor = glGetUniformLocation(shader, "uColor");
	srand((unsigned int)time(NULL));
	TA = glfwGetTime();//TIEMPO DE INCIIO PARA TRAPECIO Y ROMBO


	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//PROGRAMACI�N PARA EL CAMBIO DE COLORES
		double TAH = glfwGetTime();//Tiempo de ahora
		if (TAH - TA >= 2.0)
		{
			CR = (float)rand() / RAND_MAX;
			CV = (float)rand() / RAND_MAX;
			CA = (float)rand() / RAND_MAX;
			TA = TAH;
		}

		//Limpiar la ventana
		glClearColor(CR, CV, CA, 1.0f);
		//glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		//RECUERDA QUE SI REQUIERES M�S TRIANGULOS CAMBIES EL ULTIMO VALOR
		//glDrawArrays(GL_TRIANGLES, 0, 66);//pARA VISUALISAR CUANTOS TRIANGULOS SE VERAN
		glUniform4f(uniformColor, 1.0f, 0.0f, 0.0f, 1.0f);//rojo
		glDrawArrays(GL_TRIANGLES, 0, 18);

		glUniform4f(uniformColor, 0.0f, 1.0f, 0.0f, 1.0f);//VERDE
		glDrawArrays(GL_TRIANGLES, 18, 30);

		glUniform4f(uniformColor, 0.0f, 0.0f, 1.0f, 1.0f);//AZUL
		glDrawArrays(GL_TRIANGLES, 48, 18);

		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

		//NO ESCRIBIR NINGUNA L�NEA DESPU�S DE glfwSwapBuffers(mainWindow); 
	}


	return 0;
}