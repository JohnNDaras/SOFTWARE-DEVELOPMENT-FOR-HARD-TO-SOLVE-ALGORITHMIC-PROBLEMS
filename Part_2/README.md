**<p align = center>Polygon Optimization and Generation: Comprehensive Approach with Local Search, Simulated Annealing, Subdivision, and Ant Colony Optimization**

Konstantinos Kitsos - 1900082

Ioannis Daras - sdi1800040

Georgios Tzathas - sdi1600168
            
**Compilation**

    cgal_create_CMakeLists -s optimal_polygon

    cmake -DCGAL_DIR=/usr/lib/CGAL .

    Copy the COPYtoCmakeLists.txt to CMakeLists.txt

    make

**LOCAL SEARCH:**

Implementation that examines, for each edge, for each possible path with a length < L

a) Main algorithm: local.cpp
Functions used: utils_local.cpp

b) From utils_local.hpp

     typedef struct info {
     Point_2 u1;
     Point_2 u2;
     Points V; // Path
     int DA; // Difference in area
     } info;

(u1, u2) represent the edge, V the path, and DA the area difference. Also we define DA in local.cpp as the area difference between two consecutive polygons.

c) std::vector<info> T:
This array stores each beneficial change. When examining each edge, we sort and apply the changes given by T[0]. In the case of max, we sort based on the largest DA difference, while, since DA is negative, in the case of min, we sort from largest to smallest.

d) The changes are applied with apply_changes1. First, apply it in each iteration to the temp_p polygon (saving only the beneficial changes in T). Then, as mentioned in 'c', after going through each edge, apply the best change (taking the best changes until DA < threshold).

SIMULATED ANNEALING.

- Local and Global transition

    - Initially, select the convex hull algorithm for the initial polygonization of a point set from the 1st task. Calculate the "energy" of the initial state with the Initial_state_energy function.

(b) Transition to a new state through a switch with
i) local order change step (local_transition function) between two points in the polygonal chain without violating the simplicity of the polygonal line. For the validity check of the change, use the CGAL kd-Tree structure by searching in the bounding box according to the theory with the command tree.search(std::back_inserter(points_to_check), fib), where fib is the bounding box and points_to_check are the points within it (q and r points are excluded from points_to_check). Check if the edges, at least one end of which is inside the bounding box (excluding consecutive edges of the requested edges because the intersection occurs exactly at the end).
ii) global order change step based on random points q, s of the chain (global_transition function). If pq, qr, and st are edges, connect points p and r, and then insert q between s and t. For the validity check of the transition, exhaustively check that the new edges do not intersect any other edge of the polygonal line (excluding consecutive edges of the requested edges because the intersection occurs exactly at the end).

(c) Then calculate the energy difference of the new state and accept or reject the transition based on the Metropolis criterion e^(-ΔE/T) > R(0,1) [random positive number <1 calculated at the beginning of the algorithm]. In the initial state, the temperature is set equal to 1 and decreases by 1/L at each step, where L is given by the user.

2.2) SUBDIVISION

a) General description: Subdivision does not perform either the local or global step in the code. It handles the remaining tasks that do not directly involve simulated annealing: division into individual polygons, marking of edges, application of polygonization (via incremental), and merging of polygons into one.

b) Main algorithm: subdivision.cpp
Functions used: utils_sub.cpp
Function incremental2: sub_incremental.cpp

c) Marking:
Structure mark:
Consists of p q r (lq eventually only in checks)

Marking is a vector of n-1 marks
!!! Attention!!!: In a polygon S[i], r is marking[i-1].r, and the left q: marking[i-1].q

d) Conditions:
The conditions for pqr are checked in the conditions function, and the points are transferred from the next to the previous polygon via transfer_points (we also set a limit on the points that can be transferred, but it is rare to need).

e) Application of incremental

e1) incrementalize-General:
Remove the marked points q, r, p, and q'
Apply the incremental to each polygon (from task 1:"Algorithmic Approaches to Polygon Formation: A Comparative Study of Incremental, Convex Hull, and Onion Methods")

Add qr and pq' through insert_edge

e2) incrementalize-Special case !!!!!! if (marking[i].p == marking[i-1].r):
The sub_incremental.cpp -> incremental2 with the changed incremental algorithm and the lines in the code of the utils_sub.cpp -> incrementalize function concern ONLY a special case of a polygon where r = p, i.e., the lower hull is only three points p, r, p'.

Then, in a new polygon, we start with these 3 points. Then add the lower hull of the inner points. This will be the initial polygon, and the only change in incremental2 is to accept the original polygon as an argument instead of creating a triangle.

Call incremental2 with sorting: y ascending

3) ANT COLONY OPTIMIZATION

*NOTE: I define in utils_aco.hpp a new class Edge, which represents an edge in the space of partial solutions (e.g., connecting a possible triangle with a feasible quadrilateral), and a class Ant, which represents an ant walking on the edges and constructing a solution. An edge stores heuristic value, pheromone value, and the polygons at its ends (source and target). Implementations of all functions can be found in utils_aco.cpp.

All functions are well-commented:

    Edge constructor: Properly initializes an edge; you can change the initial pheromone value to whatever you want.

    Overloaded operator == for edges: Needed for the std::find function to find "identical" edges, i.e., those that have been visited before.

    Edge::update_pheromone: Takes the rho and all ants as parameters to determine which edges were visited, and if it finds itself, it increases Δ. Changes the pheromone based on the relative equation.

    Overloaded operator < for ants: Needed for the std::max_element function to find the best solution. Ants are compared based on the evaluation of their solutions.

    Ant::evaluate_solution: Evaluates the solution of an ant based on whether the max or min parameter is provided.

    feasible_insertion: Similar to checking if an edge of a polygon is visible from a point, but this function tests the insertion at an index and returns true if the constructed polygon is simple. I could use the is_visible function, which I've already created for the first task, but this approach seems a bit more efficient.

    is_feasible: Checks if a point is feasible with respect to the polygon, using two criteria. All surplus points should NOT be inside the convex hull of the polygon U new_point. Also, there must be at least one feasible insertion for the point (visible edge).

    select_edge: Chooses an edge from the candidates based on the probability equation of the ant colony and returns it. It generates a random number from 0 to 1 and returns another edge depending on the range it falls into. For example, an edge with a probability of 0.1 may be in the range 0.4 - 0.5.

    Ant::build_solution: Needs the edges as parameters to see which ones have been visited before. The ant builds the solution as follows: it finds all feasible points and selects one randomly. Then, it finds all feasible insertions and stores them as candidate edges. If it encounters an edge that has been visited before, it keeps that one instead of the newly created one. With all candidate edges, it selects one and continues the process until no new points are left.

    update_trails: Updates the pheromone for each edge belonging to the trails of the ants. If elitism is present, only the path of the best ant is considered to see which edges will be strengthened; otherwise, all paths.

In aco.cpp, the algorithm initializes a triangle randomly and then follows the transparency algorithm for L cycles and K ants, where K = points.size()/4 + 1 (you can change it). Each ant builds its solution, evaluates it, and the best ant is selected in each cycle. In the end, it returns the best polygon from all cycles.
