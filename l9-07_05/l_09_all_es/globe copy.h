#ifndef ___Globe___
#define ___Globe___

#include <vector>
#include <armadillo>


class Random; 

struct City{
    double x; 
    double y; 
}; 



class Globe{

    private: 

        int _N_cities; 

        std::vector <City> _cities; 
        
        arma::mat _distances;



    public: 

        Globe(int N_cities); 
        void Fill(double R_max, Random * rnd); //Riempie la mappa


        int Get_N_cities(){return _N_cities;}
        City Get_city(int i){return _cities.at(i);}




        double Get_distance(int i, int j){return _distances(i,j);}; // returns the distance between two cities


        void Print_distances(){_distances.print();};


        double Get_path_len(const arma::irowvec & path); //returns the lenght of a path 


};






#endif  


