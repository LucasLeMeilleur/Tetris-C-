#include "bloc.h"
#include "menu.h"
#define SFML_STATIC
#include <SFML/Graphics.hpp>

#include <iostream>
#include <codecvt>





bloc* MonblocCopy;

void Deplacement(){
    do{
        MonblocCopy->mouvement("down");     
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
    }while(!MonblocCopy->Perdu());   
}

void SetText(sf::Text &Text, sf::Font &font, int posX, int posY){
    Text.setFont(font);
    Text.setCharacterSize(20);
    Text.setPosition(posX,posY);
    Text.setFillColor(sf::Color::White);
    Text.setStyle(sf::Text::Bold);
}

void WallMaker(sf::Sprite &SpriteWall){
    for (int i = 0; i < 12; i++){
        SpriteWall.setPosition(342+18*i,420);
        MonblocCopy->drawASprite(SpriteWall);
    }
    for (int i = 0; i < 20; i++){
        SpriteWall.setPosition(540,60+18*i);
        MonblocCopy->drawASprite(SpriteWall);
        SpriteWall.setPosition(342,60+18*i);
        MonblocCopy->drawASprite(SpriteWall);
    }    
}

void CadreNextMaker(sf::Sprite &SpriteWall){
    for (int i = 0; i < 6; i++){
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

void DefinirText(std::string text, sf::Text &Label, sf::Font &Font, int x, int y){

    SetText(Label, Font, x, y);
    Label.setString(text);
    Label.setCharacterSize(15);

    return;
}


int main() {
    sf::RenderWindow window(sf::VideoMode(900,540), "Tetris game");
    sf::Thread threadDeplacement(&Deplacement);
    sf::Texture TextTruc, TextWall,CmdText,StatText;

    sf::RenderTexture renderTexture;
    renderTexture.create(900, 540);
    sf::Font font;

    if (!font.loadFromFile("asset/arial.ttf")) {
        EXIT_FAILURE;
    }

    sf::Text textScore,textNiveau,textNextPiece,textLignes;


    if (!CmdText.loadFromFile("asset/Commandes.png")) { 
        std::cerr << "Erreur : Impossible de charger l'image.\n";
        return -1;
    }

    if (!StatText.loadFromFile("asset/TableauStats.png")) { 
        std::cerr << "Erreur : Impossible de charger l'image.\n";
        return -1;
    }

    sf::Sprite SpCmd(CmdText),SpStat(StatText),Wall(TextWall);
    SpCmd.setScale(0.75f, 0.75f);
    SpCmd.setPosition(600, 350);

    SpStat.setScale(0.65f, 0.65f);
    SpStat.setPosition(100, 200);


    DefinirText("Prochaine piece : ", textNextPiece, font, 582, 70);
    
    SetText(textScore, font, 180, 335);
    SetText(textNiveau, font, 110, 240);
    SetText(textLignes, font, 180, 400);

    
    Wall.setTextureRect(sf::IntRect(0,0,18,18));
    Wall.setPosition(462, 60);

    if (!TextTruc.loadFromFile("asset/tiles.png")){
        return EXIT_FAILURE;
    }
    if (!TextWall.loadFromFile("asset/tilesWall.png")){
        return EXIT_FAILURE;
    }   

    int ValeurY=0;

    menu Menu(window, font);
    window.clear();
    while(window.isOpen()){
        sf::Event event;
        bloc Monbloc(TextTruc, window, 360, 60);        
        MonblocCopy = &Monbloc;
        Monbloc.BlocAleatoire(); Monbloc.CouleurAleatoire(); Monbloc.RegenererBloc();
        bool ThreadLance = false, TouchePresse = false;

        int MenuOptions = Menu.MenuJeu();

        if(MenuOptions == 1){
            while(true){            
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
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                    Monbloc.AjouterNiveau(1); 
                }         

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) Monbloc.mouvement("right");
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) Monbloc.mouvement("left");
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                    if(!Monbloc.DetectionBlocEmpile()){
                        Monbloc.mouvement("down");
                        if(Monbloc.GetY() != ValeurY)  Monbloc.ScoreAdd("DescenteRapide", 0);
                    }
                }
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
                        Monbloc.ChangerBloc();
                }
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
                    if(!TouchePresse){
                        Monbloc.RotationBloc();
                        TouchePresse=true;
                    }
                    
                }
                else TouchePresse = false;

                renderTexture.clear();
                Monbloc.ChangementNiveau();
                textScore.setString(Monbloc.AfficherScore());
                textNiveau.setString(Monbloc.AfficherNiveau());
                textLignes.setString(Monbloc.AfficherLigneDetruite());
                window.clear(sf::Color(15, 15, 15));
                Monbloc.DessinerLeTableau();
                Monbloc.next(); Monbloc.Saved();
                window.draw(textNiveau); window.draw(textLignes); window.draw(textScore); window.draw(textNextPiece); window.draw(SpCmd); window.draw(SpStat);
                WallMaker(Wall);
                CadreNextMaker(Wall);
                window.display();


                float centerXscore = (122 + 252) / 2.0f, centerYScore = 345;
                sf::FloatRect textBoundsScore = textScore.getLocalBounds();
                textScore.setOrigin(textBoundsScore.left + textBoundsScore.width / 2.0f, textBoundsScore.top + textBoundsScore.height / 2.0f);
                textScore.setPosition(centerXscore, centerYScore);

                float centerXlvl = (122 + 252) / 2.0f, centerYlvl = 275;
                sf::FloatRect textBoundsLvl = textNiveau.getLocalBounds();
                textNiveau.setOrigin(textBoundsLvl.left + textBoundsLvl.width / 2.0f, textBoundsLvl.top + textBoundsLvl.height / 2.0f);
                textNiveau.setPosition(centerXlvl, centerYlvl);

                float centerXLigne = (122 + 252) / 2.0f, centerYLigne = 415;
                sf::FloatRect textBoundsLignes = textLignes.getLocalBounds();
                textLignes.setOrigin(textBoundsLignes.left + textBoundsLignes.width / 2.0f, textBoundsLignes.top + textBoundsLignes.height / 2.0f);
                textLignes.setPosition(centerXLigne, centerYLigne);



                if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)){   
                    sf::Texture texture;
                    texture.create(900,540);
                    texture.update(window);         
                    threadDeplacement.terminate(); 
                    Menu.Flou(texture);
                    while (true){
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::O)){
                            threadDeplacement.launch();
                            break;
                        }
                        sf::sleep(sf::milliseconds(100));
                    }                            
                }
                sf::sleep(sf::milliseconds(150));    
                ValeurY= Monbloc.GetY();     
            }
            if(window.isOpen()) window.close();
            threadDeplacement.terminate();
            Monbloc.VoirLeTableau();
            std::cout << "\n\r---------- PERDU ! ----------";
            std::cout << "\n\n\rVoici ton score : " << Monbloc.AfficherScore() << "\n";
            std::cout << "Appuyez sur entree pour quitter...";
            Monbloc.~bloc();
            std::cin.get();
        }else if(MenuOptions == 0){
            window.close();
            Monbloc.~bloc();
            MonblocCopy = nullptr;
        }
    }    
    return 0;
} 