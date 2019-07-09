#version 330

in vec2 v_uv;

out vec4 fragColor;
uniform sampler2D u_screen_texture;

void main(){
	vec3 col = texture(u_screen_texture, v_uv).xyz;
	float average = (col.r + col.g + col.b) /3;
	fragColor = vec4(vec3(average),1.0);
}
