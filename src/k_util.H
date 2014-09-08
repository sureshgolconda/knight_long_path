//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .H file to define utility functions.

    @author Suresh Golconda
*/

#ifndef KNIGHT_TYPE_DEF_H
#define KNIGHT_TYPE_DEF_H

#include <vector>
#include <string>

namespace knight_path_t
{
  /**      Parameter to store board config.   */
  struct parameters_t
  {
    int m_sx;
    int m_sy;
    int m_ex;
    int m_ey;

    parameters_t (int sx, int sy, int ex, int ey)
    {
      m_sx = sx;
      m_sy = sy;
      m_ex = ex;
      m_ey = ey;
    }
    /** Dummy constructor for allowing declaration of vector of objects */
    parameters_t ()
    { // empty constructor for providing empty tour_path_t
    }
  };

  /** Typedef for declaring vector of parameters */
  typedef std::vector<parameters_t> parameter_list_t;






  /// To maintain a tour parameters (start, end) and vector of
  /// position of cell along the path.
  struct tour_path_t
  {
    parameters_t m_para; //start-end nodes
    std::vector<int> m_tour; // order
  };
  /// To declare a vector of tour_path_t's
  typedef std::vector<tour_path_t> tour_path_list_t;


  /// A node on board
  struct node_t
  {
    int posx;
    int posy;
  };

  /// pair of node, used mostly for representing start-end
  struct node_pair_t
  {
    node_t n1;
    node_t n2;
  };





  /** To define possible stitches interms of relative coordinate
      system. Stitch is applied between sub-boards.*/
  struct stitch_t
  {
    /** +ve add to lower end (0), -ve, sub at higher end (starts with
	-1, with -1 being last index */
    int m_delx1;
    int m_dely1;

    int m_delx2;
    int m_dely2;

    /// Stitch between subboards
    stitch_t(int dx1, int dy1, int dx2, int dy2)
    {
      m_delx1 = dx1;
      m_dely1 = dy1;
      m_delx2 = dx2;
      m_dely2 = dy2;
    }

    stitch_t()
    {     }

    /// Set values of the stitch
    void set(int dx1, int dy1, int dx2, int dy2)
    {
      m_delx1 = dx1;
      m_dely1 = dy1;
      m_delx2 = dx2;
      m_dely2 = dy2;
    }
  };

  /** A vector of stitchs'*/
  typedef std::vector<stitch_t> stitch_list_t;



  /** Enum to reprsent color of a cell*/
  enum color_t
    { 
      BLACK, 
      WHITE
    };



  /** Returns true if given pair of nodes both have similar color */
  bool same_color (int x1, int y1, int x2, int y2);


  /** Returns the color of the given (x, y) node. It is assumed that
      node (0,0) white (typically in cheess). It holds for all
      sub-boards in our case, as all sub-boards have have even size
      dimensions */
  color_t get_cell_color (int x, int y);


  /** Returns the name of the given color as a string. Used for
      printing */
  std::string get_color_name(color_t color);



  /**
     Utility function to scan, src, and destination node's coordinates.
     Expected format: comma seperated four numbers. 

     \return True if scan was successful, else false
  */
  bool scan_dim_src_dest (std::string indata_, int &sx, int &sy, 
			  int &ex, int &ey);

  /**
     Prints the provided path onto screen
   */
  void print_path(std::vector<node_t> const &path);
};

#endif
