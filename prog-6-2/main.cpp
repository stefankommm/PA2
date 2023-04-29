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
//INTERFACE_________________________________________________________________________
class CWidget {
public:
    CWidget(const unsigned int id, const CRect & relPos, const CRect & absPos);
    CWidget(const CWidget & copyFrom);
    virtual ~CWidget() {}
    virtual shared_ptr<CWidget> getCopy() const = 0;
    virtual CWidget * search(const unsigned int id) = 0;
    virtual void updatePosition(CRect & parentPosition) = 0;
    virtual void print (ostream & os) const = 0;
    friend ostream & operator<<(ostream & os, const CWidget & toPrint);

    int c_id;
protected:
    CRect c_absPos;
    CRect c_resPos;
};

class CWindow {
public:

    CWindow(int id, const string &title, const CRect &absPos);
    CWindow(const CWindow& other);
    CWindow & add(const CWindow & prvok);
    virtual shared_ptr<CWindow> getCopy() const;
    CWindow * search(const int id);
    void setPosition (const CRect & newMainPosition);
    virtual void recalculatePosition(const CRect & mainWindowPos);
    virtual void print(ostream& os, bool with = false, bool last = false) const;
    friend ostream& operator<<(ostream& os, const CWindow& window);

private:
    multimap<int, shared_ptr<CWidget>> c_children;

protected:
    int c_id;
    string c_title;
    CRect c_absPos = CRect(0,0,0,0);
};

class CPanel : public CWidget{
    public:
        CPanel(int id, const CRect &relPos);
        CPanel(CPanel & toCopy);

    private:
        //c_id
        //c_title
        //c_absPos
        int depth;
        CRect c_relPos;
};


class CButton : public CWidget {
public:
    CButton(int id,
            const CRect &relPos,
            const string &name);
    virtual shared_ptr<CWidget> getCopy() const;
    virtual void print(ostream& os) const override;
private:
    string m_name;
};

class CLabel : public CWidget {
public:
    CLabel(int id,
           const CRect &relPos,
           const string &label);
    virtual shared_ptr<CWidget> getCopy() const override;
    virtual void print(ostream& os) const override;
    friend ostream& operator<<(ostream& os, const CLabel& label);

private:
    string m_label;
};

class CComboBox : public CWidget {
public:
    CComboBox(int id, const CRect &relPos);
    virtual shared_ptr<CWidget> getCopy() const override = 0;
    CComboBox& add(const string & option);
    void setSelected(const long unsigned int which);
    int getSelected();
    virtual void print(ostream& os) const override = 0;
    friend ostream & operator<<(ostream & os, const CComboBox & x);

private:

    vector<string> m_box_options;
    long unsigned int m_box_selected = 0;
};
//INTERFACE_________________________________________________________________________




class CWindow::CWindow {
public:

    CWindow(int id, const string &title, const CRect &absPos) : c_id(id), c_title(title), c_absPos(absPos) {}
    CWindow(const CWindow& other) : c_id(other.c_id), c_title(other.c_title), c_absPos(other.c_absPos) {
        // Deep copy all children
        for (const auto& child : other.c_children) {
            shared_ptr<CWidget> copy = child.second->getCopy();
            this->c_children.emplace(child.first, std::move(copy));
        }
    }
    CWindow & add(const CWidget & prvok) {
        auto edited = prvok.getCopy();
        edited->updatePosition(this->c_absPos);
        this->c_children.emplace(edited->c_id, move(edited));
        return *this;
    }
    shared_ptr<CWindow> getCopy() const {
        return shared_ptr<CWindow>( new CWindow(*this));
    }
    CWidget * search(const int id) {
        auto i = this->c_children.find(id);
        if(i == this->c_children.end()){
            return nullptr;
        }
        CWidget * ptr = i->second.get();
        return ptr;
    }
    void setPosition (const CRect & newMainPosition) {
        this->c_absPos = newMainPosition;
        // Update the position for the children
        //        for(auto & i : this->c_children){
        //            i.second->c_absPos = CRect(0,0,0,0);
        //            i.second->recalculatePosition(this->c_absPos);
        //        }
    }

    virtual void print(ostream& os, bool with = false, bool last = false) const {
        os << "[" << this->c_id << "] Window \"" << this->c_title << "\" " << this->c_absPos << "\n";
        for (auto it = this->c_children.begin(); it != this->c_children.end(); ++it) {
            it->second->print(os);
        }
    }
    friend ostream& operator<<(ostream& os, const CWindow& window) {
        window.print(os);
        return os;
    }

private:
    multimap<int, shared_ptr<CWidget>> c_children;

protected:
    int c_id;
    string c_title;
    CRect c_absPos = CRect(0,0,0,0);
    CRect c_relPos = CRect(0,0,0,0);
};

class CWidget::CWidget {
    CWidget(const unsigned int id, const CRect & relPos, const CRect & absPos) : w_id(id), w_relPos(relPos), w_absPos(absPos), w_depth() {};
    CWidget(const CWidget & copyFrom){};
    virtual shared_ptr<CWidget> getClone() const{};
    CWidget * search(const unsigned int id){};
    virtual void updatePosition(CRect & parentPosition){};
    virtual void print (ostream & os) const {
        os << "This is a CWidget << \n";
    };
    friend ostream & operator<<(ostream & os, const CWidget & toPrint) {
        toPrint.print(os);
        return os;
    };


protected:
    unsigned int w_id;
    int w_depth;
    CRect w_absPos = CRect(0,0,0,0);
    CRect w_relPos = CRect(0,0,0,0);

};

class CPanel::CPanel : public CWindow{
public:



};


class CButton::CButton : public CWidget {
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

class CInput : public CWidget{
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



class CLabel::CLabel : public CWidget {
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


class CComboBox::CComboBox : public CWidget {
public:
    CComboBox(int id,
              const CRect &relPos) : CWindow(id, "ComboBox", relPos) {
        this->c_relPos = relPos;
    };
    virtual shared_ptr<CWindow> getCopy() const override {
        return shared_ptr<CWindow>( new CComboBox(*this));
    }
    CComboBox add(const string & option) {
        m_box_options.push_back(option);
        return *this;
    }
    void setSelected(const long unsigned int which) {
        m_box_selected = which;
    }

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


#ifndef __PROGTEST__

template<typename T_>
string toString(const T_ &x) {
    ostringstream oss;
    oss << x;
    return oss.str();
}

int main(void) {
    assert (sizeof(CButton) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert (sizeof(CInput) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert (sizeof(CLabel) - sizeof(string) < sizeof(CComboBox) - sizeof(vector<string>));
    assert (sizeof(CButton) - sizeof(string) <= sizeof(CPanel) - sizeof(vector<void *>));
    assert (sizeof(CInput) - sizeof(string) <= sizeof(CPanel) - sizeof(vector<void *>));
    assert (sizeof(CLabel) - sizeof(string) <= sizeof(CPanel) - sizeof(vector<void *>));
    CWindow a(0, "Sample window", CRect(10, 10, 600, 480));
    a.add(CButton(1, CRect(0.1, 0.8, 0.3, 0.1), "Ok")).add(CButton(2, CRect(0.6, 0.8, 0.3, 0.1), "Cancel"));
    a.add(CLabel(10, CRect(0.1, 0.1, 0.2, 0.1), "Username:"));
    a.add(CInput(11, CRect(0.4, 0.1, 0.5, 0.1), "chucknorris"));
    a.add(CPanel(12, CRect(0.1, 0.3, 0.8, 0.7)).add(CComboBox(20, CRect(0.1, 0.3, 0.8, 0.1)).add("Karate").add("Judo").add("Box").add("Progtest")));
    assert (toString(a) ==
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
            "      +- Progtest\n");
    CWindow b = a;
    assert (toString(*b.search(20)) ==
            "[20] ComboBox (118,254.8,384,33.6)\n"
            "+->Karate<\n"
            "+- Judo\n"
            "+- Box\n"
            "+- Progtest\n");
    assert (dynamic_cast<CComboBox &> ( *b.search(20)).getSelected() == 0);
    dynamic_cast<CComboBox &> ( *b.search(20)).setSelected(3);
    assert (dynamic_cast<CInput &> ( *b.search(11)).getValue() == "chucknorris");
    dynamic_cast<CInput &> ( *b.search(11)).setValue("chucknorris@fit.cvut.cz");
    CPanel &p = dynamic_cast<CPanel &> ( *b.search(12));
    p.add(CComboBox(21, CRect(0.1, 0.5, 0.8, 0.1)).add("PA2").add("OSY").add("Both"));
    assert (toString(b) ==
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
            "      +- Both\n");
    assert (toString(a) ==
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
            "      +- Progtest\n");
    assert (toString(p) ==
            "[12] Panel (70,154,480,336)\n"
            "+- [20] ComboBox (118,254.8,384,33.6)\n"
            "|  +- Karate\n"
            "|  +- Judo\n"
            "|  +- Box\n"
            "|  +->Progtest<\n"
            "+- [21] ComboBox (118,322,384,33.6)\n"
            "   +->PA2<\n"
            "   +- OSY\n"
            "   +- Both\n");
    b.setPosition(CRect(20, 30, 640, 520));
    assert (toString(b) ==
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
            "      +- Both\n");
    p.add(p);
    assert (toString(p) ==
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
            "      +- Both\n");
    p.add(p);
    assert (toString(p) ==
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
            "         +- Both\n");
    p.add(p);
    assert (toString(p) ==
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
            "            +- Both\n");
    assert (toString(b) ==
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
            "               +- Both\n");
    assert (toString(a) ==
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
            "      +- Progtest\n");
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
