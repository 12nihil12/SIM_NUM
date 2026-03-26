#include <iostream>
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
//      if(j%50 == 0){
        //SYS.write_XYZ(nconf); //Write actual configuration in XYZ format //Commented to avoid "filesystem full"! 
        //nconf++;
//      }
    }
    SYS.averages(i+1); //calcola le medie cumulative al blocco i+1 e le stampa su file 
    SYS.block_reset(i+1); //resetta le variabili di servizio del blocco
  }
  SYS.finalize();

  return 0;
}