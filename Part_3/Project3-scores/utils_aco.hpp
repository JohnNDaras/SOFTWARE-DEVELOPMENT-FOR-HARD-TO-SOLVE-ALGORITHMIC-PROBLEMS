#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Segment_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef CGAL::Polygon_2<K>  Polygon_2;
typedef std::vector<Point_2> Points;
typedef std::vector<int> Indexes;
class Ant;
class Edge;
typedef std::vector<Ant> Ants;
typedef std::vector<Edge> Edges;

// An instance of an edge in the graph of partial solutions. It connects two graph nodes (Polygons), source and target.
class Edge {
    public:

    Polygon_2 source; 
    Polygon_2 target;
    double pheromone;
    double heur; // Heuristic value of the Edge, remains unchanged once set.
    int insertsion_index; // Where in the source polygon the new point is inserted to give the target polygon.

    Edge(Polygon_2 source, int insertsion_index, Point_2 new_p, std::string opt); // Constructor.
    bool operator == ( Edge second); // Operator overload
    void update_pheromone(Ants ants, double ro);
};


// An instance of an ant which traverses the edges and builds a solution.
class Ant{
    public:

    Edges trail; // The path of edges the ant followed to reach the solution.
    Polygon_2 solution; // The final polygon the ant constructed.
    double evaluation; // The objective evaluation of the solution found by the ant.

    bool operator < (Ant second);
    void build_solution(Polygon_2 polygon, Edges &edges, Points points, std::string opt, double alpha, double beta);
    void evaluate_solution(std::string opt, double max_area);

};



bool feasible_insertion(Point_2 new_p, int index, Polygon_2 pol);
bool is_feasible(Point_2 new_p, Polygon_2 polygon, Points points);
Edge select_edge(Edges edges, Polygon_2 polygon, double alpha, double beta);
void update_trails(Ants ants, Edges &edges, Ant best_ant, double ro, bool elitism);
