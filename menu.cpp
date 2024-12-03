#include "menu.h"



const char* blur_frag = R"(
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


)";


menu::menu(sf::RenderWindow& window, sf::Font &font1){
    AddrWindow = &window;
    font = &font1;

    if (!blurShader.loadFromMemory(blur_frag, sf::Shader::Fragment)) {
            throw std::runtime_error("Failed to load shader");
        }

    blurShader.setUniform("resolution", sf::Vector2f(900, 540));
    blurShader.setUniform("radius", 5.f);

    
}


void menu::Flou(sf::Texture& texture){


    sf::Sprite sprite(texture);
    sf::Text Pause;
    Pause.setFont(*font);
    Pause.setCharacterSize(20);
    Pause.setPosition(370,230);
    Pause.setFillColor(sf::Color::White);
    Pause.setStyle(sf::Text::Bold);
    Pause.setString("         Pause \n O pour continuer");

    AddrWindow->clear();

    AddrWindow->draw(sprite, &blurShader);

    AddrWindow->draw(Pause); 
    AddrWindow->display();
}