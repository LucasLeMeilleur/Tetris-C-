#include "menu.h"


menu::menu(sf::RenderWindow& window, sf::Font &font1){
    AddrWindow = &window;
    font = &font1;

    if (!blurShader.loadFromFile("blur.frag", sf::Shader::Fragment)) {
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