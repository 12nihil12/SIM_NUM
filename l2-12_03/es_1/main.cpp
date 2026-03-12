#include <iostream> 
#include <fstream>
#include <cmath>
#include "../rand_gen/random.h"

using namespace std; 


double calc_unif(Random *rnd){

    double x=rnd->Rannyu(); //genera un punto distribuito uniformemente
    return cos(M_PI*x*0.5); //calcola il valore della funzione nel punto

}

double calc_import_sampl(Random* rnd){
    double x=1-sqrt(1-rnd->Rannyu()); //genera x sampled da d(x)=2(1-x) su [0,1)
    return cos(M_PI*x*0.5)/(2*(1-x)); //calcola f(x)p(x)/d(x) e lo restituisce

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
    save_results << "I (unif),error (unif),I(i.s.),error(i.s.)"  << endl;

    /**********************************************************************************/


    //variabili di servizio

    double block_sum_1;//somma del blocco
    double block_sum_2;//somma del blocco

    double I_1=0; //stima integrale
    double I_2=0; //stima integrale

    double cum_squares_sum_1=0; //somma dei quadrati, per stima varianza
    double cum_squares_sum_2=0; //somma dei quadrati, per stima varianza

    double error_1; //deviazione standard
    double error_2; //deviazione standard

 

    /**********************************************************************************/


    for (int n=0; n < N; n++){ //itera sui blocchi
        block_sum_1=block_sum_2=0; 
        for(int l=0; l < L; l++){ //itera sui lanci per blocco
            block_sum_1+=calc_unif(&rnd); //aggiunge f(x_i) alla somma per il calcolo di I
            block_sum_2+=calc_import_sampl(&rnd); //aggiunge f(x_i) alla somma per il calcolo di I

        }

        block_sum_1/=L; //media del blocco (stima di I)
        block_sum_2/=L; //media del blocco (stima di I)

        I_1=(n*I_1 +block_sum_1)/(n+1); // Calcola la media con l'aggiunta del nuovo blocco
        I_2=(n*I_2 +block_sum_2)/(n+1); // Calcola la media con l'aggiunta del nuovo blocco

        cum_squares_sum_1=(n*cum_squares_sum_1 +block_sum_1*block_sum_1)/(n+1); // calcola la media dei quadrati 
        cum_squares_sum_2=(n*cum_squares_sum_2 +block_sum_2*block_sum_2)/(n+1); // calcola la media dei quadrati 
        
        if (n==0){
            error_1=0;
            error_2=0;

        }else{
            error_1=sqrt((cum_squares_sum_1-I_1*I_1)/n); //calcola la deviazione standard cumulativa al blocco n
            error_2=sqrt((cum_squares_sum_2-I_2*I_2)/n); //calcola la deviazione standard cumulativa al blocco n
            
        }
        
        
        save_results <<I_1*M_PI/2<< ","  << error_1 << "," << I_2*M_PI/2<< ","  << error_2 << endl; //l'integeale è la media per la costante moltipicativa della funzione
            
       

    }

       return 0; 
}