#include "Project1-1900082-1800040-1600168/utils_con_hull.hpp"
#include "Project1-1900082-1800040-1600168/utils_inc.hpp"
#include "Project1-1900082-1800040-1600168/utils_onion.hpp"
#include "utils_simulated_annealing.hpp"

double simulated_annealing(Points points,string algorithm,string edge_selection,int annealing, int L, string min_or_max)
{   
	srand(time(0));
    Tree tree;
	double T = 1.0, energy, variable;	
	float R = (float) rand()/RAND_MAX;
	Point_2 q,s,p,r;
	Points points_to_check;
	Polygon_2 polygon;
    double score = 0.0;
	cout<<"R = "<<R<<"\n\n"<<endl;
	if (algorithm == "onion")
		polygon=onion(points,1);

	else if(algorithm =="con_hull") {
		if(min_or_max=="max")
			polygon=con_hull(points,2);
    	else
        	polygon=con_hull(points,1);

	}
	else{     // incremental
		if(min_or_max=="max")
			polygon = incremental(points, "1a", "3"); 
		else
			polygon = incremental(points, "1a", "2");
	}

    energy = Initial_state_energy(polygon, points);
	switch (annealing) {
		case 1:
		{
			for(auto it = polygon.vertices_begin(); it!= polygon.vertices_end(); ++it)
				tree.insert(*it );
			score = local_transition(tree, polygon, points, q, r, points_to_check, L, energy, R, min_or_max);
			break;
		}
		case 2:
		{
			score = global_transition(polygon,points, q, s, p, r, points_to_check, L, T, energy, R, min_or_max);
			break;
		}
	}
	return score;
}
