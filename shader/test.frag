// #version 120
#version 420
uniform float mixt;
uniform vec4 inputColour;
uniform float time;
out vec4 fragColour;

void main() {
	float phase=sin(2*time);
	phase*=phase;
    fragColour = mix(vec4(0.5,0.5,0.5,1),inputColour,phase);
}