//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .C file with implementations of k_path_adapter_t class's methods.

   @author Suresh Golconda
*/

#include <cassert>
#include <algorithm>
#include "k_path_adapter.H"
#include "k_util.H"

using namespace knight_path_t;
using namespace std;

void k_path_adapter_t::initialize()
{
  /// Load all parameters. Even if disabled to use database. Might add
  /// feature to toggle the decision to use DB, inbetween.
  m_pre_paths.load_all_para();

}

bool k_path_adapter_t::get_path(int maxx, int maxy,
				int sx, int sy,
				int ex, int ey,
				vector<node_t> &path)
{

  if (m_use_db)
    {
      // printf("PATH from db\n");
      return m_pre_paths.get_path (maxx, maxy, sx, sy, ex, ey, path);
    }
  else //generate path
    {
      k_tour_t *kt = NULL;

      // get right k_tour. Maintaining the objects as member variables
      // so as to avoid creating a new instance for every call.
      if ((maxx == 6) && (maxy == 6))
	kt = &m_tour_6x6;
      else if ((maxx == 6) && (maxy == 8))
	kt = &m_tour_6x8;
      else if ((maxx == 8) && (maxy == 6))
	kt = &m_tour_8x6;
      else if ((maxx == 8) && (maxy == 8))
	kt = &m_tour_8x8;

      assert (kt != NULL);


      solution_t sol (maxx, maxy);

      vector<node_t> start_seq;
      node_t node;
      node.posx = sx;
      node.posy = sy;
      start_seq.push_back (node);

      printf("Num limit %lu\n", m_search_limits.size());
      /// Iterate the search with different limit values. Initial
      /// lower values provides a chance to search path from end to
      /// start, which showed quicker results sometimes.
      for (int limit_i=0; limit_i < m_search_limits.size(); limit_i++)
	{
	  /// Get search limit
	  int search_limit = m_search_limits[limit_i];
	  kt->set_search_limit (search_limit); //set search limit
	  printf("Searching with limit %d\n", search_limit);

	  //--- initialize
	  sol.reset(-1);
	  kt->reset();

	  //if (!kt->solveKT(sx, sy, ex, ey, sol) || true)
	  if (!kt->solveKT(sx, sy, ex, ey, sol))
	    { /// If failed, try searching from end to start
	      printf("Reverse searching at limit %d\n", search_limit);

	      kt->reset();
	      sol.reset(-1);
	      if (!kt->solveKT(ex, ey, sx, sy, sol))
		{
		  printf("FAILED_SEARCH: %d %d %d %d\n",sx, sy, ex, ey);
		}
	      else
		{ /// if reverse search found path, reverse the resultant path.
		  convert_board_path_to_list (sol, path);
		  //reverse the path
		  reverse (path.begin(), path.end());
		  //print_path (path);
		  return true;

		}
	    }
	  else
	    {
	      convert_board_path_to_list (sol, path);
	      print_path (path);
	      return true;
	    }
	}
      printf("DONE iterating on limit\n");

      // if (!kt->solveKT_new (sx, sy, ex, ey, sol, start_seq))
      // 	return false;

    }
  return true;
}




bool k_path_adapter_t::convert_board_path_to_list (solution_t const &sol_grid,
						   vector <node_t> &sol_list)
{
  // Number of nodes to expect on the grid
  int nodes_left = sol_grid.DIM_SIZE_X * sol_grid.DIM_SIZE_Y;
  int default_value = -1;


  // walk over the default value nodes and decrease the count
  for (int x=0; x< sol_grid.DIM_SIZE_X; x++)
    for (int y=0; y<sol_grid.DIM_SIZE_Y; y++)
      {
	if (sol_grid.m_values[x][y] == default_value)
	  nodes_left--;
      }

  int start_value = 0;
  int upper_limit = start_value +
    (sol_grid.DIM_SIZE_X * sol_grid.DIM_SIZE_Y);

  // repeat for possible value, till nodes_left is zero
  for (int value = start_value; (value < upper_limit) &&
	 (nodes_left > 0); value++)
    {
      bool found = false;
      for (int x=0; (x< sol_grid.DIM_SIZE_X) && !found; x++)
	for (int y=0; (y<sol_grid.DIM_SIZE_Y) && !found; y++)
	  {
	    if (sol_grid.m_values[x][y] == value)
	      {
		found = true;
		node_t node;
		node.posx = x;
		node.posy = y;
		sol_list.push_back (node);

		nodes_left--;
	      }

	  }
    }
  return (nodes_left == 0);
}
