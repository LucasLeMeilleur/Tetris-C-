#include <SFML/Graphics.hpp>
#include <string>

class bloc{

    private:
        sf::RenderWindow* AddrWindow;
        int Patterne[28];
        int score =0;
        int map[9][24];
        sf::Sprite Tiles;
        sf::Vector2f position;
        int PatterneApp[28];
        int PosX =0, PosY= 0;
        int CouleurAlea, LigneComplete, NbBloc;
        int TabX, TabY, Rotation;
        struct LesPos{
            int X1; int X2; int X3; int X4;
            int Y1,Y2,Y3,Y4;
        };
        LesPos PosTot;
        bool VPerdu= false;
        int Niveau=0, LigneDetruite=0, Vitesse=0;
        int rotation=0;
        


    public:
        bloc(const sf::Texture& Textruc ,sf::RenderWindow &window, float initialX, float initialY);
        ~bloc();
        void assembly();
        void mouvement(std::string NomMouv);
        bool checkLine();
        void SuppLine();
        inline int Score(){ return score;};
        void ScoreAdd(std::string TypePts, int Nbr);
        void DrawTiles();
        void drawASprite(sf::Sprite &Tile);
        void DessinerLeTableau();
        inline sf::Vector2f RevoyerPosition() {return Tiles.getPosition(); };
        void Ajouter(int X, int Y,int Nbr);
        void DeplacementGauche();
        void DeplacementDroite();
        void DeplacementBas();
        void InitialiserPOS();
        void RegenererBloc();
        void CouleurAleatoire();
        void BlocAleatoire();
        bool DetectionBlocEnBas();
        void ResetBloc();
        void VoirLeTableau();
        void ViderTableau();
        void DefinitionDeStruct(int Plage, int Pattern, int i, int j);
        bool Perdu();
        inline int VitesseBloc(){ return ((40*100)-(2*Niveau*100))/6;};
        void ChangementNiveau();
        inline void Effacer(){ AddrWindow->clear();};
        inline void Dessiner(){ AddrWindow->display();};
        bool DetectionBlocEmpile();
        void RotationBloc();
        bool CheckLineLineRotateH(int (&Tab)[4][4]);
        bool CheckLineLineRotateV(int (&Tab)[4][4]);
        void SuppLineRotateH(int Tab[4][4]);
        void SuppLineRotateV(int Tab[4][4]);


};
