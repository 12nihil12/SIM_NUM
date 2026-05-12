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

    
    if(argc!=2){
        cout<< "Uso del programma: " << argv[0] << " <0/1>" <<endl; 
        cout << "0 per città sulla circonferenza"<<endl; 
        cout << "1 per città nel quadrato"<<endl; 
        return 0; 
    }

    char which; 

    if(stoi(argv[1])==0){
        which='C'; 
    }else if(stoi(argv[1])==1){
        which='S'; 
    }else{
        cout<< "Uso del programma " << argv[0] << ": ./" << argv[0] << " <0/1>" <<endl; 
        cout << "0 per città sulla circonferenza"<<endl; 
        cout << "1 per città nel quadrato"<<endl; 
        return 0; 
    }


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

   int N_cities=34; 
   int pop_size=200; 
   int N_generations=1000; 

   double Pm=0.05; 
   double Px=0.7; 
   double sel_p=2.0;

    Globe globe(N_cities); 

    ofstream fout; 
    string filename1,filename2;
    
 



    if(which=='C'){
        globe.Fill_Circumference(1.0,&rnd);
        filename1="cities_on_circle.csv"; 
        filename2="circle_paths.csv"; 


    }else{
        globe.Fill_Square(1.0,&rnd); 
        filename1="cities_in_square.csv"; 
        filename2="square_paths.csv"; 



        
    }
    
    globe.Calculate_distances();

    fout.open(filename1); 
    fout << left << setw(15) << "x" << "y" <<endl; 
    globe.Print_Cities(fout);

    

    fout.close();

    fout.open(filename2);


    fout << left << "Gen     Best path" << endl; 

    Population pop(pop_size,&globe,&rnd); 

    pop.Initialize(Pm,Px,sel_p); 


    for(int i=0; i < N_generations; i++){

        pop.Order(); //Ordina la generazione corrente

        
        fout << i << "      "; //stampa la generazione
        pop.Get_individual(0).Printwlen(fout); //stampa il miglior percorso

        pop.Spawn(); //Crea la nuova generazione

     

    }








    

}


