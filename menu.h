#include <SFML/Graphics.hpp>
#include <string>

class menu{
    private:
        sf::RenderWindow* AddrWindow;
        sf::Shader blurShader;
        sf::Font* font;
        void Flou(sf::Texture& texture);  
    public:
        menu(sf::RenderWindow& window, sf::Font& font1);
        ~menu();
        void Pause(sf::Texture& texture);
        int MenuJeu();
        int MenuPerdu(std::string score, sf::Texture& texture);  
};

