/*cgal_create_CMakeLists -s <desired_name_of_executable>
cmake -DCGAL_DIR=/usr/lib/CGAL .
make*/
#include "utils_local.hpp"

//Polygon_2 incremental(Points points, std::string initialization, std::string edge_selection);

int indexx(Polygon_2 pol , Point_2 vertex){
  auto it=find(pol.vertices_begin(), pol.vertices_end(), vertex); 
  return it - pol.vertices_begin(); 
}

void apply_changes1( Polygon_2 & pol ,info solution){  //   
    Points V=solution.V; //=solution.V[0];
    Point_2 u2 =solution.u2;

    if(solution.V.size()<1) { std::cout<<" empty Path error\n"; exit(1); }
    for(int i = V.size()-1;i>=0 ; i--){
        pol.erase(pol.vertices_begin()+ indexx(pol,V[i]));
        pol.insert(pol.vertices_begin() + indexx(pol, u2), V[i]);  //insert between u1 and u2    
    }
}

bool compareMAX(const info &a, const info &b)
{
    return a.DA > b.DA;
}

bool compareMIN(const info &a, const info &b)
{
    return a.DA < b.DA;
}