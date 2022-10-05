#include <iostream>
#include <SDL.h>
#include <GL/freeglut.h>
#include <chrono>

#define WIN_W 1600
#define WIN_H 900

struct rotAngles { float x; float y; float z; };

//global variables
std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();
SDL_Window *window;
float rotSpeed = 30.0f;

rotAngles rotAng = {0, 0, 0};

//timer function
float getDeltaTime()
{
	auto now = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.0f;
	lastUpdate = now;
	return deltaTime;
}

//angle animation function
void calcMotionAngle(float &rotAng, float dt, float spd)
{
	rotAng += spd * dt;
	if (rotAng > 360) rotAng -= 360;
}

//draws a triangle ad specified location
void drawTriangle(float x, float y, float z, float r)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();

	glTranslatef(x, y, z);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-r, -r, 0.0f);
	glVertex3f(0.0f, r, 0.0f);
	glVertex3f(r, -r, 0.0f);
	glEnd();

	glPopMatrix();
}

//draws a cube rolled at specified angles
void drawCube(float xrf, float yrf, float zrf)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// Move the cube a bit deeper into screen
	glTranslatef(0.0f, 0.0f, -7.0f);	

	// Euler rotate by X
	glRotatef(xrf, 1.0f, 0.0f, 0.0f);
	// Euler rotate by Y
	glRotatef(yrf, 0.0f, 1.0f, 0.0f);
	// Euler rotate by Z
	glRotatef(zrf, 0.0f, 0.0f, 1.0f);
	
	// Draw a cube wia GL_QUADS primitive
	glBegin(GL_QUADS);					

	glColor3f(0.0f, 1.0f, 0.0f);
	//Blue side (top)
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glColor3f(1.0f, 0.5f, 0.0f);
	//Orange side (bottom)
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	//Red side (front)
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glColor3f(1.0f, 1.0f, 0.0f);
	//Yellow side (back)
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	// Blue side (left)
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glColor3f(1.0f, 0.0f, 1.0f);
	// Purple side (right)
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glEnd();		

	glPopMatrix();

}

//draws a line between 2 points in 3d space
void drawLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 2.0f);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glEnd();

	glPopMatrix();
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawTriangle(5.0f, 0.0f, -10.0f, 1.0f);
	drawCube(rotAng.x, rotAng.y, rotAng.z);
}

//sets up SDL side of things
void initSDLLib()
{
	// -- init SDL --

	//basic SDL initialization
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Unable to init SDL, error: " << SDL_GetError() << std::endl;
		exit(1);
	}

	//setting up SDL OpenGL compatibility
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

	//setting up window as OpenGL window
	window = SDL_CreateWindow("Cube",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIN_W, WIN_H,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == NULL) {
		exit(1);
	}

	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
}

//sets up OpenGL side of things
void initOpenGLLib()
{
	glClearColor(0.0f, 0.7f, 0.5f, 1.0f);
	//glClearDepth(1.0);

	glDepthFunc(GL_LESS); //depth calc mode
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-2,2,-2,2,-20,20);
	gluPerspective(45.0f, (float)WIN_W / (float)WIN_H, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
}

//our main func
int main(int argc, char *argv[])
{
	initSDLLib();
	initOpenGLLib();

	bool running = true;

	while (running) //main loop
	{

		//SDL event loop part
		SDL_Event event; //SDL even struct

		while (SDL_PollEvent(&event)) //main SDL loop
		{
			switch (event.type) //analyze event type
			{
			case SDL_QUIT:	//if window closed
				running = false;
				break;
			case SDL_KEYDOWN: //key pressed
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE: //exit by esc
					running = false;
					break;
				case SDLK_EQUALS: //+
					rotSpeed += 30;
					std::cout << "Speed=" << rotSpeed << std::endl;
					break;
				case SDLK_MINUS: //-
					rotSpeed -= 30;
					std::cout << "Speed=" << rotSpeed << std::endl;
					break;
				}
			}
		}

		//calc cube rotation
		float dt = getDeltaTime();
		calcMotionAngle(rotAng.x, dt, rotSpeed);
		calcMotionAngle(rotAng.y, dt, rotSpeed);
		calcMotionAngle(rotAng.z, dt, rotSpeed);

		std::cout << "Current FPS = " << (float)(1.0f / dt) << std::endl;
		
		//OUR loop event part
		render();

		//call buffer swap
		SDL_GL_SwapWindow(window);
	}

	return 0;

}