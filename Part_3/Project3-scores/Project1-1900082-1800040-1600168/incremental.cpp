#include "utils_inc.hpp"

Polygon_2 incremental(Points points, std::string initialization, std::string edge_selection){

    Polygon_2 polygon, convex_hull;
    srand(time(0));

    // 1) SORT THE POINTS
    if(initialization == "1a")
        sort(points.begin(), points.end(), descx);
    else if(initialization == "1b")
        sort(points.begin(), points.end(), ascx);
    else if(initialization == "2a")
        sort(points.begin(), points.end(), descy);
    else
        sort(points.begin(), points.end(), ascy);

    // 2) INITIALIZE POLYGON AND CONVEX HULL AS THE TRIANGLE (P1,P2,P3)
    for(int i=0; i<3; i++)
        polygon.push_back(points[i]);

    if (polygon.is_clockwise_oriented())
        polygon.reverse_orientation(); // every polygon will be counter-clockwise oriented

    CGAL::convex_hull_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(convex_hull));

    // 3) Insert every remaining point to the polygon iteratively
    for(int i = 3; i < points.size(); i++){

        Point_2 point = points[i]; //next point for insertion
        Segments red_edges; //visible edges of the convex hall from the next point
        Segments visible_edges; //visible edges of the polygon line from the next point

        // Find red edges
        for (EdgeIterator ei = convex_hull.edges_begin(); ei != convex_hull.edges_end(); ++ei){
            if (is_visible(point, *ei, convex_hull)){
                red_edges.push_back(*ei);
                //std::cout<<"Red edge:  "<<*ei <<"\n";
            }
        }

        // Find visible edges behind every red edge
        for (Segment_2 re: red_edges){
            EdgeCirculator source = polygon.edges_circulator() + find_vertex(re.source(), polygon);
            EdgeCirculator target = polygon.edges_circulator() + find_vertex(re.target(), polygon);
            for (EdgeCirculator ei = source; ei != target; ++ei){
                if (is_visible(point, *ei, polygon)){
                    visible_edges.push_back(*ei);
                    //std::cout<<"Visible edge:  "<<*ei <<"\n";
                } 
            }
        }

        // Select edge and insert the point
        Segment_2 selection = select_edge(visible_edges, edge_selection, point);
        int insertion_index =  find_vertex(selection.target(), polygon);
        polygon.insert(polygon.vertices_begin() + insertion_index, point);
        convex_hull.clear();
        CGAL::convex_hull_2(polygon.vertices_begin(), polygon.vertices_end(), std::back_inserter(convex_hull));
        //std::cout <<"\n Point "<< i << " inserted to polygon\n";
    }

    return polygon;
}