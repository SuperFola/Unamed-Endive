uniform sampler2D texture;
uniform float sigma;  // best is 0.5

float dark = -16.0 * sigma;

float sigmoid(float n) {
    return 1.0 / (1 + exp(dark * (n - 0.8)));
}

void main() {
    vec2 coord = gl_TexCoord[0].xy;
    vec4 pixel_color = texture2D(texture, coord);
    
    vec4 color;
    color.r = sigmoid(pixel_color.r);
    color.g = sigmoid(pixel_color.g);
    color.b = sigmoid(pixel_color.b);
    color.a = sigmoid(pixel_color.a);
    
    gl_FragColor = color;
}