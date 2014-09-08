//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .H file to define k_path_adapter_t class. Adapter function for
   getting path. Can either load from pre-computed results or perform
   fresh search using k_tour_t.

   @author Suresh Golconda
*/


#ifndef KNIGHT_PATH_ADAPTER_H
#define KNIGHT_PATH_ADAPTER_H

#include <string>
#include <vector>

#include "k_util.H"
#include "k_tour.H"
#include "k_getpath.H"


namespace knight_path_t
{

  class k_path_adapter_t
  {
    // static const int m_search_limit_1 =10;
    // static const int m_search_limit_2 =100;

    std::vector<int> m_search_limits;

    // objects to search
    k_tour_t m_tour_6x6;
    k_tour_t m_tour_6x8;
    k_tour_t m_tour_8x6;
    k_tour_t m_tour_8x8;

    // object to get precomputed values
    k_getpath_t m_pre_paths;
    bool m_use_db; // if true, uses k_getpath, else pre-computes

    /** Loads all required parameter*/
    void initialize();

    /** Converts given path as matrix, to vector of nodes format*/
    bool convert_board_path_to_list (solution_t const &sol_grid,
				     std::vector <node_t> &sol_list);

  public:
    /** Takes in one bool argument which specifies if to use
	pre-computed database (if true).*/
    k_path_adapter_t(bool use_db):m_tour_6x6 (6,6, 20),
				  m_tour_6x8 (6,8, 20),
				  m_tour_8x6 (8,6, 20),
				  m_tour_8x8 (8,8, 20)
    {

      m_use_db = use_db;
      m_pre_paths.load_all_para();
      m_search_limits.push_back (2);
      m_search_limits.push_back (10);
      m_search_limits.push_back (30);
      m_search_limits.push_back (200);

    }

    /** Finds the path for given board of size (maxx, maxy), from
	given start point to given end point.  @returns true if
	successful. Else return false*/
    bool get_path(int maxx, int maxy,
		  int sx, int sy,
		  int ex, int ey,
		  std::vector<node_t> &path);

  };
};

#endif
