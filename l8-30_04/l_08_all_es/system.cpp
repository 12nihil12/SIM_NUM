#include <cmath>
#include "random.h"
#include "system.h"



System::System(double x,double sigma = 1, double mu=0){
    
    _sigma = sigma; 
    _mu=mu;
    _x=x; 
    
}


double System::Metro_Sample(unsigned int N, arma::vec & samples, Random *rnd, double delta,unsigned int N_E){ //crea un vettore di N samples secondo Metropolis e restituisce l'accettanza


    
    for(int i=0; i<N_E; i++){ //step di equilibrazione
        Metro_Move(rnd,delta); //esegue la mossa
    }


    samples.set_size(N);

    int accepted=0; 
    bool is_accepted=false;

    for(int i=0; i<N; i++){
        is_accepted=Metro_Move(rnd,delta); //esegue la mossa
        if(is_accepted){ accepted++; }//se è stata accettata aumenta il contatore
        samples(i)=_x; //salva la x corrente
    }


    return double(accepted)/N; 


}




bool System::Metro_Move(Random * rnd, double delta){
        
    //prova una mossa
    Metro_try_move(rnd,delta); 
   
    //ne calcola l'accettazione
    double p= Metro_eval_move(); 

  
    if (Metro_accept(p,rnd)){ //se la mossa viene accettata
        Metro_make_move(); //la esegue  mossa
        return true; 
    }

    return false; 

}










void System::Metro_try_move(Random * rnd, double delta){
    _x_new=rnd->Gauss(_x,delta); 
    return;
}


double System::Metro_eval_move(){
    return pow(Wave_function(_x_new)/Wave_function(_x),2);
}


void System::Metro_make_move(){
    _x=_x_new;
    return;
}





double System::Potential(double x){
    return pow(x,4) - 5/double(2) * x * x; 

}

double System::Wave_function(double x){ //funzione d'onda, non normalizzata

    double den= 0.5*1.0/(_sigma*_sigma); // 1/denominatore, comune ad entrambi

    double shift_right=-pow(x-_mu,2)*den; 
    double shift_left=-pow(x+_mu,2)*den; 

    return exp(shift_left) + exp(shift_right);
}

double System::Hamiltonian_on_WF(double x){

    double c=1.0/(_sigma*_sigma);
    double arg=x*_mu*c; 
  
    double kinetic_part=- 0.5*c*((x*x+_mu*_mu)*c -1.0 - 2.0*arg*tanh(arg));
   
    return kinetic_part+ Potential(x);
}







arma::vec System::Potential(arma::vec x){
    return arma::pow(x,4) - 5/double(2) * arma::square(x); 

}

arma::vec System::Wave_function(arma::vec x){ //funzione d'onda, non normalizzata

    double den= 0.5*1.0/(_sigma*_sigma); // 1/denominatore, comune ad entrambi

    return arma::exp(-arma::square(x-_mu)*den) + arma::exp(-arma::square(x+_mu)*den);
}

arma::vec System::Hamiltonian_on_WF(arma::vec x){

    double c=1.0/(_sigma*_sigma);
    arma::vec arg=x*_mu*c; 
   
    return - 0.5*c*((arma::square(x)+_mu*_mu)*c -1.0 - 2.0*arg%arma::tanh(arg))+ Potential(x);
}






Result System::Hamiltonian_exp_val(NumberOf N, Random * rnd,double delta, arma::vec & x,double & acceptance){

    
    acceptance=Metro_Sample(N.steps*N.blocks,x,rnd,delta,N.eq_steps); //calcola x distribuita secondo il modulo quadro di psi

    arma::vec H_on_WF=Hamiltonian_on_WF(x); //per ogni x calcola l'hamiltoniana applicata a psi(x) diviso per psi(x)

    return Data_blocking(N.blocks,N.steps,H_on_WF); //col data blocking, calola l'integrale   

}



