//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .H file to define k_32_search_t class, which implements search
   capabilities specifically 32x32.

   @author Suresh Golconda
*/



#ifndef KNIGHT_32_SEARCH_H
#define KNIGHT_32_SEARCH_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

#include "k_tour.H"
#include "k_getpath.H"
#include "k_sub_board.H"
#include "k_path_adapter.H"

using namespace std;
namespace knight_path_t
{
  class k_32_search_t
  {
  private:
    stitch_list_t m_stitches_to_right;
    stitch_list_t m_stitches_to_left;
    stitch_list_t m_stitches_to_top; //-ve y axis
    stitch_list_t m_stitches_to_bottom;

    stitch_list_t m_stitches_to_top_right;
    stitch_list_t m_stitches_to_top_left;
    stitch_list_t m_stitches_to_bottom_right;
    stitch_list_t m_stitches_to_bottom_left;

    //todo: temporary variables for testing some feature
    int m_global_gsx;
    int m_global_gsy;
    int m_global_gex;
    int m_global_gey;

    //color_t m_global_start_color;
    color_t m_sub_board_start_color;

    static const int MAX_MOVES = 8;
    static const int m_king_xMove[MAX_MOVES];
    static const int m_king_yMove[MAX_MOVES];

    /// Flag to tell if to use pre-computed paths or perform fresh
    /// search
    bool m_use_db;




    /** Gets the sequence (order) in which to visit sub-boards*/
    bool get_subboard_sequence(int ex, int ey, solution_t &sol,
			       vector<node_t> const &start_seq);

    /** For given positin p1, and p2 along the axis for start/end
	points. returns which one (&sub) of 5 sub-board along that axis can be of
	length 8-cells */
    bool find_position (int p1, int p2, int &sub);

    /** Determines the subx, suby, which repesents the row and column
	of sub-board with size 8. While all others will have size
	6-cells*/
    void get_sub_board_conf(int sx, int sy,
			    int ex, int ey,
			    int &subx, int &suby);

    /** Returns the sub-board (&node) within which given (x,y) will
	fall into. */
    void get_sub_board_node (int x, int y, node_t &node,
			     int subx, int suby);

    /**
       Creates a sub-board grid with 5x5 sub-boards, with subx, suby as
       nodes with 8 size
    */
    void create_sub_board_grid (sub_board_grid_t &sb_grid,
				int subx, int suby);

    /**
       Displays the sub-boards
    */
    void display_sub_board_grid(sub_board_grid_t const &sb_grid);

    /** Using the solution for sequence, returns the coordinates of
	next sub-board (next_sb_indxy) that follows after given
	sub-board (sb_indxy)*/
    bool get_next_sub_board_index (sub_board_grid_t const &sb,
				   solution_t const &sol,
				   int sb_indx, int sb_indy,
				   int &next_sb_indx, int &next_sb_indy);



    /** Initializes the list of stitches that is possible for each
	direction*/
    void initialize_stitches();

    /**
       Returns true if (i) 1st or 2nd nodes is not global start, global end nodes
       (ii) 1st node is not already marked as entry node (to the sub-board)
       (iii) 2nd node is not already marked as exit node (to the sub-board)
       (iv) the start node should be same color as color (global start)

    */
    bool valid_stitch_points (k_sub_board_t const &board1,
			      int new_x1, int new_y1,
			      k_sub_board_t const &board2,
			      int new_x2, int new_y2);

    /**
       Returns true if could apply one of the stitches resulting in valid
       move. Else returns false
    */
    bool apply_valid_stitch(sub_board_grid_t &sb,
			    stitch_list_t const &stitches,
			    int sb_indx, int sb_indy,
			    int next_sb_indx, int next_sb_indy);

    /** Returns the list of possible stitches that can be applied for
	given delx, dely*/
    void get_stitches (int delx, int dely, stitch_list_t &stitches);

    /** Returns one stitch that has satisfied all validation
	conditions*/
    void stitching_points (sub_board_grid_t &sb,
			   int sb_indx, int sb_indy,
			   int next_sb_indx, int next_sb_indy);

    /**
       Sets the stitching points between sub-boards, by setting (sx, sy),
       (ex, ey) for each sub-board.
    */
    void find_stitching_points (sub_board_grid_t &sb,
				solution_t const &sol,
				int sb_indx=-1, int sb_indy=-1);

    /**
       converts a solution from grid format to list format.  Implemented
       by scanning the whole grid multiple times. This approach is less
       optimal than going to neighboring cells to find next node in the
       solution.

       The reason for picking less optimal is to leave an open scope for
       being used on knight's grid or regular king - type movement.

       Presently configured to expect values on the grid range from 0 to
       MAXX * MAXY. And -1 is used for default fill value.
    */
    bool convert_board_path_to_list (solution_t const &sol_grid,
				     vector <node_t> &sol_list);

    /** Converts the sub_path to global coordinates (from sub-board
	coordinates) and appends to given global_path*/
    void append_as_global_path (k_sub_board_t const &board,
				vector<node_t> const &sub_path,
				vector<node_t> &global_path);


    /** Finds path from given start_node, to end_node along the
	sub-boards as listed in sub_board_path_list.*/
    bool find_complete_path (sub_board_grid_t &sb_grid,
			     vector<node_t> const &sub_board_path_list,
			     int sx_, int sy_, int ex_, int ey_,
			     //k_getpath_t &kpath,
			     k_path_adapter_t &kpath,
			     node_t const &start_node,
			     node_t const &end_node,
			     vector<node_t> &global_path,
			     bool skip_first_sub_board = false);

    /**
       Gets all possible pair of entry and exit sub-boards from start
       node
    */
    void get_sub_board_neighs_loop_list(node_t const &start_node,
					vector<node_pair_t> &neigh_pair_list);


    /**
       Returns true if at sub-board level 5x5, there is a possible
       path starting at n1, followed by n2 and ending at n_last.
    */
    bool feasible_sub_board_path (node_t const &n1, node_t const &n2,
				  node_t const &n_last);



    /** For given (node) in sub-board (board), checks if their is any
	knight's path that falls into sub-board (neigh_sub_board). If
	so returns true and returns global xy coordinates of theis
	neighbor (global_endxy)*/
    bool neigh_falls_in_sub_board (node_t const &node,
				   k_sub_board_t const &board,
				   sub_board_grid_t const &sb_grid,
				   node_t const &neigh_sub_board,
				   int subx, int suby,
				   int &global_endx, int &global_endy);

    /**
       When global start and end points fall within same sub-board, this
       function finds the neighboring sub-board to exit from 1st subboard
       and and neighboring subboard to enter back.
    */
    bool find_exit_entry_subboard (sub_board_grid_t const &sb_grid,
				   vector <node_t> const sub_path,
				   node_t const &start_node,
				   k_sub_board_t const &start_sub_board,
				   int subx, int suby,
				   // pair exiting the start sub-board
				   node_pair_t &exit_node_pair,
				   // pair entering the start sub-board
				   node_pair_t &entry_node_pair,
				   // exit sub-board from start sub-board
				   node_t &exit_sub_board,
				   // entry sub-board back into start sub-board
				   node_t &entry_sub_board);

  public:
    /** Constructor with a flag that determines if to use pre-computed
	paths or search from scratch*/
    k_32_search_t (bool use_db)
    {m_use_db = use_db; }


    /** Searches for a path from global (gsx, gsy) to global (gex,
	gey). Returns path as (complete_path)*/
    void search_longest_path_32x(int gsx, int gsy, int gex, int gey,
				 vector<node_t> &complete_path);



  };

};

#endif
