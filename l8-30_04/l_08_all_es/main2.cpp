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



double delta(int j){
    if (j < 2){
        return 0.7;
    }else if(j < 8){
        return 0.5; 
    }else if(j <15){
        return 0.2; 
    }else if (j < 21){
        return 0.01; 
    }else if (j < 26){
        return 0.001;
    }else {
        return 0.0001; 
    }
}

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
    unsigned int N_steps=5000;//step per blocco 

    double delta_in=0.9; //WARNING: CHANGING THIS WILL RESULT IN EVERYTHING BREAKING DOWN

    ofstream print_best; //per i migliori parametri, stampa in dettaglio il calcolo dell'integrale e la distribuzione delle x 



    //per variabili temporanee metropolis
    vec x,H_on_WF; //x e f(x) per valore medio hamiltoniana
    x.set_size(N_steps*N_blocks); 
    H_on_WF.set_size(N_steps*N_blocks);



    /**********************************************************************************/


    //PARAMETRI SISTEMA E INIZIALIZZAZIONE

    double mu_0=0.804044;
    double sigma_0=0.627135;
    
    System sys(0.,sigma_0,mu_0); //Inizializza il sistema in x=0,sigma=sigma_0, mu=mu_0

    

    //per gli ultimi valori di sigma e mu, stampa nel dettaglio

    //calcola e stampa la distribuzione delle x secondo il modulo quadro della funzione d'onda
    sys.Metro_Sample(N_blocks*N_steps,x,&rnd,delta_in);
    print_best.open("x_dist.dat"); 
    x.print(print_best); 
    print_best.close(); 
    

    //calcola l'integrale ,stampando il dettaglio
    sys.Hamiltonian_on_WF(x,H_on_WF); 
    Data_blocking(N_blocks,N_steps,H_on_WF,true,"integral.dat"); 


 
  
    return 0; 
}


