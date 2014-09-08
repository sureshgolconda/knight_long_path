//Copyright (c) 2014. All Right Reserved Suresh Golconda.
/**
   .H file to define k_sub_board_t class, which implements operations at sub_board level

   @author Suresh Golconda
*/
#ifndef KNIGHT_SUB_BOARD_H
#define KNIGHT_SUB_BOARD_H


namespace knight_path_t
{
  /**
     For performing sub-board related operations
  */
  class k_sub_board_t
  {
  private:
    // limiting the access to dimension so can guarantee proper
    // allocation and deletion of memory, while seting the dimensions
    int maxx;  // dimensions of grid
    int maxy;
  public:
    int base_gx;
    int base_gy; //global-xy which is the base for sub-board

    int lsx; // local -start and end points
    int lsy;
    int lex;
    int ley;

    int **m_cells;

    k_sub_board_t(int maxx_,  int maxy_):
      maxx(maxx_), maxy(maxy_)
    {
      base_gx =-1; base_gy =-1;
      lsx =-1; lsy =-1;
      lex =-1; ley =-1;

      // setup the board
      m_cells = new int* [maxx];
      for (int i=0; i< maxx; i++)
	m_cells[i] = new int [maxy];

    }

    k_sub_board_t()
    {
      base_gx =-1; base_gy =-1;
      lsx =-1; lsy =-1;
      lex =-1; ley =-1;

      maxx = 0;
      maxy =0;
    }


    /** To re-set the dimensions of the grid */
    void set_max_xy(int maxx_, int maxy_);


    /** Returns the y-dim size*/
    int get_maxx() const
    {
      return maxx;
    }

    /** Returns the x-dim size*/
    int get_maxy() const
    {
      return maxy;
    }

    /**
       returns true if given point is an entry point, else false
    */
    bool is_entry_point (int x, int y)
    {
      return (x == lsx) && (y == lsy);
    }

    /**
       returns true if given point is an exit point, else false
    */
    bool is_exit_point (int x, int y)
    {
      return (x == lex) && (y == ley);
    }

    /**
       converts from local sub-grid coordinate to global  coordinates
    */
    int to_global_x (int x_) const
    {
      return base_gx + x_;
    }
    /**
       converts from local sub-grid coordinate to global  coordinates
    */
    int to_global_y (int y_) const
    {
      return base_gy + y_;
    }
    /**
       converts from global coordinate to local sub-grid coordinates
    */
    int to_local_x (int x_) const
    {
      return x_ - base_gx;
    }
    /**
       converts from global coordinate to local sub-grid coordinates
    */
    int to_local_y (int y_) const
    {
      return y_ - base_gy;
    }


    /// deleting following functions provided by c++, to prevent
    /// shallow copy of this objects: As there is dynamic memory
    /// allocated
    k_sub_board_t(k_sub_board_t const&) = delete;
    void operator=(k_sub_board_t const &) = delete;


    ~k_sub_board_t()
    {
      free_mem();
    }

    /**
       deletes the two dimensiona array created for storing m_cells
    */
    void free_mem()
    {
      for (int i=0; i< maxx; i++)
	{
	  delete[] m_cells[i];
	}
      delete[] m_cells;
    }
  };


  /** Grid of sub-boards
   */
  struct sub_board_grid_t
  { // Array convention: m_boards [XI] [YI]
    k_sub_board_t m_boards[5][5];
  };


};

#endif
