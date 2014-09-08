knight_long_path
================

Project to find the longest path in a 32x32 chess board grid.


-----------------------------------

- Tried to generate end to end path for 8x8 grid:
- Filtered out following cases:
  -- color (start) == color (end)
  -- end <before> start. To avoid symetry
  -- 

- While exploring skipping nodes
  -- skipping intermediate nodes if it is equal to end node.
  -- 
-----------------------------------------
-- To make sure for a 8x8 grid, there is a touring path from any cell to any cell. Implementing the system to walk over every possible valid start-end combination and generating touring path.

Keeping a limit of 100,000,000 iteration for search, could find following number of paths:
Found:
Failed:

	- Need to search failed paths reversing start/end.
	- OR, need to search failed paths with different heuristics.

Tried to generate all possible start-end knight tour path:
      - tried to iterate the search and note done every found path with unique start-end. Soon realized that system was spending too much time finding multiple paths between same pair of start-end nodes.
      - re-wrote the system to find path to a targeted end point. It does not make sense as same heuristic and search will be made as before. The performance boost came up when based on desired target node, intermediate search was cut down (another optimization applied).

      -Some of the searches (star-end pairs) were taking too much time, then added limit on search to 100M. For the failed search swapping the end and start nodes made getting results quicker.

-----------------------------------------
Total explored paths:
      - 8x8: 594 (556G, 38B) (36G) (2G at 740 million iteration)
      - 8x6: 433 (414G, 19B) (19G)
      - 6x8: 362 (339G, 23B) (23G)
      - 6x6: 312 (310G, 2B) (2G)

Upgarded sytem total: formula ((maxx * maxy)*(maxx * maxy))/4
      - 8x8: 1024  (1020G, 4B) 
      - 8x6: 576   (571G, 5B)
      - 6x8: 576   (574G, 2B)
      - 6x6: 324   (324G)

-----------------------------------------
	- board 6x8 is 90 turn of board 8x6

-----------------------------------------
sub-board:
=========
	Each subboard will have one start and one end cell. If it holds global start node, it forms the start cell, else an entry point into sub-board forms a start-cell.
	Similarly, if sub-board holds global end node, it forms the end cell, else an exit point from sub-board forms a end-cell.
	Except for the case, when start, end node falls within same sub-graph.
	
-----------------------------------------
Then wrote a program to read the discovered paths to make sure every possible search combination (start-end) is covered.


leaving point:
	stitch the sub board paths to form one big path k2_main.C: find_complete_path()
---------------------------------

Issues discovered, while stitching need to considering that each
sub-board's start point is of same color as global start point. 
	This is simpler as all sub-boards have their sizes of even length

---------------------------------
TODO: 
     X - having some seg fault when running with 8x6, 6x8
      - Need to look into failed path generation.
      - add destructor with memory in board.H (knight_problem), similar to type_def.H:sub_board_t
     X - need to handle special cases of start and end falling in same sub-board
     X- need to handle if c (start) == c(end)

Status:
All issues cleared. Need to test by nesting over start-end combinations.

---------------------------------
Convention
==========
- 2-dimensional array for grid have 1st index for 'x', 2nd for 'y' eg: grid[xi][yi]
- All feasible nested for loops walk through X, followed by Y. Except for display, which walks through 'Y' followed by 'X' (row after row display)

---------------------------------
Clean up:
- copyright, date, author.
- all input arguments should be const
- main.C usage, eg.

- delete trailing white spaces
- auto indentation
- doxygen doc
- member variable notation
- temp variables
- set: input arguments before, return arguments last
- all possible functions as private
- line length



- code documentation

- Algorithm documentation
- Some diagrams
- Merge solveKT wiht solveKT_new

- enclose all optional print within m_debug flag
- Check for error user input (start-end node is same)


order of files:
--------------
k_tour
k_util
sub_board
k_searcher
k_path_adapter
k_getpath
k_32search
board

======================================
Run with valgrind for memory analysis:
a) compile with ($PROFILE) UNSET.
b) run as: 
$valgrind --leak-check=yes --leak-check=full --show-reachable=yes ../bin/knight_long_path -a


======================================
Programming Standards followed:

- For classes which have dynamically memory allocation, their copy constructors are disabled to prevent shallow copy: k_sub_board_t, solution_t

- All variables passed by reference are marked 'const' if they are just inputs
- 

========================================
Troubleshooting:
----------------

If latex conversion generation to pdf is crashing with error (""). 
Solution:
Install texlive-fonts-recommended, ghostscript modules
	$sudo apt-get install texlive-fonts-recommended ghostscript
