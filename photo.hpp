#ifndef PHOTO_HPP
#define PHOTO_HPP

#include <string>
#include <vector>

class PhotoEntry {

    private:
    
    static int id;
    std::string title;
    std::string path;
    std::string date;
    std::string location;
    std::vector <std::string>* tags;
    std::string notes;

    public:
    
    PhotoEntry();
    PhotoEntry(std::string title, std::string path, std::string date, std::string location, std::vector <std::string>* tags, std::string notes);

    void remove();
    ~PhotoEntry();

    void setTitle(std::string title);
    void setPath(std::string path);
    void setDate(std::string date);
    void setLocation(std::string location);
    void setTags(std::vector<std::string>* tags);
    void setNotes(std::string notes);

    void shortPrint();
    void longPrint();
};

#endif