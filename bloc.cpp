#include "bloc.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <ctime>

#include <iostream>


bloc::bloc(const sf::Texture& TextTruc, sf::RenderWindow &window, float initialX, float initialY): Tiles(TextTruc){
    std::srand(std::time(0));
    InitialiserPOS();
    CouleurAleatoire();
    BlocAleatoire();
    AddrWindow = &window;    
    Tiles.setTextureRect(sf::IntRect(0,0,18,18));
    Tiles.setPosition(initialX, initialY);
    TabX = initialX;
    TabY = initialY;

    int PatterneA[28] = { 
                 0,2,4,6,   //I
                 0,2,4,5,    // L
                 1,3,4,5,   // L reversed      
                 0,2,3,4,   // T 
                 0,2,3,5,    // Z
                 1,2,3,4,     // Z reversed             
                 0,1,2,3     // carré
                    };

    for (int i = 0; i < 28; i++){
        PatterneApp[i] = PatterneA[i];
    }

    for(int i = 0; i <28;  i++) Patterne[i] = PatterneApp[i];

    for(int i = 0; i <9;  i++){
        for(int j = 0; j<24; j++){    // Mise en place de la map
            map[i][j]= 0;
        }
    }
}

bloc::~bloc(){

}

void bloc::DefinitionDeStruct(int Plage, int Pattern, int i, int j){
     if(Plage-Pattern == 0){
        PosTot.X1 = i;
        PosTot.Y1 = j;
    }else if(Plage-Pattern == 1){
        PosTot.X2 = i;
        PosTot.Y2 = j;
    }else if(Plage-Pattern == 2){
        PosTot.X3 = i;
        PosTot.Y3 = j;
    }else if(Plage-Pattern == 3){
        PosTot.X4 = i;
        PosTot.Y4 = j;
    }
}

void bloc::assembly(){

    const int KpatternInit = NbBloc*4;
    int Kpattern = KpatternInit;

    for (int j = 0; j < 4; j++){            
        for (int i = 0; i < 2; i++){
            
            if((Kpattern != KpatternInit)&&(Kpattern%4 == 0)){
                VoirLeTableau();
                break;
            }
            
            if(PatterneApp[Kpattern]%2 == 0){
                map[i][j] = CouleurAlea;
                DefinitionDeStruct(Kpattern,KpatternInit,i,j);

                if(Patterne[Kpattern+1] != (Patterne[Kpattern]+1)){
                    Kpattern++;           
                    break;
                }     
                Kpattern++;    
                continue;     
            }
            if(PatterneApp[Kpattern]%2 == 1){
                
                if(i==0){
                    map[i+1][j] = CouleurAlea;
                    DefinitionDeStruct(Kpattern,KpatternInit,i+1,j);
                    Kpattern++;
                    break;
                }
                map[i][j] = CouleurAlea;
                DefinitionDeStruct(Kpattern,KpatternInit,i,j);
                Kpattern++;
            }
            VoirLeTableau();
        }
    }
}

bool bloc::checkLine(){
    for(int i = 0; i<24; i++){
        int compteur = 0;
        for(int j = 0; j<9; j++){
            if(map[j][i] > 0) compteur++;
            if(map[j][i] == 0) break;
            if(compteur == 9) {
                LigneComplete = i;
 
                return true;
            }
        }
    }   
    

    return false;
}

void bloc::mouvement(std::string NomMouv){    
    if(NomMouv == "left") DeplacementGauche();
    if(NomMouv == "right") DeplacementDroite();
    if(NomMouv == "down") DeplacementBas();
}


void bloc::DrawTiles(){
    AddrWindow->draw(Tiles);
}

void bloc::drawASprite(sf::Sprite &Tile){
    AddrWindow->draw(Tile);
}

void bloc::DessinerLeTableau(){
    for(int i = 0; i<9; i++){ 
        for(int j = 0; j<24; j++){
            if (map[i][j] >= 1) {

                int Couleur = map[i][j];

                Tiles.setTextureRect(sf::IntRect(18*Couleur,0,18,18));
                Tiles.setPosition(sf::Vector2f(TabX+18*i, TabY+18*j));
                DrawTiles();
            }
        }
    }
    
}

void bloc::Ajouter(int X, int Y, int  Nbr){
    map[X][Y] = Nbr;
}

void bloc::DeplacementBas(){

        


    if(
        ( (PosTot.Y1 >= 0 && PosTot.Y1 < 23) && (PosTot.Y2 >= 0 && PosTot.Y2 < 23) && (PosTot.Y3 >= 0 && PosTot.Y3 < 23) && (PosTot.Y4 >= 0 && PosTot.Y4 < 23))
        &&
        (      ((map[PosTot.X1][PosTot.Y1+1] == 0) || PosTot.Y1+1 == PosTot.Y2 || PosTot.Y1+1 == PosTot.Y3)
            && ((map[PosTot.X2][PosTot.Y2+1] == 0) || PosTot.Y2+1 == PosTot.Y3 || PosTot.Y2+1 == PosTot.Y4) 
            && ((map[PosTot.X3][PosTot.Y3+1] == 0) || (PosTot.Y3+1 == PosTot.Y4 && PosTot.X3-1 == PosTot.X4) || (PosTot.Y3+1 == PosTot.Y4 && PosTot.X3 == PosTot.X4))
            && ((map[PosTot.X4][PosTot.Y4+1] == 0))
        
        )
    
    ){
        Ajouter(PosTot.X1,PosTot.Y1, 0);
        Ajouter(PosTot.X2,PosTot.Y2, 0);
        Ajouter(PosTot.X3,PosTot.Y3, 0);
        Ajouter(PosTot.X4,PosTot.Y4, 0);

        Ajouter(PosTot.X1,PosTot.Y1+1, CouleurAlea);
        Ajouter(PosTot.X2,PosTot.Y2+1, CouleurAlea);
        Ajouter(PosTot.X3,PosTot.Y3+1, CouleurAlea);
        Ajouter(PosTot.X4,PosTot.Y4+1, CouleurAlea);

        PosTot.Y1++;PosTot.Y2++;PosTot.Y3++;PosTot.Y4++;
    }

}

void bloc::DeplacementDroite(){


    if(
        (PosTot.X1 < 8 && PosTot.X2 < 8 && PosTot.X3 < 8 && PosTot.X4 <8)
        &&
        (      ((map[PosTot.X1+1][PosTot.Y1] == 0) || PosTot.X1+1 == PosTot.X2)
            && ((map[PosTot.X2+1][PosTot.Y2] == 0) || PosTot.X2+1 == PosTot.X3) 
            && ((map[PosTot.X3+1][PosTot.Y3] == 0) || PosTot.X3+1 == PosTot.X4)
            && ((map[PosTot.X4+1][PosTot.Y4] == 0))
        
        )   

    ){
        Ajouter(PosTot.X1,PosTot.Y1, 0);
        Ajouter(PosTot.X2,PosTot.Y2, 0);
        Ajouter(PosTot.X3,PosTot.Y3, 0);
        Ajouter(PosTot.X4,PosTot.Y4, 0);

        Ajouter((PosTot.X1+1),PosTot.Y1, CouleurAlea);
        Ajouter((PosTot.X2+1),PosTot.Y2, CouleurAlea);
        Ajouter((PosTot.X3+1),PosTot.Y3, CouleurAlea);
        Ajouter((PosTot.X4+1),PosTot.Y4, CouleurAlea);

        PosTot.X1++; PosTot.X2++; PosTot.X3++; PosTot.X4++; 
    }

    

}

void bloc::DeplacementGauche(){


    if(        
        (PosTot.X1 >= 1 && PosTot.X2 >= 1 && PosTot.X3 >= 1 && PosTot.X4 >= 1)
        &&
        (      ((map[PosTot.X1-1][PosTot.Y1] == 0))
            && ((map[PosTot.X2-1][PosTot.Y2] == 0) || PosTot.X2-1 == PosTot.X1) 
            && ((map[PosTot.X3-1][PosTot.Y3] == 0) || PosTot.X3-1 == PosTot.X2)
            && ((map[PosTot.X4-1][PosTot.Y4] == 0) || PosTot.X4-1 == PosTot.X3)
        )   
    ){

        Ajouter(PosTot.X4,PosTot.Y4, 0);
        Ajouter(PosTot.X3,PosTot.Y3, 0);
        Ajouter(PosTot.X1,PosTot.Y1, 0);
        Ajouter(PosTot.X2,PosTot.Y2, 0);

        Ajouter((PosTot.X1-1),PosTot.Y1, CouleurAlea);
        Ajouter((PosTot.X2-1),PosTot.Y2, CouleurAlea);
        Ajouter((PosTot.X3-1),PosTot.Y3, CouleurAlea);
        Ajouter((PosTot.X4-1),PosTot.Y4, CouleurAlea);

       PosTot.X1--; PosTot.X2--; PosTot.X3--; PosTot.X4--;     
    }
}

void bloc::InitialiserPOS(){    
    if (PosTot.Y1 == 0) VPerdu = true;
    PosTot.X1=0; PosTot.X2=0; PosTot.X3=0; PosTot.X4=0;
    PosTot.Y1=0; PosTot.Y2=0; PosTot.Y3=0; PosTot.Y4=0;
}

void bloc::RegenererBloc(){
    CouleurAleatoire();
    BlocAleatoire();
    assembly();
}

void bloc::CouleurAleatoire(){
    CouleurAlea = rand() %6 + 1;
}

void bloc::BlocAleatoire(){
    NbBloc = rand() % 7;
}

bool bloc::DetectionBlocEnBas(){
    if(PosTot.Y4 == 23 || PosTot.Y1 == 23 || PosTot.Y2 == 23 || PosTot.Y3 == 23) return true;
    return false;
}

bool bloc::DetectionBlocEmpile(){
    if(map[PosTot.X1][PosTot.Y1+1] >= 1 && !( ((PosTot.X2 == PosTot.X1) && (PosTot.Y2 == PosTot.Y1+1)) || ((PosTot.X1 == PosTot.X3) && (PosTot.Y1+1 == PosTot.Y3)) || ((PosTot.X1 == PosTot.X4) && (PosTot.Y1+1 == PosTot.Y4)) ) ) return true;
    if(map[PosTot.X2][PosTot.Y2+1] >= 1 && !( ((PosTot.X2 == PosTot.X1) && (PosTot.Y2+1 == PosTot.Y1)) || ((PosTot.X2 == PosTot.X3) && (PosTot.Y2+1 == PosTot.Y3)) || ((PosTot.X2 == PosTot.X4) && (PosTot.Y2+1 == PosTot.Y4)) ) ) return true;
    if(map[PosTot.X3][PosTot.Y3+1] >= 1 && !( ((PosTot.X3 == PosTot.X1) && (PosTot.Y3+1 == PosTot.Y1)) || ((PosTot.X2 == PosTot.X3) && (PosTot.Y2 == PosTot.Y3+1)) || ((PosTot.X3 == PosTot.X4) && (PosTot.Y3+1 == PosTot.Y4)) ) ) return true;
    if(map[PosTot.X4][PosTot.Y4+1] >= 1 && !( ((PosTot.X4 == PosTot.X1) && (PosTot.Y4+1 == PosTot.Y1)) || ((PosTot.X4 == PosTot.X3) && (PosTot.Y4+1 == PosTot.Y3)) || ((PosTot.X2 == PosTot.X4) && (PosTot.Y2 == PosTot.Y4+1)) ) ) return true;
    return false;
}

void bloc::ResetBloc(){
    InitialiserPOS();
    RegenererBloc();
}

void bloc::VoirLeTableau(){
    std::cout <<"\n\n";
    for(int i =0; i <24 ; i++){
        for(int j=0 ; j<9; j++){
            std::cout << map[j][i];
        }       
        std::cout << "\n";
    }
}

void bloc::ViderTableau(){
    for(int i=0; i<24; i++){
        for(int j=0; i<9; j++) map[i][j] = 0;
    }
}

bool bloc::Perdu(){
    return VPerdu;
}

void bloc::SuppLine(){

    LigneDetruite++;
    int LigneComplete = 0;
    int mapAnnexe[9][24];

    for (int i = 0; i < 24; i++){
        for (int j = 0; j < 9; j++) mapAnnexe[j][i] = map[j][i];  
    }
    for(int i = 0; i<24; i++){
        int compteur = 0;
        for(int j = 0; j<9; j++){
            if(map[j][i] > 0) compteur++;
            if(map[j][i] == 0) break;
            if(compteur == 9){
                LigneComplete = i;
            }
        }
    } 
    for(int i = 0; i<9 ; i++){
        for (int j= 2; j < LigneComplete+1; j++){
            map[i][j] = mapAnnexe[i][j-1];    
        }       
    }
}

void bloc::ScoreAdd(std::string TypePts, int Nbr){
    if(TypePts == "Ligne"){

        int PtsBase = Nbr *100;
        int ScoreTmp = PtsBase + (100*(Nbr));
        score += ScoreTmp;
    }

    if(TypePts == "DescenteRapide"){
        score += Niveau*1;
    }

    if(TypePts == "DescenteNow"){
        score += Niveau*2;
    }
}

void bloc::ChangementNiveau(){
    if((Niveau+1)*10 == LigneDetruite) Niveau++;
}