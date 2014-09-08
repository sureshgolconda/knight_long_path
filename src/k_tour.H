//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .H file with code for finding knight's touring path. Here by
   "TOUR"ing means, hamilton path covering all the cells exactly
   once. With an extension of hamilton path with one-less path.

   This class searches grid for such paths. EXTENSIVELY tested on
   grids of size: 6x6, 6x8, 8x6, and 8x8.

   Defines two class: solution_t and k_tour_t

   @author Suresh Golconda
*/

#ifndef KNIGHT_TOUR_H
#define KNIGHT_TOUR_H

#include <stdio.h>
#include <csignal>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <queue>
#include <vector>
#include <algorithm>

#include "k_util.H"


namespace knight_path_t
{
  class solution_t
  {
  public:
    /// dimensions along either axis
    const int DIM_SIZE_X;
    const int DIM_SIZE_Y;


    /// Varaible to hold (cost) values of each cell.
    int **m_values;

    solution_t (int dim_size_x, int dim_size_y):
      DIM_SIZE_X (dim_size_x), DIM_SIZE_Y (dim_size_y)
    {

      /// Allocates memory for 2D array, which is delete in destructor
      m_values = new int* [DIM_SIZE_X];
      for (int i=0; i< DIM_SIZE_X; i++)
	m_values[i] = new int [DIM_SIZE_Y];
    }


    /** A utility function to print cost values of the grid's cells*/
    void display_board();


    /** Returns x, y index of cell where solution starts */
    bool get_start_index (int &xi, int &yi) const;


    /** Returns x, y index of cell where solution ends */
    bool get_end_index (int &xi, int &yi) const;


    /** Searches for given coordinates (x,y) of node with given
	(val).  Function returns false if could not find the node. Else
	return true.*/
    bool search_for (int val, int &xi, int &yi) const;


    /**  Reset all the nodes/cells to given value   */
    void reset (int value);

    solution_t(solution_t const&) = delete;
    void operator=(solution_t const &) = delete;

    ~solution_t()
    {
      for (int i=0; i< DIM_SIZE_X; i++)
	delete[] m_values[i];
      delete[] m_values;

    }
  };





  /** 
   * k_tour_t class definition 
   */
  /// This is tour_t def
  class k_tour_t
  {
  private:
    //static int const N=8;

    int const DIM_SIZE_X;
    int const DIM_SIZE_Y;

    bool m_for_knight;

    unsigned int m_steps;
    unsigned int m_big_step;
    bool m_abort;
    int m_sx;
    int m_sy;

    int m_search_limit;

    static const int MAX_MOVES = 8;

    static const int m_knight_xMove[MAX_MOVES];
    static const int m_knight_yMove[MAX_MOVES];

    static const int m_king_xMove[MAX_MOVES];
    static const int m_king_yMove[MAX_MOVES];


    /** Child node representing the neighboring node explored from
	given node. Used for searching path.*/
    struct child_t
    {
    public:
      int index;
      int count;

      bool operator < (child_t const &sn_) const
      {
	return (sn_.count > count);
      };
    };


    /** typedef for easy declaration of variables for storing list of
	nodes to explore*/
    typedef std::deque <child_t> child_list_t;
    int num_solutions;


    /** variable to point to the list of possible moves during
	exploration. It can point to either an array with knight's
	movement or an array with king's movement. king's movement
	list is used when searching at sub-board level. knight's
	movement is used when searching for knight's movement within a
	sub-board */
    const int *xMove;
    const int *yMove;


    /* A utility function to check if i,j are valid indexes for N*N chessboard */
    int isSafe(solution_t const &sol, int x, int y); //DIM_SIZE_X][DIM_SIZE_Y])



    /** Linear print of record for book keeping     */
    void print_solution(solution_t const &sol, int ex, int ey);




    /** Returns true if in the solution, node (x, y) is reached via
	diagnal link. Applies only when path is not of knight_type. */
    bool reached_via_diagnal (solution_t const &sol, int x, int y);



    /** Gets the order in which to visit the children node */
    void get_ordered_children(solution_t const &sol,
			      int sx, int sy, child_list_t &cl);





    /* A recursive utility function to solve Knight Tour problem */
    //template <size_t rows, size_t cols>
    bool solveKTUtil(int x, int y, int ex, int ey,
		     int movei, int desired_sol_movei, solution_t &sol);



  public:
    k_tour_t(int xim, int ydim,
	     int search_limit, bool for_knight = true): DIM_SIZE_X (xim),
							DIM_SIZE_Y (ydim),
							m_search_limit (search_limit)
    {
      m_for_knight = for_knight;

      if(m_for_knight)
	{
	  xMove = &m_knight_xMove[0];
	  yMove = &m_knight_yMove[0];
	}
      else
	{
	  xMove = &m_king_xMove[0];
	  yMove = &m_king_yMove[0];
	}

      reset();
    }


    /** Resets the member variables for new search*/
    void reset();

    /** Reset the search depth limit */
    void set_search_limit(int limit)
    { m_search_limit = limit; }

    /* This function solves the Knight Tour problem using Backtracking.  This
       function mainly uses solveKTUtil() to solve the problem. It returns false if
       no complete tour is possible, otherwise return true and prints the tour.
       Please note that there may be more than one solutions, this function
       prints one of the feasible solutions.  */
    bool solveKT(int sx, int sy, int ex, int ey, solution_t &sol);


    //todo: merge solveKT_new() and solveKT()

    bool solveKT_new(int sx, int sy, int ex, int ey, solution_t &sol,
		     std::vector <node_t> const &start_seq);

    bool solveKT_bi (int sx, int sy, int ex, int ey, solution_t &sol,
		     std::vector <node_t> const &start_seq);

  };
};

#endif
