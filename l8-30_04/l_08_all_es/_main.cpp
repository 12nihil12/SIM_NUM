
























#include <iostream> 
#include <fstream>
#include <cmath>
#include <armadillo>
#include <iomanip>
#include <string>
#include <map>

#include "functions.h"
#include "random.h"
#include "system.h"

using namespace std;
using namespace arma; 

constexpr const char * RAND_GEN_PATH= "../rand_gen/";


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


    //MonteCarlo

    unsigned int N_blocks=100; //numero blocchi
    unsigned int N_steps=1000;//step per blocco 


    //Metropolis interno (per sampling funzione d'onda)

    unsigned int N_eq_steps=100; //step di equilibrazione (per il metropolis interno)
    double delta_in=0.8;


    //Metropolis esterno (per ottimizzazione parametri di sistema) 


    //beta 

    double T_start=2.0; //temperatura iniziale
    double alpha=0.8; //coefficiente per progressione geometrica
    int B=30; //numero valori di beta considerati
    double beta=1/T_start; //beta iniziale
    
    //Step metropolis per ogni beta
    int R_start=100; //numero di step metropolis per beta_start
    int R=R_start;

    //delta

    double delta_out_start=0.5;//delta per il metropolis principale
    double delta_out=delta_out_start;
  
    /**********************************************************************************/


    //STAMPA RISULTATI PRINCIPALI
    
    ofstream print_results;

    print_results.open("Results.dat");

    print_results << "j" << setw(12) << "beta" << setw(12) << "delta" << setw(12) << "acc_outer " << setw(12) << "avg_acc_inn  "<< setw(12)  << "last_exp_H \n"; 

    /**********************************************************************************/


    //VARIABILI DI SERVIZIO

    vec x; //vettore in cui salvare x (di servizio a Hamiltonian_exp_val )
    double accept_in; //accettanza del metropolis interno (di servizio a Hamiltonian_exp_val)

    string filename; //per il nome dei file dei risultati intermedi (singolo beta)
    Result exp_H; //per il valore medio dell'hamiltoniana 
    map <string,double> acceptance; //accettanza del metropolis esterno,media dell'accettanza del metropolis interno


    /**********************************************************************************/


    //PARAMETRI SISTEMA E INIZIALIZZAZIONE

    double mu_0=1.2; 
    double sigma_0=0.5;
    
    System sys(0.,sigma_0,mu_0,delta_in,N_blocks,N_steps); //Inizializza il sistema in x=0,sigma=sigma_0, mu=mu_0
    exp_H=sys.Hamiltonian_exp_val(&rnd,accept_in,x); //calcola il valore atteso dell'hamiltoniana coi parametri dati
    sys.Set_H_exp_val(exp_H); //lo salva


    /**********************************************************************************/
    

    beta=30; 

    delta_out=0.001;
 

    for(int j=0; j < 100; j++){
       
       
        //delta_out=delta_out_start/(pow(double(1+j),0.5));


       

        filename="OUTPUT/" +to_string(j) + ".csv" ; //crea il file in cui salvare i risultati

        //evolve il sistema con metropolis per R passi col beta e i parametri delta dati, restituisce l'accettanza e salva i risultati
        acceptance=sys.Metro_Evolve_pams(R,beta,delta_out,&rnd,true,filename.c_str());

        exp_H = sys.Get_H_exp_val();

        print_results << j << setw(12) << beta << setw(12) << delta_out << setw(12) << acceptance["outer"]  <<  setw(12) << acceptance["avg_inner"] <<  setw(12) <<  exp_H.value <<endl ; 


        //beta/=alpha; //progressione geometrica beta

        delta_out+=0.007;

    }


    
    /**********************************************************************************/



 
  
    return 0; 
}



