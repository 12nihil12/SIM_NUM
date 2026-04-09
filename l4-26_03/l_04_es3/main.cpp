#include <iostream>
#include <filesystem> 
#include "system.h"

using namespace std;

int main (int argc, char *argv[]){

  int nconf = 1;
  System SYS;
  SYS.initialize();



  SYS.initialize_properties();
  SYS.block_reset(0);

  for(int i=0; i < SYS.get_nbl(); i++){ //loop over blocks

    for(int j=0; j < SYS.get_nsteps(); j++){ //loop over steps in a block

      SYS.step();
      SYS.measure();
    }
   
    SYS.averages(i+1); //calcola le medie cumulative al blocco i+1 e le stampa su file 

    SYS.block_reset(i+1); //resetta le variabili di servizio del blocco
  
  }

  //rinomina i file di temperatura ed energia potenziale, di modo che non vengano sovrascritti


  SYS.finalize();

  /*
  
  filesystem::path source = string(OUTPUT_DIR) +"temperature.dat";
  filesystem::path destination = string(OUTPUT_DIR) +"temperature_for.dat";
  filesystem::path source1 = string(OUTPUT_DIR) +"potential_energy.dat";
  filesystem::path destination1 = string(OUTPUT_DIR) + "potential_energy_for.dat";

  
  filesystem::rename(source, destination);  
  filesystem::rename(source1, destination1);  



  ofstream write_new_input;

  write_new_input.open(string(INPUT_DIR) +"input.dat");

  write_new_input << "SIMULATION_TYPE        0 \n"
                  << "RESTART                1\n"
                  << "TEMP                   2.0\n"
                  << "NPART                  108\n"
                  << "RHO                    0.05\n"
                  << "R_CUT                  5.0\n"
                  << "DELTA                  0.002\n"
                  << "NBLOCKS                50\n"
                  << "NSTEPS                 100\n"
                  << "\n"
                  << "ENDINPUT\n";




  write_new_input.close()
  SYS.initialize();



  SYS.initialize_properties();
  SYS.block_reset(0);

  for(int i=0; i < SYS.get_nbl(); i++){ //loop over blocks

    for(int j=0; j < SYS.get_nsteps(); j++){ //loop over steps in a block

      SYS.step();
      SYS.measure();
    }
   
    SYS.averages(i+1); //calcola le medie cumulative al blocco i+1 e le stampa su file 

    SYS.block_reset(i+1); //resetta le variabili di servizio del blocco
  
  }

  //rinomina i file di temperatura ed energia potenziale, di modo che non vengano sovrascritti


  SYS.finalize();
  

  */
  return 0;
}