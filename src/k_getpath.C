//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .C file with implementations of k_getpath_t class's methods.

   @author Suresh Golconda
*/

#include "k_getpath.H"
#include <cstring>
#include <fstream>

using namespace std;
using namespace knight_path_t;


void k_getpath_t::load_parameters(tour_path_list_t &pl, string filename)
{

  printf("Loading file: %s\n", filename.c_str());
  string line;
  ifstream myfile (filename);

  if (myfile.is_open())
    {
      while (getline (myfile,line))
	{
	  tour_path_t tp;
	  scan_path_string (tp, line);
	  pl.push_back(tp);
	}
    }
  myfile.close();
}


bool k_getpath_t::scan_path_string (tour_path_t &tp, string indata_)
{

  char *str = strdup (indata_.c_str());;
  const char delimiters[] = ",(): ";

  char *ch_key = strtok (str, delimiters);

  ch_key = strtok (NULL, delimiters); //sx
  tp.m_para.m_sx = atoi(ch_key);

  ch_key = strtok (NULL, delimiters); //sy
  tp.m_para.m_sy = atoi(ch_key);

  ch_key = strtok (NULL, delimiters); //ex
  tp.m_para.m_ex = atoi(ch_key);

  ch_key = strtok (NULL, delimiters); //ey
  tp.m_para.m_ey = atoi(ch_key);

  ch_key = strtok (NULL, delimiters);

  while (ch_key != NULL)
    {
      tp.m_tour.push_back(atoi(ch_key));
      ch_key = strtok (NULL, delimiters);
    }
  return true;
}



bool k_getpath_t::get_path (int maxx, int maxy, int sx, int sy,
			    int ex, int ey, vector<node_t> &node_list)
{
  /// internally calls get_path with results provided as
  /// tour_path_t. Then converts results to a vector of nodes.
  tour_path_t tp;
  if (!get_path(maxx, maxy, sx, sy, ex, ey, tp))
    return false;


  // Compute number of valid points in the tp
  int num_valid_points =0;
  for (int i=0; i< tp.m_tour.size(); i++)
    {
      if (tp.m_tour[i] != -1) // if invalid default value
	num_valid_points++;
      // printf("(val=%d, i=%d, v=%d) ",tp.m_tour[i], i, num_valid_points);
    }



  // printf("tour path size=%lu, num valid=%d\n",
  // 	 tp.m_tour.size(), num_valid_points);
  node_t nodes[num_valid_points];

  // Convert from tp to node_list
  for (int i=0; i< tp.m_tour.size(); i++)
    {
      int ind = tp.m_tour[i]; // index in the board

      if (ind != -1) // if valid index
	{
	  nodes[ind].posx = i % maxx;
	  nodes[ind].posy = i / maxx;
	}
    }

  // create a list to return
  node_list.clear();
  for (int i=0; i< num_valid_points; i++)
    node_list.push_back (nodes[i]);

  return true;
}


/** Checks maxx, maxy to determine which database to look for path. */
bool k_getpath_t::get_path (int maxx, int maxy, int sx, int sy,
			    int ex, int ey, tour_path_t &tp)
{
  // printf("searching for: (%d %d) (%d %d), list=%lu\n", sx, sy, ex, ey,
  // 	 m_6x6_para.size());


  /// Checks maxx, maxy to determine which database to look for path.
  if (maxx == 6 && maxy == 6)     //6x6
    {
      if ((sx + sy) %2 != (ex+ey)%2) // if different colors then complete path
	return get_path_board(m_6x6_para, sx, sy, ex, ey, tp);
      else // else path of size one less
	{
	  printf("searching ODD path\n");
	  return get_path_board(m_6x6_odd_para, sx, sy, ex, ey, tp);
	}
    }

  else if (maxx == 6 && maxy == 8)  //6x8
    {
      return get_path_board(m_6x8_para, sx, sy, ex, ey, tp);
    }

  else if (maxx == 8 && maxy == 6)  //8x6
    {
      return get_path_board(m_8x6_para, sx, sy, ex, ey, tp);
    }

  else if (maxx == 8 && maxy == 8)  //8x8
    {
      return get_path_board(m_8x8_para, sx, sy, ex, ey, tp);
    }

  else
    {
      printf("ERROR: invalid maxx,maxy (%d %d)\n", maxx, maxy);
      return false;
    }
}




bool k_getpath_t::get_path_board(tour_path_list_t const &tp_list,
				 int sx, int sy, int ex, int ey,
				 tour_path_t &tp)
{
  for (int i=0; i< tp_list.size(); i++)
    {
      parameters_t para = tp_list[i].m_para;
      // printf("i=%d, (%d %d) (%d %d)\n", i,
      // 	 para.m_sx, para.m_sy,
      // 	 para.m_ex, para.m_ey);

      if ((para.m_sx == sx) && (para.m_sy == sy) &&
	  (para.m_ex == ex) && (para.m_ey == ey))
	{
	  tp = tp_list[i];
	  return true;
	}
      else
	{
	  if ((para.m_ex == sx) && (para.m_ey == sy) &&
	      (para.m_sx == ex) && (para.m_sy == ey))
	    {
	      tp.m_para.m_sx = sx;
	      tp.m_para.m_sy = sy;
	      tp.m_para.m_ex = ex;
	      tp.m_para.m_ey = ey;


	      // need to reverse path, but complementing the value, ie.
	      int max_value = -1;
	      // Find maximum value in the list. It is assumed minimum is 0, excluding -1
	      for (int j=0; j< tp_list[i].m_tour.size();j++)
		max_value = max (max_value, tp_list[i].m_tour[j]);


	      for (int j=0; j< tp_list[i].m_tour.size(); j++)
		{
		  if (tp_list[i].m_tour[j] == -1)
		    tp.m_tour.push_back (-1);  // do not reverse un visited cell's position
		  else
		    tp.m_tour.push_back (max_value - tp_list[i].m_tour[j]);
		}

	      // for (int j=0; j< tp.m_tour.size(); j++)
	      // 	{
	      // 	  printf("{%d} ", tp.m_tour[j]);
	      // 	}
	      // printf("\n");

	      return true;
	    }
	}

    }
  return false;
}








// ------------ Functions for testing ---------------------

bool k_getpath_t::test_all_paths()
{
  test_paths_for_board (6, 6, true); //including same color source-destination
  // test_paths_for_board (6, 8);
  // test_paths_for_board (8, 6);
  // test_paths_for_board (8, 8);

  return true;
}

bool k_getpath_t::test_paths_for_board(int maxx, int maxy, bool allow_same_color)
{


  //temp
  // tour_path_t temp_tp;
  // if (!get_path (maxx, maxy, 0, 1, 0, 0, temp_tp))
  //   {
  //     printf("NO PATH ??\n");
  //   }
  // return false;

  bool success = true;
  printf("verifying all paths exist for board (%d %d), same_color=%d\n", maxx, maxy,
	 allow_same_color);

  for (int sx=0; sx < maxx; sx++)
    for (int sy=0; sy<maxy; sy++)
      for (int ex=0; ex < maxx; ex++)
	for (int ey=0; ey<maxy; ey++)
	  {
	    if ((sx != ex) || (sy != ey)) // source and destination is not same
	      if ((allow_same_color) ||(!same_color(sx, sy, ex, ey)))
		{
		  tour_path_t tp;

		  if (!get_path (maxx, maxy, sx, sy,
				 ex, ey, tp))
		    {
		      printf("ERROR: Path missing in db: (%d %d), (%d %d)\n",
			     sx, sy, ex, ey);
		      success = false;
		    }
		  else
		    { // verify path is correct, valid knight move
		      // printf("yes:(%d %d), (%d %d)\n",
		      // 	   sx, sy, ex, ey);
		    }
		}
	  }
  return success;
}

void k_getpath_t::print_tour_path (tour_path_list_t const &pl)
{
  //printing
  printf("tour path list size %lu\n", pl.size());

  for (int i=0; i< pl.size(); i++)
    {
      tour_path_t tp = pl[i];
      printf("path read: (%d %d) (%d, %d)(%lu): ",
  	     tp.m_para.m_sx, tp.m_para.m_sy,
  	     tp.m_para.m_ex, tp.m_para.m_ey,
  	     tp.m_tour.size());
      for (int j=0; j< tp.m_tour.size(); j++)
  	{
  	  printf("%3d ", tp.m_tour[j]);
  	}
      printf("\n");
    }
}
