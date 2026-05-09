#include "globe.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <exception>




#include "random.h"

using namespace std; 
using namespace arma;

Globe::Globe(int N_cities){

    if(N_cities <=0){
        throw runtime_error("Not a valid number of cities"); 
    }
 

    _N_cities=N_cities; 

    _cities.reserve(N_cities); 

    _distances.set_size(N_cities,N_cities);


    
}


void Globe::Fill(double R_max,Random *rnd){


    //variabili di servizio
    double r, theta;
    City city,city1;  
    double alpha; 

    //Genera N_cities città con coordinate random sul piano, fino a  distanza  R_max
    for (int i=0; i < _N_cities; i++){
        r=rnd->Rannyu()*R_max;
        theta=rnd->Rannyu()*M_PI; 
        city.r=r; 
        city.theta=theta; 
        _cities.push_back(city);
        
    }



    //Per ogni coppia di città, calcola la distanza tra esse

    //riempie la parte superiore della matrice


    cout << endl; 
    for(uword i=0; i < _distances.n_rows -1 ; i++){
        for(uword j=i+1; j <_distances.n_cols; j++){
            city=_cities.at(i); 
            city1=_cities.at(j);
        
            alpha=city.theta -city1.theta; //angolo compreso 
            _distances(i,j)=(sqrt(city.r*city.r +city1.r*city1.r - 2*city.r*city1.r*cos(alpha))); //teorema di Carnot
             
            
            

        }
    }
 
    _distances.diag().zeros();//gli elementi sulla diagonale sono nulli (distanza tra una città e sè stessa è zero)
  
    _distances+=_distances.t(); //riempie la parte inferiore con gli stessi elementi 

    return;
    
}





double Globe::Get_path_len(const irowvec & path){

    double lenght=0; 

    if(any( path < 0 || path >= _N_cities)){
        throw runtime_error("Path contains an invalid index");
    }



    for(int k=0; k< static_cast <int>( path.n_elem) -1; k++){
        lenght+=_distances(path(k),path(k+1));
    }


    return lenght;
}