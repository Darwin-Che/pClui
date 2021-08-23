#ifndef CLUI_H
#define CLUI_H

#include <string>
#include <vector>
#include <curses.h>
#include <memory>
#include <initializer_list>
#include <map>

#include "Util.h"

namespace Clui {

/*      Interaction Guide
    1) use hjkl to move within a panel from fields to fields
    2) use HJKL to move between panels
    3) press 'm' to enter modify mode, will be able to modify an entry, press Enter to finish
    4) press 'i' to enter insert mode, insert a row, behaviour depend on panel type and row type
    5) press 'd' to delete a row, behaviour depend on panel type
 */

/*      Code Usage Guide (testx.h)
    1) Can have only one Window instance, currently only support Window_Grid with grid layout
    2) Must create a Container that satisfy the following property
    3) Use the Window's function and the container instance to create Panels (private constructor)
    4) Call Window start_service() to start a dedicated thread for clui interactions and updates
 */

/*
        Custom Container for Panel_Vector
    Incurred functions by Panel_Vector:
        int size()
        std::string get_field(int i, int j)
        void set_field(int i, int j, const std::string & s)
        void insert_row(const std::string & s)
        void remove_row(int i)
*/

/*
        Custom Rows
    Incurred functions by BVector:
        static int Row::fnum()
        Row(const std::string & s)
        std::string get_field(int j)
        void set_field(int j, const std::string & s)
*/

struct Cursor {
    int r;
    int c;
};

struct Rectangle {
    int top;
    int left;
    int bottom;
    int right;
    int area() const;
};

template <class R>
class BVector {
private:
    std::vector<R> data;
public:
    BVector(int n = 0);
    static int fnum();
    int size();
    void insert_row(const std::string & row);
    void remove_row(int i);
    std::string get_field(int i, int j);
    void set_field(int i, int j, const std::string & s);
};

class Panel_Base {
public :
    virtual ~Panel_Base() = 0;
    virtual std::string get_cur_field() = 0;
    virtual void update_field(const std::string & s) = 0;
    // not used
    // virtual void update_row(const std::string & s);
    virtual void insert_row(const std::string & s) = 0;
    virtual void remove_row() = 0;

    virtual void request_redraw() = 0;

    /* future features */
    // virtual void select_row();
    // virtual void unselect_row();
    // virtual void swap_row_up();
    // virtual void swap_row_down();

    virtual void cursor_left() = 0;
    virtual void cursor_right() = 0;
    virtual void cursor_up() = 0;
    virtual void cursor_down() = 0;
};

class Window_Grid {
private:
    int width;
    int height;
    int gridw;
    int gridh;
    int gsizew;
    int gsizeh;
    // top, left, pointer; // sort by top,left
    std::map<std::pair<int,int>, Panel_Base *> pos_map_r;
    // left, top, pointer; // sort by left,top
    std::map<std::pair<int,int>, Panel_Base *> pos_map_c;
    Panel_Base * cur_panel; // current highlight Panel
    void draw_grid();
    void cursor_left();
    void cursor_right();
    void cursor_up();
    void cursor_down();
    std::string edit_str(const std::string & str);
public:
    Window_Grid(int gr, int gc);
    ~Window_Grid();
    Rectangle set_panel(Panel_Base * panel, const Rectangle & rect);
    void update();
    void start_service();
};

/*
    V is the wrapper for the vector like data structure
        support get, set, and notify
        std::string get_field(int r, int j);
*/
template <class V> 
class Panel_Vector : public Panel_Base{
private:
    int height;
    int width;
    std::shared_ptr<Window_Grid> window;
    std::shared_ptr<V> v;
    std::vector<int> field_width; // size is R::fnum() + 1
    int visual_start_row;
    WINDOW * panel_win;
    Cursor cursor;
    int row_size(int r);
    void redraw();
public : 
    // field_width param size is R::fnum()
    // Panel_Vector(std::shared_ptr<Window_Grid> w, 
    //             Rectangle r,
    //             std::shared_ptr<V> v_p,
    //             std::initializer_list<int> field_width_p);
    Panel_Vector(std::shared_ptr<Window_Grid> w, 
                Rectangle r,
                std::shared_ptr<V> v_p);
    void set_field_width(std::initializer_list<int> field_width);
    void setup_panel_win(const Rectangle r);
    void init_cursor();
    ~Panel_Vector();

    void request_redraw();

    std::string get_cur_field();
    void update_field(const std::string & s);
    void insert_row(const std::string & s);
    void remove_row();

    void cursor_left();
    void cursor_right();
    void cursor_up();
    void cursor_down();
};

}

#include "BVector.cc"
#include "Panel.cc"
#include "Window_Grid.cc"



#endif
