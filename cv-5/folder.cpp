#include <iostream>

#include "folder.hpp"

const int FOLDER_SIZE = 10;


Folder::Folder(const Folder& other): name(other.name)
{
    documents_count = other.documents_count;
    data->documents = new Document[FOLDER_SIZE];
    for (int i = 0; i < documents_count; i++)
        data->documents[i] = other.data->documents[i];

}


Folder::Folder (const string & _name) : name(_name) {
    data->documents = new Document[FOLDER_SIZE];
    data = new DocumentsData{
        new Document[FOLDER_SIZE],
        0,
        1
    };

}

ostream & operator << (ostream & os, const Folder & f);

Folder::~Folder() { delete [] data->documents; }

ostream & operator << (ostream & os, const Folder & f) {
    os << "Folder " << f.name <<" Contains " << f.documents_count << " documents:" << endl;
    for (int i = 0; i < f.documents_count; ++i)
        os << "\t" << f.data->documents[i] << endl;
    return os;
}

Folder & Folder::operator += (const Document & document) {
    if (documents_count == FOLDER_SIZE)
        throw("Folder is full");
    data->documents[documents_count] = document;
    documents_count++;
    return *this;
}

Folder &Folder::operator=(const Folder &other)  {
    if(this != &other) {
        delete[] data->documents;
        name = other.name;
        documents_count = other.documents_count;
        data->documents = new Document[FOLDER_SIZE];
        for (int i = 0; i < documents_count; i++) {
            data->documents[i] = other.data->documents[i];
        }
    }
    return *this;
}


