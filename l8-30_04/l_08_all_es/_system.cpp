#include <cmath>

#include "random.h"
#include "system.h"

using namespace std; 
using namespace arma; 



System::System(double x,double sigma, double mu,double delta,unsigned int N_E, unsigned int N_blocks, unsigned int N_steps){
    
    _sigma = sigma; 
    _mu=mu;
    _x=x; 


    //METROPOLIS 
    _delta=delta; 
    _N_E=N_E;


    //MONTECARLO

    _N_blocks=N_blocks;
    _N_steps=N_steps; 
    _N_E=_N_E;


    _exp_H.value=NAN; //non è ancora stato calcolato
    _exp_H.error=NAN; //non è ancora stato calcolato

}







double System::Metro_Sample(unsigned int N, vec & samples, Random *rnd){ //crea un vettore di N samples secondo Metropolis e restituisce l'accettanza

    for(int i=0; i<_N_E; i++){ //step di equilibrazione
        Metro_Move(rnd); //esegue la mossa
    }

    samples.set_size(N);

    int accepted=0; 
    bool is_accepted=false;

    for(int i=0; i<N; i++){
        is_accepted=Metro_Move(rnd); //esegue la mossa

        if(is_accepted){ accepted++; }//se è stata accettata aumenta il contatore
        samples(i)=_x; //salva la x corrente
    }


    return double(accepted)/N; 


}




bool System::Metro_Move(Random * rnd){
        
    //prova una mossa
    _x_new=rnd->Gauss(_x,_delta); 
   
    //ne calcola l'accettazione
    double A= WF_mod_square(_x_new)/WF_mod_square(_x); 


    if(A > 1 || rnd->Rannyu() < A ){ // se A >1 accetta con certezza, se A < 1 accetta con probabilità A 
        _x=_x_new; //accetta la mossa 
        return true;
    }

    return false; 

}





double System::Potential(double x){
    return pow(x,4) - 5/double(2) * x * x; 

}

double System::WF_mod_square(double x){ //modulo quadro funzione d'onda, non normalizzata

    double den= 0.5*1.0/(_sigma*_sigma); // 1/denominatore, comune ad entrambi

    double shift_right=-pow(x-_mu,2)*den; 
    double shift_left=-pow(x+_mu,2)*den; 

    return pow(exp(shift_left) + exp(shift_right),2);
}

double System::Hamiltonian_on_WF(double x){

    double c=1.0/(_sigma*_sigma);
    double arg=x*_mu*c; 
  
    double kinetic_part=- 0.5*c*((x*x+_mu*_mu)*c -1.0 - 2.0*arg*tanh(arg));
   
    return kinetic_part+ Potential(x);
}



void System::Potential(vec  & x,vec & res){
    res=pow(x,4) - 5/double(2) * square(x); 
    return; 
}

void System::WF_mod_square(vec  & x,vec & res){ //funzione d'onda, non normalizzata

    double den= 0.5*1.0/(_sigma*_sigma); // 1/denominatore, comune ad entrambi

    res = square(exp(-square(x-_mu)*den) + exp(-square(x+_mu)*den));

    return; 
}

void System::Hamiltonian_on_WF(vec & x,vec & res){

    double c=1.0/(_sigma*_sigma);
    vec arg=x*_mu*c; 
   

    res= - 0.5*c*((square(x)+_mu*_mu)*c -1.0 - 2.0*arg%tanh(arg)) //parte cinetica
    + pow(x,4) - 5/double(2) * square(x); //parte potenziale
    return;
}




Result System::Hamiltonian_exp_val(Random * rnd,double & acceptance,vec & x, bool print_results,const char * filename){

    vec H_on_WF;
    H_on_WF.set_size(x.n_elem); 

    acceptance=Metro_Sample(_N_blocks*_N_steps,x,rnd); //calcola x distribuita secondo il modulo quadro di psi
    Hamiltonian_on_WF(x,H_on_WF); //per ogni x calcola l'hamiltoniana applicata a psi(x) diviso per psi(x) e la salva in H_on_Wf 
    return Data_blocking(_N_blocks,_N_steps,H_on_WF,print_results,filename); //col data blocking, calola la media di H_on_Wf (e quindi l'integrale)

}





map<string, double>  System::Metro_Evolve_pams( unsigned int R, double beta, double delta_out , Random *rnd, bool print_results, const char * filename){



    ofstream print; //stampa i risultati 

    if(print_results){
        print.open(filename); 
        if(print.is_open()){
            print << "sigma" <<"," << "mu" <<"," << "exp_H" <<"," << "error" <<"," << "acceptance" << endl; 

        }
    }

    int accepted=0;//numero mosse accettate metropolis esterno
    double sum_accept_in=0; //accettanza media del metropolis interno
    
    //variabili di servizio, per salvataggio temporaneo
    double A; 
    double accept_in; 
   
    double new_sigma,new_mu; 
    double save_sigma,save_mu; 

    //Prepara i vettori per il buffer
    _x_buffer.set_size(_N_blocks*_N_steps); 
    _H_on_Wf_buffer.set_size(_N_blocks*_N_steps); 


    Result new_exp_H;

    int R_eff=R; 

    for(int r=0; r < R; r++){
        
        
        
        //PROPOSTA DI MOSSA


        new_sigma=rnd->Gauss(_sigma,delta_out); new_mu=rnd->Gauss(_mu,delta_out); //calcola dei nuovi parametri


        if(new_sigma <0 || new_mu < 0){ //i parametri possono essere solo positivi (simmetria): se sono negativi ignora la mossa a priori
            R_eff--;//lo step non viene conteggiato per l'accettanza media del ciclo interno
            
            continue; 
        }
        
        save_sigma=_sigma; save_mu=_mu; //salva i parametri attuali 
        _sigma=new_sigma; _mu=new_mu;//modifica i parametri (proposta di mossa)

        //VALUTAZIONE DELL'ACCETTAZIONE 

        new_exp_H=Hamiltonian_exp_val(rnd,accept_in); //calola e salva il nuovo valore dell'energia

        sum_accept_in+=accept_in; //somma l'accettanza del metropolis interno per calcolarne la media 

        A= exp(- beta* (new_exp_H.value-_exp_H.value)); //calcola l'accettanza relativa alla mossa exp(-beta (E_new - E_old))


        if(A > 1 || rnd->Rannyu() < A ){ // se A >1 accetta con certezza, se A < 1 accetta con probabilità A 
            accepted++; //aumenta il numero di mosse accettate
            _exp_H=new_exp_H; //salva il nuovo valore dell'energia
            
        }else{
            _sigma=save_sigma;_mu=save_mu;//se la mossa non è stata accettata, rimette i parametri com'erano prima
    
        }

        //salva i risultati
        if(print_results && print.is_open()){
             print  << _sigma <<","<< _mu << ","<< _exp_H.value <<","<< _exp_H.error <<","<< accept_in <<"\n"; 

        }
       
    }

    if(print.is_open()){ print.close();}

    map<string, double> acceptance ={{"avg_inner",sum_accept_in/R_eff }, {"outer",double(accepted)/R }}; 
    return acceptance; //ritorna l'accettanza del metropolis esterno 

}



//Funzioni di supporto a Metro_Evolve_pams (ottimizzate per essere più veloci)

Result System::Hamiltonian_exp_val(Random * rnd,double & acceptance){

    acceptance=Metro_Sample(rnd); //calcola x distribuita secondo il modulo quadro di psi e la salva in _x_buffer
    Hamiltonian_on_WF(); //per ogni x in _x_buffer calcola l'hamiltoniana applicata a psi(x) diviso per psi(x) e la salva in _H_on_Wf_buffer
    return Data_blocking(_N_blocks,_N_steps,_H_on_Wf_buffer); //col data blocking, calola la media di H_on_Wf_buffer (e quindi l'integrale)

}

double System::Metro_Sample(Random *rnd){ 

    int N=_N_blocks*_N_steps;
    int accepted=0; 
    bool is_accepted=false;

    for(int i=0; i<N; i++){
        is_accepted=Metro_Move(rnd); //esegue la mossa

        if(is_accepted){ accepted++; }//se è stata accettata aumenta il contatore
        _x_buffer(i)=_x; //salva la x corrente
    }


    return double(accepted)/N; 


}

void System::Hamiltonian_on_WF(){

    double c=1.0/(_sigma*_sigma);
    vec arg=_x_buffer*_mu*c; 
   

    _H_on_Wf_buffer = - 0.5*c*((square(_x_buffer)+_mu*_mu)*c -1.0 - 2.0*arg%tanh(arg)) //parte cinetica
    + pow(_x_buffer,4) - 5/double(2) * square(_x_buffer); //parte potenziale
    return;
}




