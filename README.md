======================================================================
		Longest uncrossed knight's path	on a 32x32 chess board
======================================================================

Overview:
---------
Present system tries to impelement algorithm for finding the longest knight's path on a 32x32 chess board (grid).

- Present README.md gives a quick introduction to the overall system. 
- "docs/knight_longest_path_documentation.pdf" provides more code/implementaiton documentation (also docs/html/index.html) 
- "docs/knight_longest_path.pdf" provides some quick overview into the solution.


Instructions:
-------------
Compile:

$cd src

$ make

Run: (note: run the scripts from 'scripts' folder)

$cd scripts


$ ./level_5_sample_run.sh

(or) to directly run the c++ program use following syntax. To find longest path from (0,0) to (31,3):

$bin/knight_long_path -a -s 0,0 -e 31,3

Provide -d option to the 'knight_long_path' program if you prefer not to use pre-computed subboard paths.



Notes:
------
- Run the program (knight_long_path) or scripts from 'src' or 'scripts' folder. This is because the system tries to access 'data' folder via relative path '../data/'. This is not required if no pre-computed paths are used. That is running with "-d" option

- Program is analyzed with gprof for CPU usage and
  optimization. Including sample analysis in "analysis" folder.

- Program is analyzed for memory utilization using valgrind.

- docs/knight_longest_path_documentation.pdf will provide some documentation of the code. docs/html covers the same documentation in html format.

 



System requirement:
-----------------
- Linux OS. System was developed and testing on ubuntu 12.04 (dual-core, 2GB RAM).
- g++ compiler: If not already installed, can install on ubuntu machine with following command:
     $sudo apt-get install g++



System organization:
--------------------
src: Contains c++ code.
script: Contains scripts for running each of the 4 problems with required program parameters.
data: Contains some of the pre-computed longest paths for smaller grids such as: 6x6, 6x8, 8x6, 8x8.
analysis: Contains sample gprof profiling output
docs: Contains code document generated using Doxygen in html and .pdf format
bin: Contains executables generated from compiling 




Terminology/Convention:
-----------------------
- Board: Is the chess board reprsentation. Present system only considers square board.

- Path: Sequence of knight steps (cell/node position).

- x-axis: horizontal axis (0 on left)
- y-axis: vertical axis (0 on top)

- maxx: Dimension of the board along x axis
- maxy: Dimension of the board along y axis

- Indexing starts from 0 to N-1

- 2-dimensional array for grid have 1st index for 'x', 2nd for 'y' eg: grid[xi][yi]

- All feasible nested for loops walk through X, followed by Y. Except for display, which walks through 'Y' followed by 'X' (row after row display)

Algorithm:
----------
Please refer docs/knight_longest_path_solution.pdf for description on the solution implemented

Testing:
--------
- The algorithmic approach is tested by running the programs on randomly generated start-end nodes on 32x32 grid. And the test helped to discover few special/exception cases that are fixed in the present implementation.
- System implementation is testing at code-level by using gprof for CPU optimization, and valgrind for proper memory handling.


Overview of major classes/files:
--------------------------
k_util.[CH]:	 Contains utility functions
k_tour.[CH]: 	 Contains class that performs actual searching for hamilton path.
k_sub_board.[CH]: Contains operations at sub-board level
k_searcher.[CH]:  Contains implementations to generate database of paths for 6x6 and other smaller grids.
k_path_adapter.[CH]:Adapter class that transparently provides path by searching (k_tour) or looking up in pre-computed databaes (k_getpath)
k_getpath.[CH]:	 Loads up pre-computed paths for small grids and provides query based access to the paths.
k_32search.[CH]: Manages the whole system from top-level sub-board level search to low-level path, stitching sub-boards, etc.



Code practise:
--------------

- Most input objects for functions are passed by 'const reference'. 

- Classes which have dynamically allocated memory have their pointers
  private, copy constructors disabled, destructor delecate memory, constructor allocate memory, and resize() function to cleanly de-allocate old and allocate new memory.

- Avoided creating and deleting of objects heavily (especially within inner-most loop). Speeds up.

- Heavily called up small functions placed in .H, so compiler can inline them.

- As the program is done as part of research and had to figure out
  details and new ideas, it was hard to know the best design pattern
  before on. But constant effort was put to refactor the code.

- Code is documented, but math, logic, and figurative description will
  have to be covered as seperate document.



Create Doxygen documentat of the code:
--------------------------------------
- From 'src' folder, type following. May require to install doxygen.
  $ doxygen
- Will create documentation in ../docs folder.
- To create pdf of the document, enter docs/latex folder and type 'make'. Will require to have following installed: latex, texlive-fonts-recommended, ghostscript.
       $sudo apt-get install latex texlive-fonts-recommended ghostscript


Troubleshooting:
----------------
- After checkingout of the code, if not able to run the scripts, check to make sure scripts have got the executable permission "x".


Future Scope:
-------------
- Try to iterate the program to run for all possible combinations of start-end node combination. A total of 1,047,552 (if start-end cannot be same node), (1,047,552/2) if start/end nodes can be swapped.


