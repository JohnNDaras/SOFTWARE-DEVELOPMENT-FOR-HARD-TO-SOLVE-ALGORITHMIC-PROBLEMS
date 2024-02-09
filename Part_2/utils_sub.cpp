#include "utils_sub.hpp"


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

bool conditions(mark m){
	return ( (m.p.y()<m.q.y()) && (m.r.y()< m.q.y())); 
}

void transfer_point( std::vector<Polygon_2> &S , std::vector<mark> &mark,int i){  // we transfer from i+1th to the ith polygon
        if(S.size()-1< i+1) std::cout<<"invalid findex \n";
        
		//if( mark[i+1].left_q==mark[i].q) 
		S[i+1].erase(S[i+1].vertices_begin());   

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
				break;
			}

		}
	}
}



void incrimentalize(std::vector<Polygon_2> &S, std::vector<mark> marking){

	S[0].erase(S[0].vertices_begin()+findex(S[0],marking[0].p));
	S[0].erase(S[0].vertices_begin()+findex(S[0],marking[0].q));

	S[0]=incremental(S[0].vertices(),"1b","1");
	insert_edge(S[0] , marking[0].p ,  marking[0].q);
/**/
	for(int i=1; i<S.size()-1 ;++i){	

	  if(marking[i].p == marking[i-1].r){   //there is rare condition where lh is only three points ,   lq -> r ->q
		
		S[i].erase(S[i].vertices_begin()+findex(S[i],marking[i-1].q));
		S[i].erase(S[i].vertices_begin()+findex(S[i],marking[i-1].r));
		S[i].erase(S[i].vertices_begin()+findex(S[i],marking[i].q));
		
		Polygon_2 starting_pol;
		starting_pol.push_back(marking[i-1].q);
		starting_pol.push_back(marking[i-1].r);
		starting_pol.push_back(marking[i].q);

		Points lh;
		CGAL::lower_hull_points_2(S[i].vertices_begin(), S[i].vertices_end(), std::back_inserter(lh));

		auto it = lh.end();
		while (it != lh.begin())
		{
     	--it;
		starting_pol.push_back(*it);
		S[i].erase(S[i].vertices_begin() + findex(S[i],*it));

		}

		Points rest =S[i].vertices();

		S[i]=incremental2(rest, starting_pol.vertices() ,"2b","2");/**/
		starting_pol.clear();
	  }

	  else{
		S[i].erase(S[i].vertices_begin()+findex(S[i],marking[i-1].q));
		S[i].erase(S[i].vertices_begin()+findex(S[i],marking[i-1].r));

		S[i].erase(S[i].vertices_begin()+findex(S[i],marking[i].p)); 
		S[i].erase(S[i].vertices_begin()+findex(S[i],marking[i].q));

		S[i]=incremental(S[i].vertices(),"1b","2");

		insert_edge(S[i] , marking[i-1].q ,  marking[i-1]. r);
		insert_edge(S[i] , marking[i].p  ,marking[i].q);
	  }
	}

	int n=S.size()-1;
	S[n].erase(S[n].vertices_begin()+findex(S[n],marking[n-1].q));
	S[n].erase(S[n].vertices_begin()+findex(S[n],marking[n-1].r));

	S[n]=incremental( S[n].vertices(),"1b","2");
	insert_edge(S[n] , marking[n-1].q ,  marking[n-1].r);/**/

	}

void merge(std::vector<Polygon_2> S,std::vector<mark> marking , Polygon_2 & my_pol){
	my_pol=S[0] ;

	//S[0].erase(*(S[0].vertices_begin()+findex[q])
	for(int i=0 ; i<S.size()-1 ; i++){		
		Vertex_circulator it_q =S[i+1].vertices_circulator()+findex(S[i+1] ,marking[i].q);
		Vertex_circulator it_r =S[i+1].vertices_circulator()+findex(S[i+1] ,marking[i].r);

		for(Vertex_circulator v=it_r; v!= it_q ; ++v){
			my_pol.insert(my_pol.vertices_circulator()+findex(my_pol ,marking[i].q) ,*v);
		}
		//std::cout<<"simple? "<< my_pol.is_simple() <<std::endl;
	}	


}
