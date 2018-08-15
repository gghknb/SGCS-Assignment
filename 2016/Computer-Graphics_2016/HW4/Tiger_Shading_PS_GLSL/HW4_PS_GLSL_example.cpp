#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shaders/LoadShaders.h"
#include "My_Shading.h"
GLuint h_ShaderProgram_simple, h_ShaderProgram_PS; // handles to shader programs

// for simple shaders
GLint loc_ModelViewProjectionMatrix_simple, loc_primitive_color;

// for Phone Shading shaders
#define NUMBER_OF_LIGHT_SUPPORTED 4 
GLint loc_global_ambient_color;
loc_light_Parameters loc_light[NUMBER_OF_LIGHT_SUPPORTED];
loc_Material_Parameters loc_material;
GLint loc_ModelViewProjectionMatrix_PS, loc_ModelViewMatrix_PS, loc_ModelViewMatrixInvTrans_PS, loc_ModelViewProjectionMatrix;
GLint loc_u_flag_blending, loc_u_fragment_alpha;

// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> // inverseTranspose, etc.
glm::mat4 ModelViewProjectionMatrix, ModelViewMatrix;
glm::mat3 ModelViewMatrixInvTrans;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;
glm::mat4 ModelMatrix_CAR_BODY, ModelMatrix_CAR_WHEEL, ModelMatrix_CAR_NUT, ModelMatrix_CAR_DRIVER;
glm::mat4 ModelMatrix_CAR_BODY_to_DRIVER; // computed only once in initialize_camera()

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_TEXCOORD 2


// lights in scene
Light_Parameters light[NUMBER_OF_LIGHT_SUPPORTED];

/*********************************  START: camera *********************************/
typedef struct _Camera {
	float pos[3];
	float uaxis[3], vaxis[3], naxis[3];
	float fovy, aspect_ratio, near_c, far_c;
	int move;
} Camera;

Camera camera_wv;
enum _CameraType { CAMERA_WORLD_VIEWER, CAMERA_DRIVER ,CAMERA_BACKMIRROR } camera_type;

void set_ViewMatrix_for_world_viewer(void) {
	glViewport(0, 0, 800, 800);
	ViewMatrix = glm::mat4(camera_wv.uaxis[0], camera_wv.vaxis[0], camera_wv.naxis[0], 0.0f,
		camera_wv.uaxis[1], camera_wv.vaxis[1], camera_wv.naxis[1], 0.0f,
		camera_wv.uaxis[2], camera_wv.vaxis[2], camera_wv.naxis[2], 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	ViewMatrix = glm::translate(ViewMatrix, glm::vec3(-camera_wv.pos[0], -camera_wv.pos[1], -camera_wv.pos[2]));
	ViewMatrix = glm::lookAt(glm::vec3(1500.0f, 1500.0f, 1500.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ProjectionMatrix = glm::perspective(45.0f*TO_RADIAN, 800.0f / 800.0f, 1.0f, 10000.0f);
}

void set_ViewMatrix_for_driver(void) {
	glm::mat4 Matrix_CAMERA_driver_inverse;
	glViewport(0, 0, 800, 800);

	Matrix_CAMERA_driver_inverse = ModelMatrix_CAR_BODY * ModelMatrix_CAR_BODY_to_DRIVER;

	ViewMatrix = glm::affineInverse(Matrix_CAMERA_driver_inverse);
	ProjectionMatrix = glm::perspective(100.0f*TO_RADIAN, 1.0f, 1.0f, 10000.0f);
	ProjectionMatrix = glm::rotate(ProjectionMatrix, -90.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));

	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

void set_ViewMatrix_BackMirror(void) {
	glm::mat4 Matrix_CAMERA_driver_inverse;
	glViewport(150,300, 500, 250);
	Matrix_CAMERA_driver_inverse = ModelMatrix_CAR_BODY * ModelMatrix_CAR_BODY_to_DRIVER;

	ViewMatrix = glm::affineInverse(Matrix_CAMERA_driver_inverse);
	ProjectionMatrix = glm::perspective(100.0f*TO_RADIAN, 1.0f, 1.0f, 10000.0f);
	ProjectionMatrix = glm::rotate(ProjectionMatrix, 90.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));

	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
}

void initialize_camera(void) {
	camera_type = CAMERA_WORLD_VIEWER;

	camera_wv.pos[0] = 0.0f; camera_wv.pos[1] = 10.0f;  camera_wv.pos[2] = 75.0f;
	camera_wv.uaxis[0] = 1.0f; camera_wv.uaxis[1] = 0.0f; camera_wv.uaxis[2] = 0.0f;
	camera_wv.vaxis[0] = 0.0f; camera_wv.vaxis[1] = 1.0f; camera_wv.vaxis[2] = 0.0f;
	camera_wv.naxis[0] = 0.0f; camera_wv.naxis[1] = 0.0f; camera_wv.naxis[2] = 1.0f;

	camera_wv.move = 0;
	camera_wv.fovy = 30.0f, camera_wv.aspect_ratio = 1.0f; camera_wv.near_c = 5.0f; camera_wv.far_c = 10000.0f;

	set_ViewMatrix_for_world_viewer();
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	// the transformation that moves the driver's camera frame from car body's MC to driver seat
	ModelMatrix_CAR_BODY_to_DRIVER = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.5f, 2.5f));
	ModelMatrix_CAR_BODY_to_DRIVER = glm::rotate(ModelMatrix_CAR_BODY_to_DRIVER,
		TO_RADIAN*90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

/*********************************  END: camera *********************************/
// car and path object

#define N_GEOMETRY_OBJECTS 3
GLfloat *geom_obj_vertices[N_GEOMETRY_OBJECTS];

GLuint car_path_VBO, car_path_VAO;
int n_car_path_vertices;
GLfloat *car_path_vertices;
int geom_obj_n_triangles[N_GEOMETRY_OBJECTS];

GLuint geom_obj_VBO[N_GEOMETRY_OBJECTS];
GLuint geom_obj_VAO[N_GEOMETRY_OBJECTS];
int car_path_index = 0;
float slope;
float slope_x, slope_z;
float rotate_x, rotate_z;
float wheel_prev_x, wheel_prev_z;
float wheel_current_x, wheel_current_z;
float wheel_next_x, wheel_next_z;
float wheel_rotate = 0;
float wheel_rotate_rl;
float next_current_slope;
float current_prev_slope;
float timer_shading;

int rightflag = 0, leftflag = 0;
int prevx;
int car_fast = 6;
int colorflag_left = 0, colorflag_right = 0;
int highlight = 0;

void read_car_path(char *path_filename) {
	FILE *fp;
	int i;
	float *tmp_ptr;

	if ((fp = fopen(path_filename, "r")) == NULL) {
		fprintf(stderr, "Error: cannot open the car path file %s...\n", path_filename);
		exit(-1);
	}
	fscanf(fp, "%d", &n_car_path_vertices);
	tmp_ptr = car_path_vertices = (GLfloat *)malloc(n_car_path_vertices*sizeof(GLfloat)* 3);
	for (i = 0; i < n_car_path_vertices; i++) {
		fscanf(fp, "%fp", tmp_ptr);
		*tmp_ptr *= 20.0f;
		tmp_ptr++;
		fscanf(fp, "%fp", tmp_ptr);
		*tmp_ptr *= 20.0f;
		tmp_ptr++;
		fscanf(fp, "%fp", tmp_ptr);
		*tmp_ptr *= 20.0f;
		tmp_ptr++;
	//	fscanf(fp, "%f", tmp_ptr++);
	//	fscanf(fp, "%f", tmp_ptr++);
	//	fscanf(fp, "%f", tmp_ptr++);
	}
	fclose(fp);
	fprintf(stdout, "* The number of points in the car path is %d.\n", n_car_path_vertices);
}

void prepare_car(void) {
	read_car_path("Data/car_path.txt");
	// initialize vertex buffer object
	glGenBuffers(1, &car_path_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, car_path_VBO);
	glBufferData(GL_ARRAY_BUFFER, n_car_path_vertices*sizeof(GLfloat)* 3, car_path_vertices, GL_STATIC_DRAW);

	// initialize vertex array object
	glGenVertexArrays(1, &car_path_VAO);
	glBindVertexArray(car_path_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, car_path_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
}

void draw_car_path(void) {
	glUniform3f(loc_primitive_color, 1.0f, 1.0f, 0.0f);
	glBindVertexArray(car_path_VAO);
	glDrawArrays(GL_LINE_STRIP, 0, n_car_path_vertices);
	glBindVertexArray(0);
}

// for tiger animation
int cur_frame_tiger = 0;
float rotation_angle_tiger = 0.0f;

// axes object
GLuint axes_VBO, axes_VAO;
GLfloat axes_vertices[6][3] = {
	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }
};
GLfloat axes_color[3][3] = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };


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

Material_Parameters material[N_OBJECTS];

int read_geometry(GLfloat **object, int bytes_per_primitive, char *filename);

int read_triangular_mesh(GLfloat **object, int bytes_per_primitive, char *filename) {
	int n_triangles;
	FILE *fp;

	fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open the object file %s ...", filename);
		return -1;
	}
	fread(&n_triangles, sizeof(int), 1, fp);
	*object = (float *)malloc(n_triangles*bytes_per_primitive);
	if (*object == NULL) {
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...", filename);
		return -1;
	}

	fread(*object, bytes_per_primitive, n_triangles, fp);
	fprintf(stdout, "Read %d primitives successfully.\n\n", n_triangles);
	fclose(fp);

	return n_triangles;
}


void set_up_object(int object_ID, char *filename) {
	int n_bytes_per_vertex, n_bytes_per_triangle, tiger_n_total_triangles = 0;
	//char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	//for (i = 0; i < N_TIGER_FRAMES; i++) {
	//sprintf(filename, "Data/Tiger_%d%d_triangles_vnt.geom", i / 10, i % 10);
	object_n_triangles[object_ID] = read_geometry(&object_vertices[object_ID], n_bytes_per_triangle, filename);
	// assume all geometry files are effective
	tiger_n_total_triangles = object_n_triangles[object_ID];

	//if (i == 0)
	//	tiger_vertex_offset[i] = 0;
	//else
	//	tiger_vertex_offset[i] = tiger_vertex_offset[i - 1] + 3 * tiger_n_triangles[i - 1];
	//	}

	// initialize vertex buffer object
	glGenBuffers(1, &object_VBO[object_ID]);

	glBindBuffer(GL_ARRAY_BUFFER, object_VBO[object_ID]);
	glBufferData(GL_ARRAY_BUFFER, tiger_n_total_triangles*n_bytes_per_triangle, NULL, GL_STATIC_DRAW);

	//for (i = 0; i < N_TIGER_FRAMES; i++)
	glBufferSubData(GL_ARRAY_BUFFER, 0, object_n_triangles[object_ID] * n_bytes_per_triangle, object_vertices[object_ID]);

	// as the geometry data exists now in graphics memory, ...
	//for (i = 0; i < N_TIGER_FRAMES; i++)
	free(object_vertices[object_ID]);

	// initialize vertex array object
	glGenVertexArrays(1, &object_VAO[object_ID]);
	glBindVertexArray(object_VAO[object_ID]);

	glBindBuffer(GL_ARRAY_BUFFER, object_VBO[object_ID]);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (object_ID == OBJECT_SQUARE16)
	{
		material[object_ID].ambient_color[0] = 0.517647f;
		material[object_ID].ambient_color[1] = 0.439216f;
		material[object_ID].ambient_color[2] = 1.0f;
		material[object_ID].ambient_color[3] = 1.0f;

		material[object_ID].diffuse_color[0] = 0.517647f;
		material[object_ID].diffuse_color[1] = 0.439216f;
		material[object_ID].diffuse_color[2] = 1.0f;
		material[object_ID].diffuse_color[3] = 1.0f;

		material[object_ID].specular_color[0] = 0.517647f;
		material[object_ID].specular_color[1] = 0.439216f;
		material[object_ID].specular_color[2] = 1.0f;
		material[object_ID].specular_color[3] = 1.0f;

		material[object_ID].specular_exponent = 51.2f;

		material[object_ID].emissive_color[0] = 0.1f;
		material[object_ID].emissive_color[1] = 0.1f;
		material[object_ID].emissive_color[2] = 0.0f;
		material[object_ID].emissive_color[3] = 1.0f;
	}
	else if (object_ID == OBJECT_COW)
	{
		material[object_ID].ambient_color[0] = 0.180392f;
		material[object_ID].ambient_color[1] = 0.545098f;
		material[object_ID].ambient_color[2] = 0.341176f;
		material[object_ID].ambient_color[3] = 1.0f;

		material[object_ID].diffuse_color[0] = 0.180392f;
		material[object_ID].diffuse_color[1] = 0.545098f;
		material[object_ID].diffuse_color[2] = 0.341176f;
		material[object_ID].diffuse_color[3] = 1.0f;

		material[object_ID].specular_color[0] = 0.180392f;
		material[object_ID].specular_color[1] = 0.545098f;
		material[object_ID].specular_color[2] = 0.341176f;
		material[object_ID].specular_color[3] = 1.0f;

		material[object_ID].specular_exponent = 51.2f;

		material[object_ID].emissive_color[0] = 0.1f;
		material[object_ID].emissive_color[1] = 0.1f;
		material[object_ID].emissive_color[2] = 0.0f;
		material[object_ID].emissive_color[3] = 1.0f;
	}
	else if (object_ID == OBJECT_CAT)
	{
		material[object_ID].ambient_color[0] = 0.0f;
		material[object_ID].ambient_color[1] = 0.0f;
		material[object_ID].ambient_color[2] = 0.0f;
		material[object_ID].ambient_color[3] = 1.0f;

		material[object_ID].diffuse_color[0] = 0.01f;
		material[object_ID].diffuse_color[1] = 0.01f;
		material[object_ID].diffuse_color[2] = 0.01f;
		material[object_ID].diffuse_color[3] = 1.0f;

		material[object_ID].specular_color[0] = 0.527811f;
		material[object_ID].specular_color[1] = 0.526959f;
		material[object_ID].specular_color[2] = 0.526959f;
		material[object_ID].specular_color[3] = 1.0f;

		material[object_ID].specular_exponent = 51.2f;

		material[object_ID].emissive_color[0] = 0.1f;
		material[object_ID].emissive_color[1] = 0.1f;
		material[object_ID].emissive_color[2] = 0.0f;
		material[object_ID].emissive_color[3] = 1.0f;
	}
	else if (object_ID == OBJECT_BAREL)
	{
		material[object_ID].ambient_color[0] = 0.956863f;
		material[object_ID].ambient_color[1] = 0.643137f;
		material[object_ID].ambient_color[2] = 0.376471f;
		material[object_ID].ambient_color[3] = 1.0f;

		material[object_ID].diffuse_color[0] = 0.956863f;
		material[object_ID].diffuse_color[1] = 0.643137f;
		material[object_ID].diffuse_color[2] = 0.376471f;
		material[object_ID].diffuse_color[3] = 1.0f;

		material[object_ID].specular_color[0] = 0.956863f;
		material[object_ID].specular_color[1] = 0.643137f;
		material[object_ID].specular_color[2] = 0.376471f;
		material[object_ID].specular_color[3] = 1.0f;

		material[object_ID].specular_exponent = 51.2f;

		material[object_ID].emissive_color[0] = 0.1f;
		material[object_ID].emissive_color[1] = 0.1f;
		material[object_ID].emissive_color[2] = 0.0f;
		material[object_ID].emissive_color[3] = 1.0f;
	}
	else if (object_ID == OBJECT_CAR)
	{
		material[object_ID].ambient_color[0] = 0.25f;
		material[object_ID].ambient_color[1] = 0.20725f;
		material[object_ID].ambient_color[2] = 0.20725f;
		material[object_ID].ambient_color[3] = 1.0f;

		material[object_ID].diffuse_color[0] = 1.0f;
		material[object_ID].diffuse_color[1] = 0.829f;
		material[object_ID].diffuse_color[2] = 0.829f;
		material[object_ID].diffuse_color[3] = 1.0f;

		material[object_ID].specular_color[0] = 0.296648f;
		material[object_ID].specular_color[1] = 0.296648f;
		material[object_ID].specular_color[2] = 0.296648f;
		material[object_ID].specular_color[3] = 1.0f;

		material[object_ID].specular_exponent = 51.2f;

		material[object_ID].emissive_color[0] = 0.1f;
		material[object_ID].emissive_color[1] = 0.1f;
		material[object_ID].emissive_color[2] = 0.0f;
		material[object_ID].emissive_color[3] = 1.0f;
	}
}

void prepare_car2(void) {
	set_up_object(OBJECT_CAR, "Data/Car_triangles_vnt.geom");
}
void prepare_square(void) {
	// 8 = 3 for vertex, 3 for normal, and 2 for texcoord
	set_up_object(OBJECT_SQUARE16, "Data/Square16_triangles_vnt.geom");
}

void prepare_cow(void) {
	// 3 = 3 for vertex
	set_up_object(OBJECT_COW, "Data/Cow_triangles_vn.geom");
}
void prepare_barel(void) {
	// 8 = 3 for vertex, 3 for normal, and 2 for texcoord
	set_up_object(OBJECT_BAREL, "Data/Barel_triangles_vnt.geom");
}

void prepare_cat(void) {
	// 8 = 3 for vertex, 3 for normal, and 2 for texcoord
	set_up_object(OBJECT_CAT, "Data/Cat_triangles_vnt.geom");
}

void draw_object(int object_ID, float r, float g, float b) {
	glUniform3f(loc_primitive_color, r, g, b);

	glBindVertexArray(object_VAO[object_ID]);
	glDrawArrays(GL_TRIANGLES, 0, 3 * object_n_triangles[object_ID]);
	glBindVertexArray(0);
}

void prepare_axes(void) { // draw coordinate axes
	// initialize vertex buffer object
	glGenBuffers(1, &axes_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes_vertices), &axes_vertices[0][0], GL_STATIC_DRAW);

	// initialize vertex array object
	glGenVertexArrays(1, &axes_VAO);
	glBindVertexArray(axes_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

 void draw_axes(void) {
	 // assume ShaderProgram_simple is used
	 glBindVertexArray(axes_VAO);
	 glUniform3fv(loc_primitive_color, 1, axes_color[0]);
	 glDrawArrays(GL_LINES, 0, 2);
	 glUniform3fv(loc_primitive_color, 1, axes_color[1]);
	 glDrawArrays(GL_LINES, 2, 2);
	 glUniform3fv(loc_primitive_color, 1, axes_color[2]);
	 glDrawArrays(GL_LINES, 4, 2);
	 glBindVertexArray(0);
 }

 // floor object
 GLuint rectangle_VBO, rectangle_VAO;
 GLfloat rectangle_vertices[12][3] = {  // vertices enumerated counterclockwise
	 { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f },
	 { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f },
	 { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }
 };

 Material_Parameters material_floor;

 void prepare_floor(void) { // Draw coordinate axes.
	 // Initialize vertex buffer object.
	 glGenBuffers(1, &rectangle_VBO);

	 glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), &rectangle_vertices[0][0], GL_STATIC_DRAW);

	 // Initialize vertex array object.
	 glGenVertexArrays(1, &rectangle_VAO);
	 glBindVertexArray(rectangle_VAO);

	 glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
	 glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(0));
	 glEnableVertexAttribArray(0);
	 glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
 	 glEnableVertexAttribArray(1);

	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	 glBindVertexArray(0);

	 material_floor.ambient_color[0] = 0.0f;
	 material_floor.ambient_color[1] = 0.05f;
	 material_floor.ambient_color[2] = 0.0f;
	 material_floor.ambient_color[3] = 1.0f;

	 material_floor.diffuse_color[0] = 0.4f;
	 material_floor.diffuse_color[1] = 0.5f;
	 material_floor.diffuse_color[2] = 0.4f;
	 material_floor.diffuse_color[3] = 1.0f;

	 material_floor.specular_color[0] = 0.04f;
	 material_floor.specular_color[1] = 0.7f;
	 material_floor.specular_color[2] = 0.04f;
	 material_floor.specular_color[3] = 1.0f;

	 material_floor.specular_exponent = 2.5f;

	 material_floor.emissive_color[0] = 0.0f;
	 material_floor.emissive_color[1] = 0.0f;
	 material_floor.emissive_color[2] = 0.0f;
	 material_floor.emissive_color[3] = 1.0f;
 }

 void set_material_floor(void) {
	 // assume ShaderProgram_PS is used
	 glUniform4fv(loc_material.ambient_color, 1, material_floor.ambient_color);
	 glUniform4fv(loc_material.diffuse_color, 1, material_floor.diffuse_color);
	 glUniform4fv(loc_material.specular_color, 1, material_floor.specular_color);
	 glUniform1f(loc_material.specular_exponent, material_floor.specular_exponent);
	 glUniform4fv(loc_material.emissive_color, 1, material_floor.emissive_color);
 }

 void draw_floor(void) {
	 glFrontFace(GL_CCW);

	 glBindVertexArray(rectangle_VAO);
	 glDrawArrays(GL_TRIANGLES, 0, 6);
	 glBindVertexArray(0);
 }

 // tiger object
#define N_TIGER_FRAMES 12
GLuint tiger_VBO, tiger_VAO;
int tiger_n_triangles[N_TIGER_FRAMES];
int tiger_vertex_offset[N_TIGER_FRAMES];
GLfloat *tiger_vertices[N_TIGER_FRAMES];

Material_Parameters material_tiger;

int read_geometry(GLfloat **object, int bytes_per_primitive, char *filename) {
	int n_triangles;
	FILE *fp;

	// fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
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
	// fprintf(stdout, "Read %d primitives successfully.\n\n", n_triangles);
	fclose(fp);

	return n_triangles;
}

void prepare_tiger(void) { // vertices enumerated clockwise
	int i, n_bytes_per_vertex, n_bytes_per_triangle, tiger_n_total_triangles = 0;
	char filename[512];

	n_bytes_per_vertex = 8 * sizeof(float); // 3 for vertex, 3 for normal, and 2 for texcoord
	n_bytes_per_triangle = 3 * n_bytes_per_vertex;

	for (i = 0; i < N_TIGER_FRAMES; i++) {
		sprintf(filename, "Data/Tiger_%d%d_triangles_vnt.geom", i / 10, i % 10);
		tiger_n_triangles[i] = read_geometry(&tiger_vertices[i], n_bytes_per_triangle, filename);
		// assume all geometry files are effective
		tiger_n_total_triangles += tiger_n_triangles[i];

		if (i == 0)
			tiger_vertex_offset[i] = 0;
		else
			tiger_vertex_offset[i] = tiger_vertex_offset[i - 1] + 3 * tiger_n_triangles[i - 1];
	}

	// initialize vertex buffer object
	glGenBuffers(1, &tiger_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, tiger_VBO);
	glBufferData(GL_ARRAY_BUFFER, tiger_n_total_triangles*n_bytes_per_triangle, NULL, GL_STATIC_DRAW);

	for (i = 0; i < N_TIGER_FRAMES; i++)
		glBufferSubData(GL_ARRAY_BUFFER, tiger_vertex_offset[i] * n_bytes_per_vertex,
		tiger_n_triangles[i] * n_bytes_per_triangle, tiger_vertices[i]);

	// as the geometry data exists now in graphics memory, ...
	for (i = 0; i < N_TIGER_FRAMES; i++)
		free(tiger_vertices[i]);

	// initialize vertex array object
	glGenVertexArrays(1, &tiger_VAO);
	glBindVertexArray(tiger_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, tiger_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	material_tiger.ambient_color[0] = 0.24725f;
	material_tiger.ambient_color[1] = 0.1995f;
	material_tiger.ambient_color[2] = 0.0745f;
	material_tiger.ambient_color[3] = 1.0f;

	material_tiger.diffuse_color[0] = 0.75164f;
	material_tiger.diffuse_color[1] = 0.60648f;
	material_tiger.diffuse_color[2] = 0.22648f;
	material_tiger.diffuse_color[3] = 1.0f;

	material_tiger.specular_color[0] = 0.628281f;
	material_tiger.specular_color[1] = 0.555802f;
	material_tiger.specular_color[2] = 0.366065f;
	material_tiger.specular_color[3] = 1.0f;

	material_tiger.specular_exponent = 51.2f;

	material_tiger.emissive_color[0] = 0.1f;
	material_tiger.emissive_color[1] = 0.1f;
	material_tiger.emissive_color[2] = 0.0f;
	material_tiger.emissive_color[3] = 1.0f;
}
float rotation_angle_car = 0.0f;

#define rad 1.7f
#define ww 1.0f
#define N_GEOMETRY_OBJECTS 3
#define GEOM_OBJ_ID_CAR_BODY 0
#define GEOM_OBJ_ID_CAR_WHEEL 1
#define GEOM_OBJ_ID_CAR_NUT 2


// codes for the 'general' triangular-mesh object
typedef enum _GEOM_OBJ_TYPE { GEOM_OBJ_TYPE_V = 0, GEOM_OBJ_TYPE_VN, GEOM_OBJ_TYPE_VNT } GEOM_OBJ_TYPE;
// GEOM_OBJ_TYPE_V: (x, y, z)
// GEOM_OBJ_TYPE_VN: (x, y, z, nx, ny, nz)
// GEOM_OBJ_TYPE_VNT: (x, y, z, nx, ny, nz, s, t)
int GEOM_OBJ_ELEMENTS_PER_VERTEX[3] = { 3, 6, 8 };

int read_geometry_file(GLfloat **object, char *filename, GEOM_OBJ_TYPE geom_obj_type) {
	int i, n_triangles;
	float *flt_ptr;
	FILE *fp;

	fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open the geometry file %s ...", filename);
		return -1;
	}

	fscanf(fp, "%d", &n_triangles);
	*object = (float *)malloc(3 * n_triangles*GEOM_OBJ_ELEMENTS_PER_VERTEX[geom_obj_type] * sizeof(float));
	if (*object == NULL) {
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...", filename);
		return -1;
	}

	flt_ptr = *object;
	for (i = 0; i < 3 * n_triangles * GEOM_OBJ_ELEMENTS_PER_VERTEX[geom_obj_type]; i++)
		fscanf(fp, "%f", flt_ptr++);
	fclose(fp);

	fprintf(stdout, "Read %d primitives successfully.\n\n", n_triangles);

	return n_triangles;
}

void prepare_geom_obj(int geom_obj_ID, char *filename, GEOM_OBJ_TYPE geom_obj_type) {
	int n_bytes_per_vertex;

	n_bytes_per_vertex = GEOM_OBJ_ELEMENTS_PER_VERTEX[geom_obj_type] * sizeof(float);
	geom_obj_n_triangles[geom_obj_ID] = read_geometry_file(&geom_obj_vertices[geom_obj_ID], filename, geom_obj_type);

	// Initialize vertex array object.
	glGenVertexArrays(1, &geom_obj_VAO[geom_obj_ID]);
	glBindVertexArray(geom_obj_VAO[geom_obj_ID]);
	glGenBuffers(1, &geom_obj_VBO[geom_obj_ID]);
	glBindBuffer(GL_ARRAY_BUFFER, geom_obj_VBO[geom_obj_ID]);
	glBufferData(GL_ARRAY_BUFFER, 3 * geom_obj_n_triangles[geom_obj_ID] * n_bytes_per_vertex,
		geom_obj_vertices[geom_obj_ID], GL_STATIC_DRAW);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	if (geom_obj_type >= GEOM_OBJ_TYPE_VN) {
		glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	if (geom_obj_type >= GEOM_OBJ_TYPE_VNT) {
		glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	free(geom_obj_vertices[geom_obj_ID]);
}

void draw_geom_obj(int geom_obj_ID) {
	glBindVertexArray(geom_obj_VAO[geom_obj_ID]);
	glDrawArrays(GL_TRIANGLES, 0, 3 * geom_obj_n_triangles[geom_obj_ID]);
	glBindVertexArray(0);
}

void draw_wheel_and_nut(void) {
	int i;
	if (colorflag_left == 0 && colorflag_right == 1)
		glUniform3f(loc_primitive_color, 0.498f, 1.0f, 0.831f); // color name: Aquamarine
	else if (colorflag_right == 0 && colorflag_left == 1)
		glUniform3f(loc_primitive_color, 0.815f, 0.125f, 0.564f); // color name: Violet red
	else if(colorflag_right == 0 && colorflag_left == 0)
		glUniform3f(loc_primitive_color, 0.000f, 0.000f, 0.000f); // color name: BLACK
	
	draw_geom_obj(GEOM_OBJ_ID_CAR_WHEEL); // draw wheel

	for (i = 0; i < 5; i++) {
		ModelMatrix_CAR_NUT = glm::rotate(ModelMatrix_CAR_WHEEL, TO_RADIAN*72.0f*i, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix_CAR_NUT = glm::translate(ModelMatrix_CAR_NUT, glm::vec3(rad - 0.5f, 0.0f, ww));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_NUT;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

		glUniform3f(loc_primitive_color, 0.690f, 0.769f, 0.871f); // color name: LightSteelBlue
		draw_geom_obj(GEOM_OBJ_ID_CAR_NUT); // draw i-th nut
	}
}

void draw_car_dummy(void) {
	glUniform3f(loc_primitive_color, 0.498f, 1.000f, 0.831f); // color name: Aquamarine
	draw_geom_obj(GEOM_OBJ_ID_CAR_BODY); // draw body
	glLineWidth(2.0f);
	draw_axes(); // draw MC axes of body
	glLineWidth(1.0f);
	ModelMatrix_CAR_DRIVER = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.0f, 0.5f, 2.5f));
	ModelMatrix_CAR_DRIVER = glm::rotate(ModelMatrix_CAR_DRIVER, TO_RADIAN*90.0f, glm::vec3(1.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_DRIVER;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(5.0f);
	draw_axes(); // draw camera frame at driver seat
	glLineWidth(1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.9f, -3.5f, 4.5f));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, wheel_rotate_rl * -1.0f, glm::vec3(0.0f, 0.1f, 0.0f));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, wheel_rotate ,glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 0

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(3.9f, -3.5f, 4.5f));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, wheel_rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 1

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.9f, -3.5f, -4.5f));	
	ModelMatrix_CAR_WHEEL = glm::scale(ModelMatrix_CAR_WHEEL, glm::vec3(1.0f, 1.0f, -1.0f));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, wheel_rotate_rl, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, wheel_rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 2

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(3.9f, -3.5f, -4.5f));
	ModelMatrix_CAR_WHEEL = glm::scale(ModelMatrix_CAR_WHEEL, glm::vec3(1.0f, 1.0f, -1.0f));
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, wheel_rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut();  // draw wheel 3
}

void set_material_tiger(void) {
	// assume ShaderProgram_PS is used
	glUniform4fv(loc_material.ambient_color, 1, material_tiger.ambient_color);
	glUniform4fv(loc_material.diffuse_color, 1, material_tiger.diffuse_color);
	glUniform4fv(loc_material.specular_color, 1, material_tiger.specular_color);
	glUniform1f(loc_material.specular_exponent, material_tiger.specular_exponent);
	glUniform4fv(loc_material.emissive_color, 1, material_tiger.emissive_color);
}

void set_material(int object_ID) {
	// assume ShaderProgram_PS is used
	glUniform4fv(loc_material.ambient_color, 1, material[object_ID].ambient_color);
	glUniform4fv(loc_material.diffuse_color, 1, material[object_ID].diffuse_color);
	glUniform4fv(loc_material.specular_color, 1, material[object_ID].specular_color);
	glUniform1f(loc_material.specular_exponent, material[object_ID].specular_exponent);
	glUniform4fv(loc_material.emissive_color, 1, material[object_ID].emissive_color);
}

void draw_tiger(void) {
	glFrontFace(GL_CW);

	glBindVertexArray(tiger_VAO);
	glDrawArrays(GL_TRIANGLES, tiger_vertex_offset[cur_frame_tiger], 3 * tiger_n_triangles[cur_frame_tiger]);
	glBindVertexArray(0);
}

void draw(int object_ID) {
	glFrontFace(GL_CW);

	glBindVertexArray(object_VAO[object_ID]);
	glDrawArrays(GL_TRIANGLES, 0, 3 * object_n_triangles[object_ID]);
	glBindVertexArray(0);
}


#define N_GEOMETRY_OBJECTS 3
#define GEOM_OBJ_ID_CAR_BODY 0
#define GEOM_OBJ_ID_CAR_WHEEL 1
#define GEOM_OBJ_ID_CAR_NUT 2

// codes for the 'general' triangular-mesh object
//typedef enum _GEOM_OBJ_TYPE { GEOM_OBJ_TYPE_V = 0, GEOM_OBJ_TYPE_VN, GEOM_OBJ_TYPE_VNT } GEOM_OBJ_TYPE;
// GEOM_OBJ_TYPE_V: (x, y, z)
// GEOM_OBJ_TYPE_VN: (x, y, z, nx, ny, nz)
// GEOM_OBJ_TYPE_VNT: (x, y, z, nx, ny, nz, s, t)


// callbacks
void display(void) {
	//glm::mat4 ModelMatrix_big_cow, ModelMatrix_small_cow;
	//glm::mat4 ModelMatrix_big_box, ModelMatrix_small_box;


	if (camera_type == CAMERA_DRIVER) set_ViewMatrix_for_driver();
	else if (camera_type == CAMERA_BACKMIRROR) set_ViewMatrix_BackMirror();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(h_ShaderProgram_simple);

	/* car */

	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(2.0f);
	draw_axes();
	glLineWidth(1.0f);

	ModelMatrix_CAR_BODY = glm::translate(glm::mat4(1.0f), glm::vec3(car_path_vertices[car_path_index],  4.9f * 20, car_path_vertices[car_path_index + 2]));
	ModelMatrix_CAR_BODY = glm::scale(ModelMatrix_CAR_BODY,glm::vec3(20.0f, 20.0f, 20.0f));
	ModelMatrix_CAR_BODY = glm::rotate(ModelMatrix_CAR_BODY, 90*TO_RADIAN + slope, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix_CAR_BODY;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_car_dummy();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* car path*/
	ModelViewMatrix = ViewMatrix;
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(2.0f);
	draw_car_path();
	glLineWidth(1.0f);

	ModelViewMatrix = glm::scale(ViewMatrix, glm::vec3(50.0f, 50.0f, 50.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(2.0f);
	draw_axes();
	glLineWidth(1.0f);

	glUseProgram(h_ShaderProgram_PS);
  	set_material_floor();
	ModelViewMatrix = glm::translate(ViewMatrix, glm::vec3(-750.0f, 0.0f, 750.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(1500.0f, 1500.0f, 1500.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));

	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw_floor();

	/* blending*/
	if (camera_type == CAMERA_BACKMIRROR) {
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform1i(loc_u_flag_blending, 1);
		set_material_floor();

		glUniform1f(loc_u_fragment_alpha, 0.5f);

		ModelViewMatrix = ViewMatrix * ModelMatrix_CAR_BODY;
		ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(1.0f, -4.0f, 0.0f));
		ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
		ModelViewMatrix = glm::rotate(ModelViewMatrix, 90 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
		ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
		ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

		glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
		glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
		draw_floor();

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

 	set_material_tiger();
 	ModelViewMatrix = glm::rotate(ViewMatrix, -rotation_angle_tiger, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(200.0f, 0.0f, 0.0f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw_tiger();

	glUseProgram(h_ShaderProgram_simple);
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(20.0f, 20.0f, 20.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_axes();



	/* draw cat*/
	glUseProgram(h_ShaderProgram_PS);
	set_material(OBJECT_CAT);

	ModelViewMatrix = glm::rotate(ViewMatrix, -90 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(100.0f, 0.0f, -500.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(200.0f, 200.0f, 200.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw(OBJECT_CAT);

	/* draw square*/
	//glUseProgram(h_ShaderProgram_PS);
	set_material(OBJECT_SQUARE16);

	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(2.8f, 0.0f, -0.56f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, -80 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw(OBJECT_SQUARE16);

	/* draw cow*/
//	glUseProgram(h_ShaderProgram_PS);
	set_material(OBJECT_COW);

	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(5.5f, 0.4f, 1.5f));
	ModelViewMatrix = glm::rotate(ModelViewMatrix, 180 * TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw(OBJECT_COW);

	/*draw barel*/
	//glUseProgram(h_ShaderProgram_PS);
	set_material(OBJECT_BAREL);

	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(26.0f, 0.0f, -29.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw(OBJECT_BAREL);

	/* draw car*/
	set_material(OBJECT_CAR);
	ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(15.0f, 0.0f, 40.0f));
	ModelViewMatrix = glm::scale(ModelViewMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
	//ModelViewMatrix = glm::translate(ModelViewMatrix, glm::vec3(26.0f, 0.0f, -29.0f));
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_PS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_PS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_PS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw(OBJECT_CAR);

	glUseProgram(0);
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	static int flag_cull_face = 0;
	static int flag_polygon_mode = 1;

		if ((key >= '0') && (key <= '0' + NUMBER_OF_LIGHT_SUPPORTED - 1)) {
		int light_ID = (int) (key - '0');

		glUseProgram(h_ShaderProgram_PS);
		light[light_ID].light_on = 1 - light[light_ID].light_on;
		glUniform1i(loc_light[light_ID].light_on, light[light_ID].light_on);
		glUseProgram(0);

		glutPostRedisplay();
		return;
	}

	switch (key) {
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups
		break;
	case 'c':
		flag_cull_face = (flag_cull_face + 1) % 3;
		switch (flag_cull_face) {
		case 0:
			glDisable(GL_CULL_FACE);
			glutPostRedisplay();
			break;
		case 1: // cull back faces;
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			glutPostRedisplay();
			break;
		case 2: // cull front faces;
			glCullFace(GL_FRONT);
			glEnable(GL_CULL_FACE);
			glutPostRedisplay();
			break;
		}
		break;
	case 'p':
		flag_polygon_mode = 1 - flag_polygon_mode;
		if (flag_polygon_mode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	case 'd' :
		camera_type = CAMERA_DRIVER;
		glutPostRedisplay();
		break;
	case 'w' : 
		camera_type = CAMERA_WORLD_VIEWER;
		set_ViewMatrix_for_world_viewer();	
		ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		glutPostRedisplay();
		break;
	case 'b' :
		camera_type = CAMERA_BACKMIRROR;
		glutPostRedisplay();
		break;
	case 'h' :
		highlight = 1 - highlight;
		glutPostRedisplay();
 	}
}

void set_up_scene_lights(void) {
	// point_light_EC: use light 0
	light[0].light_on = 1;
	light[0].position[0] = 0.0f; light[0].position[1] = 10.0f; 	// point light position in EC
	light[0].position[2] = 0.0f; light[0].position[3] = 1.0f;

	light[0].ambient_color[0] = 0.3f; light[0].ambient_color[1] = 0.3f;
	light[0].ambient_color[2] = 0.3f; light[0].ambient_color[3] = 1.0f;

	light[0].diffuse_color[0] = 0.7f; light[0].diffuse_color[1] = 0.7f;
	light[0].diffuse_color[2] = 0.7f; light[0].diffuse_color[3] = 1.0f;

	light[0].specular_color[0] = 0.9f; light[0].specular_color[1] = 0.9f;
	light[0].specular_color[2] = 0.9f; light[0].specular_color[3] = 1.0f;

	// spot_light_WC: use light 1
	light[1].light_on = 1;
	light[1].position[0] = -200.0f; light[1].position[1] = 500.0f; // spot light position in WC
	light[1].position[2] = -200.0f; light[1].position[3] = 1.0f;

	light[1].ambient_color[0] = 0.2f; light[1].ambient_color[1] = 0.2f;
	light[1].ambient_color[2] = 0.2f; light[1].ambient_color[3] = 1.0f;

	light[1].diffuse_color[0] = 0.82f; light[1].diffuse_color[1] = 0.82f;
	light[1].diffuse_color[2] = 0.82f; light[1].diffuse_color[3] = 1.0f;

	light[1].specular_color[0] = 0.82f; light[1].specular_color[1] = 0.82f;
	light[1].specular_color[2] = 0.82f; light[1].specular_color[3] = 1.0f;

	light[1].spot_direction[0] = 0.0f; light[1].spot_direction[1] = -1.0f; // spot light direction in WC
	light[1].spot_direction[2] = 0.0f;
	light[1].spot_cutoff_angle = 20.0f;
	light[1].spot_exponent = 27.0f;

	// spot_light_WC: use light 2
	light[2].light_on = highlight;
	/* position */
	light[2].position[0] = 0.0f; light[2].position[1] = 0.0f; //spot light position in WC
	light[2].position[2] = 0.0f; light[2].position[3] = 1.0f;
	/* ambient */
	light[2].ambient_color[0] = 0.2f; light[2].ambient_color[1] = 0.2f;
	light[2].ambient_color[2] = 0.2f; light[2].ambient_color[3] = 1.0f;
	/* diffuse */
	light[2].diffuse_color[0] = 0.82f; light[2].diffuse_color[1] = 0.82f;
	light[2].diffuse_color[2] = 0.82f; light[2].diffuse_color[3] = 1.0f;
	/* specular*/
	light[2].specular_color[0] = 0.82f; light[2].specular_color[1] = 0.82f;
	light[2].specular_color[2] = 0.82f; light[2].specular_color[3] = 1.0f;
	/* spot */
	light[2].spot_direction[0] = 0.0f; light[2].spot_direction[1] = -1.0f; // spot light direction in WC
	light[2].spot_direction[2] = 0.0f;
	light[2].spot_cutoff_angle = 20.0f;
	light[2].spot_exponent = 5.0f;

	// spot_light_WC: use light 3
	light[3].light_on = highlight;
	light[3].position[0] = 0.0f; light[3].position[1] = 0.0f; // spot light position in WC
	light[3].position[2] = 0.0f; light[3].position[3] = 1.0f;

	light[3].ambient_color[0] = 0.2f; light[3].ambient_color[1] = 0.2f;
	light[3].ambient_color[2] = 0.2f; light[3].ambient_color[3] = 1.0f;

	light[3].diffuse_color[0] = 0.82f; light[3].diffuse_color[1] = 0.82f;
	light[3].diffuse_color[2] = 0.82f; light[3].diffuse_color[3] = 1.0f;

	light[3].specular_color[0] = 0.82f; light[3].specular_color[1] = 0.82f;
	light[3].specular_color[2] = 0.82f; light[3].specular_color[3] = 1.0f;

	light[3].spot_direction[0] = 0.0f; light[3].spot_direction[1] = -1.0f; // spot light direction in WC
	light[3].spot_direction[2] = 0.0f;
	light[3].spot_cutoff_angle = 20.0f;
	light[3].spot_exponent = 5.0f;

	glUseProgram(h_ShaderProgram_PS);
	glUniform1i(loc_light[0].light_on, light[0].light_on);
	glUniform4fv(loc_light[0].position, 1, light[0].position);
	glUniform4fv(loc_light[0].ambient_color, 1, light[0].ambient_color);
	glUniform4fv(loc_light[0].diffuse_color, 1, light[0].diffuse_color);
	glUniform4fv(loc_light[0].specular_color, 1, light[0].specular_color);

	glUniform1i(loc_light[1].light_on, light[1].light_on);
	// need to supply position in EC for shading
	glm::vec4 position_EC = ViewMatrix * glm::vec4(light[1].position[0], light[1].position[1],
		light[1].position[2], light[1].position[3]);
	glUniform4fv(loc_light[1].position, 1, &position_EC[0]);
	glUniform4fv(loc_light[1].ambient_color, 1, light[1].ambient_color);
	glUniform4fv(loc_light[1].diffuse_color, 1, light[1].diffuse_color);
	glUniform4fv(loc_light[1].specular_color, 1, light[1].specular_color);
	// need to supply direction in EC for shading in this example shader
	// note that the viewing transform is a rigid body transform
	// thus transpose(inverse(mat3(ViewMatrix)) = mat3(ViewMatrix)
	glm::vec3 direction_EC = glm::mat3(ViewMatrix) * glm::vec3(light[1].spot_direction[0], light[1].spot_direction[1],
		light[1].spot_direction[2]);
	glUniform3fv(loc_light[1].spot_direction, 1, &direction_EC[0]);
	glUniform1f(loc_light[1].spot_cutoff_angle, light[1].spot_cutoff_angle);
	glUniform1f(loc_light[1].spot_exponent, light[1].spot_exponent *  sin(timer_shading* TO_RADIAN));

	/* light 2(headlight)*/
	glUniform1i(loc_light[2].light_on, light[2].light_on);
	// need to supply position in EC for shading
	position_EC = ViewMatrix * glm::translate(ModelMatrix_CAR_BODY, glm::vec3(5.0f, 0.5f, -2.5f))
		* glm::vec4(light[2].position[0], light[2].position[1], light[2].position[2], light[2].position[3]);
	glUniform4fv(loc_light[2].position, 1, &position_EC[0]);
	glUniform4fv(loc_light[2].ambient_color, 1, light[2].ambient_color);
	glUniform4fv(loc_light[2].diffuse_color, 1, light[2].diffuse_color);
	glUniform4fv(loc_light[2].specular_color, 1, light[2].specular_color);
	// need to supply direction in EC for shading in this example shader
	// note that the viewing transform is a rigid body transform
	// thus transpose(inverse(mat3(ViewMatrix)) = mat3(ViewMatrix)
	direction_EC = glm::mat3(ViewMatrix) * glm::mat3(ModelMatrix_CAR_BODY) * glm::mat3(glm::rotate(glm::mat4(1.0f), -75.0f*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f)))
		* glm::vec3(light[2].spot_direction[0], light[2].spot_direction[1], light[2].spot_direction[2]);
	glUniform3fv(loc_light[2].spot_direction, 1, &direction_EC[0]);
	glUniform1f(loc_light[2].spot_cutoff_angle, light[2].spot_cutoff_angle);
	glUniform1f(loc_light[2].spot_exponent, light[2].spot_exponent);

	/* light 3(headlight)*/
	glUniform1i(loc_light[3].light_on, light[3].light_on);
	// need to supply position in EC for shading
	position_EC = ViewMatrix * glm::translate(ModelMatrix_CAR_BODY, glm::vec3(5.0f, 0.5f, 2.5f))
		*glm::vec4(light[3].position[0], light[3].position[1], light[3].position[2], light[3].position[3]);
	glUniform4fv(loc_light[3].position, 1, &position_EC[0]);
	glUniform4fv(loc_light[3].ambient_color, 1, light[3].ambient_color);
	glUniform4fv(loc_light[3].diffuse_color, 1, light[3].diffuse_color);
	glUniform4fv(loc_light[3].specular_color, 1, light[3].specular_color);
	// need to supply direction in EC for shading in this example shader
	// note that the viewing transform is a rigid body transform
	// thus transpose(inverse(mat3(ViewMatrix)) = mat3(ViewMatrix)
	direction_EC = glm::mat3(ViewMatrix) * glm::mat3(ModelMatrix_CAR_BODY) * glm::mat3(glm::rotate(glm::mat4(1.0f), -75.0f*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f)))
		* glm::vec3(light[3].spot_direction[0], light[3].spot_direction[1], light[3].spot_direction[2]);
	glUniform3fv(loc_light[3].spot_direction, 1, &direction_EC[0]);
	glUniform1f(loc_light[3].spot_cutoff_angle, light[3].spot_cutoff_angle);
	glUniform1f(loc_light[3].spot_exponent, light[3].spot_exponent);

	glUseProgram(0);
}

void mouse(int button, int state, int x, int y) {
	if ((button == GLUT_RIGHT_BUTTON)) {
		if (state == GLUT_DOWN) {
			rightflag = 1;
		}
		else rightflag = 0;
	}
	else rightflag = 0;
}

void motion(int x, int y) {
	
	/* car direction*/
	if (car_path_index >= 0 && car_path_index < (751-car_fast) *3) {
		slope_x = car_path_vertices[car_path_index + 3] - car_path_vertices[car_path_index];
		slope_z = car_path_vertices[car_path_index + 5] - car_path_vertices[car_path_index + 2];
	}
	else if (car_path_index >= (751-car_fast) * 3) {
		slope_x = car_path_vertices[car_path_index] - car_path_vertices[car_path_index - 3];
		slope_z = car_path_vertices[car_path_index -1] - car_path_vertices[car_path_index -4];
	}
	slope = dot(glm::vec3(0, 0, 1), glm::vec3(slope_x, 0, slope_z));
	slope /= sqrt(slope_x * slope_x + slope_z * slope_z);
	slope = acos(slope);

	/* move car*/
	if (rightflag == 1 && prevx > x)
	{
		if (car_path_index < 3 * (751 - car_fast))
		{
			car_path_index = car_path_index + 3 * car_fast; 
			wheel_rotate -= 20;			//wheel direction(front,back)
			/* wheel rotate(right,left)*/
			wheel_current_x = car_path_vertices[car_path_index + 3] - car_path_vertices[car_path_index];
			wheel_current_z = car_path_vertices[car_path_index + 5] - car_path_vertices[car_path_index + 2];
			wheel_next_x = car_path_vertices[car_path_index + 6] - car_path_vertices[car_path_index + 3];
			wheel_next_z = car_path_vertices[car_path_index + 8] - car_path_vertices[car_path_index + 5];
			
			next_current_slope = (wheel_next_z / wheel_next_x);
			current_prev_slope = (wheel_current_z / wheel_current_x);
			if (car_path_index + 3 > 3 * (751 - car_fast))
			{
				colorflag_left = 0;
				colorflag_right = 0;
				wheel_rotate_rl = 0.0f;
			}
		    else if (next_current_slope < current_prev_slope)
			{
				wheel_rotate_rl = -35.0f * TO_RADIAN;
				colorflag_left = 1;
			}
			else if (next_current_slope - current_prev_slope <= 0.005)
			{
				wheel_rotate_rl = 0.0f;
				colorflag_left = 0;
				colorflag_right = 0;
			}
			else if (next_current_slope > current_prev_slope)
			{
				wheel_rotate_rl = 35.0f * TO_RADIAN;
				colorflag_right = 1;
			}
		}
	}
	else if (rightflag == 1 && prevx < x)
	{
		if (car_path_index > 3 * car_fast)
		{
			car_path_index = car_path_index - 3 * car_fast;
			wheel_rotate += 20;		//wheel direction(front,back)
			/* wheel rotate(right,left)*/
			wheel_current_x = car_path_vertices[car_path_index] - car_path_vertices[car_path_index + 3];
			wheel_current_z = car_path_vertices[car_path_index + 2] - car_path_vertices[car_path_index + 5];
			wheel_next_x = car_path_vertices[car_path_index + 3] - car_path_vertices[car_path_index + 6];
			wheel_next_z = car_path_vertices[car_path_index + 5] - car_path_vertices[car_path_index + 8];
			next_current_slope = (wheel_next_z / wheel_next_x);
			current_prev_slope = (wheel_current_z / wheel_current_x);
			if (car_path_index - 3 < 3 * car_fast)
			{
				wheel_rotate_rl = 0.0f;
				colorflag_left = 0;
				colorflag_right = 0;
			}
		    else if (next_current_slope < current_prev_slope)
			{
				colorflag_left = 1;
				wheel_rotate_rl = -35.0f * TO_RADIAN;
			}
			else if (next_current_slope - current_prev_slope <= 0.005)
			{
				wheel_rotate_rl = 0;
				colorflag_left = 0;
				colorflag_right = 0;
			}
			else if (next_current_slope > current_prev_slope)
			{
				wheel_rotate_rl = 35.0f * TO_RADIAN;
				colorflag_right = 1;
			}
		}
	}
	else if(prevx == x)
		wheel_rotate = 0;
	
	prevx = x;
	
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	glutPostRedisplay();
}

void reshape(int width, int height) {
	float aspect_ratio;
	glViewport(0, 0, width, height);
	
	aspect_ratio = (float) width / height;
	ProjectionMatrix = glm::perspective(45.0f*TO_RADIAN, aspect_ratio, 1.0f, 10000.0f);
	glutPostRedisplay();
}

void timer_scene(int timestamp_scene) {
    set_up_scene_lights();
	cur_frame_tiger = timestamp_scene % N_TIGER_FRAMES;
	rotation_angle_tiger = (timestamp_scene % 360)*TO_RADIAN;
	rotation_angle_car = (timestamp_scene % 360)*TO_RADIAN;
	timer_shading = timestamp_scene % 360 * TO_RADIAN;
	glutPostRedisplay();
	glutTimerFunc(100, timer_scene, (timestamp_scene + 1) % INT_MAX);
}

void cleanup(void) {
	glDeleteVertexArrays(1, &axes_VAO);
	glDeleteBuffers(1, &axes_VBO);

	glDeleteVertexArrays(1, &tiger_VAO);
	glDeleteBuffers(1, &tiger_VBO);
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer_scene, 0);
	glutCloseFunc(cleanup);
}

void prepare_shader_program(void) {
	int i;
	char string[256];
	ShaderInfo shader_info_simple[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};
	ShaderInfo shader_info_PS[3] = {
		{ GL_VERTEX_SHADER, "Shaders/Phong.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/Phong.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram_simple = LoadShaders(shader_info_simple);
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram_simple, "u_primitive_color");
	loc_ModelViewProjectionMatrix_simple = glGetUniformLocation(h_ShaderProgram_simple, "u_ModelViewProjectionMatrix");

	h_ShaderProgram_PS = LoadShaders(shader_info_PS);
	loc_ModelViewProjectionMatrix_PS = glGetUniformLocation(h_ShaderProgram_PS, "u_ModelViewProjectionMatrix");
	loc_ModelViewMatrix_PS = glGetUniformLocation(h_ShaderProgram_PS, "u_ModelViewMatrix");
	loc_ModelViewMatrixInvTrans_PS = glGetUniformLocation(h_ShaderProgram_PS, "u_ModelViewMatrixInvTrans");

	loc_global_ambient_color = glGetUniformLocation(h_ShaderProgram_PS, "u_global_ambient_color");
	for (i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		sprintf(string, "u_light[%d].light_on", i);
		loc_light[i].light_on = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].position", i);
		loc_light[i].position = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].ambient_color", i);
		loc_light[i].ambient_color = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].diffuse_color", i);
		loc_light[i].diffuse_color = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].specular_color", i);
		loc_light[i].specular_color = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].spot_direction", i);
		loc_light[i].spot_direction = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].spot_exponent", i);
		loc_light[i].spot_exponent = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].spot_cutoff_angle", i);
		loc_light[i].spot_cutoff_angle = glGetUniformLocation(h_ShaderProgram_PS, string);
		sprintf(string, "u_light[%d].light_attenuation_factors", i);
		loc_light[i].light_attenuation_factors = glGetUniformLocation(h_ShaderProgram_PS, string);
	}
	loc_material.ambient_color = glGetUniformLocation(h_ShaderProgram_PS, "u_material.ambient_color");
	loc_material.diffuse_color = glGetUniformLocation(h_ShaderProgram_PS, "u_material.diffuse_color");
	loc_material.specular_color = glGetUniformLocation(h_ShaderProgram_PS, "u_material.specular_color");
	loc_material.emissive_color = glGetUniformLocation(h_ShaderProgram_PS, "u_material.emissive_color");
	loc_material.specular_exponent = glGetUniformLocation(h_ShaderProgram_PS, "u_material.specular_exponent");

	loc_u_flag_blending = glGetUniformLocation(h_ShaderProgram_PS, "u_flag_blending");
	loc_u_fragment_alpha = glGetUniformLocation(h_ShaderProgram_PS, "u_fragment_alpha");
}

void initialize_lights_and_material(void) { // follow OpenGL conventions for initialization
	int i;

	glUseProgram(h_ShaderProgram_PS);

	glUniform4f(loc_global_ambient_color, 0.2f, 0.2f, 0.2f, 1.0f);
	for (i = 0; i < NUMBER_OF_LIGHT_SUPPORTED; i++) {
		glUniform1i(loc_light[i].light_on, 0); // turn off all lights initially
		glUniform4f(loc_light[i].position, 0.0f, 0.0f, 1.0f, 0.0f);
		glUniform4f(loc_light[i].ambient_color, 0.0f, 0.0f, 0.0f, 1.0f);
		if (i == 0) {
			glUniform4f(loc_light[i].diffuse_color, 1.0f, 1.0f, 1.0f, 1.0f);
			glUniform4f(loc_light[i].specular_color, 1.0f, 1.0f, 1.0f, 1.0f);
		}
		else {
			glUniform4f(loc_light[i].diffuse_color, 0.0f, 0.0f, 0.0f, 1.0f);
			glUniform4f(loc_light[i].specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
		}
		glUniform3f(loc_light[i].spot_direction, 0.0f, 0.0f, -1.0f);
		glUniform1f(loc_light[i].spot_exponent, 0.0f); // [0.0, 128.0]
		glUniform1f(loc_light[i].spot_cutoff_angle, 180.0f); // [0.0, 90.0] or 180.0 (180.0 for no spot light effect)
		glUniform4f(loc_light[i].light_attenuation_factors, 1.0f, 0.0f, 0.0f, 0.0f); // .w != 0.0f for no ligth attenuation
	}

	glUniform4f(loc_material.ambient_color, 0.2f, 0.2f, 0.2f, 1.0f);
	glUniform4f(loc_material.diffuse_color, 0.8f, 0.8f, 0.8f, 1.0f);
	glUniform4f(loc_material.specular_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform4f(loc_material.emissive_color, 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform1f(loc_material.specular_exponent, 0.0f); // [0.0, 128.0]

	glUseProgram(0);
}

void initialize_OpenGL(void) {
  	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	ViewMatrix = glm::lookAt(glm::vec3(1500.0f, 1500.0f, 1500.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	initialize_lights_and_material();

	ModelMatrix_CAR_BODY = glm::mat4(1.0f);
	ModelMatrix_CAR_WHEEL = glm::mat4(1.0f);
	ModelMatrix_CAR_NUT = glm::mat4(1.0f);
}


void prepare_scene(void) {
	prepare_axes();
	prepare_floor();
	prepare_tiger();
	prepare_car();
	set_up_scene_lights();
	prepare_geom_obj(GEOM_OBJ_ID_CAR_BODY, "Data/car_body_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_geom_obj(GEOM_OBJ_ID_CAR_WHEEL, "Data/car_wheel_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_geom_obj(GEOM_OBJ_ID_CAR_NUT, "Data/car_nut_triangles_v.txt", GEOM_OBJ_TYPE_V);
	prepare_square();
	prepare_cow();
	prepare_barel();
	prepare_cat();
	prepare_car2();
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

#define N_MESSAGE_LINES 1
void main(int argc, char *argv[]) {
	// Phong Shading
	char program_name[64] = "Sogang CSE4170 HW4";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: '0', '1', 'c', 'p', 'ESC'"  };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 800);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}