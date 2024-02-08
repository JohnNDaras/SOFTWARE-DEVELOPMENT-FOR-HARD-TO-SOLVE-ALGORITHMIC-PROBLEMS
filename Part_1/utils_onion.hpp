#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon_2;

#include <CGAL/convex_hull_2.h>
#include <CGAL/Convex_hull_traits_adapter_2.h>
#include <CGAL/property_map.h>
#include <fstream>
typedef K::Point_2 Point_2;
typedef std::istream_iterator< Point_2 > point2_iterator;
typedef std::vector<Point_2>            Points;
typedef std::vector<Point_2>::iterator pveciterator;
typedef K::Segment_2                      Segment_2;
typedef Polygon_2::Edge_const_iterator    EdgeIterator;


int onion_initialization(Polygon_2 p ,int method);  // 3rd Excersice - Onion

bool visible(Point_2 p, Segment_2 edge, Polygon_2 pol);

bool is_edge_visible(Point_2 m,Point_2 m_next, Segment_2 edge, Polygon_2 pol);

int nearest_point_index(Point_2 p,Point_2 q,Points vertices);

Polygon_2 onion(Points S,int initialization);
