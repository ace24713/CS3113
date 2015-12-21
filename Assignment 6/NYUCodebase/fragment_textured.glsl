
uniform sampler2D diffuse;
uniform float effectCutoff;
varying vec2 texCoordVar;

void main() {
	if (length(texCoordVar - vec2(.5f,.5f)) < effectCutoff)
		gl_FragColor = vec4(0);
	else
		gl_FragColor = texture2D(diffuse, texCoordVar);
}