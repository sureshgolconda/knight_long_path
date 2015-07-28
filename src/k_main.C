//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/** @mainpage Knight Path Problem: This is problem to find certain
    paths that a knight can follow.

    Copyright (c) 2014. All Right Reserved Suresh Golconda.
    @author Suresh Golconda

    @par Description: The system finds the longest path from given
    start and end node on a 32x32 chess board.
*/


#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include <libgen.h> 

#include "k_tour.H"
#include "k_getpath.H"
#include "k_sub_board.H"
#include "k_32_search.H"
#include "k_searcher.H"
#include "k_util.H"
#include "k_path_adapter.H"


using namespace std;
using namespace knight_path_t;

char const *progname;


/**
   Prints the program usage options.
*/
void print_usage ()
{
  cout << "Usage: " <<progname <<"\n"
       << "\t[-m maxx,maxy]\t To specify the x and y dimension of the chess board. Coma seperated, do not include space. eg: -m 32,32.\n"

       << "\t[-s x,y]\t To specify the start coordinates for for the path. Coma seperated, do not include space. eg: -s 0,2 \n"

       << "\t[-e x,y]\t To specify the end coordinates of the path. Coma seperated, do not include space. eg: -s 10,20 \n"

       << "\t[-d]\t\t Disable using precomputed paths from database. Paths for smaller grids such as 6x6.\n"

       << "\t[-f filename]\t Used for generating hamilton path for smaller grids 6x6, 6x8, 8x8. To specify input file with list of start-end nodes to search for\n"

       << "\t[-l number]\t limit on number of iterations for each search. Default [10]\n"

       << "\t[-a]\t\t Find longest path on 32x32 grid from given start to end. Do provide start-end coordinates. Automatically use grid size of 32x32.\n"
       << endl;
  cout <<"Please refer README file for more information, and scripts folder for example usage" <<endl;

  cout <<"\t$echo \"0,0 10,15\" | ../bin/knight_long_path -a" <<endl;
  cout <<"(or)\t$ ../bin/knight_long_path -a -s 0,0 -e 10,15" <<endl;
  cout <<"Example usage: Find longest path from (0,0) to (10, 15):" <<endl;

}


/**
   Returns the longest possible path from given start to end node and
   board size
*/
int expected_path_length(int maxx, int maxy, int sx, int sy,
			 int ex, int ey)
{
  // if same color, then path length is number of nodes -1
  if (same_color (sx, sy, ex, ey))
    return (maxx * maxy) -1;
  else
    return (maxx * maxy);
}



/**
   Gets the start start-end node for 32 grid from input stream and
   finds the longest path from start-end node without visiting a node
   twice.
*/
void run_longest_path_32_grid(int gsx=-1, int gsy=-1,
			      int gex=-1, int gey=-1,
			      bool use_db = false)
{

  // --- default values ---
  int maxx=32;
  int maxy=32;

  // if no arguments are passed
  if (gsx == -1)
    {// --- read start-end node's position from input stream
      string line;
      printf("Enter start and end coordinates: eg: 0,1 2,3\n");
      getline (cin, line);
      scan_dim_src_dest (line, gsx, gsy, gex, gey);
    }
  printf("Searching for longest path on 32x32 grid from (%d %d) to (%d %d)\n",
	 gsx, gsy, gex, gey);


  // --- make sure that start and node are different
  if ((gsx == gex) && (gsy == gey))
    {
      printf("Start and end node cannot be same, as path does not allow visiting a node more than once (%d %d).\n", gsx, gsy);
      printf("Please enter unique start and end nodes\n");
      return;
    }

  // Check for valid start and end coordinates
  if (gsx < 0 || gsx >= maxx ||
      gsy < 0 || gsy >= maxy ||
      gex < 0 || gex >= maxx ||
      gey < 0 || gey >= maxy)
    {
      printf("Invalid input coordinate, provide start-end coordinate between (0,0) and (%d, %d)\n", maxx-1, maxy-1);
      return;
    }

  // --- Search for the longest path
  k_32_search_t k32 (use_db);
  vector<node_t> longest_path;
  k32.search_longest_path_32x(gsx, gsy, gex, gey, longest_path);


  // Compute expected size of the longest path
  int expected_length = expected_path_length (maxx, maxy,
					      gsx, gsy,
					      gex, gey);


  //Check if path searched is of correct length
  if (expected_length != longest_path.size())
    {
      printf("ERROR: ");
    }
  else // if correct path is found, then print to screen
    {
      //print_path (longest_path);
      display_path_32board (longest_path, gsx, gsy, gex, gey);
      printf("SUCCESS: ");
    }

  printf("Expected longest path:%d, computed path length:%lu\n",
	 expected_length,
	 longest_path.size());
}



/** Function to test the implementation of k_path_adapater_t class.
    Implements two instances of k_path_adapter_t one using database
    and other without using database.
*/
void test_path_adapter()
{
  printf("Testing path adapter \n");

  // Default values
  int maxx = 8;
  int maxy = 8;
  int sx=0;
  int sy = 0;
  int ex = 1;
  int ey = 6;
  vector<node_t> path;

  // Adapter to NOT use pre-computed database
  k_path_adapter_t path_adapter (false);

  // get path
  path_adapter.get_path (maxx, maxy, sx, sy, ex, ey, path);
  printf("Path size when not using database %lu\n", path.size());

  // Adapter to USE pre-computed database
  k_path_adapter_t path_adapter2 (true);

  path.clear();
  if (!path_adapter2.get_path (maxx, maxy, sx, sy, ex, ey, path))
    printf("FAILED from db\n");
  else
    {
      printf("Path size, when using database %lu\n", path.size());
      //print_path (path);
      display_path_32board (path, sx, sy, ex, ey);
    }

}


/** Main entry point. Will parse user options and initiate required
    methods for generating output
*/
int main(int argc_, char **argv_)
{
  int opt;
  bool usage_error = false;
  progname = basename (argv_[0]);

  int maxx =8; // default values
  int maxy =8;
  int gsx = -1;
  int gsy =-1;
  int gex =-1;
  int gey =-1;

  int search_limit = 10;
  bool use_db = true; // to use pre-computed paths for smaller grid

  string filename;

  int const TASK_CUSTOM_SEARCH = 1;
  int const TASK_SEARCH_ALL = 2;
  int const TASK_32_GRID = 3;
  int const TASK_CUSTOM_TEST = 4;
  // int const TASK_TEST_ALL_32_GRID = 5;
  int task_to_perform = TASK_SEARCH_ALL; //default


  // parse and get input controls
  while ((opt = getopt (argc_, argv_, "tdm:s:e:f:l:a?")) != -1)
    switch (opt)
      {
      case 't': // for developer's testing only
	task_to_perform= TASK_CUSTOM_TEST;
	break;

      case 'd': // to disable using pre-computed values from database
	use_db = false;
	break;

      case 'm': // verify a given path is valid
	sscanf(optarg, "%d,%d", &maxx, &maxy);
	break;

      case 's': // to provide start (xy)
	sscanf(optarg, "%d,%d", &gsx, &gsy);
	break;

      case 'e':  // to provide end (xy)
	sscanf(optarg, "%d,%d", &gex, &gey);
	break;

      case 'f': // For custom search. Custom as in points provided in input file
	task_to_perform = TASK_CUSTOM_SEARCH;
	filename = optarg;
	break;

      case 'l': // set limit on how many iteration of search is allowed
	sscanf(optarg, "%d", &search_limit);
	break;

      case 'a': // Find longest path on 32x32 grid from given start to end
	task_to_perform = TASK_32_GRID;
	break;

      default:
	usage_error = true;

      }

  if (usage_error)
    {
      print_usage();
      return 1;
    }


  // -------- Implementation -----------------
  switch (task_to_perform)
    {

    case TASK_CUSTOM_TEST:  // For running some quick custom test (debug)
      //test_custom_sample();
      test_path_adapter();
      break;


    case TASK_CUSTOM_SEARCH:
      {
	k_searcher_t k_searcher;
	//search_custom_set ("//home//suresh//learn//k4//src//t.fail.flip");
	k_searcher.search_custom_set (maxx, maxy, filename, search_limit);
      }
      break;


    case TASK_SEARCH_ALL:
      {
	k_searcher_t k_searcher;
	k_tour_t kt (maxx, maxy, search_limit);

	for (int sx=0; sx <maxx; sx++)
	  for (int sy =0; sy<maxy; sy++)
	    {
	      printf("Solution from (%d %d)\n", sx, sy);
	      int num_solutions =
		k_searcher.solutions_from (sx, sy, kt, maxx, maxy);
	      printf("Number of solutions from (%d %d)= %d\n",
		     sx, sy, num_solutions);
	    }
      }
      break;


    case TASK_32_GRID:
      printf("Runnig for 32x32\n");
      run_longest_path_32_grid(gsx, gsy, gex, gey, use_db);
      break;


    default:
      usage_error = true;
    }


  if (usage_error)
    {
      print_usage();
      return 1;
    }


  printf("Done\n");
  return 0;
}

