#include <vector>
#include <memory>
#include <curses.h>
#include "Clui.h"

namespace Clui {

Panel_Base::~Panel_Base() {
    eprintf("pure virtual destructor called \n");
}

/**************
    Constructor
**************/
// template<class V>
// Panel_Vector<V>::Panel_Vector(std::shared_ptr<Window_Grid> w, 
//                             Rectangle r,
//                             std::shared_ptr<V> v_p,
//                             std::initializer_list<int> field_width_p)
//     : window(w), v(v_p) {
//     set_field_width(field_width_p);
//     setup_panel_win(window->set_panel(this, r));
//     init_cursor();
//     request_redraw();
// }
template<class V>
Panel_Vector<V>::Panel_Vector(std::shared_ptr<Window_Grid> w, 
                            Rectangle r,
                            std::shared_ptr<V> v_p)
    : window(w), v(v_p) {
    eprintf("Panel_Vector init at %p \n", (void *) this);
    setup_panel_win(window->set_panel(this, r));
    init_cursor();
    // request_redraw();
}

template<class V>
Panel_Vector<V>::~Panel_Vector() {
    delwin(panel_win);
}

template<class V>
void Panel_Vector<V>::set_field_width(std::initializer_list<int> field_width_p) {
    if (field_width_p.size() != V::fnum()) {
        eprintf("Unmatched field with for Panel_Vector\n");
        throw std::runtime_error("ERROR!!!\n");
    }
    field_width = std::vector<int> (field_width_p.size() + 1);
    field_width[0] = 0;
    int i = 0;
    for (auto v : field_width_p) {
        field_width[i+1] = field_width[i] + v;
        ++i;
    }
}

template<class V>
void Panel_Vector<V>::setup_panel_win(const Rectangle rect_p) {
    this->height = rect_p.bottom - rect_p.top;
    this->width = rect_p.right - rect_p.left;
    this->panel_win = newwin(this->height, this->width, rect_p.top, rect_p.left);
    wborder(panel_win, 0, 0, 0, 0 ,0,0,0, 0);
    // box(this->panel_win, 0 , 0);
    wrefresh(this->panel_win);
    eprintf("panel height = %d, width = %d \n", height, width);
}

template<class V>
void Panel_Vector<V>::init_cursor() {
    this->cursor = {0,0};
    this->visual_start_row = 0;
}

/**********
 *  the # rows to display row r's information completely
 ************/
template<class V>
int Panel_Vector<V>::row_size(int r) {
    int used_lines = 1;
    // for every field
    for (int i = 0; i < V::fnum(); ++i) {
        int field_diff = field_width[i+1] - field_width[i];
        int strlen = v->get_field(r, i).length();
        // used_lines to be updated if more lines are used 
        used_lines = std::max(used_lines, (strlen+field_diff-1) / field_diff);
    }
    return used_lines;
}


/*
    Actual Redraw the panel
*/
template<class V>
void Panel_Vector<V>::redraw() {
    static int count = 0;

    eprintf("\tdraw : # %d \n", count++);
    
    // line is the line to be addch
    int line = 0;
    // row is the row of v to be output
    int row = visual_start_row;
    // fn is the number of fields the data have
    int fn = field_width.size() - 1;
    
    eprintf("total size = %d \n", v->size());

    wclear(panel_win);

    while (line < height && row < v->size()) {
        eprintf("print row %d, line %d \n", row, line);
        if (row == cursor.r) {
            // the row need to be output full
            // the number of lines used by this row
            int used_lines = 1;
            // for every field
            for (int i = 0; i < fn; ++i) {
                // field_diff is the width of this field
                int field_diff = field_width[i+1] - field_width[i] - 1;
                // str is the str format of the data in this field
                std::string str = v->get_field(row, i);
                int strlen = str.length();
                // used_lines to be updated if more lines are used 
                used_lines = std::max(used_lines, (strlen+field_diff-1) / field_diff);
                // j is the offset of lines to be addch, k is the offset of columns of addch
                int j = 0, k = 0;

                // watch for the field the cursor is currently on, highlight that field
                if (i == cursor.c) {
                    wattron(panel_win, A_STANDOUT);
                }
                wmove(panel_win, line, field_width[i]);
                for (char c : str) {
                    if (k == field_diff) {
                        k = 0; j += 1;
                        wmove(panel_win, line + j, field_width[i]);
                    }
                    waddch(panel_win, c);
                    k += 1;
                }
                wattroff(panel_win, A_STANDOUT);
            }
            // update line
            eprintf("usedlines = %d\n", used_lines);
            line += used_lines;
        } else { 
            // just use one row
            for (int i = 0; i < fn; ++i) {
                std::string str = v->get_field(row, i);
                int strlen = std::min<int>( str.length(), field_width[i+1] - field_width[i] - 1 );
                wmove(panel_win, line, field_width[i]);
                for (int j = 0; j < strlen; ++j) {
                    waddch(panel_win, str[j]);
                }
            }
            line += 1;
        }
        
        row += 1;
    }

    wrefresh(panel_win);

}


/*
    Request redraw, intend to buffer redraw requests to stablize the cost
*/
template<class V>
void Panel_Vector<V>::request_redraw() {
    // dummy impl
    redraw();
}

/*
    Interact with Edit mode in Window_Grid
*/
template<class V>
std::string Panel_Vector<V>::get_cur_field() {
    return v->get_field(cursor.c, cursor.r);
}

template<class V>
void Panel_Vector<V>::update_field(const std::string & s) {
    v->set_field(cursor.c, cursor.r, s);
    request_redraw();
}

template<class V>
void Panel_Vector<V>::insert_row(const std::string & s) {
    v->insert_row(s);
    cursor.r = v->size() - 1;
    cursor.c = 0;
    request_redraw();
}

template<class V>
void Panel_Vector<V>::remove_row() {
    v->remove_row(cursor.r);
    if (cursor.r == v->size())
        cursor.r --;
    request_redraw();
}


/********************
 *  Move Cursor
 *******************/
template<class V>
void Panel_Vector<V>::cursor_left() {
    eprintf("Here\n");
    if (cursor.c <= 0)
        return;
    cursor.c --;

    request_redraw();
}

template<class V>
void Panel_Vector<V>::cursor_right() {
    if (cursor.c >= V::fnum() - 1)
        return;
    cursor.c ++;

    request_redraw();
}

template<class V>
void Panel_Vector<V>::cursor_up() {
    if (cursor.r <= 0)
        return;
    cursor.r --;

    visual_start_row = std::min(visual_start_row, cursor.r);

    request_redraw();
}

template<class V>
void Panel_Vector<V>::cursor_down() {
    if (cursor.r >= v->size() - 1)
        return;
    cursor.r ++;

    visual_start_row = std::max(visual_start_row, cursor.r - std::max(height - row_size(cursor.r), 0));

    request_redraw();
}

}
