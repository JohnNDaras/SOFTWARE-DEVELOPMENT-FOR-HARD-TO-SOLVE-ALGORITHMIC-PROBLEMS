#include "utils_onion.hpp"



Polygon_2 onion(Points S,int initialization)
{
  ///////     MAKE THE POLYGONS VECTOR , ERASE FROM S  //////////////
  Points result;
  Polygon_2 temp,p1, p2;
  std::vector<Polygon_2> polygons;

  while( S.size()>2){  
  CGAL::convex_hull_2(S.begin(), S.end(), std::back_inserter(result));
  for(auto it = result.begin(); it!= result.end();++it){
    //std::cout <<"result:  " << *it << " , ";
    S.erase(std::remove(S.begin(), S.end(),*it), S.end());
    temp.push_back(*it);
    }
  //for(auto it = S.begin(); it!= S.end();++it){ std::cout <<"S: "<< *it << " \n"; }
  polygons.push_back(temp);
  result.clear();
  temp.clear();
  }  

  //////////////      INITIALISE VARIABLES BEFORE THE MAIN LOOP, APPLY SELECTION METHOD     //////////
  int depth=0;
  int max_depth=polygons.size();
  p1=polygons[0]; // just keep everything in p1
  std::cout<<"you chose method: "<< initialization<<"\n";
  int index_m=onion_initialization(p1,initialization);

  //////////////  MAIN LOOP  //////////////

while( depth+1<max_depth){    
//  std::cout<<"Depth: "<<depth<<"\n";
  /////1) FIND M    /////////////////////////// 
  p2=polygons[depth+1]; 
  Point_2 m =p1.vertices()[index_m];//(*outer_edge_it).source(); 

  // IF NEEDED CHANGE ORIENTATION //////
  if(depth%2==0) p2.reverse_orientation();    // Each depth: the inner polygon must have different orientation from  its predecessor
  //    by default all the polygons have counter-clockwise orientation so we only need to reverse when we want clockwise orientatin

  // FIND K   /////////////////
  Segment_2 outer_edge=*(p1.edges_begin()+index_m);
//  std::cout<<"m :"<<outer_edge.source()<<"m_next: "<<outer_edge.target()<<"\n";
  int index=nearest_point_index(m, outer_edge.target() , p2.vertices());

  Point_2 k=p2.vertices()[index];//p2.vertices()[p2_index];
//  std::cout<<"k : "<<k<<std::endl;

  //    FIND VISIBLE EDGE , THEN FIND l   /////////////////////
  Segment_2 seg1,seg2;   //possible visible edges
  if(index==0) seg1=*(p2.edges_begin() + p2.edges().size()-1);// CASE where k is 1st point // remember p2.edges_begin = p2.edges_begin() 
  else seg1=*(p2.edges_begin() + index-1);   // seg1=(l1=k-1 , k)          
  seg2=*(p2.edges_begin() + index);          //seg2 = ( k, l2=k+1)

  Point_2 l ;
  Segment_2 visible_edge;
  Point_2 m_next=outer_edge.target();
  if(is_edge_visible(m, m_next, seg1, p2))
    {visible_edge=seg1;
    l=visible_edge.source();// is_edge_visible true means l before k 
    }
  else if(is_edge_visible(m, m_next, seg2, p2))
    { visible_edge=seg2; 
    l=visible_edge.target();
    } 
  else {std::cout<<"Error , non existent visible edge \n"; exit(1);}

  ////   UPDATE INDEX    //////        
  auto temp_it=find(p2.vertices_begin(), p2.vertices_end(),visible_edge.target()); 
  index=temp_it - p2.vertices().begin(); // IN CASE visible_edge=(l,k) we have to update to the new index

  ////    ADD THE POINTS OF P2 TO P1   ////////////
  pveciterator iter; 
  int next_to_delete_index;

  for(int count=1; count<=p2.vertices().size(); ){  // prepei na ksekinaei apo to l(/k analoga thn epilogi l)
    index=index%p2.vertices().size();  
    iter=p2.vertices_begin()+index;
    
    p1.insert (p1.vertices_begin()+ index_m +count,*iter);// we start with the target of visible_edge and end with its source 
    if(count==p2.vertices().size()/2 +1 ){
        next_to_delete_index=index_m +count;
        } /**/ //  !!!   AYTO EINAI TO BHMA: PHGAINE DEKSIOSTROFA APO TH MIA KAI ARISTEROSTROFA APO THN ALLH 
    index++;
    count++;
  }

  ////    PICK NEXT EDGE TO DELETE , UPDATE INDEX_M AND DEPTH   ////
  index_m=next_to_delete_index;
  depth++;
  ////////     TELOS EPANALIPSIS     /////////////////////////////////// /**/
  }
  // ADD THE LONE POINTS  , IF EXISTING  //////////////////////
  if(S.size()==2){  
    int i;
    Point_2 m=p1.vertices()[index_m];
    int my_orientation=1; // the orientation that should be followed 
    if(depth%2==0) my_orientation = -1;   // 1=counter-clockwise , -1 = clockwise
    
    if(CGAL::orientation(m,S[0],S[1])==0){ // collinear
        if(CGAL::has_smaller_distance_to_point(m,S[0],S[1])) i=0;
        else i=1;
        }
    else if(CGAL::orientation(m,S[0],S[1])==my_orientation) i=0; //  no need to change something    
    else i=1;        //   wrong orientation , we need to add S[1] before S[0] 

    p1.insert(p1.vertices_begin()+index_m +1,S[i]);     
    S.erase(S.begin()+i);  // kane ta S[i],S[i+1] ena edge
    index_m++;
  } 
  if(S.size()==1) {p1.insert(p1.vertices_begin()+index_m +1,S[0]); S.clear();}     



return p1;
}