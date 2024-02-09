#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/property_map.h>
#include <CGAL/array.h>
#include <CGAL/Cartesian/function_objects.h>
#include <vector>
#include <numeric>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list> 
#include <sstream> 
#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <set>
#include <CGAL/Line_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/intersections.h>

using namespace std;  

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef K::Line_2 Line_2;
typedef CGAL::Polygon_2<K>  Polygon_2;
typedef K::Segment_2 Segment_2;
typedef K::Intersect_2 Intersect_2;
typedef CGAL::Convex_hull_traits_adapter_2<K,
          CGAL::Pointer_property_map<Point_2>::type > Convex_hull_traits_2;

 struct mystruct {
	Point_2  point;
	double distance;
};         

struct mystruct_lessThan
{
    bool operator()(mystruct const & a, mystruct const & b) const
    {
        return a.distance < b.distance;
    }
};

 struct all_nearests_points {
	Point_2  point1;
	Point_2  point2;
	vector<mystruct> list;
};         

struct all_nearests_points_lessThan
{
    bool operator()(all_nearests_points const & a, all_nearests_points const & b) const
    {
        return a.list[0].distance < b.list[0].distance;
    }
};



void nearest_point1(Point_2 p,Point_2 q, vector<Point_2> vertices,vector<all_nearests_points> &nearest);
void add_max_area(vector<all_nearests_points> &nearest, vector<Point_2> &diff);
void add_min_area(vector<all_nearests_points> &nearest, vector<Point_2> &diff);
void add_random(vector<all_nearests_points> &nearest, vector<Point_2> &diff);
void find_convex_hull(vector<Point_2> &points, vector<Point_2> &chull, vector<Point_2> &diff);
void delete_from_vector(vector<Point_2> &diff, Point_2 deleted);
Polygon_2 con_hull(vector<Point_2> points , int edge_selection);