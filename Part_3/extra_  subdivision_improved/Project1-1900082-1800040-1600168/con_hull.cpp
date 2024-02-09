#include "utils_con_hull.hpp"


//int main(int argc, char *argv[])
Polygon_2 con_hull(vector<Point_2> points , int edge_selection)
{       
        Polygon_2 polygon;
		//string input_file = argv[1];
		//string output_file = argv[2];
		//read_file(points,input_file);
		vector<all_nearests_points> nearest;
		vector<Point_2>  diff, chull;
		int location=0;                             
		Point_2  a;	  
		find_convex_hull(points, chull, diff);
		
		while(diff.size() > 0 )  
		{
			 for (int i=0; i<chull.size(); i++)
			 {
				if(chull[i]!= chull[2]  && chull[i+1]!=chull[3]){				
				if(i==0)
					a = chull[i];
				if(i<chull.size()-1)
					nearest_point1( chull[i], chull[i+1], diff, nearest);			
				else if(i==chull.size()-1)			
					nearest_point1( chull[i], a, diff, nearest);
				}				
			}					
						
			switch (edge_selection) {
				case 1:
				add_max_area(nearest, diff);
				break;
			case 2:
				add_min_area(nearest, diff);
				break; 
			case 3:
				add_random(nearest, diff);
				break;
			}
			
			chull.clear();			
			for (int i=0; i<nearest.size(); i++)		
				chull.push_back(nearest[i].point1);		
            if( diff.size() == 0)	
                polygon.insert(polygon.end(), chull.begin(), chull.end());
			//print_polygon(nearest, diff, chull, edge_selection, output_file);	 // Print final polygon		
			nearest.clear();
		}
  return polygon;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

