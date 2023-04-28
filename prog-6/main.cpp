#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>

using namespace std;

class CRect {
public:
    CRect(double x,
          double y,
          double w,
          double h)
            : m_X(x),
              m_Y(y),
              m_W(w),
              m_H(h) {
    }

    friend ostream &operator<<(ostream &os,
                               const CRect &x) {
        return os << '(' << x.m_X << ',' << x.m_Y << ',' << x.m_W << ',' << x.m_H << ')';
    }

    double m_X;
    double m_Y;
    double m_W;
    double m_H;
};

#endif /* __PROGTEST__ */


class CWindow {
public:

    CWindow(int id, const string &title, const CRect &absPos) : c_id(id), c_title(title), c_absPos(absPos) {}

    CWindow(const CWindow& other) : c_id(other.c_id), c_title(other.c_title), c_absPos(other.c_absPos), c_relPos(other.c_relPos) {
        // Deep copy all children
        for (const auto& child : other.c_children) {
            shared_ptr<CWindow> copy = child.second->getCopy();
            c_children.emplace(child.first, std::move(copy));
        }
    }

    CWindow & add(const CWindow & prvok) {
        auto edited = prvok.getCopy();
        edited->recalulatePosition(c_absPos);
        c_children.emplace(edited->c_id, std::move(edited));
        return *this;
    }


    virtual shared_ptr<CWindow> getCopy() const {
        return shared_ptr<CWindow>( new CWindow(*this));
    }
    // search
    CWindow * search(const int id) {
        auto i = c_children.find(id);
        if(i == c_children.end()){
            return nullptr;
        }
        CWindow * ptr = i->second.get();
        return ptr;
    }
    // setPosition
    void setPosition (const CRect & newMainPosition) {
        this->c_absPos = newMainPosition;
        for(auto & i : c_children){
            i.second->c_absPos = CRect(0,0,0,0);
            i.second->recalulatePosition(this->c_absPos);
        }
    }
    virtual void recalulatePosition(const CRect & mainWindowPos) {
        cout << c_relPos;
        this->c_absPos.m_X = mainWindowPos.m_W*(this->c_relPos.m_X) + mainWindowPos.m_X;
        this->c_absPos.m_Y = mainWindowPos.m_H*(this->c_relPos.m_Y) + mainWindowPos.m_Y;
        this->c_absPos.m_W = mainWindowPos.m_W * this->c_relPos.m_W;
        this->c_absPos.m_H = mainWindowPos.m_H * this->c_relPos.m_H;
    }
    virtual void print(ostream& os, bool with = false, bool last = false) const {
        os << "[" << c_id << "] Window \"" << c_title << "\" " << c_absPos << "\n";
        for (auto it = c_children.begin(); it != c_children.end(); ++it) {
            it->second->print(os, true, it != std::prev(c_children.end()));
        }
    }

    friend ostream& operator<<(ostream& os, const CWindow& window) {
        window.print(os);
        return os;
    }


private:
    multimap<int, shared_ptr<CWindow>> c_children;


protected:
    int c_id;
    string c_title;
    CRect c_absPos = CRect(0,0,0,0);
    CRect c_relPos = CRect(0,0,0,0);

};

class CButton : public CWindow {
public:
    CButton(int id,
            const CRect &relPos,
            const string &name) :  CWindow(id, "Button", relPos)  {
        this->m_name = name;
        this->c_relPos = relPos;
    };


    virtual shared_ptr<CWindow> getCopy() const override {
        return shared_ptr<CWindow>( new CButton(*this));
    }

    virtual void print(ostream& os,const bool with,const bool last = false) const override {
        os << "+- [" << c_id << "] Button \"" << m_name << "\" " << this->c_absPos << "\n";
    }

private:
        string m_name;
};


class CInput : public CWindow{
public:
    CInput(int id,
           const CRect relPos,
           const string &value) : CWindow(id, "Input", relPos)
   {
       this->c_relPos = relPos;
       this->m_value = value;
   }

    virtual shared_ptr<CWindow> getCopy() const override {
        return shared_ptr<CWindow>( new CInput(*this));
    }

    virtual void print(ostream& os,const bool with, const bool last = false) const override {
        os << "+- " << "[" << c_id << "] Input \"" << m_value << "\" " << c_absPos << "\n";
    }

    string getValue(void) {
        return m_value;
    }

    void setValue(const string & to){
        m_value = to;
    }

private:
    string m_value;

};

class CLabel : public CWindow{
public:
    CLabel(int id,
           const CRect &relPos,
           const string &label): CWindow(id, "Label", relPos) {
        this->c_relPos = relPos;
        this->m_label = label;
    };

    virtual shared_ptr<CWindow> getCopy() const override {
        return shared_ptr<CWindow>( new CLabel(*this));
    }

    virtual void print(ostream& os, bool with, bool last = false) const override {
        os << "+- [" << c_id << "] Label \"" << m_label << "\" " << c_absPos << "\n";
    }

    friend ostream& operator<<(ostream& os, const CLabel& label) {
        os << "+- [" << label.c_id << "] Label \"" << label.m_label << "\" " << label.c_absPos << "\n";
        return os;
    }

private:
    string m_label;
};

class CComboBox : public CWindow {
public:
    CComboBox(int id,
              const CRect &relPos) : CWindow(id, "ComboBox", relPos) {
        this->c_relPos = relPos;
    };

    virtual shared_ptr<CWindow> getCopy() const override {
        return shared_ptr<CWindow>( new CComboBox(*this));
    }

    // add
    CComboBox add(const string & option) {
        m_box_options.push_back(option);
        return *this;
    }
//     setSelected
    void setSelected(const long unsigned int which) {
        m_box_selected = which;
    }

//    // getSelected
    int getSelected() {
        return m_box_selected;
    }

    virtual void print(ostream& os, const bool with = false, const bool last = false) const override {
        if(!with){
            os << "[" << c_id << "] ComboBox " << c_absPos << "\n";

        } else {
            os << "+- [" << c_id << "] ComboBox " << c_absPos << "\n";
        }

        if(!with){
            for (long unsigned int i = 0; i < m_box_options.size(); i++) {
                os << "+-" << (i == m_box_selected ? ">" : " ") << m_box_options[i] << (i == m_box_selected ? "<\n" : "\n");
            }
        } else {
            for (long unsigned int i = 0; i < m_box_options.size(); i++) {
                if(last){
                    os << "|  " << "+-" << (i == m_box_selected ? ">" : " ") << m_box_options[i] << (i == m_box_selected ? "<\n" : "\n");
                }else{
                os << "   " << "+-" << (i == m_box_selected ? ">" : " ") << m_box_options[i] << (i == m_box_selected ? "<\n" : "\n");
                }
            }
        }

    }

    friend ostream & operator<<(ostream & os, const CComboBox & x) {
        os << "[" << x.c_id << "] ComboBox " << x.c_absPos << "\n";
        for (long unsigned int i = 0; i < x.m_box_options.size(); i++) {
            os << "   " << "+-" << (i == x.m_box_selected ? ">" : " ") << x.m_box_options[i] << (i == x.m_box_selected ? "<\n" : "\n");
        }
        return os;
    }

private:
    vector<string> m_box_options;
    long unsigned int m_box_selected = 0;
};

// output operators

#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
    ostringstream oss;
    oss << x;
    return oss . str ();
}

int main ( void )
{
    assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
    a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
    a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
    a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
    a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );
//    cout << a;
    assert ( toString ( a ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
             "+- [20] ComboBox (70,154,480,48)\n"
             "   +->Karate<\n"
             "   +- Judo\n"
             "   +- Box\n"
             "   +- Progtest\n" );
    CWindow b = a;
    cout << b;
    auto c =  *b . search ( 20 );
    cout << c;

    assert ( toString ( *b . search ( 20 ) ) ==
             "[20] ComboBox (70,154,480,48)\n"
             "+->Karate<\n"
             "+- Judo\n"
             "+- Box\n"
             "+- Progtest\n" );
    assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
    dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
    assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
    dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
    b . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
    cout << b;
    assert ( toString ( b ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
             "+- [20] ComboBox (70,154,480,48)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (70,250,480,48)\n"
             "   +->PA2<\n"
             "   +- OSY\n"
             "   +- Both\n" );
    assert ( toString ( a ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
             "+- [20] ComboBox (70,154,480,48)\n"
             "   +->Karate<\n"
             "   +- Judo\n"
             "   +- Box\n"
             "   +- Progtest\n" );
    b . setPosition ( CRect ( 20, 30, 640, 520 ) );
    cout << b;
    assert ( toString ( b ) ==
             "[0] Window \"Sample window\" (20,30,640,520)\n"
             "+- [1] Button \"Ok\" (84,446,192,52)\n"
             "+- [2] Button \"Cancel\" (404,446,192,52)\n"
             "+- [10] Label \"Username:\" (84,82,128,52)\n"
             "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
             "+- [20] ComboBox (84,186,512,52)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (84,290,512,52)\n"
             "   +->PA2<\n"
             "   +- OSY\n"
             "   +- Both\n" );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
