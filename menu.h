#include <SFML/Graphics.hpp>


class menu{
    private:
        sf::RenderWindow* AddrWindow;
        sf::Shader blurShader;
        sf::Font* font;
    public:
        menu(sf::RenderWindow& window, sf::Font& font1);
        void Flou(sf::Texture& texture);    
};

