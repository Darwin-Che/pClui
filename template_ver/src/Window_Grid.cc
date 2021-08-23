#include <curses.h>
#include "Clui.h"

namespace Clui {

Window_Grid::Window_Grid(int gh, int gw) {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    refresh();
    getmaxyx(stdscr, height, width);

    gridh = gh;
    gridw = gw;

    gsizeh = height / gridh;
    gsizew = width / gridw;

    draw_grid();
    eprintf("window height = %d, width = %d \n grid height = %d, width = %d\n", height, width, gridh, gridw);

}

Window_Grid::~Window_Grid() {
    endwin();
}

void Window_Grid::draw_grid() {
    for (int i = 1; i < gridh; ++i) {
        mvhline(gsizeh * i, 0, '-', width);
    }
    for (int i = 1; i < gridw; ++i) {
        mvvline(0, gsizew * i, '|', height);
    }
    for (int i = 1; i < gridh; ++i) {
        for (int j = 1; j < gridw; ++j) {
            mvaddch(gsizeh * i, gsizew * j, '+');
        }
    }
}

void Window_Grid::update() {
    eprintf("pos_map size = %lu \n", pos_map_r.size());
    draw_grid();
    for (std::pair<std::pair<int,int>,Panel_Base *> t : pos_map_r) {
        t.second->request_redraw();
    }
}

Rectangle Window_Grid::set_panel(Panel_Base * panel, const Rectangle & rect) {
    int r = rect.top;
    int c = rect.left;
    if (r < 0 || r >= gridh || c < 0 || c >= gridw) {
        eprintf("grid pos out of bounds\n");
    }
    pos_map_r[std::make_pair(r, c)] = panel;
    pos_map_c[std::make_pair(c, r)] = panel;
    int factor_r = height / gridh;
    int factor_c = width / gridw;

    cur_panel = panel;

    return Rectangle{rect.top*factor_r+1, 
                    rect.left*factor_c+1, 
                    rect.bottom*factor_r,
                    rect.right*factor_c};
}

void Window_Grid::cursor_left() {

}

void Window_Grid::cursor_right() {
    
}

void Window_Grid::cursor_up() {
    
}

void Window_Grid::cursor_down() {
    
}

void Window_Grid::start_service() {
    int ch;
    this->update();
    while (1) {
        ch = getch();
        switch (ch)
        {
        case 'h':
            cur_panel->cursor_left();
            break;
        case 'j':
            cur_panel->cursor_down();
            break;
        case 'k':
            cur_panel->cursor_up();
            break;
        case 'l':
            cur_panel->cursor_right();
            break;
        case 'H':
            this->cursor_left();
            break;
        case 'J':
            this->cursor_down();
            break;
        case 'K':
            this->cursor_up();
            break;
        case 'L':
            this->cursor_right();
            break;
        case 'm':
            // cur_panel->update_field(this->edit_str(cur_panel->get_cur_field()));
            break;
        case 'i':
            // cur_panel->insert_row(this->edit_str(""));
            break;
        case 'd':
            cur_panel->remove_row();
            break;
        case 'q':
            return; // return from the function
        default:
            break;
        }
    }
}



}
