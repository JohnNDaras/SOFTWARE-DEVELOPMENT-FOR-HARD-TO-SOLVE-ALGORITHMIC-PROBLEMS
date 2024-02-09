#include "utils_simulated_annealing.hpp"
#include "utils_con_hull.hpp"

Polygon_2 local_transition(Tree &tree, Polygon_2 &polygon, Points &points, Point_2 &q, Point_2 &r, Points &points_to_check, int &L,  double &energy, float &R, string min_or_max)
{	
	int polygonsize = (int) polygon.size();
	double T=1.0;
	Polygon_2 convex_hull, final_polygon;	
	CGAL::convex_hull_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(convex_hull));
	final_polygon.insert(final_polygon.end(), polygon.begin(), polygon.end());

	while(T>0)
	{
		point_selection:
		int position_of_q = rand() % (polygonsize-3); 			
		q = *(polygon.vertices_begin()+position_of_q );
		r = *(polygon.vertices_begin()+position_of_q + 1);
		polygon.insert(polygon.vertices_begin()+position_of_q ,r);    
		polygon.erase(polygon.vertices_begin()+position_of_q +2); 
		Point_2 p = *(polygon.vertices_begin()+position_of_q -1);
		Point_2 s = *(polygon.vertices_begin()+position_of_q + 2);
		
		Fuzzy_iso_box fib(q, r, 20000.0);
		Segment_2 seg1(p, r);
		Segment_2 seg2(q, s);

		tree.search( std::back_inserter( points_to_check ), fib);
		delete_vector(points_to_check , q);
		delete_vector(points_to_check , r);
		
		 for(int i=0; i< points_to_check.size() ; i++)
		{
			int ind =  index(polygon , points_to_check[i]);
			Segment_2 segment1(*(polygon.vertices_begin()+ind-1),*(polygon.vertices_begin()+ind));
			Segment_2 segment2(*(polygon.vertices_begin()+ind),*(polygon.vertices_begin()+ind+1));
			if ((intersection(segment1, seg1) && *(polygon.vertices_begin()+ind-1) != p && *(polygon.vertices_begin()+ind) != p && *(polygon.vertices_begin()+ind-1) != r && *(polygon.vertices_begin()+ind) != r)||
				(intersection(segment1, seg2)  && *(polygon.vertices_begin()+ind-1) != q && *(polygon.vertices_begin()+ind) != q && *(polygon.vertices_begin()+ind-1) != s && *(polygon.vertices_begin()+ind) != s)||
				(intersection(segment2, seg1)  && *(polygon.vertices_begin()+ind) != p && *(polygon.vertices_begin()+ind+1) != p && *(polygon.vertices_begin()+ind) != r && *(polygon.vertices_begin()+ind+1) != r)||
				(intersection(segment2, seg2) && *(polygon.vertices_begin()+ind) != q && *(polygon.vertices_begin()+ind+1) != q && *(polygon.vertices_begin()+ind) != s && *(polygon.vertices_begin()+ind+1) != s)||
				 intersection(seg1, seg2))
			{			
				polygon.insert(polygon.vertices_begin()+position_of_q ,q);    
				polygon.erase(polygon.vertices_begin()+position_of_q +2); 
				goto point_selection;
			}
		}
		
		convex_hull.clear();
		CGAL::convex_hull_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(convex_hull));
		auto area = abs(polygon.area()), ch_area = abs(convex_hull.area());
		auto cr = (double)area/ch_area;
		auto energy_diff = abs(points.size()*cr - energy);
		if(min_or_max == "min" ){
			energy_diff = abs(points.size()*cr - energy);
			energy = points.size()*cr;
		}
		else{
			energy_diff = abs(points.size()*(1-cr) - energy);
			energy = points.size()*(1-cr);
		}
		double sub = 1.0/(double)L;
		T = T - sub;
		double variable = pow( energy , -energy_diff/T);

		if(variable>R)
		{
			final_polygon.insert(final_polygon.vertices_begin()+position_of_q ,r);    
			final_polygon.erase(final_polygon.vertices_begin()+position_of_q +2);
		}
		else
		{
			polygon.insert(polygon.vertices_begin()+position_of_q ,q);    
			polygon.erase(polygon.vertices_begin()+position_of_q +2); 
		}						
	}
    return final_polygon;
}



Polygon_2 global_transition(Polygon_2 &polygon, Points &points,Point_2 &q, Point_2 &s, Point_2 &p, Point_2 &r, Points &points_to_check, int &L, double &T,double &energy, float &R)
{
	int polygonsize = (int) polygon.size();
	Polygon_2 convex_hull, final_polygon;	
	CGAL::convex_hull_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(convex_hull));
	final_polygon.insert(final_polygon.end(), polygon.begin(), polygon.end());

	while(T>0)
	{	
		point_selection:
		int position_of_q = rand() % (polygonsize-3); 
		cout<<position_of_q<<"\n";
		
		int position_of_s = position_of_q +2+ ( rand() % ( polygonsize-2 - position_of_q -1 ) );
		cout<<position_of_s<<"\n\n\n\n";
		q = *(polygon.vertices_begin()+position_of_q );
		s = *(polygon.vertices_begin()+position_of_s);
		p = *(polygon.vertices_begin()+position_of_q-1);
		r = *(polygon.vertices_begin()+position_of_q+1);
		
		Segment_2 pq(*(polygon.vertices_begin()+position_of_q-1), *(polygon.vertices_begin()+position_of_q ));
		Segment_2 qr(*(polygon.vertices_begin()+position_of_q ), *(polygon.vertices_begin()+position_of_q+1));
		
		auto triangle_pqr = CGAL::area(p,q, r);
		 polygon.insert(polygon.vertices_begin()+position_of_s+1,q);
		 polygon.erase(polygon.vertices_begin()+position_of_q ); 
		 
		 r = *(polygon.vertices_begin()+position_of_q );
		 s = *(polygon.vertices_begin()+position_of_s-1);
		 q = *(polygon.vertices_begin()+position_of_s);
		 Point_2 t = *(polygon.vertices_begin()+position_of_s+1);
		  
		 Segment_2 pr(p,r);
		 Segment_2 sq(s, q);
		 Segment_2 qt(q,t);		
		 
		 for(int i=0; i< polygon.size()-1 ; i++)
		{
			 Segment_2 segment1(*(polygon.vertices_begin()+i),*(polygon.vertices_begin()+i+1));
			 if ((intersection(segment1, pr) && 	*(polygon.vertices_begin()+i) != p && *(polygon.vertices_begin()+i) != r && *(polygon.vertices_begin()+i+1) != p && *(polygon.vertices_begin()+i+1) != r)||
				 (intersection(segment1, sq) &&  *(polygon.vertices_begin()+i) != s && *(polygon.vertices_begin()+i) != q && *(polygon.vertices_begin()+i+1) != s && *(polygon.vertices_begin()+i+1) != q) ||
				 (intersection(segment1, qt) && *(polygon.vertices_begin()+i) != q && *(polygon.vertices_begin()+i) != t && *(polygon.vertices_begin()+i+1) != q && *(polygon.vertices_begin()+i+1) != t))
			 {
					polygon.erase(polygon.vertices_begin()+position_of_s+1);
					polygon.insert(polygon.vertices_begin()+position_of_q, q ); 
					goto point_selection;
			 }		    
		}	
		
		convex_hull.clear();
		CGAL::convex_hull_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(convex_hull));
		auto area = abs(polygon.area()), ch_area = abs(convex_hull.area());
		auto cr = (double)area/ch_area;
		auto energy_diff = abs(points.size()*cr - energy);
	
		double sub = 1.0/(double)L;
		T = T - sub;
		double variable = pow( energy , -energy_diff/T);
		if(variable>R)
		{
			final_polygon.insert(final_polygon.vertices_begin()+position_of_s+1,q);
			final_polygon.erase(final_polygon.vertices_begin()+position_of_q ); 
		}
		else
		{
			polygon.erase(polygon.vertices_begin()+position_of_s+1);
			polygon.insert(polygon.vertices_begin()+position_of_q, q ); 
		}	
	}
	return final_polygon;
}


double Initial_state_energy(Polygon_2 &polygon, Points &points)
{
	Polygon_2 convex_hull;	
	CGAL::convex_hull_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(convex_hull));
    	long int area = abs(polygon.area()), ch_area = abs(convex_hull.area());
    	auto cr = (double)area/ch_area;
	double energy = points.size()*cr;
	return energy;
}

int index(Polygon_2 pol , Point_2 vertex)
{
	auto it=find(pol.vertices_begin(), pol.vertices_end(), vertex); 
	return it - pol.vertices_begin(); 
}

void delete_vector(vector<Point_2> &diff, Point_2 deleted)
{
	int location;
	for (int c = 0; c < diff.size(); c++)
		if (diff[c] == deleted)
		{	
			location = c;	
			break;
		}
	diff.erase(diff.begin()+location);	
}
