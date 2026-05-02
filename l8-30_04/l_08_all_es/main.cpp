#include <iostream> 
#include <fstream>
#include <cmath>
#include <armadillo>
#include <iomanip>
#include <string>
#include "functions.h"
#include "random.h"
#include "system.h"
using namespace std;
using namespace arma; 

constexpr const char * RAND_GEN_PATH= "../rand_gen/";



Metro Metro_Evolve( unsigned int R, double beta, const char * filename,  double delta_in, double delta_out, NumberOf N, System * sys,Random *rnd); 


int main (int argc, char *argv[]){


    //SETUP GENERATORE
    Random rnd;
    int seed[4];
    int p1, p2;
    ifstream Primes(string(RAND_GEN_PATH) +"Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
    } else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();

    ifstream input(string(RAND_GEN_PATH) +"seed.in");
    string property;
    if (input.is_open()){
        while ( !input.eof() ){
            input >> property;
            if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
            }
        }
        input.close();
    } else cerr << "PROBLEM: Unable to open seed.in" << endl;


    /**********************************************************************************/

    //PARAMETRI 

    NumberOf N; 

    N.blocks=100; //numero blocchi
    N.steps=1000;//step per blocco 
    N.eq_steps=100; //step di equilibrazione (per il metropolis interno)


  
    /**********************************************************************************/


    //PARAMETRI METROPOLIS

    Metro acceptance; //accettanza del metropolis (esterno,media dell'accettanza del metropolis interno)

    //beta
   
    double T_start=2.0;
    double T_end=0.5;
    double k=0.2; 
    double alpha=0.8;
    int B=30; //numero valori di beta
    //auto Beta=[=] (int j)  {return 1./(T_end+(T_start-T_end)*exp(-k*j));}; 
    auto Beta=[=] (double beta)  {return beta/alpha;}; 

    double beta=1/T_start;
    
    //Step metropolis per ogni beta

    int R_start=100; //numero di step metropolis per beta_start
    auto R_ev=[=](int j){return R_start;};
    int R;

    //delta

    double delta_in=0.8; //delta per il metropolis interno (sampling della funzione d'onda)
    double delta_out_start=0.5;//delta per il metropolis principale
    double delta_out;
    auto Delta=[=](int j) {return delta_out_start/(pow(double(1+j),0.5));}; 
  
    /**********************************************************************************/


    //STAMPA RISULTATI PRINCIPALI
    
    ofstream print_results;

    print_results.open("Results.dat");

    print_results << "j" << setw(12) << "beta" << setw(12) << "delta" << setw(12) << "acc_outer " << setw(12) << "avg_acc_inn  "<< setw(12)  << "last_exp_H \n"; 

    /**********************************************************************************/


    //VARIABILI DI SERVIZIO


    string filename; //per il nome dei file dei risultati intermedi (singolo beta)
    vec x ; //vettore in cui salvare x(di servizio a Hamiltonian_exp_val )
    double accept_in; //accettanza del metropolis interno (di servizio a Hamiltonian_exp_val)


    /**********************************************************************************/


    //PARAMETRI SISTEMA E INIZIALIZZAZIONE

    double mu_0=1.2; 
    double sigma_0=0.5;
    

    System sys(0.,sigma_0,mu_0); //Inizializza il sistema in x=0,sigma=sigma_0, mu=mu_0
    Result exp_H=sys.Hamiltonian_exp_val(N,&rnd,delta_in,x,accept_in); //calcola il valore atteso dell'hamiltoniana coi parametri dati
    sys.Set_H_exp_val(exp_H); //lo salva


    /**********************************************************************************/
    
   

    for(int j=0; j < B; j++){
        beta/=alpha;//modifica beta
        R=R_ev(j);
        delta_out=Delta(j);
        //cout << delta_out << endl;

        

        filename="OUTPUT/" +to_string(j) + ".csv" ; //crea il file in cui salvare i risultati

        //evolve il sistema con metropolis per R passi col beta e i parametri delta dati, restituisce l'accettanza e salva i risultati
        acceptance=Metro_Evolve(R,beta,filename.c_str(),delta_in,delta_out, N, &sys,&rnd);

        Result exp_H = sys.Get_H_exp_val();

        print_results << j << setw(12) << beta << setw(12) << delta_out << setw(12) << acceptance.outer  <<  setw(12) << acceptance.inner <<  setw(12) <<  exp_H.value <<endl ; 


    }



    /**********************************************************************************/



 
  
    return 0; 
}





Metro Metro_Evolve( unsigned int R, double beta, const char * filename,  double delta_in, double delta_out, NumberOf N, System * sys,Random *rnd){



    int accepted=0;//accettanza metropolis esterno 
    double accept_in; //accettanza metropolis interno

    double sum_accept_in=0; //di servizio, per calcolare l'accettanza media del metropolis interno

    double new_sigma,new_mu; //parametri di servizio


    ofstream print_results; //stampa i risultati 
    print_results.open(filename); 
    print_results << "sigma" <<"," << "mu" <<"," << "p,"<< "exp_H" <<"," << "error" <<"," << "acceptance" << endl; 

   
    vec x ; 
    Result new_exp_H;

    for(int r=0; r < R; r++){
        //propone una mossa
        new_sigma=rnd->Gauss(sys->Get_sigma(),delta_out); 
        new_mu=rnd->Gauss(sys->Get_mu(),delta_out); 

        if(new_sigma <0 || new_mu < 0){ //i parametri possono essere solo positivi (simmetria): se sono negativi ignora la mossa a priori
            continue; 
        }


        sys->Save_par(); //salva i parametri attuali (sigma, mu), in modo da non perderli
        sys->Set_par(new_sigma,new_mu);//modifica i parametri (proposta di mossa)

        //calcola la nuova energia
        new_exp_H=sys->Hamiltonian_exp_val(N,rnd,delta_in,x,accept_in); //salva il nuovo valore dell'energia

        sum_accept_in+=accept_in; //per calcolare l'accettanza media del metropolis interno 


        double p= exp(- beta* (new_exp_H.value-sys->Get_H_exp_val().value)); //calcola l'accettanza relativa alla mossa

        if(Metro_accept(p,rnd)){ //se la mossa è stata accettata
            accepted++; //aumenta il numero di mosse accettate
            sys->Set_H_exp_val(new_exp_H); //salva il nuovo valore dell'energia
            
        }else{
            sys->Set_saved_par();//se la mossa non è stata accettata, rimette i parametri com'erano prima
        }


        //salva l'accettanza media del metropolis interno



        //salva i risultati
        print_results  << sys->Get_sigma() <<","<< sys->Get_mu() <<"," << p << ","<< sys->Get_H_exp_val().value <<","<< sys->Get_H_exp_val().error <<","<< accept_in <<"\n"; 
        


    }


    Metro acceptance; 

    acceptance.inner=sum_accept_in/R; 
    acceptance.outer=double(accepted)/R;
    return acceptance; //ritorna l'accettanza del metropolis esterno 

}