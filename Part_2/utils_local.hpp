#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Segment_2.h>
#include "Project1-1900082-1800040-1600168/utils_inc.hpp"
#include "Project1-1900082-1800040-1600168/utils_con_hull.hpp"
#include "Project1-1900082-1800040-1600168/utils_onion.hpp"
/**/
//#include "project1.hpp"


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef K::Segment_2 Segment_2;
typedef K::Triangle_2 Triangle_2;
typedef CGAL::Polygon_2<K>  Polygon_2;
typedef Polygon_2::Edge_const_iterator EdgeIterator;
typedef Polygon_2::Edge_const_circulator EdgeCirculator;
typedef std::vector<Point_2> Points;
//typedef std::vector<Segment_2> Edges;


typedef Polygon_2::Vertex_circulator Vertex_circulator;

typedef struct info{
    Point_2 u1;
    Point_2 u2;
    Points V;  // Path
    int DA; // Difference in area 
}info;


int indexx(Polygon_2 pol , Point_2 vertex);

void apply_changes1( Polygon_2 & pol ,info solution);

bool compareMAX(const info &a, const info &b);
bool compareMIN(const info &a, const info &b);

Polygon_2 local(Points points ,std::string  polygonization , int L ,int threshold);


