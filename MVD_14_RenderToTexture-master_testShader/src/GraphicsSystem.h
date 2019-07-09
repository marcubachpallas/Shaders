//
//  Copyright � 2018 Alun Evans. All rights reserved.
//
#pragma once
#include "includes.h"
#include "Shader.h"
#include "Components.h"
#include "GraphicsUtilities.h"
#include <unordered_map>



class GraphicsSystem {
public:
	~GraphicsSystem();
    void init(int window_width, int window_height, std::string assets);
    void lateInit();
    void update(float dt);
    
	//viewport
	void updateMainViewport(int window_width, int window_height);
    void getMainViewport(int& width, int& height);
	lm::vec4 screen_background_color;

    //shader loader
	Shader* loadShader(std::string vs_path, std::string fs_path, bool compile_direct = false);

    //set the environment
    void setEnvironment(GLuint tex_id, int geom_id, GLuint program);
    
	//materials
    int createMaterial();
	Material& getMaterial(int mat_id) { return materials_.at(mat_id); }
    
    //geometry
    int createGeometryFromFile(std::string filename);

	//lights update
	bool needUpdateLights = true;
    
private:
    //resources
    std::string assets_folder_;
	std::unordered_map<GLint, Shader*> shaders_; //compiled id, pointer
    std::vector<Geometry> geometries_;
    std::vector<Material> materials_;

    //viewport
    int viewport_width_, viewport_height_;
    
	//shader stuff
	Shader* shader_ = nullptr; //current shader
	void useShader(Shader* s);
	void useShader(GLuint p);

	//materials stuff
    GLint current_material_ = -1;
    void setMaterialUniforms();

	//sorting and checking and abstracting
	void sortMeshes_();
	void resetShaderAndMaterial_();
	void updateAllCameras_();
	void checkShaderAndMaterial(Mesh& mesh);
	
	//binding and clearing
	void bindAndClearScreen_();

	//light uniform buffer object
	GLuint LIGHTS_BINDING_POINT = 1;
	GLuint light_ubo_;
	void updateLights_();
    
    //cubemap/environment
    int cube_map_geom_ = -1;
    GLuint environment_program_ = 0;
    GLuint environment_tex_ = 0;

	//framebuffer stuff
	int screen_space_geom_;
	Shader* screen_space_shader_;
	Shader* screen_space_shader_invert;
	Shader* screen_space_shader_BnW;
	Shader* screen_space_shader_Wave;
	Shader* screen_space_shader_Blur;
	Framebuffer frame_;
    //rendering
    void renderMeshComponent_(Mesh& comp);
    void renderEnvironment_();
    
	//AABB
	void setGeometryAABB_(Geometry& geom, std::vector<GLfloat>& vertices);
	AABB transformAABB_(const AABB& aabb, const lm::mat4& transform);
	bool BBInFrustum_(const AABB& aabb, const lm::mat4& model_view_projection);
	bool AABBInFrustum_(const AABB& aabb, const lm::mat4& view_projection);

};
