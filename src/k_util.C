//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .C file with implementations of utility functions.

    @author Suresh Golconda
*/

#include "k_util.H"
#include <stdio.h>
#include <cstring>


using namespace std;
using namespace knight_path_t;



bool knight_path_t::same_color (int x1, int y1, int x2, int y2)
{
  return (((x1 + y1) %2) ==  ((x2+y2)%2));
}


color_t knight_path_t::get_cell_color (int x, int y)
{
  if ((x+y) %2 == 0)
    return WHITE;
  else
    return BLACK;
}

string knight_path_t::get_color_name(color_t color)
{
  if (color == WHITE)
    return "WHITE";
  else if (color  == BLACK)
    return "BLACK";
  else
    return "NOCOLOR";
}


bool knight_path_t::scan_dim_src_dest (string indata_, 
				       int &sx, int &sy, int &ex, int &ey)
{
  char *str;
  const char delimiters[] = ", ";

  str = strdup (indata_.c_str());
  //  strcpy (str, indata.c_str ());
  char *ch_key = strtok (str, delimiters);

  sscanf (ch_key, "%d", &sx);
  ch_key = strtok (NULL, delimiters);

  if (ch_key == NULL)
    return false;
  sscanf (ch_key, "%d", &(sy));
  ch_key = strtok (NULL, delimiters);

  if (ch_key == NULL)
    return false;
  sscanf (ch_key, "%d", &(ex));
  ch_key = strtok (NULL, delimiters);

  if (ch_key == NULL)
    return false;
  sscanf (ch_key, "%d", &(ey));
  ch_key = strtok (NULL, delimiters);

  return true;
}


void knight_path_t::print_path(vector<node_t> const &path)
{
  printf("Path size=%lu\n", path.size());
  for (int i=0; i < path.size(); i++)
    printf("(%d %d) ", path[i].posx, path[i].posy);
  printf("\n");
}

