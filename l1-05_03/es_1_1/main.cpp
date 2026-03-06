#include <iostream> 
#include <fstream>
#include <cmath>
#include "../../rand_gen/random.h"

using namespace std;


 
int main (int argc, char *argv[]){

    //setup generatore
    Random rnd;
    int seed[4];
    int p1, p2;
    ifstream Primes("../../rand_gen/Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
    } else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();

    ifstream input("../../rand_gen/seed.in");
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


    int M=100000; //steps
    int N=100; //numero blocchi
    int L=M/N;//grandezza blocco 

    double block_sum=0; 
    double cum_avg=0; 
    double sqrd_cum_avg=0;
    double error=0;
    
    ofstream save_results; 
    save_results.open("results.csv");
    save_results << "cum_avg,sqrd_cum_avg,error"    << endl;
   

    if (!save_results.is_open()){
        return 0; 
    }
    for (int n=0;n<N;n++){
        block_sum=0; 
        for(int i=0;i<L;i++){

            block_sum+= rnd.Rannyu()-1/2;
        }
        block_sum/=L; //Calcola la media del blocco
    
        cum_avg= (n*cum_avg + block_sum)/(n+1); //calcola la media cumulativa delle medie dei blocchi fino al blocco n

        sqrd_cum_avg=(n*sqrd_cum_avg + block_sum*block_sum)/(n+1); //calcola la media dei quadrati delle medie dei blocchi
        if (n==0){
            error=0;
        }else{
            
            
            
            error=sqrt((sqrd_cum_avg-cum_avg*cum_avg)/n); //calcola la deviazione standard cumulativa al blocco n
        }
        
        save_results << cum_avg<< ","  << sqrd_cum_avg<<  "," << error   << endl;
      
    }

    return 0; 
}