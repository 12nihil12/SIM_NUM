#include <iostream> 
#include <fstream>
#include <cmath>
#include "../rand_gen/random.h"

using namespace std; 


void step_on_grid(Random *rnd,double *pos_grid ){
    int direction=int(rnd->Rannyu(0,2));//sceglie una direzione (x,y,z) con probabilita' uniforme
    int step=int(rnd->Rannyu(0,2)); //sceglie il valore dello step (-1,1) con probabilita' uniforme
    pos_grid[direction]+=step; //esegue il passo

    return;

}

void step_in_continuos(Random* rnd,double * pos_cont){
    double theta=rnd->Rannyu(0,M_PI);//estrae theta con probabilita' uniforme
    double phi=rnd->Rannyu(0,2*M_PI);//estrae phi con probabilita' uniforme
    pos_cont[0]=sin(theta)*cos(phi);//aggiorna la posizione sull'asse x
    pos_cont[1]=sin(theta)*sin(phi);//aggiorna la posizione sull'asse y
    pos_cont[2]=cos(theta);//aggiorna la posizione sull'asse y

    return;

}

double square_module(double * position){
    return position[0]*position[0]+position[1]*position[1]+position[2]*position[2]; 
}

void set_zeros(double * a,int size){
    for(int i=0;i<size;i++){
        a[i]=0;
    }
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

    int M=10000; //numero di randomwalk
    int N=100; //numero di blocchi
    int L=M/N; //numero di lanci per blocco
    int R=100; //numero di step per random walk
    /*********************************************************************************/

    //gestione output
    ofstream save_results; 
    save_results.open("results.csv");
    if (!save_results.is_open()){
        cout << "Problemi con l'apertura del file di output" << endl; 
        return 0; 
    }
    save_results << "I (unif),error (unif),I(i.s.),I(i.s.)"  << endl;

    /**********************************************************************************/


    //variabili di servizio

    double pos_grid[3]; 
    double pos_cont[3]; 

    double  block_sum_1[100];//somma del blocco
    //double [100] block_sum_2;//somma del blocco

    double cum_sum_1[100]; //stima integrale
    //double cum_sum_2=0; //stima integrale

    double cum_squares_sum_1[100]; //somma dei quadrati, per stima varianza
    //double cum_squares_sum_2=0; //somma dei quadrati, per stima varianza

    double error_1; //deviazione standard
    double error_2; //deviazione standard

 

    /**********************************************************************************/

    set_zeros(cum_sum_1,100);
    set_zeros(cum_squares_sum_1,100);


    for (int n=0; n < N; n++){ //itera sui blocchi
        set_zeros(block_sum_1,100);
        for(int l=0; l < L; l++){ //itera sui lanci per blocco
            pos_grid[0]=pos_grid[1]=pos_grid[2]=0;
            pos_cont[0]=pos_cont[1]=pos_cont[2]=0;
            for(int i=0;i<R;i++){
                step_on_grid(&rnd,pos_grid); //esegue uno step sulla griglia
                step_in_continuos(&rnd,pos_cont);//esegue uno step nel continuo
                block_sum_1[i]+=square_module(pos_grid);
                //block_sum_2+=square_module(pos_cont);
                block_sum_1[i]=sqrt(block_sum_1[i]/L); 
                //block_sum_2=sqrt(block_sum_2 /L); 
        
            }
        
        }

        for(int i=0;i<R;i++){
            cum_sum_1[i]=(n*cum_sum_1[i] +block_sum_1[i])/(n+1);
            cum_squares_sum_1[i]=(n*cum_squares_sum_1[i]+block_sum_1[i]*block_sum_1[i])/(n+1); // calcola la media dei quadrati 

        }


      
        
        
    

    }
    for(int i=0; i<R; i++){

         


   
        error_1=sqrt((cum_squares_sum_1[i]-cum_sum_1[i]*cum_sum_1[i])/N); //calcola la deviazione standard cumulativa al blocco n
        //error_2=sqrt((cum_squares_sum_2[i]-cum_sum_2[i]*cum_sum_2[i])/n); //calcola la deviazione standard cumulativa al blocco n
        
    
        save_results << cum_squares_sum_1[i] << ","<< error_1 << endl;
    }
        

    return 0; 
}