#ifndef UNTITLED_DOCUMENT_HPP
#define UNTITLED_DOCUMENT_HPP

#include <string>

using namespace std;

class Document {
public:
    Document(const string & _title = "", int _pages = 0) : title(_title), pages(_pages) {}
    friend ostream & operator << (ostream & os, const Document & document);
private:
    string title;
    int pages;
};

#endif //UNTITLED_DOCUMENT_HPP

