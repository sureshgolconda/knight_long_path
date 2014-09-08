//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .H file to define k_getpath_t class. Loads precomputed paths from
   database and provides an interface for query.

   @author Suresh Golconda
*/

#ifndef KNIGHT_K_GETPATH_H
#define KNIGHT_K_GETPATH_H

#include <string>
#include "k_util.H"

namespace knight_path_t
{
  class k_getpath_t
  {
  private:

    tour_path_list_t m_6x6_para;
    tour_path_list_t m_6x8_para;
    tour_path_list_t m_8x6_para;
    tour_path_list_t m_8x8_para;

    tour_path_list_t m_6x6_odd_para; // one less path size

    std::string m_6x6_filename;
    std::string m_6x8_filename;
    std::string m_8x6_filename;
    std::string m_8x8_filename;

    std::string m_6x6_odd_filename;


    /** Scans the given file and loads the paths rad to pl*/
    void load_parameters(tour_path_list_t &pl, std::string filename);

    /** Scans the given string to read one record of path and stores
	in tp*/
    bool scan_path_string (tour_path_t &tp, std::string indata_);


    /** Finds a path from given start-end coordinate from within given
	database list tp_list. @return False,if could not find any
	path, else returns true.*/
    bool get_path_board(tour_path_list_t const &tp_list,
			int sx, int sy, int ex, int ey,
			tour_path_t &tp);

  public:
    /** Set paths to pre-computed paths*/
    k_getpath_t()
    { // initialize paths to file
      m_6x6_filename = "../data/6x6.good";
      m_6x8_filename = "../data/6x8.good";
      m_8x6_filename = "../data/8x6.good";
      m_8x8_filename = "../data/8x8.good";

      m_6x6_odd_filename = "../data/6x6_odd.good";

    }

    /** Load the pre-computed parameter files*/
    void load_all_para()
    {
      load_parameters (m_6x6_para, m_6x6_filename);
      load_parameters (m_6x8_para, m_6x8_filename);
      load_parameters (m_8x6_para, m_8x6_filename);
      load_parameters (m_8x8_para, m_8x8_filename);

      load_parameters (m_6x6_odd_para, m_6x6_odd_filename);
    }


    /** Searches and returns a path on board of size (maxx, maxy) from
	given start-end location. Provides the path as a vector of
	nodes. @returns false if failed, else returns true*/
    bool get_path (int maxx, int maxy, int sx, int sy,
		   int ex, int ey, std::vector<node_t> &node_list);

    /** Searches and returns a path on board of size (maxx, maxy) from
	given start-end location. Provides the path as a
	tour_path_t. @returns false if failed, else returns true*/
    bool get_path (int maxx, int maxy, int sx, int sy,
		   int ex, int ey, tour_path_t &tp);





    // --- Functions for testing ---------
    bool test_all_paths();
    bool test_paths_for_board(int maxx, int maxy, bool allow_same_color = false);

    void print_tour_path (tour_path_list_t const &pl);

  };
};

#endif
