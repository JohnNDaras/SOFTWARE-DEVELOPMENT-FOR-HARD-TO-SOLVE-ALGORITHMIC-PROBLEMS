#include "utils_onion.hpp"



int onion_initialization(Polygon_2 p ,int method){  // 3rd Excersice - Onion
  int index;
  pveciterator it;
  switch(method){
  case 1: //random
    srand (time(NULL));
	return rand()%p.size();

  case 2: //(method=="min_x"){
    it=find(p.vertices_begin(),p.vertices_end(),*p.left_vertex());
    return it-p.vertices_begin();

  case 3://(method== "max_x"){
    it=find(p.vertices_begin(),p.vertices_end(),*p.right_vertex());
    return it-p.vertices_begin();
    
  case 4://(method== "min_y"){
    it=find(p.vertices_begin(),p.vertices_end(),*p.bottom_vertex());
    return it-p.vertices_begin();
    
  case 5://(method== "max_y"){
    it=find(p.vertices_begin(),p.vertices_end(),*p.top_vertex());
    return it-p.vertices_begin();

  default:
    std::cout<<"invalid method code\n";
    exit(-1);    
    return 0;
  }
}

bool visible(Point_2 p, Segment_2 edge, Polygon_2 pol){  // common 
    Segment_2 seg1 = Segment_2(p, edge.source());
    Segment_2 seg2 = Segment_2(p, edge.target());
    Segment_2 seg3 = Segment_2(p, CGAL::midpoint(edge.source(), edge.target()));
    int intersect_count1 = 0, intersect_count2 = 0, intersect_count3 = 0;
    for (EdgeIterator ei = pol.edges_begin(); ei != pol.edges_end(); ei++){
        if (do_intersect(*ei, seg1))
            intersect_count1++;
        if (do_intersect(*ei, seg2))
            intersect_count2++;
        if (do_intersect(*ei, seg3))
            intersect_count3++;
    }
    //std::cout<< "\ncount1: "<< intersect_count1 << "\ncount2: " << intersect_count2 <<"\n";
    if (intersect_count1 != 2 || intersect_count2 != 2 || intersect_count3 !=1) //!!! HTAN "!="
        return false;
    return true;
    }

bool is_edge_visible(Point_2 m,Point_2 m_next, Segment_2 edge, Polygon_2 pol){  // (m,m+1) , edge =(k,l)or(l,k)
    if(visible(m ,edge , pol) or visible(m_next ,edge, pol) ) return true; 
    else return false; //it is enough for ONLY ONE of m,m_next to be visible because k is visible by the whole edge(m,m_next)  
}                                                                         //... because k is the nearest point to the edge                                              


int nearest_point_index(Point_2 p,Point_2 q,Points vertices){
    std::vector<int> indexes;
    for(int i=0;i<vertices.size();i++) indexes.push_back(i); // save indexes (0,1,..,n) in vertex
    Segment_2 seg=Segment_2(p,q);
    while(indexes.size()>1){
        if(squared_distance(seg,vertices[indexes[1]]) < squared_distance(seg,vertices[indexes[0]]) ) 
           indexes.erase(indexes.begin() );    // if 2nd point < 1st point delete 2nd point's index
        else    indexes.erase(indexes.begin()+1);  // else delete 2nd point's   
    }
    return indexes[0];
}