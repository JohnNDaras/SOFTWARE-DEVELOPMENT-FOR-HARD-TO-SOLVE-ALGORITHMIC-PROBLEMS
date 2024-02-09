#include "utils_simulated_annealing.hpp"
#include <dirent.h>

using namespace std;

double aco(Points points, string opt, int L, double alpha, double beta, double ro, bool elitism);
Polygon_2 subdivision(Points points , int m);
double local(Points points ,string  minormax , string  polygonization , int L ,int threshold );
void write_scores(vector<string> input_files, string output_file, string algorithm, string init);


map<int,vector<string>> classify_points(const string &dir){
    map<int,vector<string>> files_by_points;
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return files_by_points;
    }
    while ((dirp = readdir(dp)) != NULL) {
        string file_name = string(dirp->d_name);
        if(file_name == "." || file_name == "..") continue;
        ifstream inFile(dir+"/"+file_name);
        string line;
        getline(inFile, line);
        getline(inFile, line);
        int points_count = 0;
        while (getline(inFile, line)) {
            istringstream iss(line);
            int a,b,c;
            if (!(iss >> a >> b >> c)) { break; } // error  
            points_count++;
        }
        inFile.close();
        files_by_points[points_count].push_back(dir+"/"+file_name);
    }
    closedir(dp);
    return files_by_points;
}



int main(int argc, char *argv[])
{   
    // perform command line argument checking
    if(argc < 5){
        cerr << "Not enough arguments\n" << endl;
        return -1;
    }
	// get arguments 
	string input_path, output_file;
	for(int i = 1; i < argc - 1; i++){
			if(string(argv[i]) == "-i"){
				input_path = string(argv[i+1]) ;
			}
			if(string(argv[i]) == "-o"){
				output_file = string(argv[i+1]);
			}
	}
	ofstream outFile;
	outFile.open(output_file); // open and clear contents
	outFile.close();

	vector<string> algorithms = {"local_search", "simulated_annealing"};
    vector<string> inits = {"incremental", "convex_hull", "onion"};
	auto points_by_file = classify_points(input_path);

	// For every file of a specific size, run all combinations of algorithms
    for (const auto &p : points_by_file){
		outFile.open(output_file, ios::app); // open in append mode
		outFile << "Size " << p.first << ":\n\n";
		outFile.close();
		for (string algorithm : algorithms) {
        	for (string init : inits) {
				write_scores(p.second, output_file, algorithm, init);
			}
		}
		write_scores(p.second, output_file, "ant_colony", "no init");
    }
    
	return 0;
}


void write_scores (vector<string> input_files, string output_file, string algorithm, string init){

	double score, min_score = 0, max_score = 0, min_bound = 0, max_bound = 1;
	for (auto input_file : input_files){
		Points points;

		///// READ FROM INPUT FILE /////   
		ifstream inFile;   
		ofstream outFile;
		inFile.open(input_file);
		string line;
		getline(inFile, line);
		getline(inFile, line);
		while (getline(inFile, line)){
			istringstream iss(line);
			int a,b,c;
			if (!(iss >> a >> b >> c)) { break; } // error  
			points.push_back(Point_2(b,c));		
		}
		inFile.close();

		
		if (algorithm == "local_search") {
			score = local(points, "min", init, 10, 20000);
			if (score >= min_bound)
				min_bound = score;
			min_score += score;
			score = local(points, "max", init, 10, 20000);
			if (score <= max_bound)
				max_bound = score;
			max_score += score;
		} 
		else if (algorithm == "simulated_annealing"){
			score = simulated_annealing(points, init, "1", 1, 10, "min");
			if (score >= min_bound)
				min_bound = score;
			min_score += score;
			score = simulated_annealing(points, init, "1", 1, 10, "max");
			if (score <= max_bound)
				max_bound = score;
			max_score += score;
		}
		else{
			score = aco(points, "min", 5, 1, 1, 0.5, false);
			if (score >= min_bound)
				min_bound = score;
			min_score += score;
			score = aco(points, "max", 5, 1, 1, 0.5, false);
			if (score <= max_bound)
				max_bound = score;
			max_score += score;		
		}
	}

	ofstream outFile;
	outFile.open(output_file, ios::app);
	outFile << "\nAlgorithm: " << algorithm<< " + " << init << " :" << endl;
    outFile << "Min score: " << min_score << endl;
    outFile << "Max score: " << max_score << endl;
	outFile << "Min bound: " << min_bound << endl;
	outFile << "Max bound: " << max_bound << "\n\n\n";
    outFile.close();
}