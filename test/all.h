
/**
 * @file all.h
 *
 * @brief This file describe the functions proposed for all the defined graphs for "Complexity and Calculability" housework
 **/


/**
 * @brief returns the number of vertices
 * @return the number of vertices in the graph
 **/
int orderG();

/**
 * @brief returns the number of edges
 * @return the number of edges in the graph
 **/
int sizeG();

/**
 * @brief States if the two vertices are adjacent
 * @param two vertex number, u and v. These values should be between 0 and order()-1
 * @return 1 if u and v are adjacent, 0 otherwise. Also returns 0 if one of the vertex number is not in the graph range.
 **/
int are_adjacent(int u, int v);

