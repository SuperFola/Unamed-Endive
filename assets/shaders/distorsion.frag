uniform sampler2D texture;
uniform float timer;

void main() {
    vec2 coord = gl_TexCoord[0].xy;
    
    coord.x += sin(radians(timer + coord.y * 500)) * 0.07;
    coord.y += cos(radians(timer + coord.x * 250)) * 0.03;
    
    vec4 pixel_color = texture2D(texture, coord);
    
    gl_FragColor = pixel_color;
}