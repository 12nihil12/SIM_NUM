#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <exception>
#include "random.h"

#include "functions.h"


using namespace std; 



/*


Result Data_blocking(unsigned int  N_blocks, unsigned int N_steps,arma::vec & x,bool print_results, const  char * filename){



    if(N_blocks*N_steps!= x.n_elem){

        throw std::runtime_error("Dimensions don't match"); 

    }

    arma::mat data_mat (x.memptr(), N_steps, N_blocks,  false, false ); //organizza i dati in N_blocks colonne e N_steps righe, senza copiarli


    arma::vec blocks = arma::linspace<arma::vec>(1, N_blocks, N_blocks);//progressivo dei blocchi (1,2,..., N_blocks)



    arma::vec block_avg=arma::trans(arma::mean(x,1)); //per ogni blocco (colonna), restituisce la media. 
    // Siccome il risultato è un vettore riga, lo traspone


    arma::vec cum_avg=arma::cumsum(block_avg)/blocks; //media cumulativa: 
    //per ogni blocco n, calcola la somma cumulativa delle medie dei blocchi fino ad esso
    //e poi divide per il numero progressivo di blocchi, per ottenere la media globale al blocco n
    

    arma::vec cum_avg_sqrd=arma::cumsum(arma::square(block_avg))/blocks; //media cumulativa dei quadrati: 
    //per ogni blocco n, calcola la somma cumulativa dei quadrati delle medie dei blocchi fino ad esso
    //e poi divide per il numero progressivo di blocchi, per ottenere la media dei quadrati globale al blocco n


    arma::vec error=arma::sqrt((cum_avg_sqrd-arma::square(cum_avg))/(blocks-1)); 

    error(0)=0;//l'errore con un blocco solo è reso Inf dalla divisione per 0, lo si pone 0 per definizione



    if(print_results){

        ofstream print; 
        print.open(filename); 
        print << "Block" << setw(12) << "Block_avg" << setw(12) << "Cumulative_avg" << setw(12) << "Error" << endl; 

		for(int n=0; n < N_blocks; n++){
        	print << n << setw(12) << block_avg(n) << setw(12) << cum_avg(n) << setw(12) << error(n) <<"\n"; 
			
		}

    }


    
	Result result; 

	result.value=cum_avg.back(); //assegna al risultato la media globale all'ultimo blocco
	result.error=error.back(); //assegna al risultato l'errore  all'ultimo blocco



	return result;

}


*/


bool Metro_accept(double p,Random * rnd){

    if (p>1){
        return true ; // la mossa viene accettata con certezza
    }else if (p>0 ) { //controllo di sicurezza
        double num=rnd->Rannyu(); 
        if (num < p){ //evento che ha probabilità p di avverarsi
            return true; //la mossa viene accettata con probabilità p
        }else{
            return false; //la mossa viene rifiutata con probabilità p
        }
    }
}

Result Data_blocking(unsigned int  N_blocks, unsigned int N_steps,arma::vec & x,bool print_results, const  char * filename){



    ofstream print;

    if(print_results){

        print.open(filename); 

        if(print.is_open()){
            print << "Block" << setw(12) << "Block_avg" << setw(12) << "Cumulative_avg" << setw(12) << "Error" << endl; 

        }
    
        
    }


    if(N_blocks*N_steps!= x.n_elem){

        throw std::runtime_error("Dimensions don't match"); 

    }


    double block_avg; 
    double cum_sum=0; 
    double cum_sum_sqrd=0; 

    double error; 



    for(int n=0; n< N_blocks; n++){
        block_avg=0; 

        for (int i=0; i < N_steps; i++){
            block_avg+=x(n*N_steps +i); //aggiunge il valore n*i esimo

        }

        block_avg/=N_steps; //divide la somma per il numero di step per calcolare la media
        cum_sum+=block_avg;//la aggiunge alla somma cumulativa
        cum_sum_sqrd+=block_avg*block_avg; //aggiunge il quadrato alla somma cumulativa dei quadrati


        if(n==0){
            error=0; //non si può calcolare l'errore, lo mette a zero per definizione
        }else{

            error=sqrt((cum_sum_sqrd/(n+1) -pow(cum_sum/(n+1),2))/n);
        }


        if(print_results && print.is_open()){
            print << n << setw(12) << block_avg<< setw(12) << cum_sum/(n+1) << setw(12) << error <<"\n"; 

        }
    }

    
	Result result; 

	result.value=cum_sum/N_blocks; //assegna al risultato la media globale all'ultimo blocco
	result.error=error; //assegna al risultato l'errore  all'ultimo blocco


    if(print_results){print.close();}

	return result;

}
