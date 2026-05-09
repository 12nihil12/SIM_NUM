#ifndef ___Globe___
#define ___Globe___

#include <vector>
#include <armadillo>
#include <iostream>
#include <iomanip>


class Random; 

struct City{

    //coordinate
    double x; 
    double y; 

    //overloading operatore di stampa << 
    friend std::ostream& operator<<(std::ostream& sout, const City& city) { 
        sout  << city.x << city.y;
        return sout;
    }
}; 



class Globe{

    private: 

        int _N_cities; //Numero di città

        std::vector <City> _cities; //Vettore contenente le città
        
        arma::mat _distances; //Matrice il cui ingresso i,j è la distanza tra le città in i-esima e j-esima posizione in _cities



    public: 


        //costruttore
        Globe(int N_cities); 


        //accesso e stampa data membri
        int Get_N_cities()const{return _N_cities;}
        City Get_city(int i)const {return _cities.at(i);}
        void Print_distances()const{_distances.print();};
        void Print_Cities()const{for(const auto & city: _cities){std::cout << std::left << std::setw(15) << city << std::endl; };};
        void Print_Cities(std::ostream & fout)const {for(const auto & city: _cities){fout << std::left << std::setw(15) << city << std::endl; };};


        //Riempimento mappa
        void Fill_Circumference(double R, Random * rnd); 
        void Fill_Square(double L,Random *rnd);


        //Distanze
        void Calculate_distances();  //Per ogni coppia di città, calcola la distanza tra esse
        double Get_distance(int i, int j)const {return _distances(i,j);}; // returns the distance between two cities
        double Get_path_len(const arma::irowvec & path) const ; //returns the lenght of a path 


};








#endif  


