#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/convex_hull_2.h>
//#include "onion.hpp"
//#include "utils_con_hull.hpp"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef CGAL::Polygon_2<K>  Polygon_2;
typedef Polygon_2::Edge_const_iterator EdgeIterator;
typedef std::vector<Point_2> Points;

using namespace std;

Polygon_2 incremental(Points points, std::string initialization, std::string edge_selection);
Polygon_2 con_hull(Points points, int edge_selection);
Polygon_2 onion(Points points,int onion_initialization );

void arguments(string &input_file, string &output_file, string &algorithm, string &edge_selection, string &initialization, string &onion_initialization, int argc, char *argv[]);
long int Pick_area(Polygon_2 p);


int main(int argc, char *argv[])
{   
    // perform command line argument checking
    if(argc < 9){
        cerr << "Not enough arguments\n" << endl;
        return -1;
    }    
    string input_file, output_file, algorithm, edge_selection, initialization, onion_initialization;
    arguments(input_file, output_file, algorithm, edge_selection, initialization, onion_initialization, argc, argv);
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

	auto start = chrono::high_resolution_clock::now();	
	if (algorithm == "incremental")
		polygon = incremental(points, initialization, edge_selection);
	else if (algorithm == "convex_hull")
		polygon = con_hull(points,stoi( edge_selection));
	else if (algorithm == "onion")
		polygon = onion(points,stoi(onion_initialization));
	
	else {cerr << "Invalid algorithm name\n"; return -1;}
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

	///// WRITE TO OUTPUT FILE /////
	outFile.open(output_file);
	outFile<<"Polygonization\n";

    for(auto it = polygon.vertices_begin(); it!= polygon.vertices_end(); ++it)
        outFile << *it << std::endl;
  
    for(auto it = polygon.edges_begin(); it!= polygon.edges_end(); ++it)
        outFile << *it << std::endl;  

	outFile<<"\nAlgorithm: "<< algorithm << "   ";
    if(!edge_selection.empty()) outFile << "edge_selection: " << edge_selection << "   ";
    if(!initialization.empty()) outFile << "initialization: " << initialization << "   ";
	if(!onion_initialization.empty()) outFile << "onion_initialization: " << onion_initialization;

	Polygon_2 convex_hull;
	CGAL::convex_hull_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(convex_hull));
    long int area = abs(polygon.area()), ch_area = abs(convex_hull.area());
	outFile << "\nArea: " << abs(polygon.area()) << endl;
    outFile << "Ratio: " << (double)area/ch_area << endl;
    outFile << "Construction time: " << duration.count() << endl;
//!!!!!!!!    gia Pick ekteleste th parakato entoli 	
//	outFile << "Pick_calculated_area: " <<  Pick_area(polygon) << endl;
	outFile.close();

	return 0;
}

void arguments(string &input_file, string &output_file, string &algorithm, string &edge_selection, string &initialization, string &onion_initialization, int argc, char *argv[])
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
			if(string(argv[i]) == "-edge_selection"){
				edge_selection = string(argv[i+1]);
			}
			if(string(argv[i]) == "-initialization"){
				initialization = string(argv[i+1]);
			}
			if(string(argv[i]) == "-onion_initialization"){
				onion_initialization = string(argv[i+1]);
			}          
        }
}

long int Pick_area(Polygon_2 p){   // 2nd assignment
    int b=0, in=0;
    
    int left=p.left_vertex()->x();
    int right=p.right_vertex()->x();
    int top=p.top_vertex()->y();
    int bottom=p.bottom_vertex()->y();

    for(int y=top;y>=bottom;y--){
        for(int x=left;x<=right;x++){
            if( p.has_on_boundary(Point_2(x,y))) b++;
            else if( p.has_on_bounded_side( Point_2(x,y))) in++; 
        }
    }
    return in + b/2 -1;
}

