/*cgal_create_CMakeLists -s <desired_name_of_executable>
cmake -DCGAL_DIR=/usr/lib/CGAL .
make*/


#include "Project1-1900082-1800040-1600168/utils_inc.hpp"

typedef Polygon_2::Vertex_circulator Vertex_circulator;
typedef Polygon_2::Vertex_iterator Vertex_iterator;


typedef struct mark{
	Point_2 left_q;   //mostly for checking:  mark[i+1].left_q= mark[i].p
	Point_2 p;
	Point_2 q;
	Point_2 r;
}mark;

mark Mark(Point_2 left_q , Point_2 p ,Point_2 q ,Point_2 r);
mark set_marking(Polygon_2 pol0 ,Polygon_2 pol1);

bool conditions(mark m);
void transfer_point( std::vector<Polygon_2> &S , std::vector<mark> &mark,int i);
void print_S(std::vector<Polygon_2> S, std::vector<mark> marking);
int findex(Polygon_2 pol , Point_2 vertex);
void insert_edge(Polygon_2 & pol , Point_2 p ,Point_2 q);
void incrimentalize(std::vector<Polygon_2> &S, std::vector<mark> marking);
void merge(std::vector<Polygon_2> S,std::vector<mark> marking , Polygon_2 & my_pol);
Polygon_2 incremental2(Points points,Points pol_points ,std::string initialization, std::string edge_selection);


Polygon_2 subdivision(Points points , int m);