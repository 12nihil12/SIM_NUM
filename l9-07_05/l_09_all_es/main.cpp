#include <iostream> 
#include <string>
#include <fstream>


#include "random.h"
#include "globe.h"
#include "path.h"
#include "population.h"

constexpr const char * RAND_GEN_PATH= "../rand_gen/";


using namespace std; 
using namespace arma;
int main(int argc, char *argv[]){





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



/*******************************************************************************************/



//parametri 


   double Pm=0.05; 
   double Px=0.7; 
   double sel_p=2.0;



    int N_cities,pop_size,N_gen; 
    bool cities_on_circle; 


    ifstream filein; 
    filein.open("settings.in");

    if(!filein.is_open() or filein.eof()){
        cout << "Input file not found: using default parameters: " << endl; 

        cout << "Number_of_cities=34 \n Population_size = 100 \n Number_of_generations = 1000 \n Cities_on_circle = 1" << endl; 


        N_cities=34;
        pop_size=100; 
        N_gen=1000; 
        cities_on_circle=true; 


    }

    
    filein >> property; 
    
    while(!filein.eof()){

        
        if      (property == "Population_size")          filein >> pop_size;
        else if (property == "Number_of_cities")         filein >> N_cities;
        else if (property == "Number_of_generations")    filein >> N_gen;
        else if (property == "Cities_on_circle")         filein >> cities_on_circle;
        else if (property=="END") break; 

        filein.ignore(numeric_limits<streamsize>::max(), '\n'); //salta i commenti

        filein>>property; 

    }



    if(N_gen<=0,pop_size<=0){
        cout << "Invalid parameters, using default: " << endl; 

        cout << "Number_of_cities=34 \n Population_size = 100 \n Number_of_generations = 1000 \n Cities_on_circle = 1" << endl; 
        
        N_cities=34;
        pop_size=100; 
        N_gen=1000; 
        cities_on_circle=true; 

    }



    /*******************************************************************************************/


    //Output

    ofstream fout; 
    string cities_file,paths_file;
    

    /*******************************************************************************************/

    
    //Mappa e popolazione


    Globe globe(N_cities); 


    if(cities_on_circle){
        globe.Fill_Circumference(1.0,&rnd);
        cities_file="cities_on_circle.csv"; 
        paths_file="circle_paths.csv"; 


    }else{
        globe.Fill_Square(1.0,&rnd); 
        cities_file="cities_in_square.csv"; 
        paths_file="square_paths.csv"; 



        
    }
    
    globe.Calculate_distances();

    fout.open(cities_file); 
    globe.Print_Cities(fout);
    fout.close();

    fout.open(paths_file);

    fout <<setw(6) << "Gen" << setw(12) << "Avg_len" << setw(12) << "Best_len" << "     "<< "Best_path" << endl; 



    /*******************************************************************************************/


    //Popolazione

    Population pop(pop_size,&globe,&rnd); 

    pop.Initialize(Pm,Px,sel_p); 


    /*******************************************************************************************/



    
    for(int i=0; i < N_gen; i++){

        pop.Order(); //Ordina la generazione corrente

        
        //stampa i risultati: generazione, lunghezza media della metà migliore della popolazione, lunghezza del miglior percorso
        fout << setw(6)  <<  i << setw(12) << pop.Get_best_half_avg_len() << setw(12) << pop.Get_individual(0).Get_len()<<"     ";
        //miglior percorso
        pop.Get_individual(0).Get_chromosome()->raw_print(fout); 
       
        pop.New_generation(); //Crea la nuova generazione

     

    }








    

}


