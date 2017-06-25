#version 130

uniform sampler2D texture;
uniform sampler2D distortionMapTexture;
uniform float timer;
uniform float sigma;  // between 0 and 1

void main()
{
    vec2 coord = gl_TexCoord[0].xy;
    // Get the color of the noise texture at a position the current fragment position offset by the time
    vec4 noiseTexCol = texture2D(distortionMapTexture, vec2(gl_TexCoord[0].x + 0.5*timer, gl_TexCoord[0].y + 0.5*timer));

    // Reduce the offset
    float reducedOffset = noiseTexCol.x / 80;

    // Upper part is normal
    if (coord.y+reducedOffset<sigma)
    {
        // lookup the pixel in the texture
        vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

        // multiply it by the color
        gl_FragColor = gl_Color * pixel;
    }
    else
    {
        // Get the color of the screen at the offset location
        vec4 col = texture2D(texture,  gl_TexCoord[0].xy + vec2(reducedOffset, reducedOffset));

        // Set the fragment color
        gl_FragColor = vec4 (col.r/2,col.g/2,col.b/1,col.a);
    }
}
