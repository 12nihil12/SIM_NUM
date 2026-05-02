#ifndef ___System___
#define ___System___

#include <armadillo>
#include "functions.h"

class Random;



class System{



    private: 

        double _sigma; double _mu;// parametri
        double _x; //posizione

        Result _exp_H; //valore atteso dell'hamiltoniana (col suo errore)
    
        double _x_new,_save_sigma,_save_mu; 




    public: 

        System(double,double,double);
       
        void Set_H_exp_val(Result exp_H){_exp_H=exp_H;}; 
        Result Get_H_exp_val(){return _exp_H;}; 

        double Get_x(){return _x;};
        void Set_x(double x){_x = x;};

        double Get_sigma(){return _sigma;};
        void Set_sigma(double sigma){_sigma = sigma;};

        double Get_mu(){return _mu;};
        void Set_mu(double mu){_mu = mu;};

        void Save_par(){_save_sigma=_sigma; _save_mu=_mu;};
        void Set_par(double sigma,double mu){_sigma=sigma; _mu=mu;};
        void Set_saved_par(){_sigma=_save_sigma; _mu=_save_mu;}




        double Metro_Sample(unsigned int N, arma::vec & samples, Random *rnd, double delta,unsigned int N_E=0); //crea un vettore di N samples secondo Metropolis e restituisce l'accettanza
        
        bool Metro_Move(Random * rnd, double delta); //algoritmo di Metropolis:restituisce True se la mossa è accettata e la esegue, False altrimenti (e non fa nulla)
        
        //funzioni di appoggio per Metro_move 
        void Metro_try_move(Random * rnd, double delta); //tenta una mossa di Metropolis, con parametro delta
        double Metro_eval_move();//restituisce la probabilità di accettare la mossa
        void Metro_make_move();//esegue una mossa (che dev'essere stata accettata)



        
        double Potential(double); 
        double Wave_function(double); //funzione d'onda (non normalizzata)
        double Hamiltonian_on_WF(double); 


        
        arma::vec Potential(arma::vec); 
        arma::vec Wave_function(arma::vec); //funzione d'onda (non normalizzata)
        arma::vec Hamiltonian_on_WF(arma::vec); 


        Result Hamiltonian_exp_val(NumberOf N, Random * rnd,double delta, arma::vec & x,double & acceptance); //calcola il valore atteso dell'hamiltoniana
        // (con data blocking: N_steps, N_blocks), sampling con metropolis: (passo delta, step di equilibrazione N_eq_steps) [NumberOf N: N_steps,N_blocks,N_eq_steps]
        //I N_steps*N_blocks valori sampled con metropolis vengono salvati in un vettore x passato per riferimento, per un eventuale futuro utilizzo
        //Sempre per riferimento viene passato un double dove salvare l'accettanza di metropolis
       

        


};





#endif // ___System___