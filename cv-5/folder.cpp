#include <iostream>

#include "folder.hpp"

const int FOLDER_SIZE = 10;


Folder::Folder(const Folder& other): name(other.name)
{
    documents_count = other.documents_count;
    documents = new Document[FOLDER_SIZE];
    for (int i = 0; i < documents_count; i++)
        documents[i] = other.documents[i];

}


Folder::Folder (const string & _name) : name(_name) {
    documents = new Document[FOLDER_SIZE];


}

ostream & operator << (ostream & os, const Folder & f);

Folder::~Folder() { delete [] documents; }

ostream & operator << (ostream & os, const Folder & f) {
    os << "Folder " << f.name <<" Contains " << f.documents_count << " documents:" << endl;
    for (int i = 0; i < f.documents_count; ++i)
        os << "\t" << f.documents[i] << endl;
    return os;
}

Folder & Folder::operator += (const Document & document) {
    if (documents_count == FOLDER_SIZE)
        throw("Folder is full");
    documents[documents_count] = document;
    documents_count++;
    return *this;
}

Folder &Folder::operator=(const Folder &other)  {
    if(this != &other) {
        delete[] documents;
        name = other.name;
        documents_count = other.documents_count;
        documents = new Document[FOLDER_SIZE];
        for (int i = 0; i < documents_count; i++) {
            documents[i] = other.documents[i];
        }
    }
    return *this;
}


