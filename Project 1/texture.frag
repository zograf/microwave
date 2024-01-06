#version 330 core

in vec2 chTex;
out vec4 outCol;

uniform sampler2D uTex;
uniform bool isOn;

void main() {
	if (isOn)	
		outCol = texture(uTex, chTex) * vec4(0.8, 0.9, 0.3, 1.0); 
	else
		outCol = texture(uTex, chTex); 

	//boja na koordinatama chTex teksture vezane na teksturnoj jedinici uTex
	//mijesanje 2 teksture se moze raditi sa mix(T1, T2, k) funkcijom gdje su
	//T1 i T2 pozivi texture funkcije a k koeficijent jacine druge teksture od 0 do 1 (0.3 = 70%T1 + 30%T2)
	//mijesanje sa bojom tjemena se moze odraditi mnozenjem vektora boja i tekstura
}