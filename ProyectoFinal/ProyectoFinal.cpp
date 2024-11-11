#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "modelAnim.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-100.0f, 50.0f, 50.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;
float movCamera = 0.0f;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(-95.0f, 1.0f, -150.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

bool active;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
float tiempo;
float tiempo2;
float tiempo3;
bool animActivPostales = false;
bool animActivDinero = false;
bool finAnimDinero = false;
bool animActivPuertaMesa1 = false;
bool animActivPuertaMesa2 = false;
bool animActivAbrir1 = false;
bool animActivCerrar1 = false;
bool animActivAbrir2 = false;
bool animActivCerrar2 = false;
bool activAnimAscensor = false;
float rotPostales = 0.0f;
float traslacionDinero = 0.0f;
float traslacionPuertaMesa1 = 0.0f;
float rotPuerta1 = 0.0f;
float rotPuerta2 = 0.0f;
float trasAscensor = 0.0f;
float traslPuertaArriba1 = 0.0f;
float traslPuertaArriba2 = 0.0f;
float trasPuertaAbajo1 = 0.0f;
float trasPuertaAbajo2 = 0.0f;
float trasPuertaAscIzq = 0.0f;
float trasPuertaAscDer = 0.0f;
enum EstadoPuerta { CERRADA, ABRIENDO, ABIERTA, CERRANDO };
enum EstadoAscensor { ABAJO, SUBIENDO, ARRIBA, BAJANDO };

EstadoPuerta estadoPuertaAbajo = CERRADA;
EstadoPuerta estadoPuertaArriba = CERRADA;
EstadoAscensor estadoAscensor = ABAJO;

float tiempoAnimacion = 0.0f;
// Keyframes
float posX =PosIni.x, posY = PosIni.y, posZ = PosIni.z;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotRodIzq;
	float rotInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(-95.0, 2.0 ,-250),
	glm::vec3(-65,29.5,-100),
	glm::vec3(-40,29.5,-100),
	glm::vec3(-15, 29.5,-100)
};

glm::vec3 LightP1;




void saveFrame(void)
{

	printf("posx %f\n", posX);
	
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;
	
	//KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	//rotRodIzq = KeyFrame[0].rotRodIzq;

}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
	
	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;

}




int main()
{
	// Init GLFW
	glfwInit();




	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final Equipo 6", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.frag");
	Shader animShaderA1("Shaders/animAgua.vs", "Shaders/animAgua.frag");
	Shader animShaderA2("Shaders/animFuente.vs", "Shaders/animFuente.frag");
	Shader animShaderResorte("Shaders/animResorte.vs", "Shaders/animResorte.frag");

	/*Model BotaDer((char*)"Models/Personaje/bota.obj");
	Model PiernaDer((char*)"Models/Personaje/piernader.obj");
	Model PiernaIzq((char*)"Models/Personaje/piernaizq.obj");
	Model Torso((char*)"Models/Personaje/torso.obj");
	Model BrazoDer((char*)"Models/Personaje/brazoder.obj");
	Model BrazoIzq((char*)"Models/Personaje/brazoizq.obj");
	Model Cabeza((char*)"Models/Personaje/cabeza.obj");*/

	//Objetos 
	Model postalBase((char*)"Models/ExpositorPostales/ExpositorPostal.obj");
	Model expositorPostales((char*)"Models/ExpositorPostales/ExpositorPostales.obj");
	Model postales((char*)"Models/ExpositorPostales/Postales.obj");
	Model registrarDinero((char*)"Models/mesaCaja/dinero_registradora.obj");
	Model mesaRegistradora((char*)"Models/mesaCaja/mesaRegistradora.obj");
	Model puerta1Registradora((char*)"Models/mesaCaja/puerta1_registradora.obj");
	Model puerta2Registradora((char*)"Models/mesaCaja/puerta2_registradora.obj");
	Model ventanaMesa((char*)"Models/mesaCaja/ventana_mesa.obj");
	Model adornoPiramide((char*)"Models/Piramide/piramideAdorno.obj");
	Model adornoCraneo((char*)"Models/Craneo/artesania.obj");
	Model estante1((char*)"Models/Estante1/EstanteGuayaberas.obj");
	Model estante2((char*)"Models/Estante2/EstanteLibros.obj");
	Model estante3((char*)"Models/Estante3/EstanteArtesanias.obj");
	Model guayabera1_1((char*)"Models/Guayabera 1_1/Guayabera1_1.obj");
	Model guayabera1_2((char*)"Models/Guayabera1_2/Guayabera1_2.obj");
	Model guayabera1_3((char*)"Models/Guayabera1_3/Guayabera1_3.obj");
	Model guayabera1_4((char*)"Models/Guayabera1_4/Guayabera1_4.obj");
	Model guayabera1_5((char*)"Models/Guayabera1_5/Guayabera1_5.obj");
	Model guayabera1_6((char*)"Models/Guayabera1_6/Guayabera1_6.obj");
	Model guayabera2_1((char*)"Models/Guayabera2_1/Guayabera2_1.obj");
	Model guayabera2_2((char*)"Models/Guayabera2_2/Guayabera2_2.obj");
	Model guayabera2_3((char*)"Models/Guayabera2_3/Guayabera2_3.obj");
	Model guayabera2_4((char*)"Models/Guayabera2_4/Guayabera2_4.obj");
	Model guayabera2_5((char*)"Models/Guayabera2_5/Guayabera2_5.obj");
	Model guayabera2_6((char*)"Models/Guayabera2_6/Guayabera2_6.obj");
	Model libros1((char*)"Models/Libros1/libros1.obj");
	Model libros2((char*)"Models/Libros2/libros2.obj");
	Model libros3((char*)"Models/Libros3/libros3.obj");
	Model libros4((char*)"Models/Libros4/Libros4.obj");
	Model libros5((char*)"Models/Libros5/Libros5.obj");
	Model libros6((char*)"Models/Libros6/Libros6.obj");
	Model libros7((char*)"Models/Libros7/Libros7.obj");
	Model libros8((char*)"Models/Libros8/Libros8.obj");


	//Fachada Restaurante
	Model fachadaRestaurante((char*)"Models/FachadaRestaurante/FachadaRestaurante.obj");
	Model ventanas((char*)"Models/FachadaRestaurante/ventanasRestaurante.obj");
	Model ascensor((char*)"Models/FachadaRestaurante/ascensor.obj");
	Model puertaAnimada1((char*)"Models/FachadaRestaurante/puertaAnimada1.obj");
	Model puertaAnimada2((char*)"Models/FachadaRestaurante/puertaAnimada2.obj");
	Model puertaAscensorAbajo1((char*)"Models/FachadaRestaurante/puertaAscensorAbajo1.obj");
	Model puertaAscensorAbajo2((char*)"Models/FachadaRestaurante/puertaAscensorAbajo2.obj");
	Model puertaAscensorArriba1((char*)"Models/FachadaRestaurante/puertaAscensorArriba1.obj");
	Model puertaAscensorArriba2((char*)"Models/FachadaRestaurante/puertaAscensorArriba2.obj");
	Model puertaAscensorDerecha((char*)"Models/FachadaRestaurante/puertaAscensorDerecha.obj");
	Model puertaAscensorIzquierda((char*)"Models/FachadaRestaurante/puertaAscensorIzquierda.obj");
	Model piso((char*)"Models/Piso/Piso.obj");
	Model focos((char*)"Models/Foco/Focos.obj");

	//Animaciones Complejas

	Model baseFuente((char*)"Models/Fuente/baseFuente.obj");
	Model baseAgua((char*)"Models/Fuente/AguaBase.obj");
	Model aguaFuente((char*)"Models/Fuente/AguaFuente.obj");
	Model postal((char*)"Models/AnimPostal/Postal.obj");
	Model resorte((char*)"Models/AnimPostal/resorte.obj");
	Model resorteInanimado((char*)"Models/AnimPostal/resorteInanimado.obj");
	Model personaTorso((char*)"Models/Persona/torso.obj");
	Model personaBrazo1((char*)"Models/Persona/brazo1.obj");
	Model personaBrazo2((char*)"Models/Persona/brazo2.obj");
	Model personaMuslo1((char*)"Models/Persona/muslo1.obj");
	Model personaMuslo2((char*)"Models/Persona/muslo2.obj");
	Model personaPierna1((char*)"Models/Persona/pierna1.obj");
	Model personaPierna2((char*)"Models/Persona/pierna2.obj");

	// Build and compile our shader program

	//Inicialización de KeyFrames
	
	for(int i=0; i<MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].rotInc = 0;
	}



	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};


	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");
	
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();


		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.8f, 0.7f, 0.3f); 
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.9f, 0.5f, 0.2f); 
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 0.8f, 0.5f); 



		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.07f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.017f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.07f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.017f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.80f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.80f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.07f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.017f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/


		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp



		//Carga de modelo 
		//Personaje
		
		
		view = camera.GetViewMatrix();
		glm::mat4 model(1);
		tmp = model = glm::translate(model, glm::vec3(0, 1, 0));

		/*
		model = glm::translate(model,glm::vec3(posX,posY,posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Torso.Draw(lightingShader);

	

		//Pierna Izq
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		PiernaDer.Draw(lightingShader);
	//	Pie Izq
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BotaDer.Draw(lightingShader);

		//Pierna Der
		view = camera.GetViewMatrix();
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);

		PiernaIzq.Draw(lightingShader);
		//Pie Der
		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BotaDer.Draw(lightingShader);

		//Brazo derecho
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BrazoDer.Draw(lightingShader);

	//	Brazo Izquierdo
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BrazoIzq.Draw(lightingShader);

		//Cabeza
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		Cabeza.Draw(lightingShader);
	
		//Traslucidez

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencia"), 0.0);
		//objTras.Draw(lightingShader);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(0);*/


		//FOCOS
		// 		
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::translate(model, glm::vec3(30.0f, 27.5f, 50.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		focos.Draw(lightingShader);
		model = glm::translate(model, glm::vec3(250.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		focos.Draw(lightingShader);
		model = glm::translate(model, glm::vec3(250.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		focos.Draw(lightingShader);


		//Objetos inanimados
		//PIRAMIDE
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(4.5f, 0.17f, 5.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.3f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(-0.2f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(-0.2f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(-0.2f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);




		// #####  CRANEO ####

		//Estante 1 
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(5.95f, 0.09f, 4.1f));
		model = glm::scale(model, glm::vec3(0.65f, 0.65f, 0.65f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.64f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(-0.64f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.45f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.64f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.53f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(-0.64f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);


		//Estante 2
		model = glm::translate(model, glm::vec3(1.6f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.18f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		//

		model = glm::translate(model, glm::vec3(0.0f, -0.23f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(-0.15f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(-0.18f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(-0.15f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		//
		model = glm::translate(model, glm::vec3(0.0f, -0.23f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.18f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		//

		model = glm::translate(model, glm::vec3(-0.0f, -0.25f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(-0.15f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(-0.18f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(-0.15f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoPiramide.Draw(lightingShader);


		// 3
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(8.2f, 0.09f, 4.1f));
		model = glm::scale(model, glm::vec3(0.65f, 0.65f, 0.65f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.64f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(-0.64f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.45f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.64f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.53f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(-0.64f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		adornoCraneo.Draw(lightingShader);

		// ####################### LIBROS  #####################
		//lIBROS1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(9.6f, 1.025f, 4.15f));
		model = glm::scale(model, glm::vec3(0.85f, 0.85f, 0.85f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros1.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.425f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros1.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, -0.405f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros2.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.425f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros2.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros3.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.425f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros3.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, -0.35f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros4.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.425f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros4.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.875f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros5.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.425f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros5.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.35f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros6.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.425f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros6.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros7.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.405f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros8.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.425f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		libros8.Draw(lightingShader);


		//Estante Guayaberas
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(2.8f, 0.07f, 3.7f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		estante1.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		estante1.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		estante1.Draw(lightingShader);

		//Estante Artesnaias
		model = glm::translate(model, glm::vec3(1.5f, -0.02f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		estante3.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		estante3.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		estante3.Draw(lightingShader);

		//Estante Libros
		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		estante2.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		estante2.Draw(lightingShader);

		//GUAYABERAS
		//Gayaberas1_1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(2.45f, 0.475f, 4.175f));
		model = glm::scale(model, glm::vec3(2.0f, 1.4f, 0.95f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_1.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_1.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_1.Draw(lightingShader);

		//Guayabera 1_2

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_2.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_2.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_2.Draw(lightingShader);

		//Guayabera 1_3

		model = glm::translate(model, glm::vec3(0.22f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_3.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_3.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_3.Draw(lightingShader);

		//Guayabera 1_4

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_4.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_4.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_4.Draw(lightingShader);

		//Guayabera 1_5

		model = glm::translate(model, glm::vec3(0.22f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_5.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_5.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_5.Draw(lightingShader);

		//Guayabera 1_6

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_6.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_6.Draw(lightingShader);

		model = glm::translate(model, glm::vec3(0.07f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera1_6.Draw(lightingShader);


		//Guayabera 2_1

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(2.6f, -0.12f, 4.175f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera2_1.Draw(lightingShader);

		//Guayabera 2_2
		model = glm::translate(model, glm::vec3(0.4f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera2_2.Draw(lightingShader);

		//Guayabera 2_3
		model = glm::translate(model, glm::vec3(0.725f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera2_3.Draw(lightingShader);

		//Guayabera 2_4
		model = glm::translate(model, glm::vec3(0.4f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera2_4.Draw(lightingShader);

		//Guayabera 2_5
		model = glm::translate(model, glm::vec3(0.725f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera2_5.Draw(lightingShader);

		//Guayabera 2_6
		model = glm::translate(model, glm::vec3(0.4f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		guayabera2_6.Draw(lightingShader);

		// ########################   FACHADA  #################################################
		// Fachada Final
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -250.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		fachadaRestaurante.Draw(lightingShader);

		//Piso
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.20f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 1.0f, 0.25f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		piso.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -250.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		//Ventanas
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.5);
		ventanas.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		baseFuente.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(4.5f, 0.51f, 5.65f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postal.Draw(lightingShader);

		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		resorteInanimado.Draw(lightingShader);


		// ################################   ANIMACIONES SIMPLES  ############################
		//  #################### Animaciones simples de objetos #################
		//expositor de POSTALES
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(7.0f, 0.35f, 5.65f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postalBase.Draw(lightingShader);

		model = glm::rotate(model, glm::radians(rotPostales), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		expositorPostales.Draw(lightingShader);
		//1
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//2
		model = glm::translate(model, glm::vec3(0.0f, 0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//3
		model = glm::translate(model, glm::vec3(0.0f, 0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//2_1
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//2_2
		model = glm::translate(model, glm::vec3(0.0f, -0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//2_3
		model = glm::translate(model, glm::vec3(0.0f, -0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//3_1
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//3_2
		model = glm::translate(model, glm::vec3(0.0f, 0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//3_3
		model = glm::translate(model, glm::vec3(0.0f, 0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//4_1
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//4_2
		model = glm::translate(model, glm::vec3(0.0f, -0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//4_3
		model = glm::translate(model, glm::vec3(0.0f, -0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);

		//expositor de POSTALES 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(8.5f, 0.35f, 5.65f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postalBase.Draw(lightingShader);

		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		expositorPostales.Draw(lightingShader);
		//1
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//2
		model = glm::translate(model, glm::vec3(0.0f, 0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//3
		model = glm::translate(model, glm::vec3(0.0f, 0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//2_1
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//2_2
		model = glm::translate(model, glm::vec3(0.0f, -0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//2_3
		model = glm::translate(model, glm::vec3(0.0f, -0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//3_1
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//3_2
		model = glm::translate(model, glm::vec3(0.0f, 0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//3_3
		model = glm::translate(model, glm::vec3(0.0f, 0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//4_1
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//4_2
		model = glm::translate(model, glm::vec3(0.0f, -0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);
		//4_3
		model = glm::translate(model, glm::vec3(0.0f, -0.475f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		postales.Draw(lightingShader);

		//Registradora

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(4.5f, 0.15f, 5.65f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::translate(model, glm::vec3(traslacionDinero, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		registrarDinero.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(4.5f, 0.15f, 5.65f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		mesaRegistradora.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(4.5f, 0.15f, 5.65f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, traslacionPuertaMesa1));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		puerta1Registradora.Draw(lightingShader);


		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(4.5f, 0.15f, 5.65f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		puerta2Registradora.Draw(lightingShader);


		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.25);
		ventanaMesa.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		// ##################### Animaciones simples Fachada ################################
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -250.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(-1.735f, 0.0f, 6.665f));
		model = glm::rotate(model, glm::radians(rotPuerta1), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		puertaAnimada1.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -250.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(1.75f, 0.0f, 6.68f));
		model = glm::rotate(model, glm::radians(rotPuerta2), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		puertaAnimada2.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -250.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(11.25f, 0.05f, 1.75f));
		model = glm::translate(model, glm::vec3(0.0f, trasAscensor, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		ascensor.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -250.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(10.650f, 3.765f, 1.41f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, traslPuertaArriba1));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		puertaAscensorArriba1.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -250.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(10.650f, 3.765f, 2.075f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, traslPuertaArriba2));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		puertaAscensorArriba2.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -250.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(10.650f, 0.75f, 1.415f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, trasPuertaAbajo1));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		puertaAscensorAbajo1.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -250.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(10.650f, 0.75f, 2.09f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, trasPuertaAbajo2));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		puertaAscensorAbajo2.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -250.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(10.825f, 0.785f, 1.485f));
		model = glm::translate(model, glm::vec3(0.0f, trasAscensor, trasPuertaAscIzq));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		puertaAscensorIzquierda.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -250.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(10.825f, 0.785f, 2.0f));
		model = glm::translate(model, glm::vec3(0.0f, trasAscensor, trasPuertaAscDer));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		puertaAscensorDerecha.Draw(lightingShader);


		glBindVertexArray(0);
		// #####################################     ANIMACIONES   COMPLEJAS #########################################

		animShaderA1.Use();
		tiempo = glfwGetTime();
		modelLoc = glGetUniformLocation(animShaderA1.Program, "model");
		viewLoc = glGetUniformLocation(animShaderA1.Program, "view");
		projLoc = glGetUniformLocation(animShaderA1.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(animShaderA1.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(animShaderA1.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65);
		glUniform1f(glGetUniformLocation(animShaderA1.Program, "time"), (tiempo));
		baseAgua.Draw(animShaderA1);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(animShaderA1.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);


		glBindVertexArray(0);

		animShaderA2.Use();
		tiempo2 = glfwGetTime();


		modelLoc = glGetUniformLocation(animShaderA2.Program, "model");
		viewLoc = glGetUniformLocation(animShaderA2.Program, "view");
		projLoc = glGetUniformLocation(animShaderA2.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(animShaderA2.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(animShaderA2.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.5);
		glUniform1f(glGetUniformLocation(animShaderA2.Program, "time"), (tiempo2));
		aguaFuente.Draw(animShaderA2);
		glDisable(GL_BLEND);  //Desactiva el canal alfa
		glUniform4f(glGetUniformLocation(animShaderA2.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);


		glBindVertexArray(0);


		//Anime Resorte
		animShaderResorte.Use();
		tiempo3 = glfwGetTime() * 5;
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(animShaderResorte.Program, "model");
		viewLoc = glGetUniformLocation(animShaderResorte.Program, "view");
		projLoc = glGetUniformLocation(animShaderResorte.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-95.0f, 2.0f, -150.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::translate(model, glm::vec3(4.5f, 0.55f, 5.65f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(glGetUniformLocation(animShaderResorte.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(animShaderResorte.Program, "time"), (tiempo3));
		resorte.Draw(animShaderResorte);

		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default




		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}


void animacion()
{

		//Movimiento del personaje

		if (play)
		{
			if (i_curr_steps >= i_max_steps) //end of animation between frames?
			{
				playIndex++;
				if (playIndex>FrameIndex - 2)	//end of total animation?
				{
					printf("termina anim\n");
					playIndex = 0;
					play = false;
				}
				else //Next frame interpolations
				{
					i_curr_steps = 0; //Reset counter
									  //Interpolation
					interpolation();
				}
			}
			else
			{
				//Draw animation
				posX += KeyFrame[playIndex].incX;
				posY += KeyFrame[playIndex].incY;
				posZ += KeyFrame[playIndex].incZ;

				//rotRodIzq += KeyFrame[playIndex].rotInc;

				i_curr_steps++;
			}

		}
	}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex<MAX_FRAMES)
		{
			saveFrame();
		}

		rot =-25.0f;//Variable que maneja el giro de la camara

	}


	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{
		
		movCamera = 0.01f;//Manda una velocidad de 0.01 a la camara automatica

	}
/*/
	if (keys[GLFW_KEY_2])
	{
		if (rotRodIzq<80.0f)
			rotRodIzq += 1.0f;
			
	}

	if (keys[GLFW_KEY_3])
	{
		if (rotRodIzq>-45)
			rotRodIzq -= 1.0f;
		
	}*/
	//ANIMACION POSTALES
	if (animActivPostales)
	{
		if (rotPostales < 360.0f)
			rotPostales += 1.0f;

		if (rotPostales >= 360.0f) {

			animActivPostales = false;
			rotPostales = 0.0f;
		}
	}
	//ANIMACION CAJA REGISTRADORA
	if (animActivDinero)
	{
		if (traslacionDinero > -0.3f && !finAnimDinero)
		{
			traslacionDinero -= 0.01f;

			if (traslacionDinero <= -0.3f)
			{
				finAnimDinero = true;
				//std::this_thread::sleep_for(std::chrono::seconds(1));
			}

		}
		if (finAnimDinero)
		{
			if (traslacionDinero < 0.0f)
			{
				traslacionDinero += 0.01f;
				if (traslacionDinero >= 0.0f)
				{
					finAnimDinero = false;
					animActivDinero = false;
				}
			}
		}
	}

	//ANIMACION PUERTA MESA 
	if (animActivPuertaMesa1)
	{
		if (traslacionPuertaMesa1 < 0.35f && !animActivPuertaMesa2)
		{
			traslacionPuertaMesa1 += 0.01f;

			if (traslacionPuertaMesa1 >= 0.35f)
			{
				animActivPuertaMesa2 = true;
				//std::this_thread::sleep_for(std::chrono::seconds(1));
			}

		}
		if (animActivPuertaMesa2)
		{
			if (traslacionPuertaMesa1 > 0.0f)
			{
				traslacionPuertaMesa1 -= 0.01f;

				if (traslacionPuertaMesa1 <= 0.0f)
				{
					animActivPuertaMesa1 = false;
					animActivPuertaMesa2 = false;
				}
			}
		}

	}

	//ANIMACION PUERTA PRINCIPAL 1 
	if (animActivAbrir1)
	{
		if (rotPuerta1 < 80.0f)
		{
			rotPuerta1 += 0.5f;

			if (rotPuerta1 >= 80.0f)
			{
				animActivAbrir1 = false;
			}

		}
	}
	if (animActivCerrar1)
	{
		if (rotPuerta1 > 0.0f)
		{
			rotPuerta1 -= 0.5f;

			if (rotPuerta1 <= 0.0f)
			{
				animActivCerrar1 = false;
			}

		}
	}
	//ANIMACION PUERTA PRINCIPAL 2
	if (animActivAbrir2)
	{
		if (rotPuerta2 > -80.0f)
		{
			rotPuerta2 -= 0.5f;

			if (rotPuerta2 <= -80.0f)
			{
				animActivAbrir2 = false;
			}

		}
	}
	if (animActivCerrar2)
	{
		if (rotPuerta2 < 0.0f)
		{
			rotPuerta2 += 0.5f;

			if (rotPuerta2 >= 0.0f)
			{
				animActivCerrar2 = false;
			}

		}
	}
	

	//Mov Personaje
	if (keys[GLFW_KEY_H])
	{
		posZ += 1;
	}

	if (keys[GLFW_KEY_Y])
	{
		posZ -= 1;
	}

	if (keys[GLFW_KEY_G])
	{
		posX -= 1;
	}

	if (keys[GLFW_KEY_J])
	{
		posX += 1;
	}




	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (keys[GLFW_KEY_Z]) {
		animActivPostales = true;
	}

	if (keys[GLFW_KEY_X]) {
		animActivDinero = true;
	}

	if (keys[GLFW_KEY_C]) {
		animActivPuertaMesa1 = true;
	}

	if (keys[GLFW_KEY_V]) {
		animActivAbrir1 = true;
	}

	if (keys[GLFW_KEY_B]) {
		animActivCerrar1 = true;
	}

	if (keys[GLFW_KEY_N]) {
		animActivAbrir2 = true;
	}

	if (keys[GLFW_KEY_M]) {
		animActivCerrar2 = true;
	}

	if (keys[GLFW_KEY_P]) {
		activAnimAscensor = true;
	}

}
