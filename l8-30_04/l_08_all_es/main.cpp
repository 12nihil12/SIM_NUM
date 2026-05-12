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
    unsigned int N_steps=2000;//step per blocco 


    //Metropolis interno (per sampling funzione d'onda)

    unsigned int N_E=100; //step di burn in(per il metropolis interno)

    double delta_in=2.2;

    //Metropolis esterno (per ottimizzazione parametri di sistema) 

    //beta 

    double T_start=2.0; //temperatura iniziale
    double alpha=0.78; //coefficiente per progressione geometrica
    int B=50; //numero valori di beta considerati
    double beta=1/T_start; //beta iniziale
    
    //Step metropolis per ogni beta
    int R=20; //numero di step metropolis per beta_start
   
    //delta
    double delta_out=0.5;//delta per il metropolis principale

    /**********************************************************************************/


    //STAMPA RISULTATI

    //risultati principali (uno per beta )
    ofstream print_main;
    print_main.open("Results.csv");
    print_main << left  << setw(12) <<"j" << setw(12) << "beta" << setw(12) << "delta" << setw(12) << "acc_outer" << setw(12) << "avg_acc_inn"<< setw(12)  << "last_exp_H"<< setw(12)  << "error" << setw(12) << "last_sigma" << setw(12) <<  "last_mu " << endl; 

    //risultati intemedi (un file per ogni beta)
    ofstream print_side; 
    string filename; 
    

    ofstream print_best; //per i migliori parametri, stampa in dettaglio il calcolo dell'integrale e la distribuzione delle x 
    /**********************************************************************************/


    //VARIABILI DI SERVIZIO

    double accept_inner,accept_outer; //accettanza del metropolis interno (di servizio a Hamiltonian_exp_val)
    double sum_accept_inner; //accettanza media del metropolis interno
    int accepted_outer;//numero mosse accettate metropolis esterno
    double A; 


    //per variabili temporanee metropolis
    vec x,H_on_WF,x_save; //x e f(x) per valore medio hamiltoniana
    x_save.set_size(N_steps*N_blocks);
    x.set_size(N_steps*N_blocks); 
    H_on_WF.set_size(N_steps*N_blocks);

    double new_sigma,new_mu,save_sigma,save_mu; //parametri
    Result exp_H,new_exp_H; //valore medio hamiltoniana
   

    /**********************************************************************************/


    //PARAMETRI SISTEMA E INIZIALIZZAZIONE

    double mu_0=1.0; 
    double sigma_0=1.0;
    
    System sys(0.,sigma_0,mu_0); //Inizializza il sistema in x=0,sigma=sigma_0, mu=mu_0


    //sys.Metro_Sample(N_blocks*N_steps,x,&rnd,delta_in,N_E); //calcola un vettore di _N_blocks*_N_steps x distribuite secondo il modulo quadro di psi        
    //sys.Hamiltonian_on_WF(x,H_on_WF); //per ogni x calcola l'hamiltoniana applicata a psi(x) diviso per psi(x) e la salva in H_on_Wf 
    //exp_H=Data_blocking(N_blocks,N_steps,H_on_WF); //col data blocking, calola la media di H_on_Wf (e quindi l'integrale)


    /**********************************************************************************/
    
    
    //variabili di servizio, per salvataggio temporaneo
  
   


    for(int j=0; j < B; j++){

    
        //azzera i contatori
        accepted_outer=0; 
        sum_accept_inner=0; 


        //aggiusta i parametri
        if(j==15){delta_out=0.1;}
        if(j==20){alpha=0.85; R=50;}
        else if(j==25){delta_out=0.01;} 
        else if(j==40){alpha=0.9;} //delta_out=0.001;}   

        //Per ogni beta,stampa i risultati per ogni step di evoluzione parametri con metropolis
        filename="OUTPUT/" +to_string(j) + ".csv" ; //crea il file in cui salvare i risultati
        print_side.open(filename); 
        if(print_side.is_open()){
            print_side << "sigma" <<"," << "mu" <<"," << "exp_H" <<"," << "error" <<"," << "acceptance" << endl; 

        }

        for(int r=0; r < R; r++){
            
            //ricalcola l'integrale coi parametri correnti(per impedire la propagazione di valori "fortunati" che si trovano a sinistra del valor medio)

            sys.Metro_Sample(N_blocks*N_steps,x,&rnd,delta_in,N_E); //calcola un vettore di _N_blocks*_N_steps x distribuite secondo il modulo quadro di psi
            sys.Hamiltonian_on_WF(x,H_on_WF); //per ogni x calcola l'hamiltoniana applicata a psi(x) diviso per psi(x) e la salva in H_on_Wf 
            exp_H=Data_blocking(N_blocks,N_steps,H_on_WF); //col data blocking, calola la media di H_on_Wf (e quindi l'integrale)         

        
            //PROPOSTA DI MOSSA

            save_sigma=sys.Get_sigma(); save_mu=sys.Get_mu(); //salva i parametri attuali 

            do{ 
                //calcola dei nuovi parametri
                new_sigma=save_sigma +delta_out*rnd.Rannyu(-1,1);
                new_mu=save_mu +delta_out*rnd.Rannyu(-1,1);

            }while(new_sigma <0 || new_mu < 0) ;//i parametri possono essere solo positivi (simmetria):

            sys.Set_pams(new_sigma,new_mu);//modifica i parametri (proposta di mossa)

            
            
            //VALUTAZIONE DELL'ACCETTAZIONE 


            accept_inner=sys.Metro_Sample(N_blocks*N_steps,x,&rnd,delta_in,N_E); //calcola un vettore di _N_blocks*_N_steps x distribuite secondo il modulo quadro di psi
            sum_accept_inner+=accept_inner; //somma l'accettanza del metropolis interno per calcolarne la media 
            
            sys.Hamiltonian_on_WF(x,H_on_WF); //per ogni x calcola l'hamiltoniana applicata a psi(x) diviso per psi(x) e la salva in H_on_Wf 
            new_exp_H=Data_blocking(N_blocks,N_steps,H_on_WF); //col data blocking, calola la media di H_on_Wf (e quindi l'integrale)


            A= exp(- beta* (new_exp_H.value-exp_H.value)); //calcola l'accettanza relativa alla mossa exp(-beta (E_new - E_old))


            if(A > 1 || rnd.Rannyu() < A ){ // se A >1 accetta con certezza, se A < 1 accetta con probabilità A 
                accepted_outer++; //aumenta il numero di mosse accettate
                exp_H=new_exp_H; //salva il nuovo valore dell'energia
                
                x_save=x;//salva la configurazione di x 
            }else{
                sys.Set_pams(save_sigma,save_mu);//se la mossa non è stata accettata, rimette i parametri com'erano prima
        
            }

            //salva i risultati intermedi
            if(print_side.is_open()){
                print_side << sys.Get_sigma() <<","<< sys.Get_mu() << ","<< exp_H.value <<","<< exp_H.error <<","<< accept_inner <<"\n"; 

            }
        
        }

    

        if(print_main.is_open()){
            print_main << left  << setw(12) << j << setw(12) << beta << setw(12) << delta_out << setw(12) << double(accepted_outer)/R <<  setw(12) << sum_accept_inner/R <<  setw(12) <<  exp_H.value << setw(12) << exp_H.error <<   setw(12) << sys.Get_sigma() <<  setw(12) << sys.Get_mu() << endl ; 

        }


        beta/=alpha; //progressione geometrica beta

        print_side.close();

    }



    /**********************************************************************************/



    print_main.close();


    //per gli ultimi valori di sigma e mu, stampa nel dettaglio

            
    
    //calcola e stampa la distribuzione delle x secondo il modulo quadro della funzione d'onda

    print_best.open("x_dist.dat"); 
    x_save.print(print_best); 
    print_best.close(); 
    

    

    //calcola l'integrale ,stampando il dettaglio
    sys.Metro_Sample(N_blocks*N_steps,x,&rnd,delta_in,N_E); //calcola un vettore di _N_blocks*_N_steps x distribuite secondo il modulo quadro di psi

    sys.Hamiltonian_on_WF(x,H_on_WF); 
    Data_blocking(N_blocks,N_steps,H_on_WF,true,"integral.dat"); 


 
  
    return 0; 
}


