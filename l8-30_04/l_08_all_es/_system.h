#ifndef ___System___
#define ___System___

#include <armadillo>
#include <map>
#include <string>
#include <exception>
#include "functions.h"

class Random;



class System{



    private: 

        double _sigma; double _mu;// parametri
        double _x; //posizione




        Result _exp_H; //valore atteso dell'hamiltoniana (col suo errore)
    
        
        //double _save_sigma,_save_mu; // parametri ausiliari, permettono il salvataggio di sigma e mu prima di sovrascriverli
        
        // Parametri Metropolis (per estrarre x secondo la probabilità data dal modulo quadro della funzione d'onda)
    
        
        double _delta; //parametro per la mossa (estrazione da gaussiana _x_new=G(_x,_delta))
        unsigned int _N_E; //quante mosse "a vuoto" prima di salvare i risultati in un vettore
        double _x_new;//ausiliare, per mossa metropolis


        //Parametri Montecarlo (per calcolare il valore medio dell'hamiltoniana)

        unsigned int _N_blocks; //numero blocchi
        unsigned int _N_steps; //numero step



        //ottimizzazione performance 
        arma::vec _x_buffer,_H_on_Wf_buffer; //vettori in cui salvare temporaneamente i dati durante la simulazione 
        double Metro_Sample(Random *rnd); //crea un vettore di N_steps*N_blocks samples secondo Metropolis, lo salva in
        // _x_buffer, e restituisce l'accettanza
        void Hamiltonian_on_WF(); //Restituisce H psi /psi , per calcolo <H> , utilizzando _x_buffer e salvando in _H_on_Wf_buffer
        Result Hamiltonian_exp_val(Random * rnd,double & acceptance); //calcola il valore atteso dell'hamiltoniana, utilizzando _x_buffer, _H_on_Wf_buffer



    public: 

        System(double x,double sigma, double mu,double delta,unsigned int N_E=0, unsigned int N_blocks=100, unsigned int N_steps=1000);



        //Parametri Monte Carlo

        void Set_MC_pams(unsigned int N_blocks, unsigned int N_steps){ _N_blocks=N_blocks; _N_steps=N_steps; };
        unsigned int Get_N_blocks(){ return _N_blocks;};
        unsigned int Get_N_steps(){ return _N_steps;};


        //Parametri metropolis

        void Set_Metro_Delta(double delta){ _delta=delta;};
        double Get_Metro_Delta(){ return _delta;};
        void Set_Metro_N_E(unsigned int N_E){ _N_E=N_E;};
        unsigned int Get_Metro_N_E(){ return _N_E;};

        
        // Valore atteso hamiltoniana 
        void Set_H_exp_val(Result exp_H){_exp_H=exp_H;}; 
        Result Get_H_exp_val(){if(std::isnan(_exp_H.value)) { throw std::runtime_error("Nan value encountered");} return _exp_H;}; 

        // x
        double Get_x(){return _x;};
        void Set_x(double x){_x = x;};


        //Parametri: sigma, mu
        double Get_sigma(){return _sigma;};
        void Set_sigma(double sigma){_sigma = sigma;};

        double Get_mu(){return _mu;};
        void Set_mu(double mu){_mu = mu;};

        //void Save_par(){_save_sigma=_sigma; _save_mu=_mu;};
        //void Set_par(double sigma,double mu){_sigma=sigma; _mu=mu;};
        //void Set_saved_par(){_sigma=_save_sigma; _mu=_save_mu;}




        double Metro_Sample(unsigned int N, arma::vec & samples, Random *rnd); //crea un vettore di N samples secondo Metropolis, lo salva in samples, e restituisce l'accettanza
        bool Metro_Move(Random * rnd); //algoritmo di Metropolis: prova una mossa, la valuta, e restituisce True se la mossa è accettata e la esegue, False altrimenti (e non fa nulla)
        

        //applica le seguenti funzioni a x 

        double Potential(double x); //Energia potenziale
        double WF_mod_square(double x);  //modulo quadro funzione d'onda (non normalizzata)
        double Hamiltonian_on_WF(double x); //Restituisce H psi /psi , per calcolo <H>

        
        //applica le seguenti funzioni a un vettore di x e salva il risultato in res 
        void Potential(arma::vec & x,arma::vec & res);  //Energia potenziale
        void WF_mod_square(arma::vec & x,arma::vec & res); //modulo quadro funzione d'onda (non normalizzata)
        void Hamiltonian_on_WF(arma::vec & x,arma::vec & res); //Restituisce H psi /psi , per calcolo <H>



        Result Hamiltonian_exp_val(Random * rnd,double & acceptance,arma::vec & x, bool print_results=false,const char * filename="Results.dat"); 
        //calcola il valore atteso dell'hamiltoniana
        //Per riferimento viene passato un double dove salvare l'accettanza di Metropolis, e un vettore x dove salvare i risultati 
        //print_results e filename, opzionali, vengono passate alla funzione di Data Blocking: se print_results=true, viene stampato il valore dell'integrale (col suo errore) in funzione del numero di blocchi

        
        std::map<std::string, double>  Metro_Evolve_pams( unsigned int R, double beta, double delta_out, Random *rnd, bool print_results=false, const char * filename= "Results.dat"); 
        //evolve per R step sigma,mu secondo Metropolis con accettanza A=exp(-beta(E_new - E_old)) ed E valore atteso hamiltoniana
        //se print_results = True salva le statistiche di ogni step di evoluzione nel file indicato da filename




};





#endif // ___System___