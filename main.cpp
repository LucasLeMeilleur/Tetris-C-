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
                LigneTmp++;
                MonblocCopy->SuppLine();   
                MonblocCopy->ScoreAdd("Ligne", LigneTmp);            
            }
            MonblocCopy->ResetBloc();
        }    
    }
}



int main() {
    sf::RenderWindow window(sf::VideoMode(900,540), "Tetris game");
    sf::Thread threadDeplacement(&Deplacement);
    sf::Texture TextTruc;
    if (!TextTruc.loadFromFile("tiles.png")){
        return EXIT_FAILURE;
    }
    window.clear();
    while(window.isOpen()){
        sf::Event event;
        bloc Monbloc(TextTruc, window, 360, 60);
        MonblocCopy = &Monbloc;
        Monbloc.assembly();
        bool ThreadLance = false;
        while(1){
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
                }else MonblocCopy->ResetBloc();
            }
            if(Monbloc.checkLine()) std::cout << "Ligne Complete";

            window.clear();
            Monbloc.DessinerLeTableau();  
            Monbloc.VoirLeTableau();
            window.display();
            sf::sleep(sf::milliseconds(100));        
        }
        Monbloc.VoirLeTableau();
        std::cout << "\n\r Voici ton score : " << Monbloc.Score() << "\n";
        break;
    }    
    return 0;
}



