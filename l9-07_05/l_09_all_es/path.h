#ifndef ___Path___
#define ___Path___

#include <armadillo>
#include <iomanip>


class Random; 
class Globe;


class Path{

    private: 


        Random *_rnd; //riferimento ad un generatore di numeri random
        Globe *  _globe; //riferimento ad una mappa su cui lavorare
        
        int _N_allele; //numero alleli 
        
        
        double _len; //lunghezza del percorso

        arma::irowvec _chromosome;//cromosoma: contiene il percorso, ossia l'ordine in cui visitare le città nella mappa

       
    
    public: 

        bool operator<(const Path& path) const{return _len < path.Get_len();} //overloading operatore < per sorting di path



        //costruttori
        Path(); 
        Path(Globe * _globe,Random * _rnd); 


        //Funzioni di stampa e accesso a data membri

        void Print() const {_chromosome.print();}//stampa 
        void Print(std::ostream & fout)const {_chromosome.print(fout);}//stampa su file
        void Printwlen () const ;//stampa il percorso con la sua lunghezza
        void Printwlen(std::ostream & fout) const;//stampa il percorso con la sua lunghezza su file
        double Get_len() const {return _len;}; //restituisce la lunghezza

        
        //*******************DANGER ZONE*****************
        void Set_len(double len) {_len=len;}; //fissa la lunghezza
        arma::irowvec * Get_chromosome() {return &_chromosome;} //ritorna un riferimento al cromosoma 
        /************************************************/


        void Shuffle(); //"Mischia" l'intero vettore (per inizializzazione), e ne ricalcola la lunghezza


        //Mutazioni: tutte le mutazioni avvengono escludendo a priori il primo e l'ultimo elemento (rappresentanti la città di partenza e arrivo, che è fissata)

        void Swap(); //seleziona due alleli e li scambia
        void Shift(); // seleziona un passo con probabilità uniforme e shifta il vettore di esso
        void Reverse(); //seleziona con probabilità uniforme gli estremi di una sequenza e la inverte

        //Dopo ogni mutazione la lunghezza del percorso viene ricalcolata 
        
};






#endif  


