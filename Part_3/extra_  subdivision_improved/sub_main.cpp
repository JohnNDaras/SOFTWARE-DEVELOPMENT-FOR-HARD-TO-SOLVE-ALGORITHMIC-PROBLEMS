//  BASED ON PROJECT 2

#include "utils_sub.hpp"
#include <string.h>

using namespace std;

//Polygon_2 subdivision(Points points , int m, int L  , string min_or_max);

void arguments(string &input_file, string &output_file, string &algorithm,int& L ,string & opt ,int& threshold,string & annealing,double& alpha , double& beta,double& ro ,bool& elitism ,int argc, char *argv[]);


int main(int argc, char *argv[])
{   
    // perform command line argument checking
    if(argc < 7){
        cerr << "Not enough arguments\n" << endl;
        return -1;
    }    


    string input_file, output_file, algorithm , opt , annealing  ;
    int L , threshold;
    double alpha , beta , ro;
    bool elitism; 
    arguments(input_file, output_file, algorithm, L , opt , threshold, annealing, alpha,beta ,ro ,elitism ,argc, argv);    


	Points points;

	///// READ FROM INPUT FILE /////   
	std::ifstream inFile;   
	std::ofstream outFile;
	inFile.open(input_file);
	std::string line;
	std::getline(inFile, line);
	std::getline(inFile, line);
	while (std::getline(inFile, line)){
		std::istringstream iss(line);
		int a,b,c;
		if (!(iss >> a >> b >> c)) { break; } // error  
		points.push_back(Point_2(b,c));		
	}
	inFile.close();

	Polygon_2 polygon;


subdivision(points,100 , L, opt);


	outFile.close();
	return 0;
}

void arguments(string &input_file, string &output_file, string &algorithm,int& L ,string & opt ,int &threshold,string & annealing,double& alpha , double& beta,double& ro ,bool& elitism ,int argc, char *argv[])
{
	for(int i = 1; i < argc - 1; i++){
			if(string(argv[i]) == "-i"){
				input_file = string(argv[i+1]) ;
			}
			if(string(argv[i]) == "-o"){
				output_file = string(argv[i+1]);
			}
			if(string(argv[i]) == "-algorithm"){
				algorithm = string(argv[i+1]);
			}
			if(string(argv[i]) == "-L"){
				L = stoi(string(argv[i+1]));
			}
			if(string(argv[i]) == "-max"){
				opt = "max";
			}
            else if(string(argv[i]) == "-min"){
				opt = "min";
			}
			if(string(argv[i]) == "-threshold"){
		        threshold = stoi(string(argv[i+1]));
			}
            if(string(argv[i]) == "-annealing"){
				annealing = string(argv[i+1]);
			}
            if(string(argv[i]) == "-alpha"){
				alpha = stod(string(argv[i+1]));
			}          
            if(string(argv[i]) == "-beta"){
				beta = stod(string(argv[i+1]));
			}          
            if(string(argv[i]) == "-ro"){
				ro = stod(string(argv[i+1]));
			}                    
            if(string(argv[i]) == "-elitism"){			
                elitism = stoi(string(argv[i+1]));
			}              
    }
}