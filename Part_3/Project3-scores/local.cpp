#include "utils_local.hpp"



double local(Points points ,std::string  minormax , std::string  polygonization , int L ,int threshold ){
auto start = chrono::high_resolution_clock::now();// proj3

Polygon_2 polygon;

long int n = points.size();  // proj3

if (polygonization == "onion") polygon=onion(points,1);

else if(polygonization =="con_hull") {
    if(minormax=="max")
        polygon=con_hull(points,2);
        // 	polygon = con_hull(points,stoi( edge_selection));
    else
        polygon=con_hull(points,1);

}
else{     // incremental
    if(minormax=="max")
        polygon = incremental(points, "1a", "3"); 
    else
        polygon = incremental(points, "1a", "2");
}


if(polygon.size()==0){  // proj3
    if(minormax=="max"){
    std::cout<<"exceeded cutoff in polygonization!!!!!\n";  
    return 0;  
    } // proj3
    else{
    std::cout<<"exceeded cutoff in polygonization!!!!!\n";  
    return 1;
    }
}


Polygon_2 convex_hull;  ///////  proj3
CGAL::convex_hull_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(convex_hull));
long int ch_area = abs(convex_hull.area());  /////  proj3

Polygon_2 polygon_0 = polygon ; // a copy of the intitial 


Polygon_2 temp_p ;
std::vector<info> T;
long int area0=abs(polygon.area());
long int area1=2*area0;
long int DA = area1-area0;
//int threshold = area0 /50;

int del=0;

// LOCAL SEARCH



while(abs(DA)>threshold){  del++;   //check threshold

auto now = chrono::high_resolution_clock::now();    // proj3
auto duration = chrono::duration_cast<chrono::milliseconds>(now - start); // save construction time // proj3

if(duration.count() >= 500*n){    ////////////proj3
    if(minormax=="max"){
    std::cout<<"exceeded cutoff !!!!!\n";  
    return 0;  
    } // proj3
    else{
    std::cout<<"exceeded cutoff !!!!!\n";  
    return 1;
    }
}//////////////////////////////////////////////// proj3

Vertex_circulator u1= polygon.vertices_circulator();


for(int edge_num=0; edge_num<polygon.vertices().size();edge_num++){   // for every edge  

    u1= polygon.vertices_circulator() + edge_num;
    Vertex_circulator u2= u1 +1;
        

//      CREATE EVERY POSSIBLE PATH ( LENGTH<=L)

        Point_2 v;
        for(Vertex_circulator vi= u2+1; vi != u1; ++ vi ) { // for the rest of the edges

        for(int end=0; end<L && vi+end != u1 ; end++){
        Points V;// our path
        for(int i=0; i<=end ; i++){
            V.push_back(*(vi+i));            
            //std::cout<<*u1 <<") "<< *vi <<": (V) "<< V[V.size()-1] <<"\n\n\n";
        }
//std::cout<<*u1 <<") "<< *vi <<": (V) "<< V[V.size()-1] <<"\n\n\n";


//      TEST THE CHANGE ON THE TEMPORARY POLYGON
        info changes;
        temp_p=polygon;        
        changes.V=V;
        changes.u1=*u1;
        changes.u2=*u2;
        
        apply_changes1(temp_p, changes);
 
        changes.DA=abs(temp_p.area())-area0 ;  
 
//      SAVE WANTED CHANGES
        if(minormax=="max"){
        if( temp_p.is_simple() && changes.DA>0 ) {
            T.push_back(changes);         //    T saves the "good" changes  
        }
        }

        else{
        if( temp_p.is_simple() && changes.DA<0 ) {
            T.push_back(changes);
        }
        }

        temp_p.clear();
        V.clear();
        } 
        }  
    }


    if(T.size()!=0) {

    /////       SORT   ////        
        if(minormax=="max") std::sort(T.begin(),T.end(),compareMAX);
        else std::sort(T.begin(),T.end(),compareMIN);

    /////       UPDATE POLYGON , AREAS ,DA , etc.
        apply_changes1(polygon, T[0]);   // update current polygon
        area0=abs(polygon.area());  // update current area
        DA=T[0].DA;
        /*std::cout <<"T.edge     T.v     T.DA  \n";
        for(int i=0; i < T.size() ; i++) std::cout <<T[i].u1 <<"   , FIRST point of path: " << T[i].V[0] <<"    " << T[i].DA << "\n";
        /**/

        T.clear();   // the polygon is changed so we need new solutions 
    }

    else DA=0;    // IF we had no changes DA=0

}

/////////////////////////////////



long int area = abs(polygon.area());  // proj3
return (double)area/ch_area ;   // proj3

}