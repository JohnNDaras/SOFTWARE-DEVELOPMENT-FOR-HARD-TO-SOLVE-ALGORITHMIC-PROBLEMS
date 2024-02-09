#include "utils_aco.hpp"


// The Edge constructor initializes source and target polygons, heuristic and pheromone values.
Edge::Edge(Polygon_2 source, int insertsion_index, Point_2 new_p, std::string opt){  // opt is optimization parameter.
    this->source = source;
    Polygon_2 target = source;   
    target.insert(target.begin() + insertsion_index, new_p); // Inserting new point at the right index gives the target polygon.
    this->target = target;
    pheromone = 0.1; // Small initial value.
    // Initialize heuristic value depending on max or min.
    if (opt == "max")
        heur = abs(target.area()) - abs(source.area());
    else if (opt == "min")
        heur = 1/(abs(target.area()) - abs(source.area()));
}

bool Edge::operator == (Edge second){// Overload == operator for 2 edges. Used to find already visited or previously candidate edges.
    return this->source == second.source && this->target == second.target && this->insertsion_index == second.insertsion_index;
    // Equal iff they have equal source and target polygons as well as insertion indexes.
}


// Update the edge's pheromone based on the relevant ants' trails and the factor of persistence ro.
void Edge::update_pheromone(Ants ants, double ro){
    double delta = 0;
    // For every trail of every ant, if this edge is found add the solution evaluation to the acculumation trail (delta).
    for (Ant k: ants){
        for (Edge e: k.trail){
            if (*this==e){
                delta += k.evaluation;
            }
        }
    }
    // Update pheromone.
    pheromone = (1-ro)*pheromone + delta;
}


bool Ant::operator < (Ant second){// Overload < operator for 2 ants. Used to find ant with best solution.
    return this->evaluation < second.evaluation;
}

void Ant::evaluate_solution(std::string opt, double max_area){
    if (opt == "max"){
        evaluation = abs(solution.area())/max_area;
    } 
    else if (opt == "min"){
        evaluation = 1/abs(solution.area());
    }
}


// Return whether an insertion of a new point at a specific index is feasible. 
bool feasible_insertion(Point_2 new_p, int index, Polygon_2 polygon){
    polygon.insert(polygon.begin() + index, new_p);
    return polygon.is_simple();
}


// Return whether a new point is feasible given a polygon and the list of remaining points.
bool is_feasible(Point_2 new_p, Polygon_2 polygon, Points points){
    // Create the convex hull consisting of the points of the polygon plus the new point
    Polygon_2 convex_hull; 
    Polygon_2 ch_points = polygon; 
    ch_points.push_back(new_p);
    CGAL::convex_hull_2(ch_points.begin(), ch_points.end(), std::back_inserter(convex_hull));
    // Check if no remaining point lies in the interior of the convex hull;
    for (Point_2 p : points){
        if (convex_hull.has_on_bounded_side(p)) 
            return false;       
    }
    // Check if the point has at least one feasible insertion spot (at least one visible polygon edge).
    for (int i=0; i<polygon.size(); i++){
        if (feasible_insertion(new_p, i, polygon))
            return true;
    }
    return false;
}


// Select next edge based on the ant colony probability equation.
Edge select_edge(Edges edges, Polygon_2 polygon, double alpha, double beta){ 
    double sum = 0;
    // Find the sum of (pheromone^a)*(heur_value^b) over all candidate edges.
    for (Edge e: edges){
        if (e.source == polygon)
            sum += pow((e.pheromone), alpha)* pow((e.heur), beta);        
    }
    // Select edge based on its cumulative probability.
    double cumulative = 0;
    double random = (double)rand()/(double)RAND_MAX;  // Random number from 0 to 1.
    for (Edge e : edges){
        cumulative += (pow((e.pheromone), alpha)* pow((e.heur), beta))/sum; // Accumulate probabilities with each iteration.
        if (random <= cumulative)
            return e;
    }
    // If no edge was selected, throw exception.
    throw std::domain_error("Edge probabilities should add up to 1\n");
}

// Follow edges constructing partial solution until a solution is built.
void Ant::build_solution(Polygon_2 polygon, Edges &edges, Points points, std::string opt, double alpha, double beta){
    while (!points.empty()){
        // Store all feasible points for current polygon.
        Points feasible_points;
        for (Point_2 p: points){    
            if (is_feasible(p, polygon, points))
                feasible_points.push_back(p);
        } 
        // Select feasible point at random and remove it from the remaining points.
        Point_2  new_point = feasible_points[rand()%feasible_points.size()];
        points.erase(std::remove(points.begin(), points.end(), new_point), points.end());

        Edges candidates; // Candidate graph edges to follow.
        // Find all feasible insertion spots and save the corresponding candidate edges.
        for (int i=0; i<polygon.size(); i++){ 
            if (feasible_insertion(new_point, i, polygon)){
                Edge e = Edge(polygon, i, new_point, opt);
                // If Edge alread exists (has been visited before), push that one to candidates. It already has a changed pheromone value.
                auto it = std::find(edges.begin(), edges.end(), e);
                if (it != edges.end())
                    candidates.push_back(*it);
                else
                    candidates.push_back(e);
            }
        }
        // Select edge and update the polygon and trail of the ant.
        Edge selection = select_edge(candidates, polygon, alpha, beta);
        polygon = selection.target;
        trail.push_back(selection);           
    }
    solution = polygon; // Final solution
}


// Update the pheromone of the edges of the trails followed by the ants.
void update_trails(Ants ants, Edges &edges, Ant best_ant, double ro, bool elitism){
    // If elitism == 1, only the trail of the best ant is reinforced.
    if (elitism){
        // Clear ants and insert only the best ant in the vector.
        ants.clear();
        ants.push_back(best_ant);
    }
    // Clear edges and replace them with the trails of all the ants (no duplicates).
    edges.clear();
    for(Ant k: ants){
        for (Edge e: k.trail){
            if(std::find(edges.begin(), edges.end(), e) == edges.end())
                edges.push_back(e);
        }
    }
    // Update pheromone for every edge based on either all the trails, or based only on the best ant's trail.
    for (int i=0; i<edges.size(); i++)
        edges[i].update_pheromone(ants, ro);
}
