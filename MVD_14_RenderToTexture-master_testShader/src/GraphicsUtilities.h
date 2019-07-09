#pragma once
#include "includes.h"
#include "Shader.h"
#include "Components.h"
struct AABB {
	lm::vec3 center;
	lm::vec3 half_width;
};

struct Geometry {
	GLuint vao;
	GLuint num_tris;
	AABB aabb;
	
	//constrctors
	Geometry() { vao = 0; num_tris = 0; }
	Geometry(int a_vao, int a_tris) : vao(a_vao), num_tris(a_tris) {}
	Geometry(std::vector<float>& vertices, std::vector<float>& uvs, std::vector<float>& normals, std::vector<unsigned int>& indices);
	
	//creation functions
	void createVertexArrays(std::vector<float>& vertices, std::vector<float>& uvs, std::vector<float>& normals, std::vector<unsigned int>& indices);
	void setAABB(std::vector<GLfloat>& vertices);
	int createPlaneGeometry();

	//rendering functions
	void render();
};

struct Material {
	std::string name;
	int index = -1;
	int shader_id;
	lm::vec3 ambient;
	lm::vec3 diffuse;
	lm::vec3 specular;
	float specular_gloss;

	int diffuse_map;
	int cube_map;

	Material() {
		name = "";
		ambient = lm::vec3(0.1f, 0.1f, 0.1f);
		diffuse = lm::vec3(1.0f, 1.0f, 1.0f);
		specular = lm::vec3(1.0f, 1.0f, 1.0f);
		diffuse_map = -1;
		cube_map = -1;
		specular_gloss = 80.0f;
	}
};

struct Framebuffer {
	GLuint width, height;
	GLuint framebuffer = -1;
	GLuint num_color_attachments = 0;
	GLuint color_textures[10] = { 0,0,0,0,0,0,0,0,0,0 };
	void bindAndClear();
	void initColor(GLsizei width, GLsizei height);
};

