#include "photo.hpp"
#include <iostream>

using namespace std;

int PhotoEntry::id = 0;

PhotoEntry::PhotoEntry() : 
    title(""),
    path(""),
    date(""),
    location(""),
    notes("")    
{
    this->id++;
}

PhotoEntry::PhotoEntry(string title, string path, string date = "", string location = "", std::vector <std::string>* tags = nullptr, string notes = "") :
    title(title),
    path(path),
    date(date),
    location(location),
    tags(tags),
    notes(notes)
{
    this->id++;
};


void PhotoEntry::setTitle(std::string title){
    this->title = title;
}


void PhotoEntry::setPath(std::string path){
    this->path = path;
}


void PhotoEntry::setDate(std::string date){
    this->date = date;
}


void PhotoEntry::setLocation(std::string location){
    this->location = location;
}


void PhotoEntry::setTags(std::vector<std::string>* tags){
    // if we already have something in tags, remove it 
    if(this->tags != nullptr){
        delete this->tags;
    }
    
    this->tags = tags;
}


void PhotoEntry::setNotes(std::string notes){
    this->notes = notes;
}


void PhotoEntry::shortPrint(){
    cout << "Title: " << this->title << endl;
    cout << "Path: " << this->path << endl;
};


void PhotoEntry::longPrint(){
    this->shortPrint();

    if(this->date != ""){
        cout << "Date: " << this->date << endl;
    }

    if(this->location != ""){
        cout << "Location: " << this->location << endl;
    }

    if(this->tags != nullptr){
        for(int i = 0; i < (*(this->tags)).size(); i++){
            cout << "#" << (*(this->tags))[i] << " ";
        }  
    }

    cout << endl;

    if(this->notes != ""){
        cout << "Notes: " << this->notes << endl;
    }

};

void PhotoEntry::remove(){
    if(this->tags != nullptr){
        delete this->tags;
        this->tags = nullptr;
    }
}

PhotoEntry::~PhotoEntry(){
    this->remove();
}