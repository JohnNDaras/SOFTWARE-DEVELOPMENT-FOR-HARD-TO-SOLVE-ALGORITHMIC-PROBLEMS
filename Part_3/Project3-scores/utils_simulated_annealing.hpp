#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/convex_hull_2.h>
#include <fstream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/property_map.h>
#include <vector>
#include <numeric>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Kd_tree.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/algorithm.h>
#include <CGAL/Fuzzy_sphere.h>
#include <CGAL/Search_traits_2.h>
#include <CGAL/Fuzzy_iso_box.h>
#include <CGAL/intersections.h>
#include <typeinfo>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <random>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef CGAL::Polygon_2<K>  Polygon_2;
typedef Polygon_2::Edge_const_iterator EdgeIterator;
typedef std::vector<Point_2> Points;
typedef CGAL::Convex_hull_traits_adapter_2<K,
          CGAL::Pointer_property_map<Point_2>::type > Convex_hull_traits_2;
typedef K::Segment_2 Segment_2;
typedef CGAL::Search_traits_2<K>   Traits;
typedef CGAL::Fuzzy_sphere<Traits> Fuzzy_circle;
typedef CGAL::Kd_tree<Traits> Tree;
typedef CGAL::Fuzzy_sphere<Traits> Fuzzy_sphere;
typedef CGAL::Fuzzy_iso_box<Traits> Fuzzy_iso_box;



using namespace std;
Polygon_2 con_hull(Points points, int edge_selection);
void arguments(string &input_file, string &output_file, string &algorithm, string &edge_selection, string &initialization, string &onion_initialization, int argc, char *argv[]);
int index(Polygon_2 pol , Point_2 vertex);
void delete_vector(vector<Point_2> &diff, Point_2 deleted);
long int Pick_area(Polygon_2 p);
double simulated_annealing(Points points,string algorithm,string edge_selection,int annealing, int L, string min_or_max);
double local_transition(Tree &tree, Polygon_2 &polygon,Points &points, Point_2 &p, Point_2 &q, Points &points_to_check, int &L, double &energy, float &R, string min_or_max);
double global_transition(Polygon_2 &polygon, Points &points,Point_2 &q, Point_2 &s, Point_2 &p, Point_2 &r, Points &points_to_check, int &L, double &T,double &energy, float &R, string min_or_max);
double Initial_state_energy(Polygon_2 &polygon, Points &points);
