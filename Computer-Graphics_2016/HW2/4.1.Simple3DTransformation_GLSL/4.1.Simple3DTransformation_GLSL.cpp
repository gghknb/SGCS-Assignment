#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Begin of shader setup
#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}
// End of shader setup

// Begin of geometry setup
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))
#define INDEX_VERTEX_POSITION 0

GLuint axes_VBO, axes_VAO;
GLfloat axes_vertices[6][3] = {
	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }
};
GLfloat axes_color[3][3] = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };

void prepare_axes(void) {
	// Initialize vertex buffer object.
	glGenBuffers(1, &axes_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes_vertices), &axes_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &axes_VAO);
	glBindVertexArray(axes_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glVertexAttribPointer(INDEX_VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(INDEX_VERTEX_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_axes(void) {
	glBindVertexArray(axes_VAO);
	glUniform3fv(loc_primitive_color, 1, axes_color[0]);
	glDrawArrays(GL_LINES, 0, 2);
	glUniform3fv(loc_primitive_color, 1, axes_color[1]);
	glDrawArrays(GL_LINES, 2, 2);
	glUniform3fv(loc_primitive_color, 1, axes_color[2]);
	glDrawArrays(GL_LINES, 4, 2);
	glBindVertexArray(0);
}

#define N_OBJECTS 6
#define OBJECT_SQUARE16 0
#define OBJECT_TIGER 1
#define OBJECT_COW 2
#define OBJECT_BAREL 3
#define OBJECT_CAR 4
#define OBJECT_CAT 5

GLuint object_VBO[N_OBJECTS], object_VAO[N_OBJECTS];
int object_n_triangles[N_OBJECTS];
GLfloat *object_vertices[N_OBJECTS];

int read_triangular_mesh(GLfloat **object, int bytes_per_primitive, char *filename) {
	int n_triangles;
	FILE *fp;

	fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
	fp = fopen(filename, "rb");
	if (fp == NULL){
		fprintf(stderr, "Cannot open the object file %s ...", filename);
		return -1;
	}
	fread(&n_triangles, sizeof(int), 1, fp);
	*object = (float *)malloc(n_triangles*bytes_per_primitive);
	if (*object == NULL){
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...", filename);
		return -1;
	}

	fread(*object, bytes_per_primitive, n_triangles, fp);
	fprintf(stdout, "Read %d primitives successfully.\n\n", n_triangles);
	fclose(fp);

	return n_triangles;
}

void set_up_object(int object_ID, char *filename, int n_bytes_per_vertex) {
	object_n_triangles[object_ID] = read_triangular_mesh(&object_vertices[object_ID],
		3 * n_bytes_per_vertex, filename);
	// Error checking is needed here.

	// Initialize vertex buffer object.
	glGenBuffers(1, &object_VBO[object_ID]);

	glBindBuffer(GL_ARRAY_BUFFER, object_VBO[object_ID]);
	glBufferData(GL_ARRAY_BUFFER, object_n_triangles[object_ID] * 3 * n_bytes_per_vertex,
		object_vertices[object_ID], GL_STATIC_DRAW);

	// As the geometry data exists now in graphics memory, ...
	free(object_vertices[object_ID]);

	// Initialize vertex array object.
	glGenVertexArrays(1, &object_VAO[object_ID]);
	glBindVertexArray(object_VAO[object_ID]);

	glBindBuffer(GL_ARRAY_BUFFER, object_VBO[object_ID]);
	glVertexAttribPointer(INDEX_VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(INDEX_VERTEX_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void prepare_barel(void) {
	// 8 = 3 for vertex, 3 for normal, and 2 for texcoord
	set_up_object(OBJECT_BAREL, "Data/Barel_triangles_vnt.geom", 8 * sizeof(float));
}

void prepare_car(void) {
	// 8 = 3 for vertex, 3 for normal, and 2 for texcoord
	set_up_object(OBJECT_CAR, "Data/Car_triangles_vnt.geom", 8 * sizeof(float));
}

void prepare_cat(void) {
	// 8 = 3 for vertex, 3 for normal, and 2 for texcoord
	set_up_object(OBJECT_CAT, "Data/Cat_triangles_vnt.geom", 8 * sizeof(float));
}

void prepare_square(void) {
	// 8 = 3 for vertex, 3 for normal, and 2 for texcoord
	set_up_object(OBJECT_SQUARE16, "Data/Square16_triangles_vnt.geom", 8 * sizeof(float));
}

void prepare_tiger(void) {
	// 8 = 3 for vertex, 3 for normal, and 2 for texcoord
	set_up_object(OBJECT_TIGER, "Data/Tiger_00_triangles_vnt.geom", 8 * sizeof(float));
}

void prepare_cow(void) {
	// 3 = 3 for vertex
	set_up_object(OBJECT_COW, "Data/Cow_triangles_v.geom", 3 * sizeof(float));
}

void draw_object(int object_ID, float r, float g, float b) {
	glUniform3f(loc_primitive_color, r, g, b);
	glBindVertexArray(object_VAO[object_ID]);
	glDrawArrays(GL_TRIANGLES, 0, 3 * object_n_triangles[object_ID]);
	glBindVertexArray(0);
}
// End of geometry setup

// Begin of Callback function definitions
#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f

// include glm/*.hpp only if necessary
// #include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
// ViewProjectionMatrix = ProjectionMatrix * ViewMatrix
glm::mat4 ViewProjectionMatrix, ViewMatrix, ProjectionMatrix;
// ModelViewProjectionMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix
glm::mat4 ModelViewProjectionMatrix; // This one is sent to vertex shader when ready.

float rotation_tiger = 0.0f,rotation_angle_mintcow = 0.0f,rotation_angle_tiger = 0.0f, rotation_angle_cow = 0.0f;
int flag_fill_floor = 0;
int flag_camera;
int tiger_flag = -1;
float zoom = 15.0f;
float x_camera = 0;
float y_camera = 0;
float z_camera = 0;
float R_radius = 6.0f;
float r_radius = 1.0f;


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ModelViewProjectionMatrix = glm::scale(ViewProjectionMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(3.0f);
	draw_axes();
	glLineWidth(1.0f);

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix, glm::vec3(0.0f, 0.0f, -3.5f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(3.0f);
	draw_axes();
	glLineWidth(1.0f);
	draw_object(OBJECT_SQUARE16, 142 / 255.0f, 53 / 255.0f, 239 / 255.0f); // Purple

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix, glm::vec3(3.0f, 0.0f, -3.5f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(3.0f);
	draw_axes();
	glLineWidth(1.0f);
	draw_object(OBJECT_COW, 152 / 255.0f, 255 / 255.0f, 152 / 255.0f); // Mint Green

	/* 호랑이 */
	if (rotation_tiger <= -90.0f) 
		tiger_flag = 1;
	else if (rotation_tiger >= 90.0f)
		tiger_flag = -1;
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix, glm::vec3(0, 3.0f*cos((180+rotation_tiger)*TO_RADIAN), 3.0f*sin((180+rotation_tiger)*TO_RADIAN)));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, -90.0f*TO_RADIAN,
		glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, 90.0f*TO_RADIAN,
		glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(0.01f + fabs(rotation_tiger/10000), 0.01f + fabs(rotation_tiger/10000), 0.01f + fabs(rotation_tiger/10000)));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(OBJECT_TIGER, 3 / 255.0f, 53 / 255.0f, 244 / 255.0f); // blue
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(3.0f);
	draw_axes();
	glLineWidth(1.0f);

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix, glm::vec3(-6.0f, 0.0f, -6.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(12.0f, 12.0f, 12.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(3.0f);
	draw_axes();
	glLineWidth(1.0f);
	if (flag_fill_floor == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	draw_object(OBJECT_SQUARE16, 111 / 255.0f, 78 / 255.0f, 55 / 255.0f); // Coffee
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/* 소 */
		ModelViewProjectionMatrix = glm::rotate(ViewProjectionMatrix,
		rotation_angle_mintcow*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::translate(ModelViewProjectionMatrix, 
		glm::vec3((cos(rotation_angle_mintcow * r_radius / (r_radius - R_radius))*(R_radius-r_radius)+r_radius*cos(rotation_angle_mintcow)), -1.0f,
			(R_radius-r_radius)*sin(rotation_angle_mintcow * r_radius / (r_radius - R_radius)) +r_radius*sin(rotation_angle_mintcow)));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(OBJECT_COW, 30 / 255.0f, 63 / 255.0f, 20 / 255.0f); // Green

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix,
		glm::vec3(4.0f*rotation_angle_cow / 90.0f - 4.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix,
		rotation_angle_cow*TO_RADIAN, glm::vec3(0.0, 1.0, 0.0));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(3.0f);
	draw_axes();
	glLineWidth(1.0f);
	draw_object(OBJECT_COW, 255 / 255.0f, 229 / 255.0f, 180 / 255.0f); // Peach


	/* 차 */
	ModelViewProjectionMatrix = glm::rotate(ViewProjectionMatrix,
		-rotation_angle_tiger*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = glm::translate(ModelViewProjectionMatrix, glm::vec3(0.0f, 1.0f + rotation_angle_tiger / 500, 3.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(0.1f+rotation_angle_tiger/1000, 0.1f+ rotation_angle_tiger/1000, 0.1f+ rotation_angle_tiger/1000));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_object(OBJECT_CAR, 5 / 255.0f, 7 / 255.0f, 70 / 255.0f); // Rose
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(3.0f);
	draw_axes();
	glLineWidth(1.0f);

	glutSwapBuffers();
}

void initialize_OpenGL(void) {
	glClearColor(80 / 255.0f, 74 / 255.0f, 75 / 255.0f, 1.0f); // Gray Wolf
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);

	ViewMatrix = glm::lookAt(glm::vec3(20.0f, 15.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
}

void keyboard(unsigned char key, int x, int y) {


	switch (key) {
	case 27: // ESC key
		flag_camera = 0;
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	case '0':		//초기 카메라
		flag_camera = 0;
		zoom = 15.0f;
		ProjectionMatrix = glm::perspective(zoom*TO_RADIAN, 1.5f, 1.0f, 1000.0f);	//zoom in out을 전에 했을 시 원래 시점으로 초기화
		ViewMatrix = glm::lookAt(glm::vec3(20.0f, 15.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case '1':	//1번 카메라
		flag_camera = 2;
		zoom = 15.0f;
		ProjectionMatrix = glm::perspective(zoom*TO_RADIAN, 1.5f, 1.0f, 1000.0f);	//zoom in out을 전에 했을 시 원래 시점으로 초기화
		ViewMatrix = glm::lookAt(glm::vec3(30.0f, 30.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case '2': // 2번 카메라 
		flag_camera = 0;
		zoom = 15.0f;
		ProjectionMatrix = glm::perspective(zoom*TO_RADIAN, 1.5f, 1.0f, 1000.0f);	//zoom in out을 전에 했을 시 원래 시점으로 초기화
		ViewMatrix = glm::lookAt(glm::vec3(30.0f, -30.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case '3': // 3번 카메라
		flag_camera = 0;
		zoom = 15.0f;
		ProjectionMatrix = glm::perspective(zoom*TO_RADIAN, 1.5f, 1.0f, 1000.0f);	//zoom in out을 전에 했을 시 원래 시점으로 초기화
		ViewMatrix = glm::lookAt(glm::vec3(20.0f, 0.9f, 0.0f), glm::vec3(0.0f, 0.9f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case '4': // 4번 카메라 
		flag_camera = 0;
		zoom = 15.0f;
		ProjectionMatrix = glm::perspective(zoom*TO_RADIAN, 1.5f, 1.0f, 1000.0f);	//zoom in out을 전에 했을 시 원래 시점으로 초기화
		ViewMatrix = glm::lookAt(glm::vec3(3.0f, 30.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.0f, 1.0f));
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case '5': //5번 동적 카메라
		flag_camera = 1;
		zoom = 15.0f;
		ProjectionMatrix = glm::perspective(zoom*TO_RADIAN, 1.5f, 1.0f, 1000.0f);	//zoom in out을 전에 했을 시 원래 시점으로 초기화
		ViewMatrix = glm::lookAt(glm::vec3(30.0f, 10.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	}

	if (flag_camera == 1)  //5번 카메라일시 움직임 표현
	{
		switch (key) {
		case 'a':  //카메라가 x축 기준으로 양의 방향으로 이동
			ProjectionMatrix = glm::translate(ProjectionMatrix, glm::vec3(0.3f, 0.0f, 0.0f));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 'A': //카메라가 x축 기준으로 음의 방향으로 이동
			ProjectionMatrix = glm::translate(ProjectionMatrix, glm::vec3(-0.3f, 0.0f, 0.0f));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 's':  //카메라가 y축 기준으로 양의 방향으로 이동
			ProjectionMatrix = glm::translate(ProjectionMatrix, glm::vec3(0.0f, 0.3f, 0.0f));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 'S': //카메라가 y축 기준으로 음의 방향으로 이동
			ProjectionMatrix = glm::translate(ProjectionMatrix, glm::vec3(0.0f, -0.3f, 0.0f));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 'd':  //카메라가 z축 기준으로 양의 방향으로 이동
			ProjectionMatrix = glm::translate(ProjectionMatrix, glm::vec3(0.0f, 0.0f, 0.3f));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 'D': //카메라가 z축 기준으로 음의 방향으로 이동
			ProjectionMatrix = glm::translate(ProjectionMatrix, glm::vec3(0.0f, 0.0f, -0.3f));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;

		case 'q': //카메라가 -n축 기준으로 반시계 방향으로 회전
			ProjectionMatrix = glm::rotate(ProjectionMatrix, 3.0f*TO_RADIAN, glm::vec3(0.0f, 0.0f, -1.0f));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 'Q': //카메라가 -n축 기준으로 시계 방향으로 회전
			ProjectionMatrix = glm::rotate(ProjectionMatrix, -3.0f*TO_RADIAN, glm::vec3(0.0f, 0.0f, -1.0f));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 'w': //카메라가 u축 기준으로 반시계 방향으로 회전
			ProjectionMatrix = glm::rotate(ProjectionMatrix, 3.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 'W': //카메라가 u축 기준으로 시계 방향으로 회전
			ProjectionMatrix = glm::rotate(ProjectionMatrix, -3.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 'e': //카메라가 v축 기준으로 반시계 방향으로 회전
			ProjectionMatrix = glm::rotate(ProjectionMatrix, 3.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 'E': //카메라가 v축 기준으로 시계 방향으로 회전
			ProjectionMatrix = glm::rotate(ProjectionMatrix, -3.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;

		case 'z': // 줌인 기능
			if (zoom > 3.0f)
				zoom -= 1.0f;
			ProjectionMatrix = glm::perspective(zoom*TO_RADIAN, 1.5f, 1.0f, 1000.0f);
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 'Z': //줌아웃기능
			if (zoom < 30.0f)
				zoom += 1.0f;
			ProjectionMatrix = glm::perspective(zoom*TO_RADIAN, 1.5f, 1.0f, 1000.0f);
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;

		}
	}
	else if (flag_camera == 2)  //1번 카메라일때 시선 방향 조절 기능
	{
		switch (key) {
		case 'y': //y키를 누르면 카메라 시선방향이 왼쪽으로 향함
			ViewMatrix = glm::translate(ViewMatrix, glm::vec3(30.0f, 10.0f, 15.0f));			//제자리로 카메라이동
			ViewMatrix = glm::rotate(ViewMatrix, -2.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));	//y축 기준으로 카메라 회전
			ViewMatrix = glm::translate(ViewMatrix, glm::vec3(-30.0f, -10.0f, -15.0f));			//원점으로 카메라이동
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 'i': //i키를 누르면 카메라 시선방향이 위쪽으로 향함		
			ViewMatrix = glm::translate(ViewMatrix, glm::vec3(30.0f, 10.0f, 15.0f));			//제자리로 카메라이동
			ViewMatrix = glm::rotate(ViewMatrix, -2.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 1.0f));	//y축 기준으로 카메라 회전
			ViewMatrix = glm::translate(ViewMatrix, glm::vec3(-30.0f, -10.0f, -15.0f));			//원점으로 카메라이동
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 'u': //u키를 누르면 카메라 시선방향이 오른쪽으로 향함
			ViewMatrix = glm::translate(ViewMatrix, glm::vec3(30.0f, 10.0f, 15.0f));			//제자리로 카메라이동
			ViewMatrix = glm::rotate(ViewMatrix, 2.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));	//y축 기준으로 카메라 회전
			ViewMatrix = glm::translate(ViewMatrix, glm::vec3(-30.0f, -10.0f, -15.0f));			//원점으로 카메라이동
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		case 'o': //o키를 누르면 카메라 시선방향이 밑으로 향함
			ViewMatrix = glm::translate(ViewMatrix, glm::vec3(30.0f, 10.0f, 15.0f));			//제자리로 카메라이동
			ViewMatrix = glm::rotate(ViewMatrix, 2.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 1.0f));	//y축 기준으로 카메라 회전
			ViewMatrix = glm::translate(ViewMatrix, glm::vec3(-30.0f, -10.0f, -15.0f));			//원점으로 카메라이동
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
			glutPostRedisplay();
			break;
		}
	}
}

void mousepress(int button, int state, int x, int y)  {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		flag_fill_floor = 1;
		glutPostRedisplay();
	}
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
		flag_fill_floor = 0;
		glutPostRedisplay();
	}
}

void reshape(int width, int height) {
	float aspect_ratio;
	glViewport(0, 0, width, height);

	aspect_ratio = (float)width / height;
	ProjectionMatrix = glm::perspective(15.0f*TO_RADIAN, aspect_ratio, 1.0f, 1000.0f);

	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	glutPostRedisplay();
}

void timer_scene(int timestamp_scene) {
	rotation_angle_tiger = (float)(timestamp_scene % 360) ;
	rotation_angle_cow = (float)(timestamp_scene % 181);
	rotation_angle_mintcow = (float)(timestamp_scene % 3600) * 0.5f;
	if (tiger_flag == 1) {
		rotation_tiger += 200.0f * TO_RADIAN;
	}
	else if (tiger_flag == -1) {
		rotation_tiger -= 200.0f * TO_RADIAN;
	}
	glutPostRedisplay();

	
	glutTimerFunc(100, timer_scene, (timestamp_scene + 1) % INT_MAX);
	
}

void cleanup(void) {
	glDeleteVertexArrays(1, &axes_VAO);
	glDeleteBuffers(1, &axes_VBO);

	glDeleteVertexArrays(N_OBJECTS, object_VAO);
	glDeleteBuffers(N_OBJECTS, object_VBO);
}
// End of callback function definitions

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousepress);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer_scene, 0);
	glutCloseFunc(cleanup);
}

void prepare_scene(void) {
	prepare_axes();
	prepare_square();
	prepare_tiger();
	prepare_cow();
	prepare_cat();
	prepare_car();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void print_message(const char * m) {
	fprintf(stdout, "%s\n\n", m);
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 3
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 (4.1) Simple 3D Transformations";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: 'ESC'",
		"    - Mouse used: Left Butten Click"
	};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}
