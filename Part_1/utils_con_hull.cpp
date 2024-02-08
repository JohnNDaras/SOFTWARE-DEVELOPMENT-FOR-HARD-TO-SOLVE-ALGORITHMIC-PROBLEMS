#include "utils_con_hull.hpp"

void find_convex_hull(vector<Point_2> &points, vector<Point_2> &chull, vector<Point_2> &diff)
{	
	std::vector<std::size_t> indices(points.size()), out;
	std::iota(indices.begin(), indices.end(),0);
	CGAL::convex_hull_2(indices.begin(), indices.end(), std::back_inserter(out), Convex_hull_traits_2(CGAL::make_property_map(points)));
                                           
	for( std::size_t i : out){
		//std::cout << "points[" << i << "] = " << points[i] << std::endl;
		chull.push_back(points[i]);	
  }

	auto points1 = points;
	auto chull1 = chull;
	std::sort(points1.begin(), points1.end());
	std::sort(chull1.begin(), chull1.end());
	std::set_difference( points1.begin(), points1.end(), chull1.begin(), chull1.end(), std::inserter(diff, diff.begin()) );
}


void add_max_area(vector<all_nearests_points> &nearest, vector<Point_2> &diff)
{
	all_nearests_points edge1; 
	all_nearests_points edge2;
	int location = 0;
	for (int c = 0; c < nearest.size(); c++)
    if (nearest[c].list[0].distance < nearest[location].list[0].distance)	
		location = c;		
	Point_2 deleted = nearest[location].list[0].point;
	edge1.point1 = nearest[location].point1;
	edge1.point2 = nearest[location].list[0].point;
	edge2.point1 = nearest[location].list[0].point;
	edge2.point2 = nearest[location].point2;
	nearest.erase(nearest.begin()+location);
	nearest.insert(nearest.begin() + location, edge1 );
	nearest.insert(nearest.begin() +1 + location, edge2 );
	delete_from_vector(diff, deleted);
}


void add_min_area(vector<all_nearests_points> &nearest, vector<Point_2> &diff)
{
	all_nearests_points edge1; 
	all_nearests_points edge2;
	int location = 0;
	for (int c = 0; c < nearest.size(); c++)
    if (nearest[c].list[0].distance > nearest[location].list[0].distance)
		location = c;
	Point_2 deleted = nearest[location].list[0].point;
	edge1.point1 = nearest[location].point1;
	edge1.point2 = nearest[location].list[0].point;
	edge2.point1 = nearest[location].list[0].point;
	edge2.point2 = nearest[location].point2;	
	nearest.erase(nearest.begin()+location);
	nearest.insert(nearest.begin() + location, edge1 );
	nearest.insert(nearest.begin() +1 + location, edge2 );
	delete_from_vector(diff, deleted);
}

void add_random(vector<all_nearests_points> &nearest, vector<Point_2> &diff)
{
	all_nearests_points edge1; 
	all_nearests_points edge2;
	srand(time(0));
	int location = 0;
	location = rand()%nearest.size();
	Point_2 deleted = nearest[location].list[0].point;
	edge1.point1 = nearest[location].point1;
	edge1.point2 = nearest[location].list[0].point;
	edge2.point1 = nearest[location].list[0].point;
	edge2.point2 = nearest[location].point2;
	nearest.erase(nearest.begin()+location);
	nearest.insert(nearest.begin() + location, edge1 );
	nearest.insert(nearest.begin() +1 + location, edge2 );
	delete_from_vector(diff, deleted);
}


void delete_from_vector(vector<Point_2> &diff, Point_2 deleted)
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


void nearest_point1(Point_2 p,Point_2 q, vector<Point_2> vertices, vector<all_nearests_points> &nearest){
    auto temp=vertices;
    Segment_2 l(p,q);
	vector<mystruct> mylist;
	mystruct elem;
	mystruct closest_elem;	
	all_nearests_points element;
    
    std::vector<Point_2>::iterator it;
	for (int i=0; i<temp.size(); i++)
	{
		   elem.point = temp[i];
		   elem.distance = CGAL::squared_distance(l,temp[i]);
		   mylist.push_back(elem);
    } 
    std::sort(mylist.begin(), mylist.end(), mystruct_lessThan());
	auto area = CGAL::area(p,q, mylist[0].point);
	mylist[0].distance = abs(area);
	closest_elem = mylist[0];
	element.point1 = p;
	element.point2 = q;
	element.list.push_back(closest_elem);
	nearest.push_back(element);
}
