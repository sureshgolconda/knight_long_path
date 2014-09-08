//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .C file with implementations of k_searcher_t class's methods.

   @author Suresh Golconda
*/

#include <fstream>
#include "k_searcher.H"

using namespace std;
using namespace knight_path_t;


bool k_searcher_t::start_end_ordered(int sx, int sy, int ex, int ey)
{
  if ((ex >sx) ||
      ((ex == sx) && (ey > sy)))
    return true;

  return false;
}



int k_searcher_t::solutions_from (int sx, int sy, k_tour_t &kt, int maxx, int maxy)
{
  int num_solutions = 0;
  solution_t temp_sol (maxx, maxy);
  for (int ex=0; ex<maxx; ex++)
    for (int ey=0; ey<maxy; ey++)
      {
	//	printf("(%d %d) to (%d %d)\n",sx, sy, ex, ey);
	if (same_color(sx, sy, ex, ey) && // should not be same color
	    ((sx != ex)|| (sy != ey)) && // start ! = end.todo: extra? same_color covers
	    (start_end_ordered (sx, sy, ex, ey))) // to prevent swapped search

	  {
	    printf("SEARCHING: (%d %d) to (%d %d)\n",sx, sy, ex, ey);
	    kt.reset();
	    temp_sol.reset(-1);
	    if (!kt.solveKT(sx, sy, ex, ey, temp_sol))
	      {
		kt.reset();
		temp_sol.reset(-1);
		if (!kt.solveKT(ex, ey, sx, sy, temp_sol))
		  {
		    printf("FAILED_SEARCH: %d %d %d %d\n",sx, sy, ex, ey);
		    //return num_solutions;
		  }
	      }
	    else
	      num_solutions++;

	  }
      }
  return num_solutions;
}



int  k_searcher_t::num_possible_solution_from (int maxx, int maxy)
{
  int num_solutions = 0;
  for (int sx=0; sx <maxx; sx++)
    for (int sy=0;sy < maxy; sy++)
      for (int ex=0; ex<maxx; ex++)
	for (int ey=0; ey<maxy; ey++)
	  {
	    if (!same_color(sx, sy, ex, ey) && // should be same color
		(start_end_ordered (sx, sy, ex, ey))) // to prevent swapped search

	      {
		num_solutions++;
	      }
	  }
  return num_solutions;
}


void k_searcher_t::scan_custom_set(string filename, parameter_list_t &pl,
				   bool reverse_start_end)
{

  string line;
  ifstream myfile (filename);

  if (myfile.is_open())
    {
      // to get index if required fields when text is changed
      // getline (myfile,line);
      // printf("line: %s\n", line.c_str());
      // printf("(%c)(%c) (%c)(%c)\n", line[15], line[17], line[19], line[21]);
      // return;

      while (getline (myfile,line))
	{
	  // for string: "COUNT_EXCEEDED 11, 110000000 (0 1) to (2 6)"
	  // int sx = line[30]-'0';
	  // int sy = line[32]-'0';
	  // int ex = line[39] -'0';
	  // int ey = line[41]-'0';

	  // for string: "FAILED_SEARCH: 1 2 4 2"
	  int sx = line[15]-'0';
	  int sy = line[17]-'0';
	  int ex = line[19] -'0';
	  int ey = line[21]-'0';


	  if (!reverse_start_end)
	    {
	      parameters_t para (sx, sy, ex, ey);
	      pl.push_back (para);
	    }
	  else
	    { // reverse the start and end points
	      parameters_t para (ex, ey, sx, sy);
	      pl.push_back (para);
	    }
	}
      myfile.close();
    }
}

void k_searcher_t::search_custom_set(int maxx, int maxy,
				     string filename, int search_limit)
{
  printf("Custom infile serach\n");
  parameter_list_t pl;
  solution_t temp_sol (maxx, maxy);

  scan_custom_set (filename, pl, true);


  k_tour_t kt (maxx, maxy, search_limit);

  for (int i=0; i< pl.size(); i++)
    {
      printf("Searching: (%d, %d), (%d %d)\n", pl[i].m_sx,
	     pl[i].m_sy, pl[i].m_ex, pl[i].m_ey);
      kt.reset();
      if (!kt.solveKT(pl[i].m_sx, pl[i].m_sy,
		      pl[i].m_ex, pl[i].m_ey,
		      temp_sol))
	{
	  printf("Second search\n");
	  kt.reset();
	  // search again reversing the start end
	  if (!kt.solveKT(pl[i].m_ex, pl[i].m_ey,
			  pl[i].m_sx, pl[i].m_sy,
			  temp_sol))
	    {
	      printf("FAILED_SEARCH: %d %d %d %d\n",
		     pl[i].m_sx, pl[i].m_sy,
		     pl[i].m_ex, pl[i].m_ey);
	    }
	}
    }
}

