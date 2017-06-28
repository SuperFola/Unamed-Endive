uniform sampler2D texture;
uniform float sigma;  // (1.0 += 0.5 | 0.01 | 0.001) % 6.0
uniform float width;

const int KERNEL_SIZE = 5;
float glow = 1.0 / (sigma * sqrt(2.0 * 3.14159));

float blurWeight(float x){
    return (glow * exp(-(x*x) / (2.0 * sigma * sigma)));
}

void main(){
    vec4 color = vec4(0.0);
    vec2 texCoord = gl_TexCoord[0].xy;
    
    for (int i = -KERNEL_SIZE; i <= KERNEL_SIZE; i++)
    {
        texCoord.x = gl_TexCoord[0].x + (i / width);
        color += texture2D(texture, texCoord) * blurWeight(i);
    }
    gl_FragColor = color;
}