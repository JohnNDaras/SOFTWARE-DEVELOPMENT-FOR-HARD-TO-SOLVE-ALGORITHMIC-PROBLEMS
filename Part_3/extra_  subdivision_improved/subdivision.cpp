#include "utils_sub.hpp"

Polygon_2 subdivision(Points points , int m , int L, string min_or_max){

Polygon_2 empty_pol;
if(points.size()<500){std::cerr<<"too few points\n"; exit(4);}
// SORT
std::sort(points.begin(), points.end(), ascx);

int k=points.size()/m ;
std::cout<<"k :"<< k <<std::endl;

std::vector<Polygon_2> S;
std::vector<mark> marking;
Polygon_2 temp;

int i=0;

// DIVIDE TO POLYGONS
while(points.size()>0){
	if(points.size()< m ) break;
	Point_2 q;
	if(S.size()>0) temp.push_back(*(S[i-1].vertices_end()-1)); // save p as left vertex
	for(int j=0;j<m;j++){
		temp.push_back(points[0]);
		points.erase(points.begin());
	}
	S.push_back(temp);
	temp.clear();
	
i++;
}

// ADD REMAINING
if(points.size()>m/2){   // we make new polygon
	Polygon_2 remaining;
	while(points.size()!=0){
		remaining.push_back(*points.begin());
		points.erase(points.begin());
	}
	S.push_back(remaining);
}
else if(points.size()>0){       // we simply add remaining points to the last polygon
	while(points.size()!=0){
		S[S.size()-1].push_back(*points.begin());
		points.erase(points.begin());
	}		
}

// WE SET INITIAL MARKING

for(int i=0; i<S.size()-1 ;++i)   //  i<S.size()-1 : we need i+1  
	marking.push_back( set_marking(S[i],S[i+1]));  // p,q,r are saved in the same marking ,although r belongs to a different polygon	       

// WE MAKE CORRECT MARKING

for(int i=0; i<S.size()-1 ;++i){
	while( conditions(S , marking,i) == false && S[i+1].size()>m/2){  // S[i+1].size>m/2: we put a threshold for points transfered   
		std::cout<<i<<"  unsatisfied conditions  \n";        //        ...
		transfer_point(S,marking,i);							//     So ,...
		std::cout<<"points were transfered to polygon "<<i<<"\n";
	}
														//  if threshold is reached we force exit 
	if (conditions(S , marking,i)==false) {std::cout<<i<<"Error, can't find valid q point\n"; exit(1);}  

}


//for(int i=0; i<S.size()-1 ;++i) std::cout<<i<<"lq:"<<marking[i-1].q<<"lr:"<<marking[i-1].r<<"p: "<<marking[i].p<<",q: "<<marking[i].q<<"\n";
// INCREMENTAL

print_S(S,marking);

globalize(S,marking,L,min_or_max);

Polygon_2 my_pol;

print_S(S,marking);
merge(S,marking ,my_pol);

std::cout<<"final polygon,simple? "<< my_pol.is_simple() <<std::endl;
/**/

return my_pol;    
}/**/

