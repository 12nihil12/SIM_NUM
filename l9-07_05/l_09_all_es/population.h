#ifndef ___Population___
#define ___Population___

#include <armadillo>
#include <vector>

#include "path.h"

class Random; 
class Globe;


class Population{

    private: 


        int _pop_size; //numero individui della popolazione
        int _N_allele; //numero di alleli di ciascun individuo


        Random *_rnd; //riferimento ad un generatore random
        Globe *  _globe; //riferimento ad una mappa su cui lavorare
        
        
        std::vector <Path> _individuals; //vettore di percorsi 

        Path _best_individual; //tiene in memoria l'individuo migliore tra tutte le generazioni


        //parametri

        double _sel_p; //esponente per selezione 
        double _Px; //probabilità crossover 
        double _Pm; //probabilità mutazione


    public: 


        //costruttore
        Population(int pop_size, Globe *globe, Random *rnd);
        
        

        //funzioni di stampa e di accesso ai data membri

        double Get_size()const{return _pop_size;}
        Path Get_individual(int i)const{return _individuals.at(i);}; 

        void Print()const{for (auto it = _individuals.begin(); it != _individuals.end(); ++it) {it->Printwlen();}};
        void Print(const  char * filename) const;


        //Inizializza e setta i parametri
        void Initialize(double Pm,double Px,double sel_p); 


        //Ordina la popolazione per lunghezza crescente del percorso
        void Order();
    

        //Controlla se un individuo rispetta i criteri per appartenere alla popolazione
        void Check(Path & individual); 


        //Produce la nuova generazione di popolazione, con mutazioni e crossover
        void Spawn(); 
        void Mutate(Path & individual); 
        void Crossover(Path & parent1, Path & parent2);


     
};






#endif  


