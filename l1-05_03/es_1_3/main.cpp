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


    int M=100; //sotto-intervalli
    int n=10000; //quantità di numeri random estratta per test
    int R=100;  //numero test 

    double squares_sum=0; 
    int index=0;
    int counter[M]; //contatore per i sottointervalli

    //setta il contatore a 0
   

    ofstream save_results; 
    save_results.open("results.csv");
    save_results << "chi_squared"    << endl;
   

    if (!save_results.is_open()){
        return 0; 
    }

    for (int r=0;r<R;r++){
        squares_sum=0;
        for (int m=0; m <M; m++){
            counter[m]=0; 
        }
        for(int i=0;i<n;i++){
            if (i%1000==0){
            }
            index=int(rnd.Rannyu()*M);
            counter[index]++; 
        }
        for(int m=0;m<M;m++){
            squares_sum+=pow(counter[m]-n/M,2);
            
            
        }
        save_results << squares_sum/(n/M) << endl; 

        
    }
    

    return 0; 
}