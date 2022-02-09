/*
* HW04: OpenGL 2 to 4
* Kelley Kelley
* 
* Turns HW03 from OpenGL 2 to 4
* 
* Key bindings:
* m    toggle shaders
* arrows    change view angle
* PgDn/PgUp    zoom in and out
* 0    reset view angle
* esc    exit
*/
#include "CSCIx239.h"
#include <stdbool.h>
int zh = 0; // light position
int th = 0, ph = 0; // view angles
int fov = 57; // fov
float asp = 1; // aspect ratio
float dim = 3; // size of world
int snowshader; // shader for snow cubes
int regshader; // regular per pixel shader
int sphereshader; // weird sphere shader
int lightshader; // shader for a light source
int tex; // pi texture

// cube data
unsigned int cube_vbo = 0;
unsigned int cube_vao = 0;
const int cube_size = 36;
const float cube_data[] = // vertex data
{
//  X  Y  Z  W   Nx Ny Nz    R G B A   s t
   //  Front
   +1,+1,+1,+1,   0, 0,+1,   1,0,0,1,  1,1,
   -1,+1,+1,+1,   0, 0,+1,   1,0,0,1,  0,1,
   +1,-1,+1,+1,   0, 0,+1,   1,0,0,1,  1,0,
   -1,+1,+1,+1,   0, 0,+1,   1,0,0,1,  0,1,
   +1,-1,+1,+1,   0, 0,+1,   1,0,0,1,  1,0,
   -1,-1,+1,+1,   0, 0,+1,   1,0,0,1,  0,0,
   //  Back                        
   -1,-1,-1,+1,   0, 0,-1,   0,0,1,1,  1,0,
   +1,-1,-1,+1,   0, 0,-1,   0,0,1,1,  0,0,
   -1,+1,-1,+1,   0, 0,-1,   0,0,1,1,  1,1,
   +1,-1,-1,+1,   0, 0,-1,   0,0,1,1,  0,0,
   -1,+1,-1,+1,   0, 0,-1,   0,0,1,1,  1,1,
   +1,+1,-1,+1,   0, 0,-1,   0,0,1,1,  0,1,
   //  Right                       
   +1,+1,+1,+1,  +1, 0, 0,   1,1,0,1,  0,1,
   +1,-1,+1,+1,  +1, 0, 0,   1,1,0,1,  0,0,
   +1,+1,-1,+1,  +1, 0, 0,   1,1,0,1,  1,1,
   +1,-1,+1,+1,  +1, 0, 0,   1,1,0,1,  0,0,
   +1,+1,-1,+1,  +1, 0, 0,   1,1,0,1,  1,1,
   +1,-1,-1,+1,  +1, 0, 0,   1,1,0,1,  1,0,
   //  Left                        
   -1,+1,+1,+1,  -1, 0, 0,   0,1,0,1,  1,1,
   -1,+1,-1,+1,  -1, 0, 0,   0,1,0,1,  0,1,
   -1,-1,+1,+1,  -1, 0, 0,   0,1,0,1,  1,0,
   -1,+1,-1,+1,  -1, 0, 0,   0,1,0,1,  0,1,
   -1,-1,+1,+1,  -1, 0, 0,   0,1,0,1,  1,0,
   -1,-1,-1,+1,  -1, 0, 0,   0,1,0,1,  0,0,
   //  Top                         
   +1,+1,+1,+1,   0,+1, 0,   0,1,1,1,  1,0,
   +1,+1,-1,+1,   0,+1, 0,   0,1,1,1,  1,1,
   -1,+1,+1,+1,   0,+1, 0,   0,1,1,1,  0,0,
   +1,+1,-1,+1,   0,+1, 0,   0,1,1,1,  1,1,
   -1,+1,+1,+1,   0,+1, 0,   0,1,1,1,  0,0,
   -1,+1,-1,+1,   0,+1, 0,   0,1,1,1,  0,1,
   //  Bottom                      
   -1,-1,-1,+1,   0,-1, 0,   1,0,1,1,  0,0,
   +1,-1,-1,+1,   0,-1, 0,   1,0,1,1,  1,0,
   -1,-1,+1,+1,   0,-1, 0,   1,0,1,1,  0,1,
   +1,-1,-1,+1,   0,-1, 0,   1,0,1,1,  1,0,
   -1,-1,+1,+1,   0,-1, 0,   1,0,1,1,  0,1,
   +1,-1,+1,+1,   0,-1, 0,   1,0,1,1,  1,1,
};

// initialize cube vbo
void InitCubeVBO() {
	//  Get buffer name
	glGenBuffers(1, &cube_vbo);
	//  Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	//  Copy cube to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_data), cube_data, GL_STATIC_DRAW);
	//  Release VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// initialize cube vao
void InitCubeVAO(int shader) {
	//  Shader for which to get attibute locations
	glUseProgram(shader);

	//  Create cube VAO to bind attribute arrays
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);

	//  Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	//  Vertex
	int loc = glGetAttribLocation(shader, "Vertex");
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 52, (void*)0);
	glEnableVertexAttribArray(loc);
	//  Normal
	loc = glGetAttribLocation(shader, "Normal");
	glVertexAttribPointer(loc, 3, GL_FLOAT, 0, 52, (void*)16);
	glEnableVertexAttribArray(loc);
	//  Color
	loc = glGetAttribLocation(shader, "Color");
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 52, (void*)28);
	glEnableVertexAttribArray(loc);
	//  Texture
	loc = glGetAttribLocation(shader, "Texture");
	glVertexAttribPointer(loc, 2, GL_FLOAT, 0, 52, (void*)44);
	glEnableVertexAttribArray(loc);

	//  Release VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//  Release VAO
	glBindVertexArray(0);
	//  Release shader
	glUseProgram(0);
}

// sphere data
// for spheres I actually don't use it's texture so skip that hehe
// color I also don't need per vertex just the color of the whole sphere
// so skip that too
unsigned int sphere_vbo = 1;
unsigned int sphere_vao = 1;
const int sphere_size = 1920; // change this number after print
float sphere_data[1920]; // change this number after print

// initialize sphere vbo
void InitSphereVBO() {
	// first populate sphere data
	// populate sphere values in sphere data first
	int place = 0;
	int j = 0;
	int n = 15; // how detailed I want the sphere to be
	for (int i = 0; i < n; i++) {
		float ph0 = i * 180.0 / n - 90;
		float ph1 = (i + 1) * 180.0 / n - 90;
		for (j = 0; j <= n; j++) {
			float th = j * 360.0 / n;
			// calculate first vertex
			float x = Cos(th) * Cos(ph0);
			float y = Sin(th) * Cos(ph0);
			float z = Sin(ph0);
			// fill in vertex data
			sphere_data[place] = x;
			place++;
			sphere_data[place] = y;
			place++;
			sphere_data[place] = z;
			place++;
			sphere_data[place] = 1; // w
			place++;
			// for a sphere normals are the same as vertices
			// so we don't actually need to populate them since we can
			// write our shader with that in mind
			// calculate second vertex
			x = Cos(th) * Cos(ph1);
			y = Sin(th) * Cos(ph1);
			z = Sin(ph1);
			// fill in vertex data
			// fill in vertex data
			sphere_data[place] = x;
			place++;
			sphere_data[place] = y;
			place++;
			sphere_data[place] = z;
			place++;
			sphere_data[place] = 1; // w
			place++;
			// i don't use this but keeping it for future projects
			// glTexCoord2d(th/360,ph/180+.5);
		}
	}
	// i didnt feel like doing the math for how many points you needed so i let it tell me
	//printf("%d", place);
	//  Get buffer name
	glGenBuffers(1, &sphere_vbo);
	//  Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
	//  Copy cube to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_data), sphere_data, GL_STATIC_DRAW);
	//  Release VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// initialize sphere vao
void InitSphereVAO(int shader) {
	// shader for attribute locations
	glUseProgram(shader);

	// create sphere vao to bind attribute arrays
	glGenVertexArrays(1, &sphere_vao);
	glBindVertexArray(sphere_vao);

	// bind vbo
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
	// vertex
	int loc = glGetAttribLocation(shader, "Vertex");
	// for now our sphere vbo only uses vertices
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 0, (void*) 0);
	glEnableVertexAttribArray(loc);

	//  Release VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//  Release VAO
	glBindVertexArray(0);
	//  Release shader
	glUseProgram(0);
}

unsigned int sphere_vao2 = 1;
// initialize sphere vao
void InitSphereVAO2(int shader) {
	// shader for attribute locations
	glUseProgram(shader);

	// create sphere vao to bind attribute arrays
	glGenVertexArrays(1, &sphere_vao2);
	glBindVertexArray(sphere_vao2);

	// bind vbo
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
	// vertex
	int loc = glGetAttribLocation(shader, "Vertex");
	// for now our sphere vbo only uses vertices
	glVertexAttribPointer(loc, 4, GL_FLOAT, 0, 0, (void*)0);
	glEnableVertexAttribArray(loc);

	//  Release VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//  Release VAO
	glBindVertexArray(0);
	//  Release shader
	glUseProgram(0);
}

// display function
void display(GLFWwindow* window) {
	//  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//  Enable Z-buffering in OpenGL
	glEnable(GL_DEPTH_TEST);

	// OpenGL took all this out so we gotta do everything by hand
	// set up all the matrices, perspective, view, model, modelview
	//  Eye position for perspective
	float Ex = -2 * dim * Sin(th) * Cos(ph);
	float Ey = +2 * dim * Sin(ph);
	float Ez = +2 * dim * Cos(th) * Cos(ph);

	//  Lighting parameters
	float global[] = { 0.1,0.1,0.1,1.0 };
	float ambient[] = { 0.3,0.3,0.3,1.0 };
	float diffuse[] = { 0.8,0.8,0.8,1.0 };
	float specular[] = { 1.0,1.0,1.0,1.0 };
	float position[] = { 0, 3 * Cos(zh), 3 * Sin(zh), 1.0 };

	//  Enable shader
	glUseProgram(snowshader);

	//  Set light property uniforms
	int id = glGetUniformLocation(snowshader, "fov");
	glUniform1f(id, fov);
	id = glGetUniformLocation(snowshader, "Global");
	glUniform4fv(id, 1, global);
	id = glGetUniformLocation(snowshader, "Ambient");
	glUniform4fv(id, 1, ambient);
	id = glGetUniformLocation(snowshader, "Diffuse");
	glUniform4fv(id, 1, diffuse);
	id = glGetUniformLocation(snowshader, "Specular");
	glUniform4fv(id, 1, specular);
	id = glGetUniformLocation(snowshader, "Position");
	glUniform4fv(id, 1, position);

	//  Create Projection matrix
	float proj[16];
	mat4identity(proj);
	if (fov)
		mat4perspective(proj, fov, asp, dim / 16, 16 * dim);
	else
		mat4ortho(proj, -dim * asp, +dim * asp, -dim, +dim, -dim, +dim);
	//  Create View matrix
	float view[16];
	mat4identity(view);
	if (fov)
		mat4lookAt(view, Ex, Ey, Ez, 0, 0, 0, 0, Cos(ph), 0);
	else
	{
		mat4rotate(view, ph, 1, 0, 0);
		mat4rotate(view, th, 0, 1, 0);
	}
	//  Create ModelView matrix
	float modelview[16];
	mat4copy(modelview, view);
	mat4translate(modelview, 0, 0, 0.5);
	//  Create Normal matrix
	float normat[9];
	mat4normalMatrix(modelview, normat);
	//  Set Projection, View, Modelview and Normal Matrix
	id = glGetUniformLocation(snowshader, "ProjectionMatrix");
	glUniformMatrix4fv(id, 1, 0, proj);
	id = glGetUniformLocation(snowshader, "ViewMatrix");
	glUniformMatrix4fv(id, 1, 0, view);
	id = glGetUniformLocation(snowshader, "ModelViewMatrix");
	glUniformMatrix4fv(id, 1, 0, modelview);
	id = glGetUniformLocation(snowshader, "NormalMatrix");
	glUniformMatrix3fv(id, 1, 0, normat);

	// time
	float t = glfwGetTime();
	id = glGetUniformLocation(snowshader, "time");
	glUniform1f(id, t);

	//  Bind attribute arrays using VAO (VAO knows VBO to use)
	glBindVertexArray(cube_vao);

	// Draw the cube (offset in modelview)
	glDrawArrays(GL_TRIANGLES, 0, cube_size);

	//  Release attribute arrays
	glBindVertexArray(0);

	//  Enable shader
	glUseProgram(sphereshader);

	//  Set light property uniforms
	id = glGetUniformLocation(sphereshader, "Position");
	glUniform4fv(id, 1, position);
	//  Set Projection, View, Modelview and Normal Matrix
	id = glGetUniformLocation(sphereshader, "ProjectionMatrix");
	glUniformMatrix4fv(id, 1, 0, proj);
	float modelviewweird[16];
	mat4copy(modelviewweird, modelview);
	mat4scale(modelviewweird, 10, 10, 10);
	id = glGetUniformLocation(sphereshader, "ModelViewMatrix");
	glUniformMatrix4fv(id, 1, 0, modelviewweird);
	id = glGetUniformLocation(sphereshader, "time");
	glUniform1f(id, t);
	// bind our sphere
	glBindVertexArray(sphere_vao2);
	// draw the sphere
	glDrawArrays(GL_QUAD_STRIP, 0, sphere_size);
	glBindVertexArray(0);

	// now our light source sphere
	glUseProgram(lightshader);
	//  Set Projection, View, Modelview and Normal Matrix
	id = glGetUniformLocation(lightshader, "ProjectionMatrix");
	glUniformMatrix4fv(id, 1, 0, proj);
	id = glGetUniformLocation(lightshader, "ModelViewMatrix");
	
	// I want the light source to actually appear at the light position
	// so move the modelview matrix
	// and scale it down it's too big
	float modelviewlight[16];
	mat4copy(modelviewlight, modelview);
	mat4translate(modelviewlight, position[0], position[1], position[2]);
	mat4scale(modelviewlight, .25, .25, .25);

	glUniformMatrix4fv(id, 1, 0, modelviewlight);
	// solution to light source moving
	id = glGetUniformLocation(lightshader, "Position");
	glUniform4fv(id, 1, position);

	// light source color
	float lscolor[] = { 1, .9, 0.0, 1.0 };
	id = glGetUniformLocation(lightshader, "LightColor");
	glUniform4fv(id, 1, lscolor);

	// bind our sphere
	glBindVertexArray(sphere_vao);
	// draw the sphere
	glDrawArrays(GL_QUAD_STRIP, 0, sphere_size);
	glBindVertexArray(0);

	//  Fixed pipeline
	glUseProgram(0);
	//  Render the scene and make it visible
	ErrCheck("display");
	glFlush();
	glfwSwapBuffers(window);
}

// key pressed callback
void key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// discard key releases
	if (action == GLFW_RELEASE) return;

	// switch based on key
	switch (key) {
		// close on escape key
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, 1);
		break;
		// reset view angle
	case GLFW_KEY_0:
		th = ph = 0;
		dim = 3;
		break;
		// change light position
	case GLFW_KEY_RIGHT:
		th += 5;
		break;
	case GLFW_KEY_LEFT:
		th -= 5;
		break;
	case GLFW_KEY_UP:
		ph += 5;
		break;
	case GLFW_KEY_DOWN:
		ph -= 5;
		break;
		// zoom in and out
	case GLFW_KEY_PAGE_DOWN:
		dim += 0.1;
		break;
	case GLFW_KEY_PAGE_UP:
		dim -= 0.1;
		break;
		// reload shaders
	case GLFW_KEY_R:
		snowshader = CreateShaderProg("snowcube.vert", "snowcube.frag");
		regshader = CreateShaderProg("regularperpixel.vert", "regularperpixel.frag");
		sphereshader = CreateShaderProg("weirdsphere.vert", "weirdsphere.frag");
		lightshader = CreateShaderProg("lightsource.vert", "lightsource.frag");
		break;
	}
	// wrap angles
	th %= 360;
	ph %= 360;
}

//  call this when resized
void reshape(GLFWwindow* window, int width, int height) {
	//  Get framebuffer dimensions (makes Apple work right)
	glfwGetFramebufferSize(window, &width, &height);
	//  Ratio of the width to the height of the window
	asp = (height > 0) ? (double)width / height : 1;
	//  Set the viewport to the entire window
	glViewport(0, 0, width, height);
}

// start GLFW event loops
int main(int argc, char* argv[]) {
	// initialize GLFW
	GLFWwindow* window = InitWindow("HW04 Kelley Kelley", 1, 600, 600, &reshape, &key);
	// load all the shaders
	snowshader = CreateShaderProg("snowcube.vert", "snowcube.frag");
	regshader = CreateShaderProg("regularperpixel.vert", "regularperpixel.frag");
	sphereshader = CreateShaderProg("weirdsphere.vert", "weirdsphere.frag");
	lightshader = CreateShaderProg("lightsource.vert", "lightsource.frag");
	// load the textures (I don't think I'll use it but might as well
	tex = LoadTexBMP("pi.bmp");

	// Load cube into VBO
	InitCubeVBO();
	// Create cube VAO for gl4
	InitCubeVAO(snowshader);
	// load sphere into VBO
	InitSphereVBO();
	// Create sphere VAO for gl4
	InitSphereVAO(lightshader);
	// create sphere vao for gl4 other shader
	InitSphereVAO2(sphereshader);

	// event loop
	ErrCheck("init");
	while (!glfwWindowShouldClose(window)) {
		// light animation
		zh = fmod(90 * glfwGetTime(), 360);
		// display
		display(window);
		// process events
		glfwPollEvents();
	}
	// shut down GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}