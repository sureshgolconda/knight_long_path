//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .C file with implementations of k_32_search_t class's methods.

   @author Suresh Golconda
*/

#include "k_32_search.H"

using namespace std;
using namespace knight_path_t;

const int k_32_search_t::m_king_xMove[8] = {  1, 0, -1, 0,   1, 1, -1, -1};
const int k_32_search_t::m_king_yMove[8] = {  0, -1, 0, 1,   1, -1, 1, -1};

bool k_32_search_t::get_subboard_sequence(int ex, int ey, solution_t &sol,
					  vector<node_t> const &start_seq)
{
  node_t const &start_node = start_seq.back();
  int sx = start_node.posx;
  int sy = start_node.posy;

  int maxx =5;
  int maxy = 5;
  int search_limit = 100;
  k_tour_t kt (maxx, maxy, search_limit, false);


  kt.reset();
  if (!kt.solveKT_new(sx, sy, ex, ey, sol, start_seq))
    {
      printf("failed search subboard : %d %d %d %d\n",sx, sy, ex, ey);
      //return num_solutions;
      return false;
    }
  else
    {
      //printf("YES: %d %d %d %d\n",sx, sy, ex, ey);
      return true;
    }
}

bool k_32_search_t::find_position (int p1, int p2, int &sub)
{
  int pos =0;
  const int CELLS6 = 1;
  const int CELLS8 = 0;
  int board[5] = {CELLS6, CELLS6, CELLS6, CELLS6, CELLS6};
  bool found_pos = false;

  for (int i=0; i<5; i++)
    {
      if (((p1 >= pos) && (p1 < (pos +8))) || //
	  ((p2 >= pos) && (p2 < (pos +8))))
	{
	  board[i] = CELLS6; // i-th row 6-size
	  pos +=6;
	}
      else
	{
	  board[i] = CELLS8; // i-th row 8-size
	  found_pos = true;
	  sub=i;
	  break;
	}
    }
  printf("order for (%d %d): %d %d %d %d %d\n", p1, p2,
	 board[0], board[1], board[2],
	 board[3], board[4]);

  // ---- displaying results ----
  // int pos1 =0;
  // int pos2 =0;
  // for (int i=0; i < 5;i++)
  //   {
  //     if (board[i] == CELLS6)
  // 	pos2 = pos1 + 6;
  //     else
  // 	pos2 = pos1 + 8;

  //     printf("i=%d, %d to %d\n",i, pos1, pos2);
  //     pos1 = pos2;
  //   }
  // printf("sub position %d\n", sub);
  // if (!found_pos)
  //   printf("ERROR :(\n");
  return found_pos;
}



void k_32_search_t::get_sub_board_conf(int sx, int sy,
				       int ex, int ey,
				       int &subx, int &suby)
{
  find_position (sx, ex, subx);
  find_position (sy, ey, suby);
  printf("subx=%d, suby=%d\n", subx, suby);
}



void k_32_search_t::get_sub_board_node (int x, int y, node_t &node,
					int subx, int suby)
{
  int pos1 =0;
  int pos2 =0;
  node.posx =-1;
  node.posy = -1;

  for (int i=0; i<5; i++)
    {
      pos1 =pos2;
      if (subx == i)
	pos2 = pos1 + 8;
      else
	pos2 = pos1 + 6;

      if ((pos1 <=x) && (x < pos2))
	{ //within sub-board coordinates
	  node.posx = i;
	  break;
	}
    }

  pos2=0;
  for (int i=0; i<5; i++)
    {
      pos1 =pos2;
      if (suby == i)
	pos2 = pos1 + 8;
      else
	pos2 = pos1 + 6;

      if ((pos1 <=y) && (y < pos2))
	{ //within sub-board coordinates
	  node.posy = i;
	  break;
	}
    }
}


void k_32_search_t::create_sub_board_grid (sub_board_grid_t &sb_grid,
					   int subx, int suby)
{
  int posx =0;
  int maxx =0;

  // set base_gx, base_gy
  for (int sb_xi=0; sb_xi< 5; sb_xi++) // sub board's xi index
    {

      if (subx == sb_xi) // 8 cell grid
	maxx = 8;
      else
	maxx = 6;

      int posy =0;
      int maxy =0;
      for (int sb_yi=0; sb_yi<5; sb_yi++) // sub board's yi
	{
	  if (suby == sb_yi)
	    maxy = 8;
	  else
	    maxy = 6;


	  sb_grid.m_boards[sb_xi][sb_yi].base_gx = posx;
	  sb_grid.m_boards[sb_xi][sb_yi].base_gy = posy;
	  sb_grid.m_boards[sb_xi][sb_yi].set_max_xy(maxx, maxy);

	  //sb_grid.set_max_xy(maxx, maxy);

	  posy += maxy; // preparing for next grid's base x
	}

      posx += maxx; // preparing for next grid's base x
    }
}

void k_32_search_t::display_sub_board_grid(sub_board_grid_t const &sb_grid)
{
  printf("\nSub-boards along with stitching points (Format: [base_gx,y,   maxx,y,    stx,y edxy])\n");
  for (int sb_yi =0; sb_yi <5; sb_yi++)
    {
      for (int sb_xi =0; sb_xi < 5; sb_xi++)

	{
	  k_sub_board_t const &sb = sb_grid.m_boards[sb_xi][sb_yi];
	  printf("[B(%2d %2d) M(%2d %2d) ST(%2d %2d) ED(%2d %2d)], ",
		 sb.base_gx, sb.base_gy,
		 sb.get_maxx(), sb.get_maxy(),
		 sb.lsx, sb.lsy, sb.lex, sb.ley);
	}
      printf("\n");
    }
}


bool k_32_search_t::get_next_sub_board_index (sub_board_grid_t const &sb,
					      solution_t const &sol,
					      int sb_indx, int sb_indy,
					      int &next_sb_indx, int &next_sb_indy)
{

  int val = sol.m_values[sb_indx][sb_indy];

  // search for next value
  return sol.search_for (val +1, next_sb_indx, next_sb_indy);
}


void k_32_search_t::initialize_stitches()
{
  stitch_t stitch (0, 0, 0,0);

  // --- right stitches
  m_stitches_to_right.clear();  //clear
  stitch.set (-1, 2, 1,1);
  m_stitches_to_right.push_back (stitch);

  stitch.set (-1, 2, 1, 3);
  m_stitches_to_right.push_back (stitch);

  stitch.set (-1, 3, 1, 2);
  m_stitches_to_right.push_back (stitch);

  stitch.set (-1, 3, 1, 4);
  m_stitches_to_right.push_back (stitch);


  // -- left stitches
  m_stitches_to_left.clear(); // clear
  stitch.set (0, 2, -2,1);
  m_stitches_to_left.push_back (stitch);

  stitch.set (0, 2, -2,3);
  m_stitches_to_left.push_back (stitch);

  stitch.set (0, 3, -2,2);
  m_stitches_to_left.push_back (stitch);

  stitch.set (0, 3, -2,4);
  m_stitches_to_left.push_back (stitch);


  // -- top stitches
  m_stitches_to_top.clear(); // clear
  stitch.set (2, 0, 1,-2);
  m_stitches_to_top.push_back (stitch);

  stitch.set (2, 0, 3,-2);
  m_stitches_to_top.push_back (stitch);

  stitch.set (3, 0, 2,-2);
  m_stitches_to_top.push_back (stitch);

  stitch.set (3, 0, 4,-2);
  m_stitches_to_top.push_back (stitch);

  // -- bottom stitches
  m_stitches_to_bottom.clear(); //clear
  stitch.set (2, -1, 1,1);
  m_stitches_to_bottom.push_back (stitch);

  stitch.set (2, -1, 3,1);
  m_stitches_to_bottom.push_back (stitch);


  stitch.set (3, -1, 2,1);
  m_stitches_to_bottom.push_back (stitch);

  stitch.set (3, -1, 4,1);
  m_stitches_to_bottom.push_back (stitch);

  // -- top right  (diagnal)
  m_stitches_to_top_right.clear(); //clear
  stitch.set (-1, 0, 1,-1);
  m_stitches_to_top_right.push_back (stitch);

  stitch.set (-1, 0, 0,-2);
  m_stitches_to_top_right.push_back (stitch);


  stitch.set (-2, 0, 0,-1);
  m_stitches_to_top_right.push_back (stitch);

  stitch.set (-1, 1, 0,-1);
  m_stitches_to_top_right.push_back (stitch);

  // -- top left  (diagnal)
  m_stitches_to_top_left.clear(); //clear;
  stitch.set (0, 0, -1,-2);
  m_stitches_to_top_left.push_back (stitch);

  stitch.set (0, 0, -2,-1);
  m_stitches_to_top_left.push_back (stitch);


  stitch.set (0, 1, -1,-1);
  m_stitches_to_top_left.push_back (stitch);

  stitch.set (1, 0, -1,-1);
  m_stitches_to_top_left.push_back (stitch);


  // -- bottom right  (diagnal)
  m_stitches_to_bottom_right.clear(); //clear
  stitch.set (-1, -1, 0, 1);
  m_stitches_to_bottom_right.push_back (stitch);

  stitch.set (-1, -1, 1,0);
  m_stitches_to_bottom_right.push_back (stitch);


  stitch.set (-2, -1, 0,0);
  m_stitches_to_bottom_right.push_back (stitch);

  stitch.set (-1, -2, 0,0);
  m_stitches_to_bottom_right.push_back (stitch);

  // -- bottom left  (diagnal)
  m_stitches_to_bottom_left.clear(); //clear
  stitch.set (0, -1, -1, 1);
  m_stitches_to_bottom_left.push_back (stitch);

  stitch.set (0, -1, -2, 0);
  m_stitches_to_bottom_left.push_back (stitch);


  stitch.set (1, -1, -1,0);
  m_stitches_to_bottom_left.push_back (stitch);

  stitch.set (0, -2, -1,0);
  m_stitches_to_bottom_left.push_back (stitch);
}






bool k_32_search_t::valid_stitch_points (k_sub_board_t const &board1,
					 int new_x1, int new_y1,
					 k_sub_board_t const &board2,
					 int new_x2, int new_y2)
{

  //(i) --- start node is same as global start
  if ((board1.to_global_x (new_x1) == m_global_gsx) &&
      (board1.to_global_y (new_y1) == m_global_gsy))
    {
      // printf("INFO: start node is global start node (%d, %d)",
      // 	     new_x1, new_y1);
      return false;
    }
  // --- Or 2nd node is the global end node, (then skipping using
  // --- this node as stitch target
  if ((board2.to_global_x (new_x2) == m_global_gex) &&
      (board2.to_global_y (new_y2) == m_global_gey))
    {
      // printf("INFO: end node is global end node (%d, %d)",
      // 	     new_x2, new_y2);

      return false;
    }


  // (ii) --- 1st node is already marked as start node
  if ((board1.lsx == new_x1) && (board1.lsy == new_y1))
    {
      // printf("INFO: 1st node is already marked as start (%d %d) global(%d,%d)\n",
      // 	     new_x1, new_y1, board1.to_global_x(new_x1),
      	     // board1.to_global_y (new_y1));
      return false;
    }


  // (iii) --- 2nd node is already used an exit node
  if ((board2.lex == new_x2) && (board2.ley == new_y2))
    {
      // printf("INFO: 2nd node already used as exit node (%d %d) global(%d,%d)\n",
      // 	     new_x2, new_y2, board2.to_global_x(new_x2),
      // 	     board2.to_global_y (new_y2));
      return false;
    }

  // (iv) --- Start cell color should be same as global cell color
  if (get_cell_color (new_x2, new_y2) != m_sub_board_start_color)
    {
      // printf("INFO: sub-board start cell color is not same as global (%s, %s) (%d %d)\n",
      // 	     get_color_name (m_sub_board_start_color).c_str(),
      // 	     get_color_name (get_cell_color (new_x2, new_y2)).c_str(),
      // 	     new_x2, new_y2);
      // printf("colors gl=%d, this %d, xy (%d %d)\n",
      // 	     m_sub_board_start_color, 
      // 	     get_cell_color (new_x2, new_y2), new_x2, new_y2);
      // printf("board2= base(%d %d)\n", board2.base_gx, board2.base_gy);
      return false;
    }


  return true;
}



bool k_32_search_t::apply_valid_stitch(sub_board_grid_t &sb,
				       stitch_list_t const &stitches,
				       int sb_indx, int sb_indy,
				       int next_sb_indx, int next_sb_indy)
{
  //printf("num stitches %lu", stitches.size());
  k_sub_board_t &board1 = (sb.m_boards[sb_indx][sb_indy]);
  k_sub_board_t &board2 = (sb.m_boards[next_sb_indx][next_sb_indy]);

  printf("\nBoards for stitching: (%d %d) dim (%d %d), (%d %d) dim (%d %d). # of stitches=%lu\n",
	 sb_indx, sb_indy, board1.get_maxx(), board1.get_maxy(),
	 next_sb_indx, next_sb_indy, board2.get_maxx(), board2.get_maxy(),
	 stitches.size());
  // printf("No.of stitches %lu\n", stitches.size());

  for (int i=0; i< stitches.size(); i++)
    { //for each stitch
      stitch_t st =stitches[i];
      int new_x1;
      int new_y1;

      int new_x2; // in 2nd sub-board
      int new_y2;

      // printf("\tvalid stitch?:(%d %d), (%d %d)\n",
      // 	     st.m_delx1, st.m_dely1, st.m_delx2, st.m_dely2);

      //---- X1 -----
      if (st.m_delx1 >=0)// from lower end
	{
	  new_x1 = st.m_delx1;
	  //printf("lower x1\n");
	}
      else // from upper end
	{
	  new_x1 = board1.get_maxx() + st.m_delx1; // delx1 is already -ve
	  // printf("upper x1: %d %d %d\n", new_x1, board1.get_maxx(), st.m_delx1);
	}




      //---- Y1 ------
      if (st.m_dely1 >=0) // from lower end of y
	{
	  new_y1 = st.m_dely1;
	  // printf("lower y1\n");
	}
      else
	{
	  new_y1 = board1.get_maxy() + st.m_dely1; // dely1 is -ve
	  // printf("upper y1\n");
	}



      //---- X2 -----
      // 2nd sub-board point (entry point)
      if (st.m_delx2 >=0) // from lower end
	{
	  new_x2 = st.m_delx2;
	  // printf("lower x2\n");
	}
      else // from upper end
	{
	  new_x2 = board2.get_maxx() + st.m_delx2; //delx2 is -ve
	  // printf("upper x2\n");
	}



      //---- Y2 ------
      if (st.m_dely2 >=0) // from lower end of y
	{
	  new_y2 = st.m_dely2;
	  // printf("lower y2\n");
	}
      else
	{
	  new_y2 = board2.get_maxy() + st.m_dely2; // dely2 is -ve
	  // printf("upper y2\n");
	}

      if (((new_x1 + new_y1) %2 ) == ((new_x2 + new_y2)%2))
	{
	  printf("ERROR: stitching points have same color (%d, %d), (%d %d)\n",
		 new_x1, new_y1, new_x2, new_y2);
	  assert(false);
	}

      // printf("checking for valid stitch %d\n", i);
      // --- check if new positions would be valid ---
      if (valid_stitch_points (board1, new_x1, new_y1, board2, new_x2, new_y2))
	//if (true) // valid new stitching points
	{
	  // --- if yes, update the sub-boards, return true. ---

	  printf("Stitching board sb(%d, %d) to sb(%d,%d):\t st(%d, %d) ed(%d, %d)\t dm(%d %d) (%d %d)\n",
		 sb_indx, sb_indy, next_sb_indx, next_sb_indy,
		 new_x1, new_y1, new_x2, new_y2,
		 board1.get_maxx(), board1.get_maxy(),
		 board2.get_maxx(), board2.get_maxy());
	  // printf("stitch from (%d %d) of board (%d %d) to ",
	  // 	 new_x1, new_y1, sb_indx, sb_indy);

	  // printf(" (%d %d) of board (%d %d) to \n",
	  // 	 new_x2, new_y2, next_sb_indx, next_sb_indy);

	  //board1's end points is stitch's 1st point
	  board1.lex = new_x1;
	  board1.ley = new_y1;

	  //board2's start point is the stitch's 2nd point
	  board2.lsx = new_x2;
	  board2.lsy = new_y2;
	  return true;
	}
      else
	{
	  // printf("ERROR: Not a valid stitch\n");
	  // printf("Stitching board sb(%d, %d) to sb(%d,%d):\t dm(%d %d) (%d %d)\n",
	  // 	 sb_indx, sb_indy, next_sb_indx, next_sb_indy,
	  // 	 board1.get_maxx(), board1.get_maxy(),
	  // 	 board2.get_maxx(), board2.get_maxy());

	  // printf("stitch(%d): (%d, %d), (%d %d)\n\n",i,
	  // 	 st.m_delx1, st.m_dely1, st.m_delx2, st.m_dely2);


	  //assert(false);
	}
    }

  printf("ERROR-1: No valid stitch exist. Num stitches=%lu \n", stitches.size());
  assert(false);
  return false; // failed to apply any stitch
}


void k_32_search_t::get_stitches (int delx, int dely, stitch_list_t &stitches)
{
  // stitches = m_stitches_to_right;
  // return;  //todo: hack


  if ((delx == 1) && (dely == 0)) // right
    {
      //get_right_stitches (stitches);
      stitches = m_stitches_to_right;
      // printf("RIGHT:\n");
    }
  else if ((delx == -1) && (dely == 0)) // left
    {
      stitches = m_stitches_to_left;
      // printf("LEFT:\n");
    }
  else if ((delx == 0) && (dely == -1)) // top
    {
      stitches = m_stitches_to_top;
      // printf("TOP:\n");
    }
  else if ((delx == 0) && (dely == 1)) // bottom
    {
      stitches = m_stitches_to_bottom;
      // printf("BOTTOM:\n");
    }

  else if ((delx == 1) && (dely == -1)) // right top diagnal
    {
      stitches = m_stitches_to_top_right;
      // printf("TOP RIGHT:\n");
    }
  else if ((delx == 1) && (dely == 1)) // right bottom diagnal
    {
      stitches = m_stitches_to_bottom_right;
      // printf("BOTTOM RIGHT:\n");
    }
  else if ((delx == -1) && (dely == 1)) // left bottom diagnal
    {
      stitches = m_stitches_to_bottom_left;
      // printf("BOTTOM LEFT:\n");
    }
  else if ((delx == -1) && (dely == -1)) // left top diagnal
    {
      stitches = m_stitches_to_top_left;
      // printf("TOP LEFT:\n");
    }
  else
    {
      printf("ERROR: delx, dely, not any neighbor (%d, %d)\n",
	     delx, dely);
      assert(false);
    }
}


void k_32_search_t::stitching_points (sub_board_grid_t &sb,
				      int sb_indx, int sb_indy,
				      int next_sb_indx, int next_sb_indy)
{
  // printf("\nLOOKING for stitches (%d,%d) to (%d %d)\n",
  // 	 sb_indx, sb_indy, next_sb_indx, next_sb_indy);
  //printf("STICHING: %d %d to %d %d\n", sb_indx, sb_indy,
  // next_sb_indx, next_sb_indy);
  int delx = next_sb_indx - sb_indx;
  int dely = next_sb_indy - sb_indy;
  assert ((delx >=-1) && (delx <=1) &&
	  (dely >=-1) && (dely <=1));

  stitch_list_t stitches;
  get_stitches (delx, dely, stitches);

  apply_valid_stitch(sb, stitches, sb_indx, sb_indy,
		     next_sb_indx, next_sb_indy);


}





void k_32_search_t::find_stitching_points (sub_board_grid_t &sb,
					   solution_t const &sol,
					   int sb_indx, int sb_indy)
{
  //todo: Need to solve the special case of global-star-end falling
  //within same sub-board

  // int sb_indx; // x-index to starting sub-board
  // int sb_indy; // y-index to starting sub-board

  int next_sb_indx;
  int next_sb_indy;

  // if start index not provided
  if ((sb_indx == -1) || (sb_indy ==-1))
    {
      if (!sol.get_start_index(sb_indx, sb_indy))
	return;
    }
  //  sol.get_end_index(sub_exi, sub_eyi);
  // printf("start-end sub-board index (%d %d), (%d %d)\n",
  // 	 sub_sxi, sub_syi, sub_exi, sub_eyi);

  initialize_stitches();// initialize stitching values


  while (get_next_sub_board_index (sb, sol, sb_indx, sb_indy,
				   next_sb_indx, next_sb_indy))
    {
      // get the sticking points, by marking
      stitching_points (sb, sb_indx, sb_indy, next_sb_indx, next_sb_indy);
      sb_indx = next_sb_indx;
      sb_indy = next_sb_indy;


      //todo: temp:
      //break;
    }
}





bool k_32_search_t::convert_board_path_to_list (solution_t const &sol_grid,
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

void k_32_search_t::append_as_global_path (k_sub_board_t const &board,
					   vector<node_t> const &sub_path,
					   vector<node_t> &global_path)
{
  printf("appending path for board (%dx%d), base (%d, %d), sz=%lu ",
	 board.get_maxx(), board.get_maxy(),
	 board.base_gx, board.base_gy, sub_path.size());
  printf("start=(%d %d), end (%d %d)\n",
	 board.lsx, board.lsy,board.lex, board.ley);

  node_t node;
  for (int i=0; i< sub_path.size(); i++)
    {
      node.posx = board.to_global_x(sub_path[i].posx);
      node.posy = board.to_global_y(sub_path[i].posy);
      global_path.push_back (node);

      // printf("\t l(%d %d), g(%d %d)\n",
      // 	     sub_path[i].posx, sub_path[i].posy,
      // 	     node.posx, node.posy);
    }
  //getchar();
}





bool k_32_search_t::find_complete_path (sub_board_grid_t &sb_grid,
					vector<node_t> const &sub_board_path_list,
					int sx_, int sy_, int ex_, int ey_,
					//k_getpath_t &kpath,
					k_path_adapter_t &kpath,
					node_t const &start_node,
					node_t const &end_node,
					vector<node_t> &global_path,
					bool skip_first_sub_board)
{
  int board_x = sub_board_path_list[0].posx;
  int board_y = sub_board_path_list[0].posy;

  k_sub_board_t &board_0 = sb_grid.m_boards[board_x][board_y];

  vector<node_t> sub_path;

  // --- First sub-board ---
  if (!skip_first_sub_board)
    {
      int local_sx = board_0.to_local_x (sx_);
      int local_sy = board_0.to_local_y (sy_);
      board_0.lsx = local_sx;
      board_0.lsy = local_sy;

      printf("\nget_path par-1: mx(%d %d), st(%d %d), end (%d,%d)\n",
	     board_0.get_maxx(), board_0.get_maxy(), local_sx, local_sy,
	     board_0.lex, board_0.ley);
      if (!kpath.get_path (board_0.get_maxx(), board_0.get_maxy(), local_sx, local_sy,
			   board_0.lex, board_0.ley, sub_path))
	{
	  printf("\tFailed path: size(%d,%d), s(%d %d), e(%d,%d)\n",
		 board_0.get_maxx(), board_0.get_maxy(), local_sx, local_sy,
		 board_0.lex, board_0.ley);

	  // printf("\tFailed path in sub-board (%d, %d)\n",
	  // 	     board_x,board_y);
	}
      else
	append_as_global_path (board_0, sub_path, global_path);
    }

  // --- All sub-boards except last one ---
  for (int i=1; (i)< sub_board_path_list.size(); i++)
    {
      board_x = sub_board_path_list[i].posx;
      board_y = sub_board_path_list[i].posy;
      k_sub_board_t &board_i = sb_grid.m_boards[board_x][board_y];

      sub_path.clear();
      printf("\nget_path (%d) B(%d %d) par-2: mx(%d %d), st(%d %d), end (%d,%d)\n", i,
	     board_i.base_gx, board_i.base_gy,
	     board_i.get_maxx(), board_i.get_maxy(), 
	     board_i.lsx, board_i.lsy,
	     board_i.lex, board_i.ley);

      if (!kpath.get_path (board_i.get_maxx(), board_i.get_maxy(),
			   board_i.lsx, board_i.lsy,
			   board_i.lex, board_i.ley, sub_path))
	{
	  printf("\tFailed path: size(%d,%d), s(%d %d), e(%d,%d)\n",
		 board_i.get_maxx(), board_i.get_maxy(),
		 board_i.lsx, board_i.lsy,
		 board_i.lex, board_i.ley);

	  // printf("\tFailed path in sub-board (%d, %d) \n",
	  // 	 board_x, board_y);
	}
      else
	append_as_global_path (board_i, sub_path, global_path);
    }






  for (int i=0; i+1< global_path.size(); i++)
    {
      // check for all nodes fall within grid coordinates
      if (global_path[i].posx <0 || global_path[i].posx > 31)
	printf("ERROR: global path bad -x i=%d, posx=%d\n", 
	       i, global_path[i].posx);

      if (global_path[i].posy <0 || global_path[i].posy > 31)
	printf("ERROR: global path bad -y i=%d, posy=%d\n", 
	       i, global_path[i].posy);

      //todo: check that no nodes are repeated


      // check for valid knight path
      int xdiff = global_path[i+1].posx - global_path[i].posx;
      int ydiff = global_path[i+1].posy - global_path[i].posy;

      if (((std::abs (xdiff) == 2) && std::abs (ydiff) == 1) ||
	  ((std::abs (xdiff) == 1) && std::abs (ydiff) == 2))
	{
	}
      else
	{
	  printf("ERROR: not valid knight path i=%d (%d %d), (%d %d)\n",i,
		 global_path[i].posx, global_path[i].posy,
		 global_path[i+1].posx, global_path[i+1].posx);
	}
    }
  printf("global path check complete: size=%lu\n", global_path.size());
  return true;
}





void k_32_search_t::get_sub_board_neighs_loop_list(node_t const &start_node,
						   vector<node_pair_t> &neigh_pair_list)
{
  // node_t neigh1;
  // node_t neigh2;
  node_pair_t node_pair;

  // check for neighor pair standing on the right
  if ((start_node.posx+1) <5) // right column exist
    {
      node_pair.n1.posx = start_node.posx +1;
      node_pair.n2.posx = start_node.posx +1;

      if (start_node.posy >0) // top row exist
	{
	  node_pair.n1.posy = start_node.posy -1;
	  node_pair.n2.posy = start_node.posy;
	  if (feasible_sub_board_path (start_node,
				       node_pair.n1,
				       node_pair.n2))
	    neigh_pair_list.push_back (node_pair);
	}
      if ((start_node.posy+1) < 5) // botto row exist
	{
	  node_pair.n1.posy = start_node.posy;
	  node_pair.n2.posy = start_node.posy+1;

	  if (feasible_sub_board_path (start_node,
				       node_pair.n1,
				       node_pair.n2))
	    neigh_pair_list.push_back (node_pair);
	}
    }

  // check for neighbor pair standing on left
  if (start_node.posx > 0) // left column exist
    {
      node_pair.n1.posx = start_node.posx -1;
      node_pair.n2.posx = start_node.posx -1;
      if (start_node.posy > 0) // top row exist
	{
	  node_pair.n1.posy = start_node.posy -1;
	  node_pair.n2.posy = start_node.posy;

	  if (feasible_sub_board_path (start_node,
				       node_pair.n1,
				       node_pair.n2))
	    neigh_pair_list.push_back (node_pair);
	}
      if ((start_node.posy+1) < 5)
	{
	  node_pair.n1.posy = start_node.posy;
	  node_pair.n2.posy = start_node.posy +1;

	  if (feasible_sub_board_path (start_node,
				       node_pair.n1,
				       node_pair.n2))
	    neigh_pair_list.push_back (node_pair);
	}
    }

  // check for horizontal neighbor pair on top
  if (start_node.posy >0) // top row exist
    {
      node_pair.n1.posy = start_node.posy -1;
      node_pair.n2.posy = start_node.posy -1;

      if ((start_node.posx+1) < 5) // right column exist
	{
	  node_pair.n1.posx = start_node.posx;
	  node_pair.n2.posx = start_node.posx +1;

	  if (feasible_sub_board_path (start_node,
				       node_pair.n1,
				       node_pair.n2))
	    neigh_pair_list.push_back (node_pair);
	}

      if (start_node.posx > 0) // left column exist
	{
	  node_pair.n1.posx = start_node.posx -1;
	  node_pair.n2.posx = start_node.posx;

	  if (feasible_sub_board_path (start_node,
				       node_pair.n1,
				       node_pair.n2))
	    neigh_pair_list.push_back (node_pair);
	}
    }
  if ((start_node.posy+1) < 5) // bottom row exist
    {
      node_pair.n1.posy = start_node.posy +1;
      node_pair.n2.posy = start_node.posy +1;

      if ((start_node.posx+1) < 5) // right column exist
	{
	  node_pair.n1.posx = start_node.posx;
	  node_pair.n2.posx = start_node.posx + 1;

	  if (feasible_sub_board_path (start_node,
				       node_pair.n1,
				       node_pair.n2))
	    neigh_pair_list.push_back (node_pair);
	}

      if (start_node.posx >0) // left column exist
	{
	  node_pair.n1.posx = start_node.posx -1;
	  node_pair.n2.posx = start_node.posx;

	  if (feasible_sub_board_path (start_node,
				       node_pair.n1,
				       node_pair.n2))
	    neigh_pair_list.push_back (node_pair);
	}
    }
}





bool k_32_search_t::feasible_sub_board_path (node_t const &n1, 
					     node_t const &n2,
					     node_t const &n_last)
{
  int maxx = 5;
  int maxy = 5;
  for (int x=0; x< maxx;  x++)
    for (int y=0; y<maxy; y++)
      { // Check if any node would starve with no neighbor nodes

	// Skip if (x,y) is n1, as it already has a node before and after
	if ((x == n1.posx) && (y == n1.posy))
	  continue;

	bool has_good_neighbor = false;

	for (int i=0; (!has_good_neighbor) && (i< 8); i++)
	  {
	    int nx = x + m_king_xMove[i];
	    int ny = y + m_king_yMove[i];

	    if (nx >= 0 && nx < maxx &&
		ny >= 0 && ny < maxy)
	      {
		if ( ((nx != n1.posx) || (ny != n1.posy)) &&// nx,ny != n1
		     ((nx != n2.posx) || (ny != n2.posy)) && // nx,ny != n2
		     ((nx != n_last.posx) || (ny != n_last.posy))) // nx, ny != n_last
		  {
		    // has one neighbor which is not 1st, 2nd or last node
		    has_good_neighbor = true;
		  }
	      }

	  }

	// starving? if atleast one node on board does not have proper
	// neighbor, then return failure (starving)
	if (!has_good_neighbor)
	  {
	    return false;
	  }

      }
  return true;
}







bool k_32_search_t::neigh_falls_in_sub_board (node_t const &node,
					      k_sub_board_t const &board,
					      sub_board_grid_t const &sb_grid,
					      node_t const &neigh_sub_board,
					      int subx, int suby,
					      int &global_endx, int &global_endy)
{
  // printf("neigh_fall(): nd(%d %d), ngh(%d %d)\n",
  // 	 node.posx, node.posy, neigh_sub_board.posx,
  // 	 neigh_sub_board.posy);

  int m_knight_xMove[8] = {  2, 1, -1, -2, -2, -1,  1,  2 };
  int m_knight_yMove[8] = {  1, 2,  2,  1, -1, -2, -2, -1 };

  // global-start-xy
  int gsx = board.to_global_x (node.posx);
  int gsy = board.to_global_y (node.posy);

  for (int i=0; i< 8;i++)
    {
      // global-end-xy
      int gex = gsx + m_knight_xMove [i];
      int gey = gsy + m_knight_yMove [i];

      if ((gex >=0 && gex <32) && (gey >= 0 && gey <32))
	{ // falls within the global board

	  node_t end_sub_board; // sub-board it falls into
	  get_sub_board_node (gex, gey, end_sub_board, subx, suby);
	  if ((end_sub_board.posx == neigh_sub_board.posx) &&
	      (end_sub_board.posy == neigh_sub_board.posy))
	    {
	      // printf("\t Found neighbor global (%d %d)\n", end_sub_board.posx,
	      // 	     end_sub_board.posy);
	      global_endx = gex;
	      global_endy = gey;
	      return true;
	    }
	}
    }

  return false;

}




bool k_32_search_t::find_exit_entry_subboard (sub_board_grid_t const &sb_grid,
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
					      node_t &entry_sub_board)
{

  bool found_neigh_sub_board = false;
  vector<node_pair_t> neigh_pair_list;
  get_sub_board_neighs_loop_list(start_node, neigh_pair_list);

  // For each possible neighbor pair
  for (int neigh_i =0; (neigh_i < neigh_pair_list.size()) &&
	 (!found_neigh_sub_board); neigh_i++)
    {
      // 1st neighbor following start sub board
      node_t neigh1 = neigh_pair_list[neigh_i].n1;
      // last but one neighbor for returning to start sub-board
      node_t neigh2 = neigh_pair_list[neigh_i].n2;


      printf("(%d %d) Exploring sub board neighbors (%d, %d), (%d %d)\n",
	     start_node.posx, start_node.posy,
	     neigh1.posx, neigh1.posy, neigh2.posx, neigh2.posy);


      // walk along the path within start sub-board to get pair of
      // points from which neigh1, neigh2 sub-boards can be jumped to.
      for (int i=0; (i+1)< sub_path.size(); i++)
	{
	  node_t const &p1 = sub_path[i];
	  node_t const &p2 = sub_path[i+1];

	  int exit_end_gx;
	  int exit_end_gy;
	  int entry_start_gx;
	  int entry_start_gy;

	  if ((neigh_falls_in_sub_board (p1, start_sub_board, sb_grid, 
					 neigh1, subx, suby,
					 exit_end_gx, exit_end_gy)) &&
	      (neigh_falls_in_sub_board (p2, start_sub_board, sb_grid, 
					 neigh2, subx, suby,
					 entry_start_gx, entry_start_gy)))
	    {

	      // check if there is sub-board level path
	      solution_t sol (5,5);
	      vector <node_t> temp_start_seq;
	      node_t nd;
	      nd.posx = start_node.posx;
	      nd.posy = start_node.posy;
	      temp_start_seq.push_back (nd); //start node
	      nd.posx = neigh1.posx;
	      nd.posy = neigh1.posy;
	      temp_start_seq.push_back (nd); //2nd node

	      if (get_subboard_sequence (neigh2.posx, neigh2.posy, sol,
					 temp_start_seq))
		{


		  found_neigh_sub_board = true;
		  exit_node_pair.n1.posx = start_sub_board.to_global_x(p1.posx);
		  exit_node_pair.n1.posy = start_sub_board.to_global_y(p1.posy);

		  exit_node_pair.n2.posx = exit_end_gx; //neighbor node's cell exiting to
		  exit_node_pair.n2.posy = exit_end_gy;

		  entry_node_pair.n1.posx = entry_start_gx; // neighboring node's cell entering
		  entry_node_pair.n1.posy = entry_start_gy;

		  entry_node_pair.n2.posx = 
		    start_sub_board.to_global_x (p2.posx);
		  entry_node_pair.n2.posy = 
		    start_sub_board.to_global_y (p2.posy);

		  entry_sub_board.posx = neigh2.posx;
		  entry_sub_board.posy = neigh2.posy;
		  exit_sub_board.posx = neigh1.posx;
		  exit_sub_board.posy = neigh1.posy;

		  printf("path node l(%d %d) to negh_ind(%d %d), (%d %d) to N_ind(%d %d)\n",
			 p1.posx, p1.posy, neigh1.posx, neigh1.posy,
			 p2.posx, p2.posy, neigh2.posx, neigh2.posy);
		  break;
		}
	    }
	  // search if p1 falls in neigh2, p2 in neigh1
	  if ((neigh_falls_in_sub_board (p2, start_sub_board, sb_grid, neigh1, subx, suby,
					 entry_start_gx, entry_start_gy)) &&
	      (neigh_falls_in_sub_board (p1, start_sub_board, sb_grid, neigh2, subx, suby,
					 exit_end_gx, exit_end_gy)))
	    {

	      solution_t sol (5,5);
	      vector <node_t> temp_start_seq;
	      node_t nd;
	      nd.posx = start_node.posx;
	      nd.posy = start_node.posy;
	      temp_start_seq.push_back (nd); //start node
	      nd.posx = neigh2.posx;
	      nd.posy = neigh2.posy;
	      temp_start_seq.push_back (nd); //2nd node

	      if (get_subboard_sequence (neigh1.posx, neigh1.posy, sol,
					 temp_start_seq))
		{

		  found_neigh_sub_board = true;
		  // // swap neigh1 and neigh2
		  // node_t temp_node;
		  // temp_node = neigh1;
		  // neigh1 = neigh2;
		  // neigh2 = temp_node;



		  exit_node_pair.n1.posx = start_sub_board.to_global_x(p1.posx);
		  exit_node_pair.n1.posy = start_sub_board.to_global_y(p1.posy);

		  exit_node_pair.n2.posx = exit_end_gx; //neighbor node's cell exiting to
		  exit_node_pair.n2.posy = exit_end_gy;

		  entry_node_pair.n1.posx = entry_start_gx; // neighboring node's cell entering
		  entry_node_pair.n1.posy = entry_start_gy;

		  entry_node_pair.n2.posx = start_sub_board.to_global_x (p2.posx);
		  entry_node_pair.n2.posy = start_sub_board.to_global_y (p2.posy);

		  entry_sub_board.posx = neigh1.posx;
		  entry_sub_board.posy = neigh1.posy;
		  exit_sub_board.posx = neigh2.posx;
		  exit_sub_board.posy = neigh2.posy;


		  printf("path(reverse) node l(%d %d) to negh_ind(%d %d), N_ind(%d %d) to l(%d %d)\n",
			 p1.posx, p1.posy,
			 neigh2.posx, neigh2.posy,
			 neigh1.posx, neigh1.posy,
			 p2.posx, p2.posy);
		  break;
		}
	    }
	}

    }

  if (found_neigh_sub_board)
    {
      printf("YES, found neigh\n");
      return true;
    }
  else
    {
      printf("NOT, found neigh\n");
      return false;
    }

}


void k_32_search_t::search_longest_path_32x(int gsx, int gsy,
					    int gex, int gey,
					    vector<node_t> &complete_path)
{
  m_global_gsx = gsx;
  m_global_gsy = gsy;
  m_global_gex = gex;
  m_global_gey = gey;
  m_sub_board_start_color = get_cell_color (m_global_gsx, m_global_gsy);

  int subx, suby;
  // ---- get sub-board configuration ----
  get_sub_board_conf (gsx, gsy, gex, gey, subx, suby);


  // ---- find start-end sub boards ----
  node_t start_subboard_node;
  node_t end_subboard_node;
  get_sub_board_node (gsx, gsy, start_subboard_node, subx, suby);
  get_sub_board_node (gex, gey, end_subboard_node, subx, suby);
  printf("start  g(%d %d) sub-board position s(%d %d)\n",
	 gsx, gsy, start_subboard_node.posx,
	 start_subboard_node.posy);
  printf("end g(%d %d) sub-board position s(%d %d)\n",
	 gex, gey, end_subboard_node.posx, end_subboard_node.posy);

  sub_board_grid_t sb_grid;
  create_sub_board_grid (sb_grid, subx, suby);
  // printf("Sub board grid\n");
  // display_sub_board_grid (sb_grid);


  // early declaration of variable kpath
  // k_getpath_t kpath;
  // kpath.load_all_para();
  //k_path_adapter_t path_gen (false); //set not to use db
  k_path_adapter_t path_gen (m_use_db); //set not to use db

  vector<node_t> pre_path;
  vector<node_t> post_path;
  vector<node_t> start_seq;
  node_t seq_end_node; // end node in sequence for sub-board searching




  if ((start_subboard_node.posx == end_subboard_node.posx) &&
      (start_subboard_node.posy == end_subboard_node.posy))
    {
      printf("start and end falls in one sub-graph\n");

      // need to decide which one is 2nd and last-but-one sub-board in path
      //  -- find path within 1st sub-board
      k_sub_board_t &start_subboard =
	sb_grid.m_boards[start_subboard_node.posx][start_subboard_node.posy];
      int local_sx = start_subboard.to_local_x (gsx);
      int local_sy = start_subboard.to_local_y (gsy);
      int local_ex = start_subboard.to_local_x (gex);
      int local_ey = start_subboard.to_local_y (gey);





      vector<node_t> sub_path;
      // if (!kpath.get_path (start_subboard.get_maxx(), start_subboard.get_maxy(),
      // 			   local_sx, local_sy, local_ex, local_ey,
      // 			   sub_path))
      if (!path_gen.get_path (start_subboard.get_maxx(), start_subboard.get_maxy(),
			      local_sx, local_sy, local_ex, local_ey,
			      sub_path))
	{
	  printf("\t ERROR: Failed path on par-1: mx(%d %d), st(%d,%d), ed(%d,%d)\n",
		 start_subboard.get_maxx(), start_subboard.get_maxy(),
		 local_sx, local_sy, local_ex, local_ey);
	  assert(false);
	}



      node_pair_t exit_node_pair;
      node_pair_t entry_node_pair;
      node_t exit_sub_board;
      node_t entry_sub_board;
      find_exit_entry_subboard (sb_grid, sub_path,
				start_subboard_node, start_subboard,
				subx, suby,
				exit_node_pair, entry_node_pair,
				exit_sub_board, entry_sub_board);

      // update m_sub_board_start_color for all the sub boards to find stitching point
      m_sub_board_start_color = get_cell_color (exit_node_pair.n2.posx,
						exit_node_pair.n2.posy);
      printf("exit node pair (%d %d) (%d %d)\n",
	     exit_node_pair.n1.posx, exit_node_pair.n1.posy,
	     exit_node_pair.n2.posx, exit_node_pair.n2.posy);
      printf("entry node pair (%d %d) (%d %d)\n",
	     entry_node_pair.n1.posx, entry_node_pair.n1.posy,
	     entry_node_pair.n2.posx, entry_node_pair.n2.posy);

      // set start local (start paramter) coordinates of 2nd subboard
      // and last end local coordinates for last sub-board
      k_sub_board_t &board_1 = 
	sb_grid.m_boards[exit_sub_board.posx][exit_sub_board.posy];
      board_1.lsx = board_1.to_local_x (exit_node_pair.n2.posx);
      board_1.lsy = board_1.to_local_y (exit_node_pair.n2.posy);
      printf("\nSetting exit (%d %d) sub-board sxy (%d %d)\n",
	     exit_sub_board.posx, exit_sub_board.posy,
	     board_1.lsx, board_1.lsy);

      k_sub_board_t &board_last = 
	sb_grid.m_boards[entry_sub_board.posx][entry_sub_board.posy];
      board_last.lex = board_last.to_local_x (entry_node_pair.n1.posx);
      board_last.ley = board_last.to_local_y (entry_node_pair.n1.posy);
      printf("Setting last sub-board (%d %d) exy (%d %d): B(%d %d)\n\n",
	     entry_sub_board.posx, entry_sub_board.posy,
	     board_last.lex, board_last.ley, board_last.base_gx, board_last.base_gy);


      // part of path from starting sub-board that goes before leaving sub-board
      int post_path_ind =0;
      int sub_start_board_stitch_lex = 
	start_subboard.to_local_x (exit_node_pair.n1.posx);
      int sub_start_board_stitch_ley = 
	start_subboard.to_local_y (exit_node_pair.n1.posy);

      for (int i=0; i< sub_path.size(); i++)
	{
	  pre_path.push_back (sub_path[i]);
	  post_path_ind = i +1;

	  if((sub_path[i].posx == sub_start_board_stitch_lex)  &&
	     (sub_path[i].posy == sub_start_board_stitch_ley)) // is the exit path
	    {
	      break;
	    }
	}
      printf("\nPREPATH: 0 to %d, size=%lu\n", post_path_ind, pre_path.size());
      // part of path from starting sub-board, that get appended at
      // the end of complete path.
      for (int i=post_path_ind; i< sub_path.size(); i++)
	{
	  post_path.push_back (sub_path[i]);
	}




      printf("sub-board-1's path: %lu\n", sub_path.size());
      for (int i=0; i<sub_path.size(); i++)
	printf("(%d, %d)", sub_path[i].posx, sub_path[i].posy);
      printf("\n");


      node_t node;
      node.posx = start_subboard_node.posx;
      node.posy = start_subboard_node.posy;
      start_seq.push_back (node);

      node.posx = exit_sub_board.posx;
      node.posy = exit_sub_board.posy;
      start_seq.push_back (node);

      seq_end_node.posx = entry_sub_board.posx;
      seq_end_node.posy = entry_sub_board.posy;


      printf("start seq: (%d %d) (%d %d)\n", 
	     start_seq[0].posx, start_seq[0].posy,
	     start_seq[1].posx, start_seq[1].posy);
      printf("end (%d %d)\n", seq_end_node.posx, seq_end_node.posy);


      // ---- search sub-board path ----
      solution_t sub_board_path (5, 5);

      get_subboard_sequence (seq_end_node.posx, seq_end_node.posy,
			     sub_board_path, start_seq);
      // get_subboard_sequence (seq_end_node.posx, seq_end_node.posy,
      // 			 sub_board_path, start_seq);
      vector<node_t> sub_board_path_list; // solution as a list of node
      convert_board_path_to_list (sub_board_path, sub_board_path_list);


      // printf("Sub board path before stitching:\n");
      // sub_board_path.display_board();
      printf("Sub board path as a list (%lu):\n",
	     sub_board_path_list.size());
      for (int i=0; i< sub_board_path_list.size(); i++)
	printf("(%2d,%2d) ", sub_board_path_list[i].posx,
	       sub_board_path_list[i].posy);
      printf("\n\n");


      // ---- find stiching points for each subboard ----
      find_stitching_points(sb_grid, sub_board_path,
			    start_seq.back().posx,
			    start_seq.back().posy);
      display_sub_board_grid (sb_grid);


      // ---- find individual paths and join them

      bool skip_first_sub_board = true;
      vector<node_t> middle_path;
      find_complete_path (sb_grid, sub_board_path_list,
			  // gsx, gsy, gex, gey, kpath,
			  gsx, gsy, gex, gey, path_gen,
			  start_subboard_node, end_subboard_node, 
			  middle_path, skip_first_sub_board);


      //vector<node_t> complete_path;
      for (int i=0; i< pre_path.size(); i++)
	complete_path.push_back (pre_path[i]);

      for (int i=0; i< middle_path.size(); i++)
	complete_path.push_back (middle_path[i]);

      for (int i=0; i< post_path.size();i++)
	complete_path.push_back (post_path[i]);

      printf("prepath=%lu, postpath=%lu\n", 
	     pre_path.size(), post_path.size());
      printf("COMPLETE PATH SIZE %lu\n", complete_path.size());


      // printf("cp1=%lu\n", complete_path1.size());
      // complete_path1.push_back (complete_path[0]);
      // complete_path1.push_back (complete_path[1]);
      // for (int i=0;i< 100; i++)
      // 	complete_path1.push_back (complete_path[i]);
      // printf("after pushing: cp1=%lu\n", complete_path1.size());

      // for (int i=0;i< complete_path.size(); i++)
      // 	complete_path1.push_back (complete_path[i]);


      // Update the sequence with this.

      // Set proper start-end values for each sub-board

      // call find path, while asking it to skip 1st and last sub-board path generation.

    }
  else //start and end falls in different sub-boards
    {
      node_t node;
      node.posx = start_subboard_node.posx;
      node.posy = start_subboard_node.posy;
      start_seq.push_back (node);

      seq_end_node.posx = end_subboard_node.posx;
      seq_end_node.posy = end_subboard_node.posx;








      // ---- search sub-board path ----
      solution_t sub_board_path (5, 5);
      // get start nodes for start sub-board. and exit nodes for last
      // sub-board. Others are set by stitching
      k_sub_board_t &start_subboard =
	sb_grid.m_boards[start_subboard_node.posx][start_subboard_node.posy];
      start_subboard.lsx = start_subboard.to_local_x (m_global_gsx);
      start_subboard.lsy = start_subboard.to_local_y (m_global_gsy);

      k_sub_board_t &end_subboard =
	sb_grid.m_boards[end_subboard_node.posx][end_subboard_node.posy];
      end_subboard.lex = end_subboard.to_local_x (m_global_gex);
      end_subboard.ley = end_subboard.to_local_y (m_global_gey);


      get_subboard_sequence (end_subboard_node.posx, end_subboard_node.posy,
			     sub_board_path, start_seq);
      // get_subboard_sequence (seq_end_node.posx, seq_end_node.posy,
      // 			 sub_board_path, start_seq);
      vector<node_t> sub_board_path_list; // solution as a list of node
      convert_board_path_to_list (sub_board_path, sub_board_path_list);


      printf("Sub board path:\n");
      sub_board_path.display_board();
      printf("Sub board path as a list (%lu):\n",
	     sub_board_path_list.size());
      for (int i=0; i< sub_board_path_list.size(); i++)
	printf("(%2d,%2d) ", sub_board_path_list[i].posx,
	       sub_board_path_list[i].posy);
      printf("\n");


      // ---- find stiching points for each subboard ----
      find_stitching_points(sb_grid, sub_board_path);
      display_sub_board_grid (sb_grid);


      // ---- find individual paths and join them
      find_complete_path (sb_grid, sub_board_path_list,
			  //gsx, gsy, gex, gey, kpath,
			  gsx, gsy, gex, gey, path_gen,
			  start_subboard_node, end_subboard_node,
			  complete_path);

    }
  //  return NULL;
}
