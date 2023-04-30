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


class CPanel;
//INTERFACE_________________________________________________________________________
class CWidget {
public:
    CWidget(int id, const CRect &relPos) : c_id(id), c_relPos(relPos), c_absPos(CRect(0, 0, 0, 0)) {};

    CWidget(const CWidget &copyFrom) : c_id(copyFrom.c_id),c_relPos(copyFrom.c_relPos), c_absPos(copyFrom.c_absPos)  {};

    virtual ~CWidget() = default;

    virtual shared_ptr<CWidget> getCopy() const {
        return shared_ptr<CWidget>(new CWidget(*this));
    };

    virtual void updatePosition(CRect &parentPosition) {
        this->c_absPos.m_X = parentPosition.m_W * (this->c_relPos.m_X) + parentPosition.m_X;
        this->c_absPos.m_Y = parentPosition.m_H * (this->c_relPos.m_Y) + parentPosition.m_Y;
        this->c_absPos.m_W = parentPosition.m_W * this->c_relPos.m_W;
        this->c_absPos.m_H = parentPosition.m_H * this->c_relPos.m_H;

        updateChildrenPositions();
    };

    virtual void print(ostream &os, string prefix) const{
        cout << "Widget";
    }

    virtual void printOnly(ostream &os) const{
        cout << "Widget";
    }

    friend std::ostream &operator<<(std::ostream &os, const CWidget &widget) {
        widget.printOnly(os);
        return os;
    }

    virtual void updateChildrenPositions() {};

    int c_id;
protected:
    CRect c_relPos;
    CRect c_absPos;

};


class CButton : public CWidget {
public:
    CButton(int id,
            const CRect &relPos,
            const string &name) : CWidget(id, relPos) {
        this->m_name = name;
    };

    virtual shared_ptr<CWidget> getCopy() const {
        return shared_ptr<CWidget>(new CButton(*this));
    };

    virtual void print(ostream &os, string prefix) const override{

        os << prefix.substr(0, prefix.size() - 3);
        os << "+- [" << c_id << "] Button \"" << m_name << "\" " << this->c_absPos <<  "\n";

    }

    friend ostream &operator<<(ostream &os, const CButton &x) {
        os << "[" << x.c_id << "] Button " << x.c_absPos << "\n";

        return os;
    }

private:
    string m_name;
};


class CLabel : public CWidget {
public:
    CLabel(int id,
           const CRect &relPos,
           const string &label) : CWidget(id, relPos), m_label(label) {};

    virtual shared_ptr<CWidget> getCopy() const override {
        return shared_ptr<CWidget>(new CLabel(*this));
    };

    virtual void print(ostream &os, string prefix) const override{
        os << prefix.substr(0, prefix.size() - 3);
        os << "+- [" << c_id << "] Label \"" << m_label << "\" " << c_absPos << "\n";
    }

    friend ostream &operator<<(ostream &os, const CLabel &x) {
        os << "[" << x.c_id << "] Label " << x.c_absPos << "\n";

        return os;
    }


private:
    string m_label;
};


class CInput : public CWidget {
public:
    CInput(int id, const CRect relPos, const string &value) : CWidget(id, relPos), m_value(value) {};

    virtual shared_ptr<CWidget> getCopy() const override {
        return shared_ptr<CWidget>(new CInput(*this));
    };

    virtual void print(ostream &os, string prefix) const override{
        os << prefix.substr(0, prefix.size() - 3);
        os << "+- " << "[" << c_id << "] Input \"" << m_value << "\" " << c_absPos << "\n";
    }

    string getValue(void) {
        return m_value;
    };

    void setValue(const string &to) {
        m_value = to;
    };

    friend ostream &operator<<(ostream &os, const CInput &x) {
        os << "[" << x.c_id << "] Input " << x.c_absPos << "\n";

        return os;
    }

private:
    string m_value;

};


class CComboBox : public CWidget {
public:
    CComboBox(int id, const CRect &relPos) : CWidget(id, relPos), m_box_selected(0) {};

    virtual shared_ptr<CWidget> getCopy() const override {
        return shared_ptr<CWidget>(new CComboBox(*this));
    };

    CComboBox &add(const string &option) {
        m_box_options.push_back(option);
        return *this;
    }

    void setSelected(const long unsigned int which) {
        m_box_selected = which;
    };

    int getSelected() {
        return m_box_selected;
    };

    void print(ostream &os, string prefix) const override {

        os << prefix.substr(0, prefix.size() - 3);
        os << "+- [" << c_id << "] ComboBox " << c_absPos <<  "\n";

        for (long unsigned int i = 0; i < m_box_options.size(); i++) {
            os << prefix;
            os << "+-" << (i == m_box_selected ? ">" : " ") << m_box_options[i] << (i == m_box_selected ? "<\n" : "\n");
        }

    }

    virtual void printOnly(ostream &os) const override{
        os << "[" << c_id << "] ComboBox " << c_absPos <<  "\n";

        for (long unsigned int i = 0; i < m_box_options.size(); i++) {
            os << "+-" << (i == m_box_selected ? ">" : " ") << m_box_options[i] << (i == m_box_selected ? "<\n" : "\n");
        }
    }


private:

    vector<string> m_box_options;
    long unsigned int m_box_selected = 0;
};





class CPanel : public CWidget {
public:
    CPanel(int id, const CRect &relPos) : CWidget(id, relPos), c_depth(1) {};

    CPanel(const CPanel &toCopy) : CWidget(toCopy.c_id, toCopy.c_relPos), c_depth(toCopy.c_depth) {
        this->c_absPos = toCopy.c_absPos;
        for (const auto &child: toCopy.c_children) {
            shared_ptr<CWidget> copy = child->getCopy();
            c_children.push_back(std::move(copy));
        }
    };

    virtual void printOnly(ostream &os) const override{
        cout << "Widget";
    }

    void setDepth(const int newDepth) {
        c_depth = newDepth;
    }


    virtual shared_ptr<CWidget> getCopy() const override {
        return shared_ptr<CWidget>(new CPanel(*this));
    };

    CWidget &add(const CWidget &toAdd) {
        auto copy = toAdd.getCopy();
        copy->updatePosition(this->c_absPos);
        c_children.push_back(copy);
        return *this;
    }

    void updateChildrenPositions() override {
        for (auto &child: c_children) {
            child->updatePosition(this->c_absPos);
        }
    }

    void print(ostream &os, string prefix) const override
    {

        // WINDOW PRINT
        {
            os << prefix.substr(0, prefix.size() - 3);
            os << "+- "
               << "[" << c_id << "] Panel " << c_absPos << "\n";

            auto it = c_children.begin();
            auto it_last = std::prev(c_children.end());

            while (it != c_children.end())
            {
                auto &elem = *it;
                bool is_last = (it == it_last);

                if (!is_last)
                {
                    elem->print(os, prefix + "|  ");
                }
                else
                {
                    elem->print(os, prefix + "   ");
                }

                ++it;
            }
        }
    }


    vector<shared_ptr<CWidget>> c_children;

    CWidget *search(const int id) {
        shared_ptr<CWidget> found = c_children.back();
        for(auto child : c_children){
            if(child->c_id == id){
                return child.get();
            }
        }

        for (const auto &child: c_children) {
            CPanel *panel = dynamic_cast<CPanel *>(child.get());
            if (panel) {
                CWidget *result = panel->search(id);
                if (result) {
                    return result;
                }
            }
        }
        return nullptr;
    }

    friend ostream &operator<<(ostream &os, const CPanel &x)  {
        os << "" << "[" << x.c_id << "] Panel " << x.c_absPos <<"\n";

        auto it = x.c_children.begin();
        auto it_last = std::prev(x.c_children.end());

        while (it != x.c_children.end()) {
            auto &elem = *it;
            bool is_last = (it == it_last);
            auto is_combo = dynamic_cast<CComboBox*>(it->get());

            if (is_combo != nullptr) {
                if(!is_last)
                    elem->print(os, "|  ");
                else
                    elem->print(os, "   ");
            } else if (dynamic_cast<CPanel*>(elem.get()) != nullptr){
                if(!is_last){
                    elem->print(os, "|  ");
                } else {
                    elem->print(os, "   ");
                }
            }
            else {
                elem->print(os, "   ");
            }

            ++it;
        }
        return os;
    }

private:
    //c_id
    //c_title
    //c_absPos
    int c_depth;

};



class CWindow {
public:

    CWindow(int id, const string &title, const CRect &absPos) : c_depth(0), c_id(id), c_title(title), c_absPos(absPos){};

    CWindow(const CWindow &other) : c_depth(other.c_depth), c_id(other.c_id), c_title(other.c_title), c_absPos(other.c_absPos)
                                     {
        for (const auto &child: other.c_children) {
            shared_ptr<CWidget> copy = child.second->getCopy();
            c_children.emplace(child.first, move(copy));
        }
    };

    CWindow &add(const CWidget &toAdd) {
        auto copy = toAdd.getCopy();
        copy->updatePosition(this->c_absPos);
        c_children.emplace(copy->c_id, move(copy));
        return *this;
    };

    CWidget *search(const int id) {
        auto found = c_children.find(id);
        if (found != c_children.end()) {
            return found->second.get();
        }

        for (const auto &child: c_children) {
            CPanel *panel = dynamic_cast<CPanel *>(child.second.get());
            if (panel) {
                CWidget *result = panel->search(id);
                if (result) {
                    return result;
                }
            } else {
                CWidget *widget = child.second.get();
                if (widget->c_id == id) {
                    return widget;
                }
            }
        }
        return nullptr;
    }

    void setPosition(const CRect &newMainPosition) {
        c_absPos = newMainPosition;
        for (auto i: c_children) {
            i.second->updatePosition(c_absPos);
        }
    };


    virtual void print(ostream &os, string prefix) const
    {

        os << "[" << c_id << "] Window \"" << c_title << "\" " << c_absPos << "\n";

        auto it_last = prev(c_children.end());
        for (auto it = c_children.begin(); it != c_children.end(); ++it)
        {

            auto is_combo = dynamic_cast<CComboBox *>(it->second.get());

            if (is_combo != nullptr)
            {
                if (it != it_last)
                    it->second->print(os, "|  ");
                else
                    it->second->print(os, "   ");
            }
            else if (dynamic_cast<CPanel *>(it->second.get()) != nullptr)
            {
                if(it != it_last){
                    it->second->print(os, "|  ");
                }else {
                    it->second->print(os, "   ");
                }

            }
            else
            {
                it->second->print(os, "");
            }
        }
    };


    friend ostream &operator<<(ostream &os, CWindow window) {
        window.print(os, "");
        return os;
    };

private:
    multimap<int, shared_ptr<CWidget>> c_children;

protected:
    int c_depth;
    int c_id;
    string c_title;
    CRect c_absPos;
};

//INTERFACE_________________________________________________________________________




#ifndef __PROGTEST__
template <typename T_>
string toString ( const T_ & x )
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
    assert ( sizeof ( CButton ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
    assert ( sizeof ( CInput ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );
    assert ( sizeof ( CLabel ) - sizeof ( string ) <= sizeof ( CPanel ) - sizeof ( vector<void*> ) );

    CWindow abc ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
    abc . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
    abc . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
    abc . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
    abc . add ( CPanel ( 12, CRect ( 0.1, 0.3, 0.8, 0.7 ) ));

    CPanel & i = dynamic_cast<CPanel &> ( *abc. search ( 12 ) );
    i.add(CPanel ( 13, CRect ( 0.1, 0.3, 0.8, 0.7 ) ));
    i.add(CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ));

    CPanel & f = dynamic_cast<CPanel &> ( *abc. search ( 13 ) );
    f.add(CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ));
    f.add(CPanel ( 14, CRect ( 0.1, 0.3, 0.8, 0.7 ) ));
    f.add(CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ));

    CPanel & g = dynamic_cast<CPanel &> ( *abc. search ( 14 ) );
    g.add(CPanel ( 15, CRect ( 0.1, 0.3, 0.8, 0.7 ) ));
    g.add(CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ));

//
//    CPanel & h = dynamic_cast<CPanel &> ( *abc. search ( 15 ) );
//    h.add(CPanel ( 16, CRect ( 0.1, 0.3, 0.8, 0.7 ) ));


    abc . add ( CPanel ( 13, CRect ( 0.1, 0.3, 0.8, 0.7 ) ));

    cout << abc;




    CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
    a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
    a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
    a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
    a . add ( CPanel ( 12, CRect ( 0.1, 0.3, 0.8, 0.7 ) ) . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) ) );
    assert ( toString ( a ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
             "+- [12] Panel (70,154,480,336)\n"
             "   +- [20] ComboBox (118,254.8,384,33.6)\n"
             "      +->Karate<\n"
             "      +- Judo\n"
             "      +- Box\n"
             "      +- Progtest\n" );
    CWindow b = a;
    assert ( toString ( *b . search ( 20 ) ) ==
             "[20] ComboBox (118,254.8,384,33.6)\n"
             "+->Karate<\n"
             "+- Judo\n"
             "+- Box\n"
             "+- Progtest\n" );
    assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
    dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
    assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
    dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
    CPanel & p = dynamic_cast<CPanel &> ( *b . search ( 12 ) );
    p . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
    assert ( toString ( b ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
             "+- [12] Panel (70,154,480,336)\n"
             "   +- [20] ComboBox (118,254.8,384,33.6)\n"
             "   |  +- Karate\n"
             "   |  +- Judo\n"
             "   |  +- Box\n"
             "   |  +->Progtest<\n"
             "   +- [21] ComboBox (118,322,384,33.6)\n"
             "      +->PA2<\n"
             "      +- OSY\n"
             "      +- Both\n" );
    assert ( toString ( a ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
             "+- [12] Panel (70,154,480,336)\n"
             "   +- [20] ComboBox (118,254.8,384,33.6)\n"
             "      +->Karate<\n"
             "      +- Judo\n"
             "      +- Box\n"
             "      +- Progtest\n" );
    assert ( toString ( p ) ==
             "[12] Panel (70,154,480,336)\n"
             "+- [20] ComboBox (118,254.8,384,33.6)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (118,322,384,33.6)\n"
             "   +->PA2<\n"
             "   +- OSY\n"
             "   +- Both\n" );
    b . setPosition ( CRect ( 20, 30, 640, 520 ) );
    assert ( toString ( b ) ==
             "[0] Window \"Sample window\" (20,30,640,520)\n"
             "+- [1] Button \"Ok\" (84,446,192,52)\n"
             "+- [2] Button \"Cancel\" (404,446,192,52)\n"
             "+- [10] Label \"Username:\" (84,82,128,52)\n"
             "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
             "+- [12] Panel (84,186,512,364)\n"
             "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
             "   |  +- Karate\n"
             "   |  +- Judo\n"
             "   |  +- Box\n"
             "   |  +->Progtest<\n"
             "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
             "      +->PA2<\n"
             "      +- OSY\n"
             "      +- Both\n" );
    p . add ( p );
    assert ( toString ( p ) ==
             "[12] Panel (84,186,512,364)\n"
             "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
             "|  +->PA2<\n"
             "|  +- OSY\n"
             "|  +- Both\n"
             "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "   |  +- Karate\n"
             "   |  +- Judo\n"
             "   |  +- Box\n"
             "   |  +->Progtest<\n"
             "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "      +->PA2<\n"
             "      +- OSY\n"
             "      +- Both\n" );
    p . add ( p );
    assert ( toString ( p ) ==
             "[12] Panel (84,186,512,364)\n"
             "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
             "|  +->PA2<\n"
             "|  +- OSY\n"
             "|  +- Both\n"
             "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "|  |  +- Karate\n"
             "|  |  +- Judo\n"
             "|  |  +- Box\n"
             "|  |  +->Progtest<\n"
             "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "|     +->PA2<\n"
             "|     +- OSY\n"
             "|     +- Both\n"
             "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "   |  +- Karate\n"
             "   |  +- Judo\n"
             "   |  +- Box\n"
             "   |  +->Progtest<\n"
             "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "   |  +->PA2<\n"
             "   |  +- OSY\n"
             "   |  +- Both\n"
             "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
             "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
             "      |  +- Karate\n"
             "      |  +- Judo\n"
             "      |  +- Box\n"
             "      |  +->Progtest<\n"
             "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
             "         +->PA2<\n"
             "         +- OSY\n"
             "         +- Both\n" );
    p . add ( p );
    assert ( toString ( p ) ==
             "[12] Panel (84,186,512,364)\n"
             "+- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (135.2,368,409.6,36.4)\n"
             "|  +->PA2<\n"
             "|  +- OSY\n"
             "|  +- Both\n"
             "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "|  |  +- Karate\n"
             "|  |  +- Judo\n"
             "|  |  +- Box\n"
             "|  |  +->Progtest<\n"
             "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "|     +->PA2<\n"
             "|     +- OSY\n"
             "|     +- Both\n"
             "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "|  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "|  |  +- Karate\n"
             "|  |  +- Judo\n"
             "|  |  +- Box\n"
             "|  |  +->Progtest<\n"
             "|  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "|  |  +->PA2<\n"
             "|  |  +- OSY\n"
             "|  |  +- Both\n"
             "|  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
             "|     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
             "|     |  +- Karate\n"
             "|     |  +- Judo\n"
             "|     |  +- Box\n"
             "|     |  +->Progtest<\n"
             "|     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
             "|        +->PA2<\n"
             "|        +- OSY\n"
             "|        +- Both\n"
             "+- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "   +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "   |  +- Karate\n"
             "   |  +- Judo\n"
             "   |  +- Box\n"
             "   |  +->Progtest<\n"
             "   +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "   |  +->PA2<\n"
             "   |  +- OSY\n"
             "   |  +- Both\n"
             "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
             "   |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
             "   |  |  +- Karate\n"
             "   |  |  +- Judo\n"
             "   |  |  +- Box\n"
             "   |  |  +->Progtest<\n"
             "   |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
             "   |     +->PA2<\n"
             "   |     +- OSY\n"
             "   |     +- Both\n"
             "   +- [12] Panel (176.16,371.64,327.68,178.36)\n"
             "      +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
             "      |  +- Karate\n"
             "      |  +- Judo\n"
             "      |  +- Box\n"
             "      |  +->Progtest<\n"
             "      +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
             "      |  +->PA2<\n"
             "      |  +- OSY\n"
             "      |  +- Both\n"
             "      +- [12] Panel (208.928,425.148,262.144,124.852)\n"
             "         +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
             "         |  +- Karate\n"
             "         |  +- Judo\n"
             "         |  +- Box\n"
             "         |  +->Progtest<\n"
             "         +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
             "            +->PA2<\n"
             "            +- OSY\n"
             "            +- Both\n" );
    assert ( toString ( b ) ==
             "[0] Window \"Sample window\" (20,30,640,520)\n"
             "+- [1] Button \"Ok\" (84,446,192,52)\n"
             "+- [2] Button \"Cancel\" (404,446,192,52)\n"
             "+- [10] Label \"Username:\" (84,82,128,52)\n"
             "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
             "+- [12] Panel (84,186,512,364)\n"
             "   +- [20] ComboBox (135.2,295.2,409.6,36.4)\n"
             "   |  +- Karate\n"
             "   |  +- Judo\n"
             "   |  +- Box\n"
             "   |  +->Progtest<\n"
             "   +- [21] ComboBox (135.2,368,409.6,36.4)\n"
             "   |  +->PA2<\n"
             "   |  +- OSY\n"
             "   |  +- Both\n"
             "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "   |  |  +- Karate\n"
             "   |  |  +- Judo\n"
             "   |  |  +- Box\n"
             "   |  |  +->Progtest<\n"
             "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "   |     +->PA2<\n"
             "   |     +- OSY\n"
             "   |     +- Both\n"
             "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "   |  +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "   |  |  +- Karate\n"
             "   |  |  +- Judo\n"
             "   |  |  +- Box\n"
             "   |  |  +->Progtest<\n"
             "   |  +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "   |  |  +->PA2<\n"
             "   |  |  +- OSY\n"
             "   |  |  +- Both\n"
             "   |  +- [12] Panel (176.16,371.64,327.68,178.36)\n"
             "   |     +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
             "   |     |  +- Karate\n"
             "   |     |  +- Judo\n"
             "   |     |  +- Box\n"
             "   |     |  +->Progtest<\n"
             "   |     +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
             "   |        +->PA2<\n"
             "   |        +- OSY\n"
             "   |        +- Both\n"
             "   +- [12] Panel (135.2,295.2,409.6,254.8)\n"
             "      +- [20] ComboBox (176.16,371.64,327.68,25.48)\n"
             "      |  +- Karate\n"
             "      |  +- Judo\n"
             "      |  +- Box\n"
             "      |  +->Progtest<\n"
             "      +- [21] ComboBox (176.16,422.6,327.68,25.48)\n"
             "      |  +->PA2<\n"
             "      |  +- OSY\n"
             "      |  +- Both\n"
             "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
             "      |  +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
             "      |  |  +- Karate\n"
             "      |  |  +- Judo\n"
             "      |  |  +- Box\n"
             "      |  |  +->Progtest<\n"
             "      |  +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
             "      |     +->PA2<\n"
             "      |     +- OSY\n"
             "      |     +- Both\n"
             "      +- [12] Panel (176.16,371.64,327.68,178.36)\n"
             "         +- [20] ComboBox (208.928,425.148,262.144,17.836)\n"
             "         |  +- Karate\n"
             "         |  +- Judo\n"
             "         |  +- Box\n"
             "         |  +->Progtest<\n"
             "         +- [21] ComboBox (208.928,460.82,262.144,17.836)\n"
             "         |  +->PA2<\n"
             "         |  +- OSY\n"
             "         |  +- Both\n"
             "         +- [12] Panel (208.928,425.148,262.144,124.852)\n"
             "            +- [20] ComboBox (235.142,462.604,209.715,12.4852)\n"
             "            |  +- Karate\n"
             "            |  +- Judo\n"
             "            |  +- Box\n"
             "            |  +->Progtest<\n"
             "            +- [21] ComboBox (235.142,487.574,209.715,12.4852)\n"
             "               +->PA2<\n"
             "               +- OSY\n"
             "               +- Both\n" );
    assert ( toString ( a ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
             "+- [12] Panel (70,154,480,336)\n"
             "   +- [20] ComboBox (118,254.8,384,33.6)\n"
             "      +->Karate<\n"
             "      +- Judo\n"
             "      +- Box\n"
             "      +- Progtest\n" );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
