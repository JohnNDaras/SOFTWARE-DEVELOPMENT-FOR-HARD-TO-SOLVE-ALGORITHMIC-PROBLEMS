#include "utils_inc.hpp"

// sorting order for x and y
bool ascx(Point_2 a, Point_2 b){return a.x() < b.x();}
bool descx(Point_2 a, Point_2 b){return a.x() > b.x();}
bool ascy(Point_2 a, Point_2 b){return a.y() < b.y();}
bool descy(Point_2 a, Point_2 b){return a.y() > b.y();}

// Returns an index to the position of a vertex in a polygon
int find_vertex(Point_2 p, Polygon_2 pol){
    auto it = find(pol.begin(), pol.end(), p); 
    return it - pol.begin();
}

// Returns true if an edge is visible from point p given polygon pol
bool is_visible(Point_2 p, Segment_2 edge, Polygon_2 pol){ 
    Segment_2 seg1 = Segment_2(p, edge.source());
    Segment_2 seg2 = Segment_2(p, edge.target());
    int intersect_count1 = 0, intersect_count2 = 0, intersect_count3 = 0;
    for (EdgeIterator ei = pol.edges_begin(); ei != pol.edges_end(); ++ei){
        if (do_intersect(*ei, seg1))
            intersect_count1++;
        if (do_intersect(*ei, seg2))
            intersect_count2++;
    }
    if (intersect_count1 != 2 || intersect_count2 != 2)
        return false;
    return true;
}

// Selects either random, or min area or max area
Segment_2 select_edge(Edges visible_edges, std::string selection, Point_2 p){
    if (selection == "1"){ // random
        return visible_edges[rand()%visible_edges.size()];
    }
    else if (selection == "2"){ // min area
        Segment_2 min;
        int min_area = std::numeric_limits<int>::max();
        for (Segment_2 e: visible_edges){
            Triangle_2 t(p, e.source(), e.target());
            int area = abs(t.area());
            if (area < min_area){
                min = e;
                min_area = area;
            }
        }
        return min;
    } 
    else { // max area
        Segment_2 max;
        int max_area = 0;
        for (Segment_2 e: visible_edges){
            Triangle_2 t(p, e.source(), e.target());
            int area = abs(t.area());
            if (area > max_area){
                max = e;
                max_area = area;
            }
        }
        return max;
    }
}