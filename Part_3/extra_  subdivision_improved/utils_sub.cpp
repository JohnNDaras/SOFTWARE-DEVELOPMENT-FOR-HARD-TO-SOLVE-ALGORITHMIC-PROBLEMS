#include "utils_sub.hpp"

using namespace std;

mark Mark(Point_2 left_q , Point_2 p ,Point_2 q ,Point_2 r){
	mark m;
	m.left_q=left_q;     
	m.p = p;
	m.q = q;
	m.r = r;
	return m;	
}


mark set_marking(Polygon_2 pol0 ,Polygon_2 pol1){
	const Polygon_2::Vertices& range0 = pol0.vertices(), range1 = pol1.vertices();
	Points lh0 ,lh1;
	CGAL::lower_hull_points_2(range0.begin(), range0.end(), std::back_inserter(lh0));
	CGAL::lower_hull_points_2(range1.begin(), range1.end(), std::back_inserter(lh1));
	
	Point_2 left_q=*(pol0.vertices_begin());
	Point_2 q= *(pol0.vertices_end()-1); // or pol.right_vertex but pols are sorted
	Point_2 p= *(lh0.end()-1);  // last point in lower hull , lower hull doesn't include rightmost vertex 'q'
    Point_2 r= *(lh1.begin()+1); 
	return Mark(left_q,p,q,r);

}

bool conditions(std::vector<Polygon_2> S , std::vector<mark> mark,int i){

	Polygon_2 p0= S[i];
	Polygon_2 p1= S[i+1];
	
	p0.erase(p0.right_vertex());	
	if(mark[i].p !=*p0.right_vertex() )  // second rightest must be p 
		return false;

	p1.erase(p1.left_vertex());		
	if(mark[i].r!= *p1.left_vertex())   // second leftest of next polygon must be r 
		return false;

	return ( (mark[i].p.y()<mark[i].q.y()) && (mark[i].r.y()< mark[i].q.y()));  // confirm it's pqr is like this /\ 

}

void transfer_point( std::vector<Polygon_2> &S , std::vector<mark> &mark,int i){  // we transfer from i+1th to the ith polygon
        if(S.size()-1< i+1) std::cout<<"invalid index \n";
        
		//if( mark[i+1].left_q==mark[i].q) 
		S[i+1].erase(S[i+1].vertices_begin()); //delete old q   

		Point_2 new_q=* S[i+1].vertices_begin(); 
		S[i].push_back(new_q);
		// WE DO NOT ERASE from S[i+1] to keep q in S[i+1]   //S[i+1].erase(S[i+1].vertices_begin());

		mark[i]=set_marking(S[i],S[i+1]); //mark
		//mark[i+1].left_q=new_q;
}

void print_S(std::vector<Polygon_2> S, std::vector<mark> marking){
	for(int i=0 ; i< S.size(); i++){
	std::cout<<"S["<<i<<"] : \n";
	if(i !=S.size()-1) std::cout<<"p: "<<marking[i].p<<" ,q: "<<marking[i].q<<" ,r: "<<marking[i].r<<"\n";
	for(auto it=S[i].vertices_begin() ; it!=S[i].vertices_end(); ++it)
		std::cout<<*it<<std::endl;
	}
}

int findex(Polygon_2 pol , Point_2 vertex){
  auto it=find(pol.vertices_begin(), pol.vertices_end(), vertex); 
  return it - pol.vertices_begin(); 
}

void insert_edge(Polygon_2 & pol , Point_2 p ,Point_2 q){   // inserts edge that's outside convex hull
	for (EdgeIterator ei = pol.edges_begin(); ei != pol.edges_end(); ++ei){
		if(is_visible(p,*ei, pol)){  //from Project1 "utils_inc.hpp   // p is checked
			Point_2 target= ei->target();
			Segment_2 seg = Segment_2(q, target);      // time to check q
    		int intersect_count = 0;
    		for (EdgeIterator eii = pol.edges_begin(); eii != pol.edges_end(); ++eii){
        		if (do_intersect(*eii, seg))
            		intersect_count++;
			}
			if(intersect_count ==2){
				pol.insert(pol.vertices_begin()+findex(pol,target),p);
				pol.insert(pol.vertices_begin()+findex(pol,target),q);
				std::cout<<" edge inserted successfully \n" ;
				break;
			}

		}
	}
}



void globalize(std::vector<Polygon_2> &S, std::vector<mark> marking , int L , string min_or_max){

for(int i=0; i<S.size() ;++i){	
	
	S[i]=incremental(S[i].vertices(),"1b","2");

	srand(time(0));
    Tree tree;
	double T = 1.0, energy, variable;	
	float R = (float) rand()/RAND_MAX;
	Point_2 q,s,p,r;
	Points points_to_check;
	Points points = S[i].vertices();
	Polygon_2 polygon;	
	cout<< "R = "<<R<<"\n\n"<<endl;
//	polygon = con_hull(points,stoi( edge_selection));

    energy = Initial_state_energy(polygon, points);
			
	std::vector<Point_2> marked;
	Point_2 dummy_point;  
	if(i==0){
		marked.push_back(marking[i].p);
		marked.push_back(marking[i].q);
	}else if(i==S.size()-1){
		marked.push_back(marking[i-1].q);
		marked.push_back(marking[i-1].r);
	}else{ 
		marked.push_back(marking[i-1].q);
		marked.push_back(marking[i-1].r);
		marked.push_back(marking[i].p);
		marked.push_back(marking[i].q);
	}

	S[i]=sub_global_transition(S[i],points, q, s, p, r, points_to_check, L, T, energy, R, min_or_max ,marked);

	std::cout<<i<<") simple? "<< S[i].is_simple() <<std::endl;  
	}
}


void merge(std::vector<Polygon_2> S,std::vector<mark> marking , Polygon_2 & my_pol){
	my_pol=S[0] ;
int del=0;
	//S[0].erase(*(S[0].vertices_begin()+findex[q])
	for(int i=0 ; i<S.size()-1 ; i++){		
		Vertex_circulator it_q =S[i+1].vertices_circulator()+findex(S[i+1] ,marking[i].q);
		Vertex_circulator it_r =S[i+1].vertices_circulator()+findex(S[i+1] ,marking[i].r);

		for(Vertex_circulator v=it_r; v!= it_q ; ++v){
			my_pol.insert(my_pol.vertices_circulator()+findex(my_pol ,marking[i].q) ,*v);
		}

//		my_pol.insert(my_pol.vertices_circulator()+findex(my_pol ,marking[i].q) ,*it_q);

		std::cout<<"merge "<<i <<")simple? "<< my_pol.is_simple() <<std::endl;

		///////////////////////////////////////
		if(my_pol.is_simple()==0 && del==0){
			del++;

			std::cout<<"\n\n\n"<<i<<"->p:"<<marking[i].p<<" q: "<<marking[i].q<<",r: "<<marking[i].r<<"\n";
			for(auto it=S[i].vertices_begin() ; it!=S[i].vertices_end(); ++it)
				std::cout<<*it<<std::endl;

			std::cout<<"\n\n\n"<<i+1<<"-> lq:"<<marking[i].q<<"lr:"<<marking[i].r<<"p: "<<marking[i+1].p<<",q: "<<marking[i+1].q<<"\n";
			for(auto it=S[i+1].vertices_begin() ; it!=S[i+1].vertices_end(); ++it)
				std::cout<<*it<<std::endl;

			std::cout<<"\n\n\n"<<"merged polygon: \n";
			
		}
		/////////////////////////////////////////
	}	

}




///////   GLOBAL STEP COPIED DROM UTILS_SIMULATED_ANNEALING.CPP   ////
//////////	WITH FEW CHANGES		/////////////


Polygon_2 sub_global_transition(Polygon_2 &polygon, Points &points,Point_2 &q, Point_2 &s, Point_2 &p, Point_2 &r, Points &points_to_check, int &L, double &T,double &energy, float &R, string min_or_max ,std::vector<Point_2> marked)
{
        int polygonsize = (int) polygon.size();
		Polygon_2 convex_hull, final_polygon;	
		CGAL::convex_hull_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(convex_hull));
		final_polygon.insert(final_polygon.end(), polygon.begin(), polygon.end());
        double energy_diff;
        T=10.0;
        int position_of_q;
		int position_of_s;
		Point_2 t;

///   FOR  SUBDIVISION   ////
Point_2 e1_source = marked[0];   // points of marked edges  
Point_2 e1_target = marked[1];
Point_2 e2_source;
Point_2 e2_target;
if(marked.size()>2){
	e2_source = marked[2];
	e2_target = marked[3];
	}
else {// either first or last polygon in subdivision
	e2_source= Point_2( (*polygon.right_vertex()).x() +1,0) ; // there is not second edge
	e2_target= e2_source;			// so we make e2_source&e2_target points that don't exist in the polygon
}

////////////////////////////

		while(T>0)
		{	
		point_selection:

		do {
			position_of_q = rand() % (polygonsize-3); 
			
			position_of_s = position_of_q +2+ ( rand() % ( polygonsize-2 - position_of_q -1 ) );
			q = *(polygon.vertices_begin()+position_of_q );
			s = *(polygon.vertices_begin()+position_of_s);
			
			
			p = *(polygon.vertices_begin()+position_of_q-1);
			r = *(polygon.vertices_begin()+position_of_q+1);
		}while( q!= e1_target && q!= e2_target && q!= e1_source && q!= e2_source && s!= e1_source && s!= e2_source); 
//  vector<Point_2> marked     e1_source  e1_target  e2 
			Segment_2 pq(p, q);
			Segment_2 qr(q, r);
			
			auto triangle_pqr = CGAL::area(p,q, r);
            polygon.insert(polygon.vertices_begin()+position_of_s+1,q);
            polygon.erase(polygon.vertices_begin()+position_of_q ); 
            
 
            //r = *(polygon.vertices_begin()+position_of_q );
            //s = *(polygon.vertices_begin()+position_of_s-1);
            //q = *(polygon.vertices_begin()+position_of_s);
            t = *(polygon.vertices_begin()+position_of_s+1);
			  
            Segment_2 pr(p,r);
            Segment_2 sq(s, q);
            Segment_2 qt(q,t);		
            
            for(int i=0; i< polygon.size()-1 ; i++)
			{
				 Segment_2 segment1(*(polygon.vertices_begin()+i),*(polygon.vertices_begin()+i+1));
				 if ((intersection(segment1, pr) && 	*(polygon.vertices_begin()+i) != p && *(polygon.vertices_begin()+i) != r && *(polygon.vertices_begin()+i+1) != p && *(polygon.vertices_begin()+i+1) != r)||
					 (intersection(segment1, sq) &&  *(polygon.vertices_begin()+i) != s && *(polygon.vertices_begin()+i) != q && *(polygon.vertices_begin()+i+1) != s && *(polygon.vertices_begin()+i+1) != q) ||
					 (intersection(segment1, qt) && *(polygon.vertices_begin()+i) != q && *(polygon.vertices_begin()+i) != t && *(polygon.vertices_begin()+i+1) != q && *(polygon.vertices_begin()+i+1) != t))
				 {
						polygon.erase(polygon.vertices_begin()+position_of_s);
						polygon.insert(polygon.vertices_begin()+position_of_q, q ); 
						goto point_selection;
				}		    
			}	
			
			convex_hull.clear();
			CGAL::convex_hull_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(convex_hull));
			auto area = abs(polygon.area()), ch_area = abs(convex_hull.area());
			auto cr = (double)area/ch_area;
			//auto energy_diff = abs(points.size()*cr - energy);



			if(min_or_max == "min" ){
				energy_diff = abs(points.size()*cr - energy);
			}
			else{
				energy_diff = abs(points.size()*(1-cr) - energy);
			}
		
			double sub = 1.0/(double)L;
			T = T - sub;
			double variable = pow( energy , -energy_diff/T);  
			if(variable < R)
			{
				final_polygon.insert(final_polygon.vertices_begin()+position_of_s+1,q);
				final_polygon.erase(final_polygon.vertices_begin()+position_of_q ); 
			}
			else
			{
				polygon.erase(polygon.vertices_begin()+position_of_s);
				polygon.insert(polygon.vertices_begin()+position_of_q, q ); 
			}	
		}
		return final_polygon;
}








