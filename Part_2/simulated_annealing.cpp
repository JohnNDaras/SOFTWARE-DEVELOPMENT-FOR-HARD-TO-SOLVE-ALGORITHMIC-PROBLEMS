#include "utils_simulated_annealing.hpp"
#include "utils_con_hull.hpp"

Polygon_2 simulated_annealing(Points points,string &output_file,string algorithm,string edge_selection,int annealing, int &L, string &min_or_max)
{   
	srand(time(0));
    	Tree tree;
	double T = 1.0, energy, variable;	
	float R = (float) rand()/RAND_MAX;
	Point_2 q,s,p,r;
	Points points_to_check;
	Polygon_2 polygon;	
	cout<<R<<"\n\n"<<endl;
	polygon = con_hull(points,stoi( edge_selection));

    	energy = Initial_state_energy(polygon, points);

	switch (annealing) {
		case 1:
		{
			for(auto it = polygon.vertices_begin(); it!= polygon.vertices_end(); ++it)
				tree.insert(*it );
			local_transition(tree, polygon, points, q, r, points_to_check, L, energy, R, min_or_max);
			break;
		}
		case 2:
		{
			polygon = global_transition(polygon,points, q, s, p, r, points_to_check, L, T, energy, R);
			break;
		}
	}
	return polygon;
}
