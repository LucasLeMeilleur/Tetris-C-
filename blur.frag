uniform sampler2D texture;
uniform vec2 resolution;
uniform float radius;

const float PI = 3.1415926535897932384626433832795;

float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2.0 * sigma * sigma)) / (2.0 * PI * sigma * sigma);
}

void main() {
    vec2 texCoord = gl_TexCoord[0].xy;
    vec4 color = vec4(0.0);
    
    float sigma = radius / 2.0;
    float sum = 0.0;
    
    for (float x = -radius; x <= radius; x++) {
        for (float y = -radius; y <= radius; y++) {
            vec2 offset = vec2(x, y) / resolution;
            float weight = gaussian(length(vec2(x, y)), sigma);
            color += texture2D(texture, texCoord + offset) * weight;
            sum += weight;
        }
    }
    
    gl_FragColor = color / sum;
}
