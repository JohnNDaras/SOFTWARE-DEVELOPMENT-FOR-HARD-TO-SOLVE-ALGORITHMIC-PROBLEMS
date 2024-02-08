#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Segment_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef K::Segment_2 Segment_2;
typedef K::Triangle_2 Triangle_2;
typedef CGAL::Polygon_2<K>  Polygon_2;
typedef Polygon_2::Edge_const_iterator EdgeIterator;
typedef Polygon_2::Edge_const_circulator EdgeCirculator;
typedef std::vector<Point_2> Points;
typedef std::vector<Segment_2> Edges;

bool ascx(Point_2 a, Point_2 b);
bool descx(Point_2 a, Point_2 b);
bool ascy(Point_2 a, Point_2 b);
bool descy(Point_2 a, Point_2 b);
int find_vertex(Point_2 p, Polygon_2 pol);
bool is_visible(Point_2 p, Segment_2 edge, Polygon_2 pol);
Segment_2 select_edge(Edges visible_edges, std::string selection, Point_2 p);