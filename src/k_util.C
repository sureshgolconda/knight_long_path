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

void knight_path_t::display_path_32board(vector<node_t> const &path,
					 int gsx, int gsy, int gex, int gey)
{
  printf("\nSolution:\n");
  printf("Following is one of the longest path on 32x32 grid, from (%d %d) to (%d %d), length= %lu\n",
	 gsx, gsy, gex, gey, path.size());
  int maxx = 32;
  int maxy = 32;
  int values[maxx][maxy];

  for (int y=0; y<maxy; y++)
    for (int x=0; x<maxx; x++)
      values[x][y] = -1;

  for (int i=0; i< path.size(); i++)
    {
      values[path[i].posx][path[i].posy] = i;
    }

  for (int y=0; y<maxy; y++)
    {
    for (int x=0; x<maxx; x++)
      {
	if (values[x][y] == -1)
	  printf("     ");
	else
	  printf(" %4d", values[x][y]);
      }
    printf("\n");
    }
}

