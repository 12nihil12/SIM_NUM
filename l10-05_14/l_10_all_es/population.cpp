#include "population.h"

#include <algorithm>

#include "random.h"
#include "globe.h"
#include "path.h"


using namespace std; 
using namespace arma; 

Population::Population(int pop_size, Globe *globe, Random *rnd){

    if(globe->Get_N_cities() < 3){
        throw runtime_error("Not enough cities for a circular path.");
    }

    _pop_size=pop_size; 
    _globe=globe;
    _rnd=rnd; 


    _N_allele=_globe->Get_N_cities() +1; 

    _individuals.reserve(_pop_size); 
}


void Population::Initialize(double Pm,double Px,double sel_p){


    _Pm=Pm;
    _Px=Px;
    _sel_p=sel_p; 


    Path individual(_globe,_rnd); 

    for(int i=0; i < _pop_size; i++){
        individual.Shuffle(); //Mischia il cromosoma del percorso corrente in modo randomico
        _individuals.push_back(individual); //Aggiunge il percorso alla popolazione
        
    }
}



void Population::Order(){
    sort(_individuals.begin(),_individuals.end()); //riordina la popolazione in funzione della lunghezza del percorso

    double sum; 
    int counter=0; 
    for(int i=_individuals.size()/2; i< _individuals.size(); i++){
        sum+=_individuals.at(i).Get_len(); 
        counter++; 
    }

    _best_half_avg_len=sum/counter; 
}


void Population::Check(Path & individual){
    auto *chromosome=individual.Get_chromosome(); 

    if(chromosome->n_elem!=_N_allele){ //Il percorso deve essere lungo quanto fissato dalla popolazione
        throw runtime_error("Path lenght does not match population");
    }

    if((*chromosome)(0) != 0 || (*chromosome)(chromosome->n_elem -1)!=0) {//Deve iniziare e finire con 0
        throw runtime_error("Path does not start or end with the first city");
    }
    irowvec unique_elements=unique(*chromosome); 
    if(unique_elements.n_elem != chromosome->n_elem -1 ){//Deve contenere ogni città (e quindi ogni numero una volta), tranne lo 0 che appare due
        throw runtime_error("Unexpected repeated cities in path");
    }

    
}


void Population::New_generation(){
  

    int index1,index2; 
    double r;
            
    vector <Path> new_gen; //nuova generazione di percorsi

    Path parent1,parent2; //temporaneo, per salvataggio genitori

    for (int i=0; i < _pop_size; i+=2 ){

        r=_rnd->Rannyu();
        index1=static_cast<int>(_pop_size*pow(r,_sel_p)); //seleziona il genitore 1

        do{
            r=_rnd->Rannyu();
            index2=static_cast<int>(_pop_size*pow(r,_sel_p)); //seleziona il genitore 2
        }while(index1==index2); 

        //li salva temporaneamente
        parent1=Path(_individuals.at(index1)); 
        parent2=Path(_individuals.at(index2)); 

        if(_rnd->Rannyu() < _Px){ Crossover(parent1,parent2);}; //con probabilità Px fa un crossover tra i due
        if(_rnd->Rannyu() < _Pm){ Mutate(parent1);};//con probabilità Pm muta il genitore1 
        if(_rnd->Rannyu() < _Pm){ Mutate(parent2);}; //con probabilità Pm muta il genitore2
        //La nuova lunghezza del percorso viene automaticamente calcolata dentro la funzione di crossover o mutazione
    

        //controlla se i nuovi percorsi soddisfano i requisiti (altrimenti viene lanciata un'eccezione e il programma termina immediatamente)
        Check(parent1); 
        Check(parent2);
        //aggiunge i due nuovi percorsi alla nuova generazione
        new_gen.push_back(parent1); 
        new_gen.push_back(parent2); 
   


    }

    _individuals=new_gen;//salva la nuova generazione
    
}



void Population::Mutate(Path & individual){
    
    int which=_rnd->Rannyu(0,3); //sceglie casualmente con probabilità uniforme una delle tre mutazioni possibili

    if(which==0){
        individual.Swap();//seleziona due alleli e li scambia
    }else if(which==1){
        individual.Shift(); // seleziona un passo con probabilità uniforme e shifta il vettore di esso
    }else if(which==2){
        individual.Reverse(); //seleziona con probabilità uniforme gli estremi di una sequenza e la inverte
    }
    

    //La nuova lunghezza del percorso viene calcolata dentro le specifiche funzioni

}



void Population::Crossover(Path & parent1, Path & parent2){

    //crea dei puntatori ai cromosomi dei percorsi (per modificarli)
    auto * chromosome1= parent1.Get_chromosome();
    auto * chromosome2= parent2.Get_chromosome();

    int cut_point=static_cast<int>(_rnd->Rannyu(1,chromosome1->n_elem)); //estrae a caso un punto di taglio (escluso il primo e l'ultimo carattere)


    span swap_span=span(cut_point,chromosome1->n_elem-1); //span del vettore da tagliare: dal punto di taglio fino alla fine

    //i valori contenuti nello span che viene tagliato sono ora mancanti dal vettore
    irowvec missing_values1=(*chromosome1)(swap_span); 
    irowvec missing_values2=(*chromosome2)(swap_span); 


    //vettori di appoggio, per il "rimpiazzo" dello span da scambiare
    irowvec replacement1(missing_values1.n_elem); 
    irowvec replacement2(missing_values2.n_elem); 


    int i=0;
    for(const auto value: (*chromosome2)(span(1,chromosome2->n_elem -1))){ //tra i valori del vettore 2 escluso il primo
    
        if(any(missing_values1==value)){ //cerca quelli che mancano al vettore 1
            replacement1(i)=value; //li aggiunge nell'ordine in cui appaiono nel vettore 2
            i++; 
        }
    }

    i=0; 
    for(const auto value: (*chromosome1) (span(1,chromosome1->n_elem -1))){//tra i valori del vettore 1 escluso il primo
        if(any(missing_values2==value)){//cerca quelli che mancano al vettore 2
            replacement2(i)=value; //li aggiunge nell'ordine in cui appaiono nel vettore 1
            i++;
        }
    }


    //effettua il "rimpiazzo"
    (*chromosome1)(swap_span)=replacement1;
    (*chromosome2)(swap_span)=replacement2;


    //Ricalcola la lunghezza
    parent1.Set_len(_globe->Get_path_len(*chromosome1)); 
    parent2.Set_len(_globe->Get_path_len(*chromosome2)); 



}






void Population::Print(const  char * filename)const{
    ofstream fout; 

    fout.open(filename); 

    if(!fout.is_open()){
        throw runtime_error("File not found"); 
    }

    fout << left << "Lenght" << "    | Path" << endl;  
    for (auto it = _individuals.begin(); it != _individuals.end(); ++it) {it->Print(fout);}

    fout.close(); 


};


