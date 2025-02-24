#include "bloc.h"
#include "menu.h"
#ifndef SFML_STATIC
#define SFML_STATIC
#endif
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
    Text.setPosition(sf::Vector2f(posX,posY));
    Text.setFillColor(sf::Color::White);
    Text.setStyle(sf::Text::Bold);
}



void DefinirText(std::string text, sf::Text &Label, sf::Font &Font, int x, int y){

    SetText(Label, Font, x, y);
    Label.setString(text);
    Label.setCharacterSize(15);

    return;
}


int main() {
    sf::RenderWindow window(sf::VideoMode(900, 540), "Tetris game");
    window.setActive(true);

    window.setFramerateLimit(60);  
    sf::Thread threadDeplacement(&Deplacement);
    sf::Texture TextTruc, TextWall,StatText, FondPrincipal;

    sf::RenderTexture renderTexture;
    if (!renderTexture.create(900, 540)) {
        std::cerr << "Impossible de créer la RenderTexture" << std::endl;
        return -1;
    }
    sf::Font font;

    if (!font.loadFromFile("asset/arial.ttf")) {
        EXIT_FAILURE;
    }

    sf::Text textScore,textNiveau,textNextPiece,textLignes;


    if (!FondPrincipal.loadFromFile("asset/FondPrincipal.png")) { 
        std::cerr << "Erreur : Im345possible de charger l'image.\n";
        return -1;
    }

    
    sf::Sprite SpStat(StatText), FondP(FondPrincipal);

    SpStat.setScale(0.65f, 0.65f);
    SpStat.setPosition(100, 200);

    FondP.setPosition(0,0);

    DefinirText("Prochaine piece : ", textNextPiece, font, 582, 70);
    
    SetText(textScore, font, 180, 335);
    SetText(textNiveau, font, 110, 240);
    SetText(textLignes, font, 180, 400);


    if (!TextTruc.loadFromFile("asset/tiles.png")){
        return EXIT_FAILURE;
    }
   
    int ValeurY=0;

    menu Menu(window, font);
    window.clear();

    int MenuOptions = Menu.MenuJeu();
    while(window.isOpen()){
        MonblocCopy = nullptr;
        sf::Event event;
        bloc Monbloc(TextTruc, &window, 360, 136 );        
        MonblocCopy = &Monbloc;
        Monbloc.BlocAleatoire(); Monbloc.CouleurAleatoire(); Monbloc.RegenererBloc();
        bool ThreadLance = false, TouchePresse = false;
        while(MenuOptions == 1){
            while(!MonblocCopy->Perdu()){            

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
                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                    window.close();
                    break;
                }                
                      

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
                    Monbloc.mouvement("right");

                    Monbloc.VisualiserBloc();
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
                    Monbloc.mouvement("left");

                    Monbloc.VisualiserBloc();
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
                    if(!Monbloc.DetectionBlocEmpile()){
                        Monbloc.mouvement("down");
                        if(Monbloc.GetY() != ValeurY)  Monbloc.ScoreAdd("DescenteRapide", 0);

                        Monbloc.VisualiserBloc();
                    }
                }
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)){
                        Monbloc.ChangerBloc();

                        Monbloc.VisualiserBloc();
                }
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)){
                    if(!TouchePresse){
                        std::cout << "Rotation";
                        Monbloc.RotationBloc();

                        Monbloc.VisualiserBloc();
                        TouchePresse=true;
                    }
                }
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)){
                    if(!TouchePresse){
                        Monbloc.AtterirEnBas();
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
                window.draw(FondP);

                Monbloc.DessinerLeTableau();
                Monbloc.next(); Monbloc.Saved();
                window.draw(textNiveau); window.draw(textLignes); window.draw(textScore);
                
                Monbloc.VisualiserBloc();
                window.display();
                
            
                float centerXscore = (650 + 827) / 2.0f, centerYScore = 282;
                sf::FloatRect textBoundsScore = textScore.getLocalBounds();
                textScore.setOrigin(textBoundsScore.left + textBoundsScore.width / 2.0f, textBoundsScore.top + textBoundsScore.height / 2.0f);
                textScore.setPosition(centerXscore, centerYScore);

                float centerXlvl = (66 + 246) / 2.0f, centerYlvl = 470;
                sf::FloatRect textBoundsLvl = textNiveau.getLocalBounds();
                textNiveau.setOrigin(textBoundsLvl.left + textBoundsLvl.width / 2.0f, textBoundsLvl.top + textBoundsLvl.height / 2.0f);
                textNiveau.setPosition(centerXlvl, centerYlvl);

                float centerXLigne = (66 + 246) / 2.0f, centerYLigne = 345;
                sf::FloatRect textBoundsLignes = textLignes.getLocalBounds();
                textLignes.setOrigin(textBoundsLignes.left + textBoundsLignes.width / 2.0f, textBoundsLignes.top + textBoundsLignes.height / 2.0f);
                textLignes.setPosition(centerXLigne, centerYLigne);



                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)){   

                    sf::Texture texture;
                    texture.create(900, 540);  
                    texture.update(window);       
                    
                    Menu.Pause(texture);  
                    threadDeplacement.terminate(); 
                    while (true){
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)){
                            threadDeplacement.launch();
                            break;
                        }
                        sf::sleep(sf::milliseconds(100));
                    }                            
                }
                sf::sleep(sf::milliseconds(150));    
                ValeurY= Monbloc.GetY();     
            }


            threadDeplacement.terminate();
            Monbloc.~bloc();
            MonblocCopy = nullptr;
            
            sf::Texture textureFond;
            textureFond.create(900,540);
            textureFond.update(window);       
            int ChoixMenuPerdu = Menu.MenuPerdu(Monbloc.AfficherScore(), textureFond);

            if(ChoixMenuPerdu == 1){
                if(window.isOpen()) window.close();
            }else{
                threadDeplacement.terminate();
                break;
            }
            
        }
        if(MenuOptions == 0){
            window.close();
            Monbloc.~bloc();
            MonblocCopy = nullptr;
        }
    }    
    return 0;
} 