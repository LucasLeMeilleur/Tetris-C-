#include "bloc.h"
#ifndef SFML_STATIC
#define SFML_STATIC
#endif

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <ctime>

#include <iostream>


bloc::bloc(const sf::Texture& TextTruc, sf::RenderWindow* window, int initialY, int initialX): AddrWindow(window), Tiles(TextTruc){
    std::srand(std::time(0));
    InitialiserPOS();
    CouleurAleatoire();
    BlocAleatoire();
    Tiles.setTextureRect(sf::IntRect(0,0,18,18));
    Tiles.setPosition(initialX, initialY);
    TabY = initialY;
    TabX = initialX;
    VPerdu = false;

    int PatterneA[28] = { 
                 0,2,4,6,   //I
                 0,2,4,5,    // L
                 1,3,4,5,   // L reversed      
                 0,2,3,4,   // T 
                 0,2,3,5,    // Z
                 1,2,3,4,     // Z reversed             
                 0,1,2,3     // carré
    };

    for(int i = 0; i < 28; i++) PatterneApp[i] = PatterneA[i];
    for(int i = 0; i <28;  i++) Patterne[i] = PatterneApp[i];

    //Mise en place de la map colonne ligne  (à inverser LOL)
    for(int i = 0; i <20;  i++) for(int j = 0; j<10; j++) map[i][j]= 0;
}

bloc::~bloc(){

}

void bloc::DefinitionDeStruct(int Plage, int Pattern, int i, int j){
    switch (Plage){
    case 0:
        PosTot.X1 = i;
        PosTot.Y1 = j;
        break;
    case 1:
        PosTot.X2 = i;
        PosTot.Y2 = j;
        break;
    case 2:

        PosTot.X3 = i;
        PosTot.Y3 = j;
        break;
    case 3:

        PosTot.X4 = i;
        PosTot.Y4 = j;
        break;
    default:
        return;
        break;
    }
}



void bloc::assembly(){
    const int KpatternInit = NbBloc*4;
    int Kpattern = KpatternInit, tab1D[8];
    for(int i=0;i<8;i++){
        if(Patterne[Kpattern]==i){
            tab1D[i]= CouleurAlea; 
            Kpattern++;
        }else tab1D[i]=0; 
    }
    int k=0, w=0;
    for(int i=0;i<4;i++)for(int j=3;j<5;j++){
        if(tab1D[k] >= 1){
            DefinitionDeStruct(w,KpatternInit,j,i);
            w++;
        }
        map[i][j]=tab1D[k++];
    }
}

void bloc::next(){   
    const int KpatternInit = NbBlocSuivant*4;
    int Kpattern = KpatternInit, k=0;
    int tab1D[8],tab2D[4][2];
    for(int i=0;i<8;i++){
        if(Patterne[Kpattern]==i){
            tab1D[i]= CouleurAleaSuivant; 
            Kpattern++;
        }else tab1D[i]=0; 
    }     

    for(int i=0;i<4;i++)for(int j=0;j<2;j++) tab2D[i][j]=tab1D[k++];

    int X, Y;
    if(NbBlocSuivant == 0){
        X = ((813-656)/2)+655-9;
        Y = ((190-75)/2)+75 - 36;
    }else{
        X = ((813-656)/2)+655-18;
        if(NbBlocSuivant == 6){
            Y = ((190-75)/2)+75 - 16;
        }else{
            Y = ((190-75)/2)+75 - 27;
        }
    }

    
    for(int i=0;i<4;i++)for(int j=0;j<2;j++){        
        if(tab2D[i][j] >= 1){
            Tiles.setTextureRect(sf::IntRect(18*CouleurAleaSuivant,0,18,18));
            Tiles.setPosition(sf::Vector2f((X+(j*18)),(Y+(i*18))));
            AddrWindow->draw(Tiles);
        }
    }

}


bool bloc::checkLine(){

    for(int i = 0; i<10; i++){
        int compteur = 0;
        for(int j = 0; j<20; j++){
            if(map[i][j] > 0) compteur++;
            if(map[i][j] == 0) break;
            if(compteur == 10) {
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
    for(int i = 0; i<20; i++){ 
        for(int j = 0; j<10; j++){
            if (map[i][j] >= 1) {
                int Couleur = map[i][j];
                Tiles.setTextureRect(sf::IntRect(18*Couleur,0,18,18));
                Tiles.setPosition(sf::Vector2f(TabY+18*j,TabX+18*i));
                DrawTiles();
            }
        }
    }
    
}

void bloc::Ajouter(int X, int Y, int  Nbr){
    map[Y][X] = Nbr;
}

void bloc::DeplacementBas(){
    if(
        ((PosTot.Y1 >= 0 && PosTot.Y1 < 19) && (PosTot.Y2 >= 0 && PosTot.Y2 < 19) && (PosTot.Y3 >= 0 && PosTot.Y3 < 19) && (PosTot.Y4 >= 0 && PosTot.Y4 < 19))
        &&
        (      ((map[PosTot.Y1+1][PosTot.X1] == 0) || PosTot.Y1+1 == PosTot.Y2 || PosTot.Y1+1 == PosTot.Y3 || PosTot.Y1+1 == PosTot.Y4)
            && ((map[PosTot.Y2+1][PosTot.X2] == 0) || PosTot.Y2+1 == PosTot.Y3 || (PosTot.Y2+1 == PosTot.Y4 && PosTot.X2 == PosTot.X4)) 
            && ((map[PosTot.Y3+1][PosTot.X3] == 0) || (PosTot.Y3+1 == PosTot.Y4 && PosTot.X3-1 == PosTot.X4) || (PosTot.Y3+1 == PosTot.Y4 && PosTot.X3 == PosTot.X4))
            && ((map[PosTot.Y4+1][PosTot.X4] == 0) || (PosTot.Y4+1 == PosTot.Y1 && PosTot.X1 == PosTot.X4))      
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
        VoirLeTableau();
    }
}

void bloc::DeplacementDroite(){
    if(
        (PosTot.X1 < 9 && PosTot.X2 < 9 && PosTot.X3 < 9 && PosTot.X4 <9)
        &&
        (      ((map[PosTot.Y1][PosTot.X1+1] == 0) || PosTot.X1+1 == PosTot.X2)
            && ((map[PosTot.Y2][PosTot.X2+1] == 0) || PosTot.X2+1 == PosTot.X3) 
            && ((map[PosTot.Y3][PosTot.X3+1] == 0) || PosTot.X3+1 == PosTot.X4)
            && ((map[PosTot.Y4][PosTot.X4+1] == 0) )
        )   
    )
    {
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
        (      ((map[PosTot.Y1][PosTot.X1-1] == 0))
            && ((map[PosTot.Y2][PosTot.X2-1] == 0) || PosTot.X2-1 == PosTot.X1) 
            && ((map[PosTot.Y3][PosTot.X3-1] == 0) || PosTot.X3-1 == PosTot.X2)
            && ((map[PosTot.Y4][PosTot.X4-1] == 0) || PosTot.X4-1 == PosTot.X3)
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
    NbBloc = NbBlocSuivant;
    CouleurAlea = CouleurAleaSuivant;
    CouleurAleatoire();
    BlocAleatoire();
    assembly();
}

void bloc::CouleurAleatoire(){
    CouleurAleaSuivant = rand() %6 + 1;
}

void bloc::BlocAleatoire(){
    NbBlocSuivant = rand() % 7;
}

bool bloc::DetectionBlocEnBas(){
    if(PosTot.Y4 == 19 || PosTot.Y1 == 19 || PosTot.Y2 == 19 || PosTot.Y3 == 19) return true;
    
    return false;
}

bool bloc::DetectionBlocEmpile(){
    if(map[PosTot.Y1+1][PosTot.X1] >= 1 && !( ((PosTot.Y2 == PosTot.Y1+1) && (PosTot.X2 == PosTot.X1)) || ((PosTot.Y1+1 == PosTot.Y3) && (PosTot.X1 == PosTot.X3)) || ((PosTot.Y1+1 == PosTot.Y4) && (PosTot.X1 == PosTot.X4)) ) ) return true;
    if(map[PosTot.Y2+1][PosTot.X2] >= 1 && !( ((PosTot.Y2+1 == PosTot.Y1) && (PosTot.X2 == PosTot.X1)) || ((PosTot.Y2+1 == PosTot.Y3) && (PosTot.X2 == PosTot.X3)) || ((PosTot.Y2+1 == PosTot.Y4) && (PosTot.X2 == PosTot.X4)) ) ) return true;
    if(map[PosTot.Y3+1][PosTot.X3] >= 1 && !( ((PosTot.Y3+1 == PosTot.Y1) && (PosTot.X3 == PosTot.X1)) || ((PosTot.Y3+1 == PosTot.Y2) && (PosTot.X3 == PosTot.X2)) || ((PosTot.Y3+1 == PosTot.Y4) && (PosTot.X3 == PosTot.X4)) ) ) return true;
    if(map[PosTot.Y4+1][PosTot.X4] >= 1 && !( ((PosTot.Y4+1 == PosTot.Y1) && (PosTot.X4 == PosTot.X1)) || ((PosTot.Y4+1 == PosTot.Y3) && (PosTot.X4 == PosTot.X3)) || ((PosTot.Y4+1 == PosTot.Y2) && (PosTot.X4 == PosTot.X2)) ) ) return true;
    return false;
}

void bloc::ResetBloc(){
    DejaSave = false;
    int LigneTmp =0;  
        while(checkLine()){
            SuppLine();
            LigneTmp++;           
        }
    if(LigneTmp >= 1) ScoreAdd("Ligne", LigneTmp);
    
    const int* Y[] = {&PosTot.Y1, &PosTot.Y2, &PosTot.Y3, &PosTot.Y4};
    
    for(int i = 0; i < 4 ; i++){
        if(*Y[i] <= 3 ){
            VPerdu = true;
            return;
        }
    }

    rotation =0;
    InitialiserPOS();
    RegenererBloc();
    
}

void bloc::VoirLeTableau(){
    std::cout <<"\n\n";
    for(int i =0; i <20 ; i++){
        for(int j=0 ; j<10; j++){
            std::cout << map[i][j];
        }       
        std::cout << "\n";
    }
}

void bloc::ViderTableau(){
    for(int i=0; i<20; i++){
        for(int j=0; j<10; j++) map[i][j] = 0;
    }
}

bool bloc::Perdu(){
    return VPerdu;
}

void bloc::SuppLine(){

    LigneDetruite++;
    LigneDetruiteTot++;
    int LigneComplete = 0;
    int mapAnnexe[20][10];

    for (int i = 0; i < 20; i++){
        for (int j = 0; j < 10; j++) mapAnnexe[i][j] = map[i][j];  
    }

    for(int i = 0; i<20; i++){
        int compteur = 0;
        for(int j = 0; j<10; j++){
            if(map[i][j] > 0) compteur++;
            if(map[i][j] == 0) break;
            if(compteur == 10){
                LigneComplete = i;
            }
        }
    } 


    for(int i = LigneComplete; i>0; i--){
        for (int j= 0; j < 10; j++){
            map[i][j] = mapAnnexe[i-1][j];  
        }       
    }
}

void bloc::ScoreAdd(std::string TypePts, int Nbr){
    if(TypePts == "Ligne"){
        int PtsBase = Nbr *100;
        
        int ScoreTmp = PtsBase + (100*(Nbr-1));
        score += ScoreTmp;
    }

    else if(TypePts == "DescenteRapide"){
        
        score += (Niveau+1)*1;
    }

    else if(TypePts == "DescenteNow"){
        score += (Niveau+1)*2;
    }
}

void bloc::ChangementNiveau(){
    if(Niveau == 29) return;
    if((Niveau+1)*4 <= LigneDetruite){
        Niveau++;
        LigneDetruite = 0;
    }
}

void bloc::RotationBloc(){

    

    if(NbBloc == 6) return;
    if((NbBloc == 0) && (PosTot.X1 > 6)) return;
    if((NbBloc == 0) && (rotation == 1) && (PosTot.Y4 >= 16)) return;



    if((NbBloc !=1 ) && (PosTot.X1 >= 9))return;
    if((NbBloc !=1 ) && (rotation == 1) && ((PosTot.Y4 >= 17) || (PosTot.X1 >= 8)))return;
    



    int tab16Cases[4][4], tab16CasesR[4][4];
    const int* X[] = {&PosTot.X1, &PosTot.X2, &PosTot.X3, &PosTot.X4};
    const int* Y[] = {&PosTot.Y1, &PosTot.Y2, &PosTot.Y3, &PosTot.Y4};
    int XPlusPtit= *X[0], YPlusPtit= *Y[0];

    for (int i = 0; i < 4; i++){
        if(*X[i]<XPlusPtit) XPlusPtit = *X[i];
    }
    for (int i = 0; i < 4; i++){
        if(*Y[i]<YPlusPtit) YPlusPtit = *Y[i];
    }


    //Si bloc dans la rangée alors on ne fait pas

    switch (NbBloc){
        case 0:
            switch (rotation){
            case 0:
                for (int i = YPlusPtit; i < YPlusPtit+2; i++)for (int j = XPlusPtit+2; j < XPlusPtit+4; j++)
                        if(map[i][j] >0 ) return;
                break;
            case 1:
                for (int i = YPlusPtit+2; i < YPlusPtit+4; i++)for (int j = XPlusPtit; j < XPlusPtit+2; j++)
                        if(map[i][j] >0 ) return;
                break;
            }
            break;
        default:
            switch (rotation){
                case 0:
                    for (int i = YPlusPtit; i < YPlusPtit+2; i++)for (int j = XPlusPtit+2; j < XPlusPtit+3; j++)
                            if(map[i][j] >0 ) return;
                    break;
                case 1:
                    for (int i = YPlusPtit+2; i < YPlusPtit+3; i++)for(int j = XPlusPtit; j < XPlusPtit+2; j++)
                        if(map[i][j] >0 ) return;
                    break;
            }
            break;
    }



    //Initialiser le tableau Pour rotate
    for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++) tab16Cases[i][j] = 0;

    //On initialise tab16case avec map

    switch (NbBloc){
        case 0:
            switch (rotation){
                case 0:
                    for(int i = 0; i < 4; i++)for(int j = 0; j < 2; j++)tab16Cases[j][i] = map[YPlusPtit+i][XPlusPtit+j];
                    break;
                case 1:
                    for(int i = 0; i < 2; i++)for(int j = 0; j < 4; j++)tab16Cases[j][i] = map[YPlusPtit+i][XPlusPtit+j];
                    break;
            }
            break;
        default:
            switch (rotation){
                case 0:
                    for(int i = 0; i < 3; i++)for(int j = 0; j < 2; j++)tab16Cases[j][i] = map[YPlusPtit+i][XPlusPtit+j];
                    break;
                case 1:
                    for(int i = 0; i < 2; i++) for(int j = 0; j < 3; j++)tab16Cases[j][i] = map[YPlusPtit+i][XPlusPtit+j];
                    break;
            }
            break;
    }




    //Rotation

    for(int i=0;i<4;i++)for(int j=0;j<4;j++) tab16CasesR[i][3-j]=tab16Cases[i][j];
    for(int i=0;i<4;i++)for(int j=0;j<4;j++) tab16Cases[i][j]=tab16CasesR[i][j];
    

std::cout << "TableCaseR";
for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){ std::cout << tab16CasesR[i][j];} 
    std::cout <<  std::endl;
}
std::cout << std::endl;

    while(CheckLineLineRotateH(tab16Cases)){
        SuppLineRotateH(tab16Cases);
    }
    while(CheckLineLineRotateV(tab16Cases)){
        SuppLineRotateV(tab16Cases);
    }

std::cout << "TableCase";
for(int i=0;i<4;i++)for(int j=0;j<4;j++) std::cout << tab16Cases[i][j] << std::endl;

std::cout << "Message de debug1" << std::endl;
    // Efface le contenu de map 

    switch(NbBloc){
        case 0:
            for (int i = YPlusPtit; i < YPlusPtit+4; i++)for(int j = XPlusPtit; j < XPlusPtit+2; j++)map[i][j] = 0;       
            for (int i = YPlusPtit; i < YPlusPtit+2; i++)for(int j = XPlusPtit; j < XPlusPtit+4; j++)map[i][j] = 0;
            break;
        default:
            for (int i = YPlusPtit; i < YPlusPtit+3; i++)for(int j = XPlusPtit; j < XPlusPtit+2; j++)map[i][j] = 0;
            for (int i = YPlusPtit; i < YPlusPtit+2; i++)for(int j = XPlusPtit; j < XPlusPtit+3; j++)map[i][j] = 0;
            break;
    }


std::cout << "Message de debug2" << std::endl;
    // Tab dans map

    
    switch (NbBloc){
    case 0:
        for (int i = YPlusPtit; i < YPlusPtit+4; i++)for(int j = XPlusPtit; j < XPlusPtit+2; j++) map[i][j] = tab16Cases[i-YPlusPtit][j-XPlusPtit];
        for (int i = YPlusPtit; i < YPlusPtit+2; i++)for(int j = XPlusPtit; j < XPlusPtit+4; j++) map[i][j] = tab16Cases[i-YPlusPtit][j-XPlusPtit];
        break;
    
    default:
        for (int i = YPlusPtit; i < YPlusPtit+3; i++)for(int j = XPlusPtit; j < XPlusPtit+2; j++) map[i][j] = tab16Cases[i-YPlusPtit][j-XPlusPtit];
        for (int i = YPlusPtit; i < YPlusPtit+2; i++)for(int j = XPlusPtit; j < XPlusPtit+3; j++) map[i][j] = tab16Cases[i-YPlusPtit][j-XPlusPtit];
        break;
    }
    int k = 0;

    VoirLeTableau();

    //Initiation des poses en fonction du bloc (a optimiser si pas flemme) switch case:

    std::cout << PosTot.X1 << " " << PosTot.X2 << " " << PosTot.X3 << " " << PosTot.X4 << std::endl; 
    std::cout << PosTot.Y1 << " " << PosTot.Y2 << " " << PosTot.Y3 << " " << PosTot.Y4 << std::endl; 

    std::cout << YPlusPtit << std::endl;
    std::cout << XPlusPtit << std::endl;

    switch (NbBloc){
        case 0:
            switch (rotation){
                case 0:
                    for (int i = YPlusPtit; i < YPlusPtit+2; i++){for(int j = XPlusPtit; j < XPlusPtit+4; j++){
                        if(map[i][j] > 0){
                            switch (k){
                                case 0:
                                    PosTot.X1 = j; PosTot.Y1 = i; k++;
                                    break;                                
                                case 1:
                                    PosTot.X2 = j; PosTot.Y2 = i; k++; 
                                    break;
                                case 2:
                                    PosTot.X3 = j; PosTot.Y3 = i; k++;  
                                    break;
                                case 3:
                                    PosTot.X4 = j; PosTot.Y4 = i; k++; 
                                    break;  
                                case 4: break; case 5: break; case 6: break; case 7: break;
                            } 
                        } 
                    }if(k >=4) break;}
                    break;
                
                case 1:
                    for (int i = YPlusPtit; i < YPlusPtit+4; i++){for(int j = XPlusPtit; j < XPlusPtit+2; j++){
                        if(map[i][j] > 0){
                            switch (k){
                                case 0:
                                    PosTot.X1 = j; PosTot.Y1 = i; k++;
                                    break;                                
                                case 1:
                                    PosTot.X2 = j; PosTot.Y2 = i; k++; 
                                    break;
                                case 2:
                                    PosTot.X3 = j; PosTot.Y3 = i; k++;  
                                    break;
                                case 3:
                                    PosTot.X4 = j; PosTot.Y4 = i; k++; 
                                    break; 
                                case 4: break; case 5: break; case 6: break; case 7: break;
                            } 
                        } 
                    }if(k >=4) break;}
                    break;
            }
            break;
        
        default:
            switch (rotation){
                case 0:
                    for (int i = YPlusPtit; i < YPlusPtit+2; i++){for(int j = XPlusPtit; j < XPlusPtit+3; j++){
                        if(map[i][j] > 0){
                            switch (k){
                                case 0:
                                    PosTot.X1 = j; PosTot.Y1 = i; k++;
                                    break;                                
                                case 1:
                                    PosTot.X2 = j; PosTot.Y2 = i; k++; 
                                    break;
                                case 2:
                                    PosTot.X3 = j; PosTot.Y3 = i; k++;  
                                    break;
                                case 3:
                                    PosTot.X4 = j; PosTot.Y4 = i; k++; 
                                    break;  
                                case 4: break; case 5: break; case 6: break; case 7: break;
                            } 
                        } 
                    }if(k >=4) break;}
                    break;
                
                case 1:
                    for (int i = YPlusPtit; i < YPlusPtit+3; i++){for(int j = XPlusPtit; j < XPlusPtit+2; j++){
                        if(map[i][j] > 0){
                            switch (k){
                                case 0:
                                    PosTot.X1 = j; PosTot.Y1 = i; k++;
                                    break;                                
                                case 1:
                                    PosTot.X2 = j; PosTot.Y2 = i; k++; 
                                    break;
                                case 2:
                                    PosTot.X3 = j; PosTot.Y3 = i; k++;  
                                    break;
                                case 3:
                                    PosTot.X4 = j; PosTot.Y4 = i; k++; 
                                    break;   
                                case 4: break; case 5: break; case 6: break; case 7: break;
                            } 
                        } 
                    }if(k >=4) break;}
                    break;
                }
            break;
    }


    std::cout << PosTot.X1 << " " << PosTot.X2 << " " << PosTot.X3 << " " << PosTot.X4 << std::endl; 

    std::cout << PosTot.Y1 << " " << PosTot.Y2 << " " << PosTot.Y3 << " " << PosTot.Y4 << std::endl; 
    std::cout << rotation << std::endl;
    rotation++;
    rotation %= 2;
}

bool bloc::CheckLineLineRotateH(int (&Tab)[4][4]){


    int Count = 0; 
    bool Finito = false;

for (int i = 0; i < 4; i++){
    for (int j = 0; j < 4; j++){
        std::cout <<    Tab[i][j];

}std::cout << "\n";}


    for(int i=0;i<4;i++){
        Count = 0;
        for(int j=0;j<4;j++){ 
            if(i == 0 && (Tab[i][j] >= 1)){
                Finito = true;
                break;
            }
            if(Tab[i][j] == 0) Count++;
            else break;
        }         
        if(Count == 4) return true;
        if (Finito) break;
    }
    
    return false;
}

bool bloc::CheckLineLineRotateV(int (&Tab)[4][4]){

    int Count = 0;
    bool Finito = false;

    for(int j=0;j<4;j++){
        Count = 0;
        for(int i=0;i<4;i++){ 
            if(j == 0 && (Tab[i][j] >= 1)){
                Finito = true;
                break;
            }
            else if(Tab[i][j] == 0) Count++;
            else break;
        }
        if (Finito) break;
        if(Count == 4){
            std::cout << "V" << std::endl;
            return true;
        }
    }

    std::cout <<"Count: " << Count << std::endl;
    return false;
}

void bloc::SuppLineRotateH(int Tab[4][4]){

    int Tab2[4][4];
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){ 
            Tab2[i][j] = Tab[i][j];
        }
    }



    for(int i=0;i<4;i++)for(int j=1;j<4;j++) Tab[i][j] = 0; 
    for(int i=0;i<4;i++)for(int j=1;j<4;j++) Tab[i-1][j] = Tab2[i][j]; 


for (int i = 0; i < 4; i++){
    for (int j = 0; j < 4; j++){
        std::cout <<    Tab[i][j];

}
std::cout << "\n";
    }
}

void bloc::SuppLineRotateV(int Tab[4][4]) {
    int Tab2[4][4];    
    for(int i=0;i<4;i++) for(int j=0;j<4;j++) Tab2[i][j] = Tab[i][j];
    for(int i=0;i<4;i++) for(int j=1;j<4;j++) Tab[i][j] = 0; 
    for(int i=0;i<4;i++) for(int j=0;j<4;j++) Tab[i][j-1] = Tab2[i][j]; 


    
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                std::cout <<    Tab[i][j];

        }
        std::cout << "\n";
            }
}


void bloc::ChangerBloc(){

    if(DejaSave) return;

    DejaSave = true;

    rotation = 0;
    if(PosTot.Y1 >= 5) return;

    switch (BlocSaved){
        case 8:
            BlocSaved = NbBloc;
            NbBloc = NbBlocSuivant;

            CouleurSaved = CouleurAlea;
            CouleurAlea = CouleurAleaSuivant;

            RemplacerBlocSave(); 
            break;
        
        default:
            int BlocTmp = NbBloc;
            NbBloc = BlocSaved;
            BlocSaved = BlocTmp;

            int ColorTmp = CouleurAlea;
            CouleurAlea = CouleurSaved;
            CouleurSaved = ColorTmp;

            RemplacerBlocSave();
            break;
    }
}


void bloc::RemplacerBlocSave(){
    
    map[PosTot.X1][PosTot.Y1] = 0;
    map[PosTot.X2][PosTot.Y2] = 0;
    map[PosTot.X3][PosTot.Y3] = 0;
    map[PosTot.X4][PosTot.Y4] = 0;
    assembly();
}

void bloc::Saved(){
    if(BlocSaved == 8) return;
    const int KpatternInit = BlocSaved*4;
    int Kpattern = KpatternInit, k=0;
    int tab1D[8],tab2D[4][2];
    for(int i=0;i<8;i++){
        if(Patterne[Kpattern]==i){
            tab1D[i]= CouleurSaved; 
            Kpattern++;
        }else tab1D[i]=0; 
    }     
    for(int i=0;i<4;i++)for(int j=0;j<2;j++) tab2D[i][j]=tab1D[k++];

    int X, Y;
    if(BlocSaved == 0){
        X = ((237-80)/2)+80;
        Y = ((252-138)/2)+138 - 36;
    }else{
        X = ((237-80)/2)+80-18;
        if(BlocSaved == 6){
            Y = ((252-138)/2)+138 - 16;
        }else{
            Y = ((252-138)/2)+138 - 27;
        }
    }


    for(int i=0;i<4;i++)for(int j=0;j<2;j++){        
        if(tab2D[i][j] >= 1){
            Tiles.setTextureRect(sf::IntRect(18*CouleurSaved,0,18,18));
            Tiles.setPosition(sf::Vector2f((X+(j*18)),(Y+(i*18))));
            AddrWindow->draw(Tiles);
        }
    }
}

void bloc::AtterirEnBas(){
    
    for (int i = 0; i < 20; i++){
        if(!DetectionBlocEmpile()){
            DeplacementBas();    
            if (Niveau == 0) score += 2;
            else score += Niveau*2;
        }
    }
    ResetBloc();
    
}

void bloc::VisualiserBloc(){

    sf::Texture texture;
    if (!texture.loadFromFile("asset/tiles.png")) {
        EXIT_FAILURE;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(18*CouleurAlea,0,18,18));
    sprite.setColor(sf::Color(255, 255, 255, 128)); // Opacité à 50%



    const int* X[] = {&PosTot.X1, &PosTot.X2, &PosTot.X3, &PosTot.X4};
    const int* Y[] = {&PosTot.Y1, &PosTot.Y2, &PosTot.Y3, &PosTot.Y4};
    int XPlusGrand= *X[0], XPlusPetit= *X[0], YPlusGrand= *Y[0];

    for (int i = 0; i < 4; i++){
        if(*X[i]>XPlusGrand) XPlusGrand = *X[i];
    }
    for (int i = 0; i < 4; i++){
        if(*X[i]<XPlusPetit) XPlusPetit = *X[i];
    }
    
    for (int i = 0; i < 4; i++){
        if(*Y[i]>YPlusGrand){
            YPlusGrand = *Y[i];
        }
    }

    for(int i=YPlusGrand; i<20; i++){


        int Y = i-YPlusGrand;
        if(
            (map[PosTot.Y1 + Y][PosTot.X1] == 0) &&
            (map[PosTot.Y2 + Y][PosTot.X2] == 0) &&
            (map[PosTot.Y3 + Y][PosTot.X3] == 0) &&
            (map[PosTot.Y4 + Y][PosTot.X4] == 0)
        && 
            ((
                (map[PosTot.Y1 + Y+1][PosTot.X1] >=1)||
                (map[PosTot.Y2 + Y+1][PosTot.X2] >=1)||
                (map[PosTot.Y3 + Y+1][PosTot.X3] >=1)||
                (map[PosTot.Y4 + Y+1][PosTot.X4] >=1)
            
            )
            || 
                (i==19)
            )

        ){
            sprite.setPosition(360+18*PosTot.X1,136+18*(PosTot.Y1+Y));
            AddrWindow->draw(sprite);
            sprite.setPosition(360+18*PosTot.X2,136+18*(PosTot.Y2+Y));
            AddrWindow->draw(sprite);
            sprite.setPosition(360+18*PosTot.X3,136+18*(PosTot.Y3+Y));
            AddrWindow->draw(sprite);
            sprite.setPosition(360+18*PosTot.X4,136+18*(PosTot.Y4+Y));
            AddrWindow->draw(sprite);   
            break;
        }
        
    }


    
    
}


int bloc::GetY(){
    return PosTot.Y1;
}

void bloc::Recommencer(){
    VPerdu = false;
    score = 0;
    LigneDetruiteTot = 0;
    LigneDetruite = 0;
    LigneComplete = 0;
    Niveau = 0;
    ViderTableau();

    BlocSaved = 0;
    CouleurSaved = 0;
    NbBloc = 0; NbBlocSuivant=0; CouleurAleaSuivant=0; CouleurAlea =0;

    ResetBloc();
}