#include <string>
#include "Clui.h"
#include <sstream>

class T1Row {
public:
    int i0;
    int i1;
    /* 
        static int Row::fnum()
        Row(const std::string & s)
        std::string get_field(int j)
        void set_field(int j, const std::string & s) 
    */
    static int fnum() {
        return 2;
    }

    T1Row();
    T1Row(const std::string & s);
    std::string get_field(int j);
    void set_field(int j, const std::string & s);
};

T1Row::T1Row() 
: i0(0), i1(0){
}

T1Row::T1Row(const std::string & s) {
    std::stringstream ss (s);
    ss >> i0 >> i1;
}

std::string T1Row::get_field(int j) {
    if (j == 0) {
        return std::to_string(i0);
    } 
    if (j == 1) {
        return std::to_string(i1);
    }
    printf("out of bounds\n");
    exit(1);
}

void T1Row::set_field(int j, const std::string & s) {
    int n = s.length();
    if (j == 0) {
        if (n < 1)
            goto err_short;
        i0 = s[0] - '0';
        return;
    } 
    if (j == 1) {
        if (n < 1)
            goto err_short;
        i1 = s[0] - '1';
        return;
    }
    printf("out of bounds\n");
    exit(1);
    return;
err_short:
    printf("string too short\n");
    exit(1);
    return;
}


void test1() {
    using namespace std;
    using namespace Clui;

    auto log = Clui::Log::init_log();

    using T1Vector = BVector<T1Row>;
    using T1Panel = Panel_Vector<T1Vector>;

    auto win_grid = make_shared<Window_Grid> (3, 3);
    auto bvector = make_shared<T1Vector> ();
    auto pv = make_shared<T1Panel> (win_grid, Rectangle{1,1,3,2}, bvector); // field_width
    pv->set_field_width({4,4});

    // init data
    bvector->insert_row("11111 11111");
    bvector->insert_row("22222 22222");

    win_grid->start_service();

    return;

}

int main () {
    test1();
    return 0;
}