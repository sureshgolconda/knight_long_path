//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .C file with implementations of k_sub_board_t class's methods.

   @author Suresh Golconda
*/

#include "k_sub_board.H"
using namespace std;
using namespace knight_path_t;

void k_sub_board_t::set_max_xy(int maxx_, int maxy_)
{
  // delete old memory
  if (maxx != 0 && maxy != 0)
    free_mem();

  maxx = maxx_;
  maxy = maxy_;

  // allocate new memory
  m_cells = new int* [maxx];
  for (int i=0; i< maxx; i++)
    m_cells[i] = new int [maxy];
}

