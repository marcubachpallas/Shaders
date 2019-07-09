#version 330

in vec2 v_uv;

out vec4 fragColor;
uniform sampler2D u_screen_texture;

void main(){
	vec2 textCords = v_uv;
	textCords.x += sin(textCords.y * 4*2*3.14159)/75;
	vec3 col = texture(u_screen_texture, textCords).xyz;
	//col.clamp(0.0,1.0);
	fragColor = vec4(col,1.0);
	//fragColor = vec4(1.0,0.0,0.0,1.0);
}
