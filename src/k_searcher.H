//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .H file to define k_searcher_t class, which implements functions
   required for generating database of paths for smaller grids such as
   6x6, 6x8, 8x6, 8x8.

   @author Suresh Golconda
*/

#ifndef KNIGHT_SEARCHER_H
#define KNIGHT_SEARCHER_H

#include <string>

#include "k_util.H"
#include "k_tour.H"


namespace knight_path_t
{

  class k_searcher_t
  {
  private:
    /**
       returns true if end point comes after start
    */
    bool start_end_ordered(int sx, int sy, int ex, int ey);

    /** Returns number of possible start-end combinations that should
	be computed*/
    int  num_possible_solution_from (int maxx, int maxy);


    /**
       scans the input file lines such as following to get start and end coordinates
       COUNT_EXCEEDED 11, 110000000 (5 6) to (2 6)
       COUNT_EXCEEDED 11, 110000000 (5 6) to (5 1)
    */
    void scan_custom_set(std::string filename, parameter_list_t &pl,
			 bool reverse_start_end = false);



  public:

    /** Reads iput file to read start-end node pairs, then compute
	path for those pairs*/
    void search_custom_set(int maxx, int maxy,
			   std::string filename, int search_limit);

    /** Compute longest path for all possible destination nodes,
	starting from given (sx, sy).*/
    int solutions_from (int sx, int sy, k_tour_t &kt, int maxx, int maxy);

  };
}
#endif
