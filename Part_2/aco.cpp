#include "utils_aco.hpp"

Polygon_2 aco(Points points, std::string opt, int L, double alpha, double beta, double ro, bool elitism){ 

    Polygon_2 polygon, convex_hull;
    Edges edges;
    srand(time(0)); // Rand seed.

    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(convex_hull));
    double max_area = convex_hull.area(); // Upper bound for polygon area.
    int K = points.size()/4 + 1; // Number of ants.


    ////// INITIALIZE TRIANGLE RANDOMLY //////

    std::random_shuffle(points.begin(), points.end()); // Randomize points.
    // Insert first 2 points.
    for (int i=0; i<2; i++){
        polygon.push_back(points[0]);
        points.erase(points.begin());
    }

    // Select the 3rd point so that no remaining point is inside the triangle (feasible).

        for (int i=0; i<points.size(); i++){
            if (is_feasible(points[i], polygon, points)){
                polygon.push_back(points[i]);
                points.erase(points.begin() + i);
                break;
            }
        }


        Ants ants; // Create ant vector.
        Polygon_2 best_solution;
        Ant best_ant;
        double best_eval = 0;

        for (int l=0; l<L; l++){
            for (int k=0; k<K; k++){
                Ant ant;
                ant.build_solution(polygon, edges, points, opt, alpha, beta);
                ant.evaluate_solution(opt, max_area);
                ants.push_back(ant);
            }

            // Save best solution so far in all cycles.
            best_ant = *max_element(ants.begin(), ants.end());
            if(best_ant.evaluation > best_eval){
                best_solution = best_ant.solution;
                best_eval = best_ant.evaluation;
            }
            // Update trails and clear this cycle's ants.
            update_trails(ants, edges, best_ant, ro, elitism);
            ants.clear();
    }
    // Ensure positive area.
    if (best_solution.is_clockwise_oriented())
        best_solution.reverse_orientation();
    return best_solution;
}
