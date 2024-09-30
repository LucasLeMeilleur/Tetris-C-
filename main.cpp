#include "bloc.hpp"
#include <SFML/Graphics.hpp>

#include <iostream>






bloc* MonblocCopy;

void Deplacement(){
    while(!MonblocCopy->Perdu()){
        MonblocCopy->DeplacementBas();     
        sf::sleep(sf::milliseconds(MonblocCopy->VitesseBloc()));   
        if(MonblocCopy->DetectionBlocEnBas() || MonblocCopy->DetectionBlocEmpile()){   
            int LigneTmp =0;  
            while(MonblocCopy->checkLine()){
                MonblocCopy->SuppLine();
                LigneTmp++;           
            }
            if(LigneTmp >= 1) MonblocCopy->ScoreAdd("Ligne", LigneTmp);
            MonblocCopy->ResetBloc();
        }           
    }
    
}

void SetText(sf::Text &Text, sf::Font &font, int posX, int posY){

    Text.setFont(font);
    Text.setCharacterSize(20);
    Text.setPosition(posX,posY);
    Text.setFillColor(sf::Color::White);
    Text.setStyle(sf::Text::Bold);


}

void WallMaker(sf::Sprite &SpriteWall){
    for (int i = 0; i < 11; i++)
    {
        SpriteWall.setPosition(342+18*i,492);
        MonblocCopy->drawASprite(SpriteWall);
    }
    for (int i = 0; i < 24; i++){
        SpriteWall.setPosition(522,60+18*i);
        MonblocCopy->drawASprite(SpriteWall);
        SpriteWall.setPosition(342,60+18*i);
        MonblocCopy->drawASprite(SpriteWall);
    }    
}

void CadreNextMaker(sf::Sprite &SpriteWall){
    for (int i = 0; i < 6; i++)
    {
        SpriteWall.setPosition(582+18*i,102);
        MonblocCopy->drawASprite(SpriteWall);
        SpriteWall.setPosition(582+18*i,210);
        MonblocCopy->drawASprite(SpriteWall);
    }
    for (int i = 0; i < 7; i++){
        SpriteWall.setPosition(582,102+18*i);
        MonblocCopy->drawASprite(SpriteWall);
        SpriteWall.setPosition(672,102+18*i);
        MonblocCopy->drawASprite(SpriteWall);
    }    

}

int main() {
    sf::RenderWindow window(sf::VideoMode(900,540), "Tetris game");
    sf::Thread threadDeplacement(&Deplacement);
    sf::Texture TextTruc;
    sf::Texture TextWall;

    sf::Font font;


    font.loadFromFile("arial.ttf");
    if (!font.loadFromFile("arial.ttf")) {
        EXIT_FAILURE;
    }

    sf::Text textScore;
    sf::Text textNiveau;
    sf::Text textNextPiece;
    SetText(textNextPiece, font, 582, 70);
    SetText(textScore, font, 582, 300);
    SetText(textNiveau, font, 582, 250);

    sf::Sprite Wall(TextWall);

    textNextPiece.setString("Prochaine piece : ");
    textNextPiece.setCharacterSize(15);

    Wall.setTextureRect(sf::IntRect(0,0,18,18));
    Wall.setPosition(462, 60);

    if (!TextTruc.loadFromFile("tiles.png")){
        return EXIT_FAILURE;
    }
    if (!TextWall.loadFromFile("tilesWall.png")){
        return EXIT_FAILURE;
    }


    window.clear();
    while(window.isOpen()){
        sf::Event event;
        bloc Monbloc(TextTruc, window, 360, 60);
        MonblocCopy = &Monbloc;
        Monbloc.BlocAleatoire(); Monbloc.CouleurAleatoire();
        Monbloc.RegenererBloc();
        bool ThreadLance = false;
        bool TouchePresse = false;
        while(1){
            if(MonblocCopy->Perdu()) break;
            if (ThreadLance == false){ 
                threadDeplacement.launch();
                ThreadLance = true;
            }            
            while(window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    break;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                window.close();
                break;
            }                
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) Monbloc.mouvement("right");
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) Monbloc.mouvement("left");
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                if(!Monbloc.DetectionBlocEmpile()){
                    Monbloc.mouvement("down");
                    Monbloc.ScoreAdd("DescenteRapide", 0);
                }
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
                if(!TouchePresse){
                    Monbloc.VoirLeTableau();
                    Monbloc.RotationBloc();
                    TouchePresse=true;
                }
                
            }
            else TouchePresse = false;

            Monbloc.ChangementNiveau();
            textScore.setString(Monbloc.Score());
            textNiveau.setString(Monbloc.AfficherNiveau());
            window.clear();

            Monbloc.DessinerLeTableau();
            Monbloc.next();

            window.draw(textNiveau);  
            window.draw(textScore); 
            window.draw(textNextPiece);
            

            WallMaker(Wall);
            CadreNextMaker(Wall);
            window.display();
            
            sf::sleep(sf::milliseconds(150));        
        }
        threadDeplacement.terminate();
        Monbloc.VoirLeTableau();
        std::cout << "\n\r---------- PERDU ! ----------";
        std::cout << "\n\n\rVoici ton score : " << Monbloc.Score() << "\n";
        break;
    }    
    return 0;
}