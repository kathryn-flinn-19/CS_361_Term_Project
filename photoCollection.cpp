#include "photoCollection.hpp"
#include <iostream>


using namespace std;

PhotoCollection::~PhotoCollection(){
    for(int i = (this->collection).size() - 1; i >= 0; i--){
        // free the tags
        (*(this->collection)[i]).remove();

        // free dynamically allocated photo
        delete (this->collection)[i];

        // remove item from vector
        (this->collection).pop_back();
    }
}

void PhotoCollection::add_photo(PhotoEntry * photo){
    collection.push_back(photo);
}

PhotoEntry* PhotoCollection::get_photo(int index){
    if(index < 0 || index >= (this->collection).size()){
        return nullptr;
    }

    return (this->collection)[index];
}

void PhotoCollection::remove_photo(int index){

}

int PhotoCollection::get_num_photos(){
    return (this->collection).size();
}