#ifndef ___functions___
#define ___functions___

#include <armadillo>


class Random; 



struct Result{
   double value;
   double error; 
};


struct Metro{
   double inner;
   double outer;
};


struct NumberOf{
    unsigned int blocks;
    unsigned int steps;
    unsigned int eq_steps;
}; 




bool Metro_accept(double p, Random * rnd); 


Result Data_blocking(unsigned N_blocks, unsigned N_steps,arma::vec & x,bool print_results=false, const  char * filename="results.txt");



#endif // ___functions___