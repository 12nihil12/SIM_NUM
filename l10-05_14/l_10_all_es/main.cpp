#include <iostream> 
#include <string>
#include <fstream>
#include <limits>
#include "mpi.h"


#include "random.h"
#include "globe.h"
#include "path.h"
#include "population.h"

constexpr const char * RAND_GEN_PATH= "../rand_gen/";


using namespace std; 
using namespace arma;

int main(int argc, char *argv[]){

    //SETUP MPI

    int size, rank;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /*******************************************************************************************/


    //SETUP GENERATORE
    Random rnd;
    int seed[4];
    int p1, p2;
    ifstream Primes(string(RAND_GEN_PATH) +"Primes");


    if (Primes.is_open()){
        for(int i=0; i <= rank; i++){
            Primes >> p1 >> p2 ;
        }
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




 
    /*******************************************************************************************/


    //PARAMETRI

    
 
    double Px=0.7; //crossover
    double Pm=0.05; //mutazione
    double p=2;//selezione
   
    int pop_size,N_gen,N_migr; 
    bool migrations_allowed; 

    //rank 0 carica i parametri
    if(rank==0){

        ifstream filein; 
        filein.open("settings.in");

        if(!filein.is_open() or filein.eof()){
            cout << "Input file not found: using default parameters: " << endl; 

            cout << " Population_size = 1000 \n Number_of_generations = 1000 \n Migrations_are_allowed = 1 \n Generations_to_migration = 100" << endl; 


            pop_size=1000; 
            N_gen=1000; 
            migrations_allowed=true; 
            N_migr=100; 

        }

        
        filein >> property; 
        
        while(!filein.eof()){

            
            if      (property == "Population_size")          filein >> pop_size;
            else if (property == "Number_of_generations")    filein >> N_gen;
            else if (property == "Migrations_are_allowed")   filein >> migrations_allowed;
            else if (property == "Generations_to_migration") filein >> N_migr;
            else if (property=="END") break; 

            filein.ignore(numeric_limits<streamsize>::max(), '\n'); //salta i commenti

            filein>>property; 

        }
    


        if(N_migr<=0,N_gen<=0,pop_size<=0){
            cout << "Invalid parameters, using default: " << endl; 

            cout << " Population_size = 1000 \n Number_of_generations = 1000 \n Migrations_are_allowed = 1 \n Generations_to_migration = 100" << endl; 

            pop_size=1000; 
            N_gen=1000; 
            migrations_allowed=true; 
            N_migr=100; 
        }


    }


    //e li manda a tutti gli altri

    MPI_Bcast(&pop_size,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&N_gen,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&migrations_allowed,1,MPI_C_BOOL,0,MPI_COMM_WORLD);
    MPI_Bcast(&N_migr,1,MPI_INT,0,MPI_COMM_WORLD);


    /*******************************************************************************************/


    //Output

    ofstream fout; 
    string outfile = "OUTPUT/rank_" +to_string(rank) + "_pop.csv"; 

    fout.open(outfile);

    fout <<setw(6) << "Gen" << setw(12) << "Avg_len" << setw(12) << "Best_len" << "     "<< "Best_path" << endl; 


    /*******************************************************************************************/

  
    //Mappa e popolazione

    
    string position_file= "cap_prov_ita.dat";

    Globe globe(position_file.c_str()); 

    int N_cities=globe.Get_N_cities(); 


    int chromosome_size=N_cities+1; 
    globe.Calculate_distances();


    Population pop(pop_size,&globe,&rnd); 
    pop.Initialize(Pm,Px,p); 


    /*******************************************************************************************/

    //Comunicazioni

    MPI_Status status; 
    
    irowvec * send_best_chromosome;
    irowvec receive_best_chromosome (chromosome_size, arma::fill::none); 

    irowvec communication_order=linspace <irowvec>(0,size-1,size); 

    int sender,receiver; 
  



    /*******************************************************************************************/

    for(int i=0; i < N_gen; i++){

        pop.Order(); //Ordina la generazione corrente

        if(i%N_migr==0){//migrazione
          
            
            //prepara un riferimento al miglior individuo per essere spedito
            Path best_path(pop.Get_individual(0));
            send_best_chromosome=best_path.Get_chromosome(); 

        
         

            if(size>=2 and migrations_allowed){

                communication_order=shuffle(communication_order); //utilizza un ordine diverso ad ogni giro

                
                for(int j=0; j<size; j++){
                    if(rank==communication_order(j)){ 
                        sender=communication_order((j-1 + size)%size); //seleziona il mittente per cui rank è ricevitore
                        receiver=communication_order((j+1)%size); //seleziona il recivitore per cui rank è mittente
                        break; 
                    }
                }
                

                //scambia coi vicini il miglior individuo
                MPI_Sendrecv(
                    send_best_chromosome->memptr(), chromosome_size, MPI_LONG_LONG, receiver, rank, 
                    receive_best_chromosome.memptr(),chromosome_size,MPI_LONG_LONG,sender,sender,
                    MPI_COMM_WORLD,&status
                );


                (*best_path.Get_chromosome())=receive_best_chromosome;
                pop.Set_individual(0,best_path); //sostituisce il suo miglior individuo con quello ricevuto



            }

         
        }   

        
        
        //stampa i risultati: generazione, lunghezza media della metà migliore della popolazione, lunghezza del miglior percorso
        fout << setw(6)  <<  i << setw(12) << pop.Get_best_half_avg_len() << setw(12) << pop.Get_individual(0).Get_len()<<"     ";
        //miglior percorso
        pop.Get_individual(0).Get_chromosome()->raw_print(fout); 
       

        pop.New_generation(); //Crea la nuova generazione

   

    }



    fout.close(); 
    MPI_Finalize(); 


    


    return 0;


    

}


