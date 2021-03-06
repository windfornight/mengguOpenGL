attribute vec3 pos;
attribute vec2 texcoord;
attribute vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

varying vec2 V_Texcoord;

void main()
{
	vec4 position=vec4(pos.x*2,pos.y*2,pos.z,1.0);
	V_Texcoord=texcoord;
    gl_Position=position;
}