#version 330

//varyings and out color
in vec2 v_uv;
in vec3 v_normal;
in vec3 v_light_dir;
in vec3 v_cam_dir;
in vec3 v_vertex_world_pos;
out vec4 fragColor;

//basic material uniforms
uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform float u_specular_gloss;

//texture uniforms
uniform int u_use_diffuse_map;
uniform sampler2D u_diffuse_map;

//light structs and uniforms
struct Light {
    vec4 position;
    vec4 direction;
    vec4 color;
    float linear_att;
    float quadratic_att;
    float spot_inner_cosine;
    float spot_outer_cosine;
    int type; // 0 - directional; 1 - point; 2 - spot
};

const int MAX_LIGHTS = 8;
uniform int u_num_lights;

layout (std140) uniform Lights
{
    Light lights[MAX_LIGHTS]; 
};


void main(){

	vec3 mat_diffuse = u_diffuse; //colour from uniform
	//multiply by texture if present
	if (u_use_diffuse_map != 0)
		mat_diffuse = mat_diffuse * texture(u_diffuse_map, v_uv).xyz;

	//ambient light
	vec3 final_color = u_ambient * mat_diffuse;
	

	//loop lights
	for (int i = 0; i < u_num_lights; i++){

        float attenuation = 1.0;
        
        float spot_cone_intensity = 1.0;
        
		vec3 L = normalize(-lights[i].direction.xyz); // for directional light
		vec3 N = normalize(v_normal); //normal
		vec3 R = reflect(-L,N); //reflection vector
		vec3 V = normalize(v_cam_dir); //to camera
        
        if (lights[i].type > 0) {
        
            vec3 point_to_light = lights[i].position.xyz - v_vertex_world_pos;
            L = normalize(point_to_light);

            // soft spot cone
            if (lights[i].type == 2) {
                vec3 D = normalize(lights[i].direction.xyz);
                float cos_theta = dot(D, -L);
                
                float numer = cos_theta - lights[i].spot_outer_cosine;
                float denom = lights[i].spot_inner_cosine - lights[i].spot_outer_cosine;
                spot_cone_intensity = clamp(numer/denom, 0.0, 1.0);


            }
            
            //attenuation
            float distance = length(point_to_light);
            attenuation = 1.0 / (1.0 + lights[i].linear_att * distance + lights[i].quadratic_att * (distance * distance));
        }
        
        
		//diffuse color
		float NdotL = max(0.0, dot(N, L));
		vec3 diffuse_color = NdotL * mat_diffuse * lights[i].color.xyx;
							 
		//specular color
		float RdotV = max(0.0, dot(R, V)); //calculate dot product
		RdotV = pow(RdotV, u_specular_gloss); //raise to power for glossiness effect
		vec3 specular_color = RdotV * lights[i].color.xyz * u_specular;

		//final color
        final_color += (diffuse_color + specular_color) * attenuation * spot_cone_intensity;
	}

	fragColor = vec4(final_color, 1.0);
}
