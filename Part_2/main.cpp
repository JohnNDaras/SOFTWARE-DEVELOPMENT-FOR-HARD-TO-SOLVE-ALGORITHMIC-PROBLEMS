#include "utils_simulated_annealing.hpp"

using namespace std;

Polygon_2 aco(Points points, std::string opt, int L, double alpha, double beta, double ro, bool elitism);
Polygon_2 subdivision(Points points , int m);
Polygon_2 local(Points points ,std::string  polygonization , int L ,int threshold);

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

	///// CONSTRUCT THE POLYGON /////
	Polygon_2 polygon;

	int annealing_code;
    if(annealing=="local") annealing_code=1;
    else if(annealing=="global") annealing_code=2;



	auto start = chrono::high_resolution_clock::now(); // start timer
	if (algorithm == "local_search")
	    polygon = local(points ,opt, L, threshold);
	else if (algorithm == "simulated_annealing"){
        if(annealing=="subdivision") std::cout<<"subdivision";
        else
		polygon = simulated_annealing(points, output_file, "cpnvex_hull", "1",annealing_code, L, opt);
    }        
	else if (algorithm == "ant_colony")
		polygon = aco(points, opt, L, alpha, beta, ro, elitism);
	else {cerr << "Invalid algorithm name\n"; return -1;}

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start); // save construction time

	///// WRITE TO OUTPUT FILE /////
	outFile.open(output_file);
	outFile<<"Polygonization\n";

    for(auto it = polygon.vertices_begin(); it!= polygon.vertices_end(); ++it)
        outFile << *it << std::endl;
  
    for(auto it = polygon.edges_begin(); it!= polygon.edges_end(); ++it)
        outFile << *it << std::endl;  

	outFile<<"\nAlgorithm: "<< algorithm << "   "<<opt;
	

	Polygon_2 convex_hull;
	CGAL::convex_hull_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(convex_hull));
    long int area = abs(polygon.area()), ch_area = abs(convex_hull.area());
	outFile << "\nArea: " << abs(polygon.area()) << endl;
    outFile << "Ratio: " << (double)area/ch_area << endl;
    outFile << "Construction time: " << duration.count() << endl;

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
