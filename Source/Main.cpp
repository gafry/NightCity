#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <libv2\v2mplayer.h>
#include <libv2\libv2.h>

#include "Shader.h"
#include "Shaders.h"
#include "Camera.h"
#include "House.h"
#include "Flats.h"
#include "Skyscraper.h"
#include "Road.h"
#include "Skybox.h"
#include "Generator.h"

#define PAPRSKY FALSE

#define SEED 4201

vector<House> _housesArray;
vector<Flats> _flatsArray;
vector<Skyscraper> _skyscrapersArray;
vector<Road> _roadsArray;

static V2MPlayer player;
extern "C" const sU8 theTune[];

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
int getLightPosition(float objx, float objy, float objz, float *modelview, float *projection, int *viewport, float *windowCoordinate);
void createHouses();
void createFlats();

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0, 0, 0));
// neni potreba ve finalni verzi
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Texture * Texture::_instance = nullptr;

#ifdef NDEBUG
extern "C" void mainCRTStartup()
#else
void main()
#endif
{
	// GLFW inicializace, nastaveni verze OpenGL na 4.5
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// vytvoøení okna
	//GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "NightCity", NULL, NULL);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "NightCity", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return ;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// pri finalni verzi neni potreba
	//glfwSetCursorPosCallback(window, mouse_callback);

	// pri finalni verzi neni potreba
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD inicializace
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return ;
	}
	
	// random SEED
	srand(SEED);

	// inicializace hudby
	player.Init();
	player.Open(theTune);
	dsInit(player.RenderProxy, &player, GetForegroundWindow());

	// nastaveni projekcni matice
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 50000.0f);

	// vytvoreni shaderu, kody shaderu jsou ulozeny v Shaders.h
	Shader volumetricShader(volumetricVertexShader, volumtricFragmentShader);
	Shader ordinaryShader(ordinaryVertexShader, ordinaryFragmentShader);
	Shader skyboxShader(skyboxVertexShader, skyboxFragmentShader);
	Shader instancingShader(instancingVertexShader, instancingFragmentShader);
	Shader ordinaryBlackShader(ordinaryBlackVertexShader, ordinaryBlackFragmentShader);
	Shader instancingBlackShader(instancingBlackVertexShader, instancingBlackFragmentShader);

	// nastaveni projekcnich matic do shaderu
	ordinaryShader.use();
	ordinaryShader.setMat4("projection", projection);

	ordinaryBlackShader.use();
	ordinaryBlackShader.setMat4("projection", projection);

	instancingShader.use();
	instancingShader.setMat4("projection", projection);

	instancingBlackShader.use();
	instancingBlackShader.setMat4("projection", projection);

	skyboxShader.use();
	skyboxShader.setMat4("projection", projection);
	skyboxShader.setInt("skybox", 0);

	volumetricShader.use();
	volumetricShader.setMat4("projection", projection);

	volumetricShader.use();
	volumetricShader.setInt("screenTexture", 0);
	volumetricShader.setInt("coloredScreenTexture", 1);

	// data pro post-process zpracovani volumetrickych paprsku,
	// potrebujeme k tomu dva framebuffery, jeden pro occlusion mapu
	// a druhy s normalnim vykreslenim sceny
	float quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	// screen VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// cernobily buffer
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// textura
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// RBO
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); 

	// barevny buffer
	unsigned int framebuffer2;
	glGenFramebuffers(1, &framebuffer2);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
	// textura
	unsigned int textureColorbuffer2;
	glGenTextures(1, &textureColorbuffer2);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer2, 0);
	// RBO
	unsigned int rbo2;
	glGenRenderbuffers(1, &rbo2);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo2);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo2);

	// bind vychoziho bufferu
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// vytvoreni skyboxu
	Skybox skybox;

	// vytvoreni textur
	Texture * texture = Texture::getInstance();

	// vytvoreni silnic, bloku a budov
	createHouses();
	createFlats();
	Generator generator(2.0f);
	generator.generate();

	// nastaveni VAO pro vsechny domy
	for (int q = 0; q < _housesArray.size(); q++) {
		_housesArray[q].processVAO();
	}

	// nastaveni VAO pro vsechny panelove domy
	for (int q = 0; q < _flatsArray.size(); q++) {
		_flatsArray[q].processVAO();
	}

	// nastaveni VAO pro vsechny silnice
	for (int q = 0; q < _roadsArray.size(); q++) {
		_roadsArray[q].processVAO();
	}

	// promenne pro FPS a ms/frame pocitadlo, neni potreba pri finalni verzi
	/*float lastTime = glfwGetTime();
	int nbFrames = 0;*/

	// inicializace kamery a pomocnych promennych
	// pro synchronizaci pohybu kamery s hudbou
	camera.init();
	bool musicStarted = false;
	float musicTimeStart = glfwGetTime();
	float endingTime = 0.0f;
	int isRunning = 2;

	// vykreslovaci smycka
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();

		// pocitadlo FPS a ms/frame, neni potreba pri finalni verzi
		//nbFrames++;
		//if (currentTime - lastTime >= 1.0f) { 
		//	//printf("%f ms/frame    %.0f FPS\n", 1000.0f / float(nbFrames), 1000 / (1000.0f / float(nbFrames)));
		//	nbFrames = 0;
		//	lastTime += 1.0f;
		//}

		// spocitani delty
		if (lastFrame != 0.0f) {
			deltaTime = currentTime - lastFrame;
			lastFrame = currentTime;
		}
		if (deltaTime < 0.002f || deltaTime > 0.1f)
		{
			deltaTime = 0.002f;
			lastFrame = currentTime;
		}

		// update pozice kamery podle casu hudby, na zacatku jsou vlozeny
		// dve cerne pauzy podle vyvoje hudby
		float musicTime = currentTime - musicTimeStart;
		if (isRunning != 3) 
		{
			/*if (musicTime > 2 && musicTime < 7.7f)
			{
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glfwSwapBuffers(window);
				glfwPollEvents();

				continue;
			}
			else if (musicTime >= 7.7f && musicTime <= 9.4f)
			{
				camera.setPointCounter(1);
				isRunning = camera.update(deltaTime);
			}
			else if (musicTime > 9.4f && musicTime < 15)
			{
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glfwSwapBuffers(window);
				glfwPollEvents();

				continue;
			}
			else if (musicTime >= 15 && musicTime < 22.35f)
			{
				camera.setPointCounter(2);
				isRunning = camera.update(deltaTime);
			}
			else if (musicTime >= 22.35f && musicTime < 29.7f)
			{
				camera.setPointCounter(3);
				isRunning = camera.update(deltaTime);
			}
			else if (musicTime >= 29.7f && musicTime < 31)
			{
				camera.setPointCounter(4);
				isRunning = camera.update(deltaTime);
			}
			else if (musicTime >= 37 && musicTime < 38)
			{
				camera.setPointCounter(5);
				isRunning = camera.update(deltaTime);
			}
			else if (musicTime >= 44.3f && musicTime < 46)
			{
				camera.setPointCounter(6);
				isRunning = camera.update(deltaTime);
			}
			else*/ isRunning = camera.update(deltaTime);
		}
		
		// kdyz dojdeme na konec trajektorie kamery,
		// nastavime endingTime, podle ktereho na konci
		// zmizi budovy
		if (isRunning == 0)
		{
			endingTime = glfwGetTime();
			isRunning = 3;
		}

		// ukonceni vykreslovaci smycky
		if (glfwGetTime() - endingTime > 9 && isRunning == 3) isRunning = 4;		

		// nastaveni pozice kamery do shaderu (kvuli vypoctu mlhy)
		glm::vec3 cameraPosition = camera.GetPosition();
		instancingShader.use();
		instancingShader.setVec3("cameraPosition", cameraPosition);
		ordinaryShader.use();
		ordinaryShader.setVec3("cameraPosition", cameraPosition);

		// zpracovani vstupu, ve finalni verzi pouze tlacitko ESC
		processInput(window);

		// ziskani view matice kamery
		glm::mat4 view = camera.GetViewMatrix();

		// zjisteni pozice svetla ze ktereho vychazeji paprsky
		int viewport[5] = { 0, 0, SCR_WIDTH, SCR_HEIGHT };
		float lightPos[3] = { 0, 0 };
		getLightPosition(1000000, 300, 0, (float*)glm::value_ptr(view), (float*)glm::value_ptr(projection), viewport, lightPos);

		/***************************************** occlusion mapa ****************************************************/
		// pokud je isRunning == 2, je vykreslovano s volumetrickymi paprsky
		if (isRunning == 2 && PAPRSKY)
		{
			// aktivace prvniho bufferu
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

			// vycisteni obrazovky a bufferu
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// nastaveni promennych
			glEnable(GL_DEPTH_TEST);
			glCullFace(GL_FRONT);
			glEnable(GL_CULL_FACE);

			// vykresleni vsech budov
			ordinaryBlackShader.use();
			ordinaryBlackShader.setMat4("view", view);
			for (int q = 0; q < _skyscrapersArray.size(); q++) {
				_skyscrapersArray[q].draw(ordinaryBlackShader);
			}

			instancingBlackShader.use();
			instancingBlackShader.setMat4("view", view);
			for (int q = 0; q < _housesArray.size(); q++) {
				_housesArray[q].draw(instancingBlackShader);
			}
			for (int q = 0; q < _flatsArray.size(); q++) {
				if (_flatsArray[q].getPositionsSize() > 0) _flatsArray[q].draw(instancingBlackShader);
			}

			// vyskresleni ulic, cull facing neni potreba
			glDisable(GL_CULL_FACE);
			instancingBlackShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
			for (int q = 0; q < _roadsArray.size(); q++) {
				_roadsArray[q].draw();
			}

			// vykresleni skyboxu
			skyboxShader.use();
			skyboxShader.setMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
			skybox.Draw(true);

		/***************************************** normalni render ****************************************************/
			// aktivace druheho bufferu
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
		}

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// nastaveni promennych
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT);
		glEnable(GL_CULL_FACE);

		// vykresleni vsech budov
		ordinaryShader.use();
		ordinaryShader.setMat4("view", view);
		if (musicTime > 8)
		for (int q = 0; q < _skyscrapersArray.size(); q++) {
			if (isRunning < 3) _skyscrapersArray[q].draw(ordinaryShader);
			else if (glfwGetTime() - endingTime < 8) _skyscrapersArray[q].draw(ordinaryShader);
		}

		instancingShader.use();
		instancingShader.setMat4("view", view);
		if (musicTime > 17)
		for (int q = 0; q < _housesArray.size(); q++) {
			if (isRunning < 3) _housesArray[q].draw(instancingShader);
			else if (glfwGetTime() - endingTime < 4) _housesArray[q].draw(instancingShader);
		}
		if (musicTime > 12)
		for (int q = 0; q < _flatsArray.size(); q++) {
			if (isRunning < 3) {
				if (_flatsArray[q].getPositionsSize() > 0) _flatsArray[q].draw(instancingShader);
			}
			else if (glfwGetTime() - endingTime < 6) {
				if (_flatsArray[q].getPositionsSize() > 0) _flatsArray[q].draw(instancingShader);
			}
		}

		// vyskresleni ulic, cull facing neni potreba
		glDisable(GL_CULL_FACE);
		instancingShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
		if (musicTime > 3)
		for (int q = 0; q < _roadsArray.size(); q++) {
			if (isRunning < 3) _roadsArray[q].draw();
			else if (glfwGetTime() - endingTime < 2) _roadsArray[q].draw();
		}

		// vykresleni skyboxu
		skyboxShader.use();
		skyboxShader.setMat4("view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
		//skybox.Draw();

		/***************************************** spojeni bufferu, vysledny render s volumetrickym svetlem ****************************************************/
		if (isRunning == 2 && PAPRSKY)
		{
			// prepnuti na default framebuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);

			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			volumetricShader.use();
			volumetricShader.setMat4("view", view);
			volumetricShader.setVec2("lPosition", { lightPos[0] / SCR_WIDTH, lightPos[1] / SCR_HEIGHT });

			glBindVertexArray(quadVAO);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer2);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// swap bufferu
		glfwSwapBuffers(window);
		glfwPollEvents();

		// start hudby, pokud jeste nehraje
		if (!musicStarted)
		{
			//player.Play();
			musicStarted = true;
			musicTimeStart = glfwGetTime();
		}

		// vyskoceni ze smycky, pokud je konec
		if (isRunning == 4) break;
	}

	// zavreni prehravace
	dsClose();
	player.Close();

	// uklid
	glfwTerminate();
	return ;
}

// kontrola inputu
void processInput(GLFWwindow *window)
{
	// ve finalni verzi pouze klavesa ESC
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);	
	/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);*/
}

// když nastane zmìna velikosti okna, nastaví správné zobrazení
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// ve finalni verzi neni potreba menit pohled mysi
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	float xoffset = xpos - lastX;
//	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//	lastX = xpos;
//	lastY = ypos;
//
//	camera.ProcessMouseMovement(xoffset, yoffset);
//}

// vrati polohu svetla na obrazovce pres pointer
int getLightPosition(float objx, float objy, float objz, float *modelview, float *projection, int *viewport, float *windowCoordinate)
{
	float fTempo[8];

	fTempo[0] = modelview[0] * objx + modelview[4] * objy + modelview[8] * objz + modelview[12]; // w is always 1
	fTempo[1] = modelview[1] * objx + modelview[5] * objy + modelview[9] * objz + modelview[13];
	fTempo[2] = modelview[2] * objx + modelview[6] * objy + modelview[10] * objz + modelview[14];
	fTempo[3] = modelview[3] * objx + modelview[7] * objy + modelview[11] * objz + modelview[15];

	fTempo[4] = projection[0] * fTempo[0] + projection[4] * fTempo[1] + projection[8] * fTempo[2] + projection[12] * fTempo[3];
	fTempo[5] = projection[1] * fTempo[0] + projection[5] * fTempo[1] + projection[9] * fTempo[2] + projection[13] * fTempo[3];
	fTempo[6] = projection[2] * fTempo[0] + projection[6] * fTempo[1] + projection[10] * fTempo[2] + projection[14] * fTempo[3];
	fTempo[7] = -fTempo[2];

	if (fTempo[7] == 0.0f) 
		return 0;
	fTempo[7] = 1.0f / fTempo[7];

	fTempo[4] *= fTempo[7];
	fTempo[5] *= fTempo[7];
	fTempo[6] *= fTempo[7];

	windowCoordinate[0] = (fTempo[4] * 0.5f + 0.5f)*viewport[2] + viewport[0];
	windowCoordinate[1] = (fTempo[5] * 0.5f + 0.5f)*viewport[3] + viewport[1];

	windowCoordinate[2] = (1.0f + fTempo[6])*0.5f;
	return 1;
}

// vytvori ruzne typy domu
void createHouses() 
{
	int size = 36;
	for (int i = 0; i < 360; i++) {
		_housesArray.push_back(House(rand() % 36 + 36, size));
		if (i % 10 == 9) size++;
	}

	size = 36;
	for (int i = 0; i < 360; i++) {
		_housesArray.push_back(House(size, rand() % 36 + 36));
		if (i % 10 == 9) size++;
	}
}

// vytvori ruzne typy panelovych domu
void createFlats()
{
	int size = 72;
	for (int i = 0; i < 720; i++) {
		_flatsArray.push_back(Flats(rand() % 72 + 72, size));
		if (i % 10 == 9) size++;
	}

	size = 72;
	for (int i = 0; i < 720; i++) {
		_flatsArray.push_back(Flats(size, rand() % 72 + 72));
		if (i % 10 == 9) size++;
	}
}

