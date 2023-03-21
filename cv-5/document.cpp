#include <iostream>

#include "document.hpp"

ostream & operator << (ostream & os, const Document & document) {
    return os << "Title: " << document.title << ", pages: " << document.pages;
}

