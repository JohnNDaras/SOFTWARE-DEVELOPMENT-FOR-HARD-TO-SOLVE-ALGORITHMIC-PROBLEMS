#include "utils_local.hpp"

#define ALG "incremental"


Polygon_2 local(Points points ,std::string  polygonization , int L ,int threshold){

Polygon_2 polygon;


if (ALG == "onion") polygon=onion(points,1);
else polygon = incremental(points, "1a", "1"); 
 
Polygon_2 polygon_0 = polygon ; // a copy of the intitial 


Polygon_2 temp_p ;
std::vector<info> T;
long int area0=polygon.area();
long int area1=2*area0;
long int DA = area1-area0;
//int threshold = area0 /50;

int del=0;

// LOCAL SEARCH

while(abs(DA)>threshold){  del++;   //check threshold

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
 
        changes.DA=temp_p.area()-area0 ;  
 
//      SAVE WANTED CHANGES
        if(polygonization=="max"){
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
        if(polygonization=="max") std::sort(T.begin(),T.end(),compareMAX);
        else std::sort(T.begin(),T.end(),compareMIN);

    /////       UPDATE POLYGON , AREAS ,DA , etc.
        apply_changes1(polygon, T[0]);   // update current polygon
        area0=polygon.area();  // update current area
        DA=T[0].DA;
        std::cout <<"new area: "<<area0<<std::endl;
        /*std::cout <<"T.edge     T.v     T.DA  \n";
        for(int i=0; i < T.size() ; i++) std::cout <<T[i].u1 <<"   , FIRST point of path: " << T[i].V[0] <<"    " << T[i].DA << "\n";
        /**/

        T.clear();   // the polygon is changed so we need new solutions 
    }

    else DA=0;    // IF we had no changes DA=0

}


////    JUST PRINTING  /////

std::cout<<"\n\n\n\n after " << del << " iterations  : \n";

if(polygon.is_simple()){ std::cout<<"simple\n"; std::cout<<"area:" <<polygon_0.area()<<"->"<< polygon.area()<<"\n" ;  }
/////////////////////////////////

return polygon;
} 


/*
int main(int argc ,char* argv[]){

std::string polygonization;
polygonization="max";
    Points points;
	///// READ FROM INPUT FILE /////   
	std::ifstream inFile;   
	std::ofstream outFile;
	//inFile.open("/home/kostas3000/Downloads/instances/data/uniform/uniform-0000045-1.instance");
    inFile.open("/home/kostas3000/Downloads/instances/data/uniform/uniform-0000045-2.instance");
	std::string line;
	std::getline(inFile, line);
	std::getline(inFile, line);
	while (std::getline(inFile, line)){
		std::istringstream iss(line);
		int a,b,c;
		if (!(iss >> a >> b >> c)) { break; } // error  
		points.push_back(Point_2(b,c));		
	}
	inFile.close();



Polygon_2 polygon;

polygon=local(points,"min",5,1500);

return 0;

}/**/