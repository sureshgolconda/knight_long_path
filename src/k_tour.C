//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .C file with implementations of k_tour_t class's methods.

    @author Suresh Golconda
*/


#include "k_tour.H"
#include <cassert>
#include <stdlib.h>

using namespace std;
using namespace knight_path_t;

// 8 moves of knight
const int k_tour_t::m_knight_xMove[8] = {  2, 1, -1, -2, -2, -1,  1,  2 };
const int k_tour_t::m_knight_yMove[8] = {  1, 2,  2,  1, -1, -2, -2, -1 };

// 8 moves for king order, 1'o clock to 12'o clock
// const int k_tour_t::m_king_xMove[8] = {  1, 1, 1, 0, -1, -1,  -1,  0 };
// const int k_tour_t::m_king_yMove[8] = {  -1, 0,  1,  1, 1, 0, -1, -1 };

/// Re-ordered adjacent neighbors exploration, used for checking search performance.
const int k_tour_t::m_king_xMove[8] = {  1, 0, -1, 0,   1, 1, -1, -1};
const int k_tour_t::m_king_yMove[8] = {  0, -1, 0, 1,   1, -1, 1, -1};



/***************** solution_t related function  *********************/

void solution_t::display_board()
{
  for (int y = 0; y < DIM_SIZE_Y; y++)
    {
      for (int x = 0; x < DIM_SIZE_X; x++)
	printf(" %2d ", m_values[x][y]);
      printf("\n");
    }
}



bool solution_t::get_start_index (int &xi, int &yi) const
{
  int start_val =0;
  return search_for (start_val, xi, yi);
}



bool solution_t::get_end_index (int &xi, int &yi) const
{
  int end_val =DIM_SIZE_X * DIM_SIZE_Y -1;
  return search_for (end_val, xi, yi);
}



bool solution_t::search_for (int val, int &xi, int &yi) const
{
  for (int y = 0; y < DIM_SIZE_Y; y++)
    for (int x = 0; x < DIM_SIZE_X; x++)
      {
	if (m_values[x][y] ==val)
	  {
	    xi= x;
	    yi =y;
	    return true;
	  }
      }

  return false;
}


void solution_t::reset (int value)
{
  for (int y = 0; y < DIM_SIZE_Y; y++)
    for (int x = 0; x < DIM_SIZE_X; x++)
      m_values[x][y] = value;

}




/***************** k_tour_t related function  *********************/
// ---------- Public functions  --------------------------


int k_tour_t::isSafe(solution_t const &sol, int x, int y) //DIM_SIZE_X][DIM_SIZE_Y])
{
  if ( x >= 0 && x < DIM_SIZE_X && 
       y >= 0 && y < DIM_SIZE_Y && 
       sol.m_values[x][y] == -1)
    return 1;
  return 0;
}


void k_tour_t::print_solution(solution_t const &sol, int ex, int ey)

{
  printf("SOLUTION: (%d, %d), (%d, %d): ", 
	 m_sx, m_sy, ex, ey);

  for (int yi=0; yi < DIM_SIZE_Y; yi++)
    for (int xi=0; xi < DIM_SIZE_X; xi++)
      printf("%2d ", sol.m_values[xi][yi]);

  printf("\n");
  // printf("Solution count=%d\n", num_solutions);
  // display_board(sol);
}


bool k_tour_t::reached_via_diagnal (solution_t const &sol, int x, int y)
{
  //  return false;
  int px, py; // previous/parent node
  int val = sol.m_values[x][y];
  bool found = false;
  for (int xi=0; (!found) && (xi< DIM_SIZE_X); xi++)
    for (int yi=0; (!found) && (yi<DIM_SIZE_Y); yi++)
      {
	if (sol.m_values[xi][yi] == (val -1))
	  {
	    px = xi;
	    py = yi;
	    found = true;
	  }
      }

  if (found)
    {
      int del_x = x - px;
      int del_y = y - py;
      if ((abs (del_x) == 1) &&
	  (abs (del_y) == 1))
	return true; //diagnally linked
      else
	return false;
    }
  else
    {

      printf("ERROR: could not find parent node (%d %d)\n", x, y);
      assert (found);
    }
  return false;

}


void k_tour_t::get_ordered_children(solution_t const &sol,
				    int sx, int sy, child_list_t &cl)
{
  // count of number of outward moves from each child node
  cl.clear();

  int nextx, nexty;
  int nnx, nny;
  child_t child;

  for (int i=0;i< 8; i++)
    {// for each of the child node
      nextx = sx + xMove[i];
      nexty = sy + yMove[i];

      if ( nextx >= 0 && nextx < DIM_SIZE_X && //valid position
	   nexty >= 0 && nexty < DIM_SIZE_Y && 
	   sol.m_values[nextx][nexty] == -1) // free child
	{// calculate # possible nodes
	  int nn_count =0;
	  for (int j=0; j<8; j++)
	    {
	      nnx = nextx + xMove[j];
	      nny = nexty + yMove[j];
	      if (nnx >= 0 && nnx < DIM_SIZE_X &&
		  nny >= 0 && nny < DIM_SIZE_Y &&
		  (sol.m_values[nnx][nny] == -1) && //free
		  ((nnx != sx) || (nny != sy))) // not parent
		nn_count ++;
	      // printf("\tj=%d, xy(%d,%d), s=%d, count=%d\n",
	      // 	     j, nnx, nny, sol[nnx][nny], nn_count);

	    }
	  child.index = i;
	  child.count = nn_count;
	  cl.push_back(child);
	  // printf("child node: (%d %d), count=%d\n",
	  // 	 nextx,nexty, nn_count);
	}      
    }
  sort (cl.begin(), cl.end());
}











bool k_tour_t::solveKTUtil(int x, int y, int ex, int ey, 
			   int movei, int desired_sol_movei, solution_t &sol)
{
  // printf("solveUtil: movei=%d,des=%d, s(%d %d), e(%d %d),\n", 
  // 	     movei, desired_sol_movei,x, y, ex, ey);
  // getchar();
  m_steps++;
  unsigned period = 10000000;
  if (m_steps % period == 0) // every 1000 steps
    {
      m_big_step++;

      printf("count=%u,(big_step=%d) movei=%d from (%d %d), to (%d %d)\n", 
	     m_steps/period, m_big_step, movei, m_sx, m_sy, ex, ey);

      // limit the execution loop: to 50 * 10,000,000
      if (m_big_step > m_search_limit)
	{
	  printf("COUNT_EXCEEDED %d, %d (%d %d) to (%d %d)\n", 
		 m_big_step, m_steps, m_sx, m_sy, ex, ey);
	  // m_big_step =0;
	  // m_steps = 0;
	  m_abort = true;
	  return false;
	}
    }


  int next_x, next_y;
  //if (movei == DIM_SIZE_X * DIM_SIZE_Y)
  if (movei == desired_sol_movei)
    {
      num_solutions ++;
      //print_solution(sol);
      if ((x == ex) && (y == ey))
	{
	  // check if the last cell is reached via diagnal link.  This
	  // is a fix for the case where global last node is the only
	  // node to enter for a particular diagnal sub-board. This is
	  // too generic solution for abovev scenario, but is a quick
	  // one.
	  if ((!m_for_knight) && reached_via_diagnal (sol, ex, ey) )
	    {
	      // printf("Skipping path for reaching last node via diagnal path\n");
	      // getchar();
	      return false;
	    }
	  
	  // printf("FOUND destinationi (%d %d) (%d %d)\n", 
	  // 	     m_sx, m_sy, ex, ey);
	  print_solution (sol, ex, ey);
	  return true;
	}
      
      // if found required destination, return true
      return false;
    }
  

  child_list_t cl;
  get_ordered_children(sol, x, y, cl);
  // display_board(sol);
  // printf("children (%d %d): %lu\n", x, y, cl.size());
  // for (int i=0; i< cl.size(); i++)
  //   {
  //     printf("(%d,%d) ", cl[i].index, cl[i].count);
  //   }
  // printf("\n");
  // getchar();




  /* Try all next moves from the current coordinate x, y */
  for (int i=0; (i< cl.size())
	 && (!m_abort); i++)
    {
      int k = cl[i].index;
      // printf("exploring child %d, ct=%d\n", cl[i].index, cl[i].count);
      // getchar();
      
      
      // int k= cl.top().index; // get the best child to explore
      // cl.pop();

      next_x = x + xMove[k];
      next_y = y + yMove[k];
      if (isSafe(sol, next_x, next_y))
	{
	  if (((movei < DIM_SIZE_X * DIM_SIZE_Y -3)
	       && (next_x == ex) && (next_y == ey)))
	    {
	      //printf("skipping last node at movie =%d\n", movei);
	    }
	  else
	    {
	      
	      sol.m_values[next_x][next_y] = movei;
	  
	      if (solveKTUtil(next_x, next_y, ex, ey, 
			      movei+1, desired_sol_movei, sol) == true)
		return true;
	      else
		sol.m_values[next_x][next_y] = -1;// backtracking
	    }
	  
	}
    }
 
  return false;
}

// ---------- Public functions  --------------------------


void k_tour_t::reset()
{
  m_abort = false;
  m_steps = 0;
  m_big_step =0;
  num_solutions =0;
}




bool k_tour_t::solveKT(int sx, int sy, int ex, int ey, solution_t &sol)
{
  m_abort = false;
  reset();
  m_sx = sx;
  m_sy = sy;

 
  /* Initialization of solution matrix */
  for (int x = 0; x < DIM_SIZE_X; x++)
    for (int y = 0; y < DIM_SIZE_Y; y++)
      sol.m_values[x][y] = -1;
 
  sol.m_values[sx][sy]  = 0;

  // Solution length expected, based on color of start and end node
  int desired_sol_movei;
  if ( (sx+sy)%2 == (ex+ey)%2) // same colors, then skip one cell
    desired_sol_movei = DIM_SIZE_X * DIM_SIZE_Y -1;
  else // if opposite color, then can cover all cells
    desired_sol_movei = DIM_SIZE_X * DIM_SIZE_Y;
  printf("Desired sol length %d\n", desired_sol_movei);
 
  /* Start from 0,0 and explore all tours using solveKTUtil() */
  if(!solveKTUtil(sx, sy, ex, ey, 1, desired_sol_movei, sol)) // if failed
    {
      //printf("Solution does not exist");
      return false;
    }
  else  // if found a path
    {
      printf("NUM_STEPS for (%d %d) (%d %d): %d\n",
	     sx, sy, ex, ey, m_steps);
      sol.display_board();
    }
 
  return true;
}





bool k_tour_t::solveKT_new(int sx, int sy, int ex, int ey, solution_t &sol,
			   std::vector <node_t> const &start_seq)
{
  //  printf("start seq lenght %lu\n", start_seq.len());

  m_abort = false;
  reset();
  m_sx = sx;
  m_sy = sy;

  /* Initialization of solution matrix */
  for (int x = 0; x < DIM_SIZE_X; x++)
    for (int y = 0; y < DIM_SIZE_Y; y++)
      sol.m_values[x][y] = -1;


  sol.m_values[sx][sy]  = 0;
  // set the entries with rank
  for (int i=0; i< start_seq.size(); i++)
    {
      sol.m_values[start_seq[i].posx][start_seq[i].posy] = i;
    }
 


  // Solution length expected, based on color of start and end node
  int desired_sol_movei;
  if (m_for_knight)
    { // for knight's movement.
      if ( (sx+sy)%2 == (ex+ey)%2) // same colors, then skip one cell
	desired_sol_movei = DIM_SIZE_X * DIM_SIZE_Y -1;
      else // if opposite color, then can cover all cells
	desired_sol_movei = DIM_SIZE_X * DIM_SIZE_Y;
    }
  else // for king type movement (fors sub-board search)
    {
      desired_sol_movei = DIM_SIZE_X * DIM_SIZE_Y;
    }
      
  printf("Desired sol length %d\n", desired_sol_movei);

  int search_sx; // start-x for searching
  int search_sy; // start-x for searching
  search_sx = start_seq[ start_seq.size()-1].posx;
  search_sy = start_seq[ start_seq.size()-1].posy;
  printf(" search sxy (%d %d)\n", search_sx, search_sy);
 
  /* Start from 0,0 and explore all tours using solveKTUtil() */
  if(!solveKTUtil(search_sx, search_sy, ex, ey, 
		  start_seq.size(), desired_sol_movei, sol)) // if failed
    {
      printf("Solution does not exist");
      return false;
    }
  else  // if found a path
    {
      printf("NUM_STEPS for (%d %d) (%d %d): %d\n",
	     sx, sy, ex, ey, m_steps);
      sol.display_board();
    }
 
  return true;
}


// bool solveKT_bi (int sx, int sy, int ex, int ey, solution_t &sol,
// 		 std::vector <node_t> const &start_seq)
// {
//   reset();
//   if (!solveKT_new (sx, sy, ex, ey, sol))
//     { // if not found solution

//       printf("reverse searching\n");
//       rest();
//       sol.reset(-1);
//       if (!solveKT_new (ex, ey, sx, sy, sol))
// 	{
// 	  printf("FAILED SEARCH bi-direction (%d %d) to (%d %d), board size(%d %d)\n",
// 		 sx, sy, ex, ey, DIM_SIZE_X, DIM_SIZE_Y);

// 	  return false;
// 	}
//     }

//   return true;
// }
