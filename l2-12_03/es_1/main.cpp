#include <iostream> 
#include <fstream>
#include <cmath>
#include "../rand_gen/random.h"

using namespace std; 


double important_sampling(Random *rnd){ //samples distribution 3/2(1-x^2)
    double 



}


int main(){


    //setup generatore

    Random rnd;
    int seed[4];
    int p1, p2;
    ifstream Primes("../rand_gen/Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
    } else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();

    ifstream input("../rand_gen/seed.in");
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

    //parametri

    int M=10000; //numero di punti per il sampling 
    int N=100; //numero di blocchi

    int L=M/N; //numero di lanci per blocco

    /*********************************************************************************/

    //gestione output
    ofstream save_results; 
    save_results.open("results.csv");
    if (!save_results.is_open()){
        cout << "Problemi con l'apertura del file di output" << endl; 
        return 0; 
    }
    save_results << "I,error"  << endl;

    /**********************************************************************************/


    //variabili di servizio

    double block_sum;//somma del blocco
    double I=0; //stima integrale
    double cum_squares_sum=0; //somma dei quadrati, per stima varianza
    double error; //deviazione standard
    double x,f; 

    /**********************************************************************************/


    for (int n=0; n < N; n++){ //itera sui blocchi
        block_sum=0; 
        for(int l=0; l < L; l++){ //itera sui lanci per blocco
            x=rnd.Rannyu(); //genera un punto distribuito uniformemente
            f=cos(M_PI*x*0.5); //calcola il valore della funzione nel punto
            block_sum+=f; //aggiunge f(x_i) alla somma per il calcolo di I
        }

        block_sum/=L; //media del blocco (stima di I)
        I=(n*I +block_sum)/(n+1); // Calcola la media con l'aggiunta del nuovo blocco
        cum_squares_sum=(n*cum_squares_sum +block_sum*block_sum)/(n+1); // calcola la media dei quadrati 
        
        if (n==0){
            error=0;
        }else{
            error=sqrt((cum_squares_sum-I*I)/n); //calcola la deviazione standard cumulativa al blocco n
        }
        
        
        save_results <<I*M_PI/2<< ","  << error << endl; //l'integeale è la media per la costante moltipicativa della funzione
            
       

    }

       return 0; 
}