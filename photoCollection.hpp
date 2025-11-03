#include <string>
#include <vector>
#include "photo.hpp"

class PhotoCollection {
    private:
    std::vector <PhotoEntry*> collection;
    
    public:

    ~PhotoCollection();

    void add_photo(PhotoEntry * photo);
    
    PhotoEntry* get_photo(int index);

    void remove_photo(int index);
    
    int get_num_photos();
};