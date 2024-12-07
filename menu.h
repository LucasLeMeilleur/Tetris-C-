#include <SFML/Graphics.hpp>


class menu{
    private:
        sf::RenderWindow* AddrWindow;
        sf::Shader blurShader;
        sf::Font* font;
    public:
        menu(sf::RenderWindow& window, sf::Font& font1);
        ~menu();
        void Flou(sf::Texture& texture);  
        int MenuJeu();
        int MenuPerdu();  
};

