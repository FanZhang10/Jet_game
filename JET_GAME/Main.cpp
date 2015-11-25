#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#include <windows.h>	   // Standard header for MS Windows applications

#include <stdio.h>
#include <stdlib.h>
#include <freeglut.h>	   // Free openGL Utility Toolkit (FreeGLUT)
#include <math.h>

#include <fstream>
#include <vector>
#include <cstdint> 

#include "Graph.h"
#include "Jet.h"


Graph *graph;
Jet *jet;;


int width = 0;
int height = 0;
short BitsPerPixel = 0;
std::vector<unsigned char> Pixels;

#define MAX_NO_TEXTURES 10
#define CUBE_TEXTURE 0
GLuint texture[MAX_NO_TEXTURES];



// Define a constant for the value of PI
#define GL_PI 3.1415926f

// Rotation amounts
static GLfloat xRot = 180.0f;
static GLfloat yRot = 0.0f;
static GLfloat zRot = 180.0f;

static GLfloat xPos = 0.0f;
static GLfloat yPos = 0.0f;
static GLfloat zPos = 0.0f;

static GLfloat xMover = 0.0f;
static GLfloat yMover = 0.0f;


GLuint WindowId;

GLuint buttonTexture = 0;

// Handle collision

float collisionXpos = 0.0f;
float collisionYpos = 0.0f;
float JetWingLength = 28.5f;
float xJetWing = 28.5f;
float yJetWing = 0.0f;
float firstYposition = 50.0f;//set for gameState = 2 button
float secondYposition = -50.0f;//set for gameState = 2 button
int numCollisions = 0;
int backgroundTimeCounter = 0;
int menuDisappearCounter = 0;
int gameState = 0;
int displayCollision = 0;
static int currentLevel = 1;
const int backgroundTimeLength = 10000; // set how long user wanna play

///////////////////////////////////////// OpenAL Displayng /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

#include"Framework.h"

#define	TEST_WAVE_FILE		"backgroundMusic.wav"
#define	TEST_WAVE_FILE_1		"keyeffectMusic.wav"
#define	TEST_WAVE_FILE_2		"keyeffectMusic2.wav"

ALuint      uiSource[3];
ALint       iState[3];
ALuint      uiBuffer[3];


int audioInit()
{
	// Initialize Framework
	ALFWInit();

	ALFWprintf("PlayStatic Test Application\n");

	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
		return 0;
	}

	// Generate an AL Buffer
	alGenBuffers(3, uiBuffer);

	// Load Wave file into OpenAL Buffer
	if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath(TEST_WAVE_FILE), uiBuffer[0]))
	{
		ALFWprintf("Failed to load %s\n", ALFWaddMediaPath(TEST_WAVE_FILE));
	}

	// Generate a Source to playback the Buffer
	alGenSources(3, uiSource);

	// Attach Source to Buffer
	alSourcei(uiSource[0], AL_BUFFER, uiBuffer[0]);


	//////////////////////////////////////
	// Generate an AL Buffer - 1

	// Load Wave file into OpenAL Buffer
	if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath(TEST_WAVE_FILE_1), uiBuffer[1]))
	{
		ALFWprintf("Failed to load %s\n", ALFWaddMediaPath(TEST_WAVE_FILE_1));
	}

	// Attach Source to Buffer
	alSourcei(uiSource[1], AL_BUFFER, uiBuffer[1]);


	//////////////////////////////////////
	// Generate an AL Buffer - 2

	// Load Wave file into OpenAL Buffer
	if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath(TEST_WAVE_FILE_2), uiBuffer[2]))
	{
		ALFWprintf("Failed to load %s\n", ALFWaddMediaPath(TEST_WAVE_FILE_2));
	}

	// Attach Source to Buffer
	alSourcei(uiSource[2], AL_BUFFER, uiBuffer[2]);


	return 0;
}


int closeAllSoundPlay() {

	// Clean up by deleting Source(s) and Buffer(s)
	alSourceStop(uiSource[0]);
	alSourceStop(uiSource[1]);
	alSourceStop(uiSource[2]);

	alDeleteSources(3, uiSource);
	alDeleteBuffers(3, uiBuffer);

	ALFWShutdownOpenAL();

	ALFWShutdown();

	return 0;
}




///////////////////////////////////////// OpenGL Displayng /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

GLuint LoadTexture(const char * filename, int width, int height)
{
	GLuint texture;
	unsigned char * data;
	FILE * file;

	//The following code will read in our RAW file
	file = fopen(filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	//glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->sizeX, pImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	free(data); //free the texture
	return texture; //return whether it was successful
}


// Called to draw scene
void RenderScene(void) {
	float normal[3];	// Storeage for calculated surface normal
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);

	glLoadIdentity();


	/////set menu
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[6]);

	glLoadIdentity();					// Reset The Current Modelview Matrix
	glTranslatef(0.0f, 0.0f, -79.0f);				// Move Right 1.5 Units And Into The Screen 6.0
	glScaled(120.0f, 90.0f, 1.0f);

	glBegin(GL_QUADS);									// Draw A Quad
	// Front Face

	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

//////////////////////////////////////////start menu//////////////////////////////////////////////
	if (gameState == 0) { //airplan rowed
		/////set background sound
		alGetSourcei(uiSource[0], AL_SOURCE_STATE, &iState[0]);
		if (iState[0] != AL_PLAYING)
			alSourcePlay(uiSource[0]);
		/////set menu
		//first name button

		glPushMatrix();
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		glLoadIdentity();					// Reset The Current Modelview Matrix
		glTranslatef(0.0f, 50.0f, -60.0f);				// Move Right 1.5 Units And Into The Screen 6.0
		glScaled(40.0f, 10.0f, 20.0f);

		glBegin(GL_QUADS);									// Draw A Quad
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

		glEnd();
		glPopMatrix();

		//second button
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		glLoadIdentity();					// Reset The Current Modelview Matrix
		glTranslatef(0.0f, -50.0f, -60.0f);				// Move Right 1.5 Units And Into The Screen 6.0
		glScaled(40.0f, 10.0f, 20.0f);

		glBegin(GL_QUADS);									// Draw A Quad
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

		glEnd();
		glPopMatrix();

		//user Guide button	
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[4]);

		glLoadIdentity();					// Reset The Current Modelview Matrix
		glTranslatef(-85.0f, -75.0f, -60.0f);	// set help button position
		glScaled(20.0f, 5.0f, 20.0f);

		glBegin(GL_QUADS);									// Draw A Quad
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);


		/////////////draw airplan
		// Save the matrix state and do the rotations
		glPushMatrix();

		glTranslatef(xPos, yPos, zPos);
		glScalef(0.5f, 0.5f, 0.5f);

		glRotatef(zRot, 0.0f, 0.0f, 1.0f);
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot++, 0.0f, 1.0f, 0.0f);

		jet->rendering();
		

	}else if (gameState == 1) { //start button disappear
//////////////////////////////////////////start menu disappear//////////////////////////////////////////////
		/////set background sound
		alGetSourcei(uiSource[0], AL_SOURCE_STATE, &iState[0]);
		if (iState[0] != AL_PLAYING)
			alSourcePlay(uiSource[0]);
		/////set menu

		menuDisappearCounter++;
		if (menuDisappearCounter == 160)
			gameState = 2;	//switch game state to game model

		//first button
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		glLoadIdentity();					// Reset The Current Modelview Matrix

		glTranslatef(0.0f, firstYposition--, -60.0f);				// Move Right 1.5 Units And Into The Screen 6.0
		glScaled(40.0f, 10.0f, 20.0f);

		glBegin(GL_QUADS);									// Draw A Quad
		// Front Face

		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

		glEnd();
		glPopMatrix();

		//second button
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		glLoadIdentity();					// Reset The Current Modelview Matrix
		//glTranslatef(0.0f, -50.0f, -60.0f);
		glTranslatef(0.0f, secondYposition++, -60.0f);				// Move Right 1.5 Units And Into The Screen 6.0
		glScaled(40.0f, 10.0f, 20.0f);
		glBegin(GL_QUADS);									// Draw A Quad
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

		glEnd();
		glPopMatrix();

		////////////////////////////
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		
		// Save the matrix state and do the rotations
		glPushMatrix();

		//glTranslatef(xPos, yPos, zPos);
		glTranslatef(0.0f, 0.0f, 0.0f);
		glScalef(0.5f, 0.5f, 0.5f);

		glRotatef(zRot, 0.0f, 0.0f, 1.0f);
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);

		jet->rendering();

	}
	else if (gameState == 2) {// have cube 
//////////////////////////////////////////Game model//////////////////////////////////////////////
		/////set background sound
		
		if (backgroundTimeCounter < backgroundTimeLength){
			alGetSourcei(uiSource[1], AL_SOURCE_STATE, &iState[1]);
			if (iState[1] != AL_PLAYING){
				alSourcePlay(uiSource[1]);
			}
		}
		else if (backgroundTimeCounter >= backgroundTimeLength){
			gameState = 3;
			alSourceStop(uiSource[1]);//stop background music
		}
		if (numCollisions >= 8) {
			/////////////////chech win or lose///////////
			gameState = 4;
			alSourceStop(uiSource[1]);//stop background music
		}
		backgroundTimeCounter++;

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);

		graph->updateNode();
		for (int i = 0; i < graph->nodes.size(); i++)
		{
			Node * nodePtr = graph->nodes.elementAt(i);
			nodePtr->locationZ -= 0.1f;

			if (nodePtr->locationZ < 30.0) {
				nodePtr->locationX *= 1.05f;
				nodePtr->locationY *= 1.05f;
			}
		}

		for (int i = 0; i < graph->nodes.size(); i++)
		{
			float x = graph->nodes.elementAt(i)->getLocationX();
			float y = graph->nodes.elementAt(i)->getLocationY();
			float z = graph->nodes.elementAt(i)->getLocationZ();
		
			float xyScae = (1000.0f - z) / 100.0f;

			if (graph->nodes.elementAt(i)->isColloded == false) {
				glPushMatrix();
				glDisable(GL_LIGHTING);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, texture[8]);

				glTranslatef(x, y, -10.0);
				glScalef(xyScae, xyScae, 10.0f);

				glBegin(GL_QUADS);									// Draw A Quad
				// Front Face

				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

				glEnd();
				glPopMatrix();
			}
			else {

				glPushMatrix();
				glDisable(GL_LIGHTING);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, texture[7]);

				glTranslatef(x, y, -10.0);
				glScalef(xyScae, xyScae, 10.0f);

				glBegin(GL_QUADS);									// Draw A Quad
				// Front Face

				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

				glEnd();
				glPopMatrix();
			}
		}
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);         

		// Save the matrix state and do the rotations
		glPushMatrix();

		yPos += yMover;
		xPos += xMover;
		if (yPos > 65.0f){
			yPos = 65.0f;
		}

		if (yPos < -65.0f) {
			yPos = -65.0f;
		}

		if (xPos > 75.0f){
			xPos = 75.0f;
		}

		if (xPos < -75.0f) {
			xPos = -75.0f;
		}


		glTranslatef(xPos, yPos, zPos);
		glScalef(0.5f, 0.5f, 0.5f);

		glRotatef(zRot, 0.0f, 0.0f, 1.0f);
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);

		jet->rendering();
		// Restore the matrix state
		glPopMatrix();
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);


		for (int i = 0; i < graph->nodes.size(); i++)
		{
			Node * nodePtr = graph->nodes.elementAt(i);
			float x = graph->nodes.elementAt(i)->getLocationX();
			float y = graph->nodes.elementAt(i)->getLocationY();
			float z = graph->nodes.elementAt(i)->getLocationZ();
			float dist = 0.0f;
			if (z > -30.0f && z < 75.0f) {
				dist = (x - xPos) * (x - xPos) + (y - yPos) * (y - yPos);
				if (dist < 300 && displayCollision < 1) {

					displayCollision = 60;
					collisionXpos = x;
					collisionYpos = y;
					numCollisions++;
					nodePtr->isColloded = true;
					break;
				}

				dist = (x - (xPos + xJetWing)) * (x - (xPos + xJetWing)) + (y - (yPos + yJetWing)) * (y - (yPos + yJetWing));
				if (dist < 300 && displayCollision < 1) {

					displayCollision = 60;
					collisionXpos = x;
					collisionYpos = y;
					numCollisions++;
					nodePtr->isColloded = true;
					break;
				}

				dist = (x - (xPos - xJetWing)) * (x - (xPos - xJetWing)) + (y - (yPos - yJetWing)) * (y - (yPos - yJetWing));
				if (dist < 300 && displayCollision < 1) {

					displayCollision = 60;
					collisionXpos = x;
					collisionYpos = y;
					numCollisions++;
					nodePtr->isColloded = true;
					break;
				}

			}
		}


		if (displayCollision > 0) {
			displayCollision--;
			/////set effect sound
			alGetSourcei(uiSource[2], AL_SOURCE_STATE, &iState[2]);
			if (iState[2] != AL_PLAYING)
				alSourcePlay(uiSource[2]);


		}

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);

		//////////// set health bar///////////////
		glPushMatrix();
		glLineWidth(8.0);
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glVertex3f(-75.0, 75.0, 0.0);//set health bar size
		glVertex3f(75.0 - numCollisions * 20.0, 75.0, 0.0);//set health bar size
		glEnd();

		glColor3f(0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(75.0 - numCollisions * 20.0, 75.0, 0.0);//set health bar size
		glVertex3f(75.0, 75.0, 0.0);//set health bar size
		glEnd();

		glPopMatrix();
		glColor3f(1.0, 1.0, 1.0);
			
	}
	else if (gameState == 3) {// if user win
	////////////////////////////////////////////user win///////////////////////////////////////////
		/////set background sound
		alGetSourcei(uiSource[0], AL_SOURCE_STATE, &iState[0]);
		if (iState[0] != AL_PLAYING)
			alSourcePlay(uiSource[0]);
		/////set menu
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[2]);

		glLoadIdentity();					// Reset The Current Modelview Matrix
		glTranslatef(0.0f, 50.0f, -60.0f);				// Move Right 1.5 Units And Into The Screen 6.0
		glScaled(40.0f, 20.0f, 20.0f);

		glBegin(GL_QUADS);									// Draw A Quad
		// Front Face

		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

		glEnd();

		glPopMatrix();


		glPushMatrix();

		glBindTexture(GL_TEXTURE_2D, texture[1]);

		glLoadIdentity();					// Reset The Current Modelview Matrix
		glTranslatef(0.0f, -50.0f, -60.0f);				// Move Right 1.5 Units And Into The Screen 6.0
		glScaled(40.0f, 10.0f, 20.0f);

		glBegin(GL_QUADS);									// Draw A Quad
		// Front Face

		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

		glEnd();

		glPopMatrix();

		////////////////////////////

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		
		// Save the matrix state and do the rotations
		glPushMatrix();

		glTranslatef(0.0f, 0.0f, 0.0f);
		glScalef(0.5f, 0.5f, 0.5f);

		glRotatef(zRot, 0.0f, 0.0f, 1.0f);
		glRotatef(160, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot++, 0.0f, 1.0f, 0.0f);

		jet->rendering();

	}else if (gameState == 4){// if user lose
	///////////////////////user lose/////////////////////////
		/////set background sound
		alGetSourcei(uiSource[0], AL_SOURCE_STATE, &iState[0]);
		if (iState[0] != AL_PLAYING)
			alSourcePlay(uiSource[0]);
		/////set menu
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[3]);

		glLoadIdentity();					// Reset The Current Modelview Matrix
		glTranslatef(0.0f, 50.0f, -60.0f);				// Move Right 1.5 Units And Into The Screen 6.0
		glScaled(40.0f, 20.0f, 20.0f);

		glBegin(GL_QUADS);									// Draw A Quad
		// Front Face

		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

		glEnd();

		glPopMatrix();


		glPushMatrix();

		glBindTexture(GL_TEXTURE_2D, texture[1]);

		glLoadIdentity();					// Reset The Current Modelview Matrix
		glTranslatef(0.0f, -50.0f, -60.0f);				// Move Right 1.5 Units And Into The Screen 6.0
		glScaled(40.0f, 10.0f, 20.0f);

		glBegin(GL_QUADS);									// Draw A Quad
		// Front Face

		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

		glEnd();

		glPopMatrix();

		////////////////////////////


		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);

		// Save the matrix state and do the rotations
		glPushMatrix();

		//glTranslatef(xPos, yPos, zPos);
		glTranslatef(0.0f, 0.0f, 0.0f);
		glScalef(0.5f, 0.5f, 0.5f);

		glRotatef(zRot, 0.0f, 0.0f, 1.0f);
		glRotatef(160, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot++, 0.0f, 1.0f, 0.0f);

		jet->rendering();
	}
	else if (gameState == 5){// if press i, go in help menu
///////////////////////help menu/////////////////////////
	/////set background sound
	alGetSourcei(uiSource[0], AL_SOURCE_STATE, &iState[0]);
	if (iState[0] != AL_PLAYING)
		alSourcePlay(uiSource[0]);
	/////set menu
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);

	glLoadIdentity();					// Reset The Current Modelview Matrix
	glTranslatef(0.0f, 0.0f, -60.0f);				// Move Right 1.5 Units And Into The Screen 6.0
	glScaled(80.0f, 60.0f, 10.0f);

	glBegin(GL_QUADS);									// Draw A Quad
	// Front Face

	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	// Save the matrix state and do the rotations
	glPushMatrix();


	glTranslatef(70.0f, -50.0f, 0.0f);
	glScalef(0.5f, 0.5f, 0.5f);

	glRotatef(zRot, 0.0f, 0.0f, 1.0f);
	glRotatef(140, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot++, 0.0f, 1.0f, 0.0f);

	jet->rendering();

}
	// Display the results
	glutSwapBuffers();
}


void LoadBitmap(const char* FilePath)
{
    std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
    if (!hFile.is_open()) throw std::invalid_argument("Error: File Not Found.");
    
    hFile.seekg(0, std::ios::end);
    long Length = hFile.tellg();
    hFile.seekg(0, std::ios::beg);
    std::vector<std::uint8_t> FileInfo(Length);
    hFile.read(reinterpret_cast<char*>(FileInfo.data()), 54);
    
    if(FileInfo[0] != 'B' && FileInfo[1] != 'M')
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. Bitmap Required.");
    }
    
    if (FileInfo[28] != 24 && FileInfo[28] != 32)
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. 24 or 32 bit Image Required.");
    }
    
    BitsPerPixel = FileInfo[28];
    width = FileInfo[18] + (FileInfo[19] << 8);
    height = FileInfo[22] + (FileInfo[23] << 8);
    std::uint32_t PixelsOffset = FileInfo[10] + (FileInfo[11] << 8);
    std::uint32_t size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
    Pixels.resize(size);
    
    hFile.seekg (PixelsOffset, std::ios::beg);
    hFile.read(reinterpret_cast<char*>(Pixels.data()), size);
    hFile.close();
}

void InitGL(void)     // Create Some Everyday Functions
{
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(46.0f / 256.0f, 204.0f / 256.0f, 250.0f / 256.0f, 1.0f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glEnable ( GL_COLOR_MATERIAL );
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   
    glEnable ( GL_TEXTURE_2D );
    glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

	glGenTextures(1, &texture[0]);
    LoadBitmap("name.bmp");
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pixels.data());

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



	glGenTextures(1, &texture[1]);
    LoadBitmap("start.bmp");
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pixels.data());

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenTextures(1, &texture[2]);
	LoadBitmap("victor.bmp");
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pixels.data());

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenTextures(1, &texture[3]);
	LoadBitmap("lose.bmp");
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pixels.data());

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glGenTextures(1, &texture[4]);
	LoadBitmap("help.bmp");
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pixels.data());

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glGenTextures(1, &texture[5]);
	LoadBitmap("guide.bmp");
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pixels.data());

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenTextures(1, &texture[6]);
	LoadBitmap("sky.bmp");
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pixels.data());

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenTextures(1, &texture[7]);
	LoadBitmap("bomb.bmp");
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pixels.data());

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenTextures(1, &texture[8]);
	LoadBitmap("cube.bmp");
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, Pixels.data());

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


// This function does any needed initialization on the rendering
// context. 
void SetupRC() {
	// Light values and coordinates
	GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };

	glEnable(GL_DEPTH_TEST);	// Hidden surface removal


	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Setup and enable light 0
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glEnable(GL_LIGHT0);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);
	
	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	glMateriali(GL_FRONT,GL_SHININESS,128);
	

	//glEnable(GL_TEXTURE_2D);
	//buttonTexture = LoadTexture("marsh2.bmp", 256, 256);
	InitGL();

	// Light blue background
	glClearColor(46.0f/256.0f, 204.0f/256.0f, 250.0f/256.0f, 1.0f );
}


// Function called when timer ends
void timer(int id) {
	glutPostRedisplay();         // Tell GLUT to call it's glutDisplayFunc
}

// Set new timer and render
void display(void)
{
	glutTimerFunc(17, timer, 1); //Call timer function in at least 17 milliseconds
	glutSetWindow(WindowId);      //Tell glut which window we are working on (only needed for multiple windows)
	RenderScene();                //Call our render function
}


void SpecialKeys(int key, int x, int y) {

	xMover = 0.0f;
	yMover = 0.0f;


	if(key == GLUT_KEY_UP)
		yMover += 0.5f;

	if(key == GLUT_KEY_DOWN)
		yMover -= 0.5f;

	if(key == GLUT_KEY_LEFT)
		xMover -= 0.5f;

	if(key == GLUT_KEY_RIGHT)
		xMover += 0.5f;

	// Refresh the Window
	glutPostRedisplay();
}

void keypressed(unsigned char key, int x, int y)
{
	if (key == 'w') xRot -= 5.0f;
	if (key == 's') xRot += 5.0f;
	if (key == 'a') zRot += 5.0f;
	if (key == 'd') zRot -= 5.0f;
	if (key == 'z'){ 

		if (gameState == 0) {
			gameState = 1;
		}
		else if (gameState == 3 ) {
			currentLevel += 1;
			backgroundTimeCounter = 0;
			graph->buildGraph(currentLevel);
			gameState = 1;
			menuDisappearCounter = 0;
			numCollisions = 0;
			firstYposition = 50.0f;//set for gameState = 2 button
			secondYposition = -50.0f;//set for gameState = 2 button
			alSourcePlay(uiSource[0]);
			glTranslatef(0.0f, 0.0f, 0.0f);
		}
		else if (gameState == 4) {
			//currentLevel = currentLevel;
			backgroundTimeCounter = 0;
			graph->buildGraph(currentLevel);
			gameState = 1;
			menuDisappearCounter = 0;
			numCollisions = 0;
			firstYposition = 50.0f;//set for gameState = 2 button
			secondYposition = -50.0f;//set for gameState = 2 button
			alSourcePlay(uiSource[0]);
			glTranslatef(0.0f, 0.0f, 0.0f);
		}
	}

	if (key == 'i'){
		if (gameState != 2)
			gameState = 5;//show the help page
	}
	if (key == 'b'){
		if (gameState == 5)
			gameState = 0;//go back to home page
			glTranslatef(0.0f, 0.0f, 0.0f);
	}

	if (xRot > 356.0f)
		xRot = 0.0f;

	if (xRot < -1.0f)
		xRot = 355.0f;

	if (zRot > 356.0f)
		zRot = 0.0f;

	if (zRot < -1.0f)
		zRot = 355.0f;



	if (gameState == 2) {
		float angle;

		if (zRot >= 180) { //CCW
			angle = zRot;
			xJetWing = JetWingLength * cos(GL_PI / 180.0 * angle);
			yJetWing = JetWingLength * sin(GL_PI / 180.0 * angle);
		}
		else {
			angle = zRot;
			xJetWing = JetWingLength * cos(GL_PI / 180.0 * angle);
			yJetWing = JetWingLength * sin(GL_PI / 180.0 * angle);
		}
	}


	// Refresh the Window
	glutPostRedisplay();
}

void ChangeSize(int w, int h) {
	GLfloat nRange = 80.0f;
	GLfloat	 lightPos[] = { -50.f, 50.0f, 100.0f, 1.0f };

	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// Set Viewport to window dimensions
    glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
    if (w <= h) 
		glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    else 
		glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
}

int main(int argc, char **argv) {
	///////////////////////////////////// openAL /////////////////////////////////////
		audioInit();
		// Play Source : background Music 
		alSourcePlay(uiSource[0]);
	////////////////////////////////////// openGL /////////////////////////////////////
	// set window values
	graph = new Graph();
	graph->buildGraph(currentLevel);

	jet = new Jet();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);				// set window size
	WindowId = glutCreateWindow("Jet Game");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(&keypressed);
	glutDisplayFunc(display);
	SetupRC();
	glutMainLoop();

	return 0;
}






