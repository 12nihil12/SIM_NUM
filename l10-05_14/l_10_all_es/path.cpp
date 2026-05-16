#include "path.h"


#include "random.h"
#include "globe.h"


using namespace arma;
using namespace std;



Path::Path(){
    _N_allele=-1; //Path non inizializzato
}



Path::Path(Globe * globe,Random * rnd){

    _rnd=rnd; //Generatore random
    _globe=globe; //Mappa delle città
    _N_allele=_globe->Get_N_cities() +1; //Il percorso passa da ogni città una e una sola volta, e finisce nella città di partenza

    _chromosome=linspace <irowvec>(0,_N_allele-1,_N_allele); 
    _chromosome(_N_allele -1)=0; 

}


Path::Path(Globe * globe,Random * rnd,irowvec * chromosome){
    _rnd=rnd; //Generatore random
    _globe=globe; //Mappa delle città
    
    _chromosome=*chromosome;

    _len=_globe->Get_path_len(_chromosome); 
}





void Path::Shuffle(){

    int index; 
    auto _mutable= _chromosome(span(1,_chromosome.n_elem -2)); //crea una view di armadillo escludento il primo e l'ultimo elemento dell'array (punto di partenza e di arrivo)

    for(int k=0; k < _mutable.n_elem; k++){
        index=static_cast <int> (_rnd->Rannyu(0,_mutable.n_elem) );
        _mutable.swap_cols(k,index); //scambia i due alleli
    
    }


    _len=_globe->Get_path_len(_chromosome); //ricalcola la lunghezza

}





void Path::Swap(){
    
    auto _mutable= _chromosome(span(1,_chromosome.n_elem -2)); //crea una view di armadillo escludento il primo e l'ultimo elemento dell'array (città di partenza e di arrivo,fissa)


    //genera due indici casuali
    int index1=static_cast <int> (_rnd->Rannyu(0,_mutable.n_elem));
    int index2=static_cast <int> (_rnd->Rannyu(0,_mutable.n_elem));

    _mutable.swap_cols(index1,index2); //scambia i due alleli

    _len=_globe->Get_path_len(_chromosome); //ricalcola la lunghezza

    
}



void Path::Shift(){


    int m=static_cast <int> (_rnd->Rannyu(1,_chromosome.n_elem -3 )); //quantità di cui shiftare il vettore

    //per gli alleli interni (ossia escludendo il primo e l'ultimo, che rappresentano la città di partenza (e di arrivo), da tenere fissa)
    _chromosome(span(1,_chromosome.n_elem -2))= shift(_chromosome(span(1,_chromosome.n_elem -2)),m);//effettua lo shift di m 
   
 
    _len=_globe->Get_path_len(_chromosome); //ricalcola la lunghezza

}



void Path::Reverse(){


    //seleziona gli indici di inizio e fine della sequenza da invertire (escludendo il primo e l'ultimo)
    int begin=static_cast <int> (_rnd->Rannyu(1,_chromosome.n_elem -2 )); 
    int end=static_cast <int> (_rnd->Rannyu(begin+1,_chromosome.n_elem -2 )); 

    _chromosome(span(begin,end))= reverse(_chromosome(span(begin,end)));//inverte la sequenza 

    _len=_globe->Get_path_len(_chromosome); //ricalcola la lunghezza

}








