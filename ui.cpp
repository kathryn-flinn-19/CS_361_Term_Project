#include <iostream>
#include <sstream>
#include "ui.hpp"

using namespace std;

bool is_int(std::string input){
    int itr = 0;
    while(itr < input.length()){
        if(!isdigit(input[itr])){
            return false;
        }
        itr++;
    }

    return true;
}

bool is_valid_int(int x, PhotoCollection & collection){
    return x > 0 && x <= collection.get_num_photos();
}

char parse_option_input(char c){
    // if it's a lowercase, convert to capital
    if(c >= 97 && c <= 122){
        c = (char) (c - 32);
    }

    // if it's not a capital letter post-conversion:
    if(c > 90 || c < 65){
        cout << "Invalid character! Please try again." << endl;
        return '\0';
    }

    return c;
}

void wayfinder(char c, PhotoCollection &collection){
    if(c == 'A'){
        add_photo(collection);
    } else if(c ==  'H'){
        help();
    } else if(c == 'V' || c == 'S'){
        display_all(collection);
    } else {
        // c must actually be an int; display details page for photo
        view_details(collection, (int) c);
    }
}

void display_page_options(char current_page){
    cout << "What would you like to do? " << endl;
    cout << "> Options: " << endl;

    if(current_page == 'W'){
        cout << "> Enter S to start" << endl;
    } else {
        
        if(current_page != 'A'){
            cout << "> Enter A to add a photo" << endl;
        } 
        
        if(current_page != 'V'){
            cout << "> Enter V to view all photos" << endl;
        } else {
            cout << "> Enter a number to view that photo's details" << endl;
        }
        
        if(current_page != 'H'){
            cout << "> Enter H for help" << endl;
        } 

        if(current_page == 'D'){
            cout << "> Enter B to go back" << endl;
        }
    }

    cout << "> Enter X to exit" << endl;

    cout << "> Your choice: ";
}

void run_program(){
    PhotoCollection collection;
    
    welcome();

    std::string option;
    char go_to;

    do {
        getline(cin, option);
        go_to = parse_option_input(option[0]);
    } while(go_to == '\0');

    while(go_to != 'X'){
        wayfinder(go_to, collection);

        // grab their input again chungus
        do {
            getline(cin, option);

            if(!is_int(option)){
                go_to = parse_option_input(option[0]);
            } else {
                int photo_num = std::stoi(option);
                if(is_valid_int(photo_num, collection)){
                    go_to = (char) photo_num;
                } else {
                    go_to = '\0';
                    cout << "Error! Photo number not recognized; try again" << endl;
                }
            }
        } while(go_to == '\0');

    }

    cout << "Bye!" << endl;
}

void welcome(){
    cout << "-----------------------------------------------------------------" << endl;
    cout << "> Hello, and welcome to My Photo Library!" << endl;
    cout << "> Have all your favorite photos in one place and organize them to your heart's content!" << endl;
    cout << "> Additionally, store all the special details about your photo to make sure you remember ";
    cout << "everything that makes your favorite photos special." << endl << endl;

    display_page_options('W');
}

void display_all(PhotoCollection & collection){
    cout << "-----------------------------------------------------------------" << endl;

    if(collection.get_num_photos() == 0){
        cout << "You don't have any photos yet!" << endl << endl;
    } else {

        for(int i = 0; i < collection.get_num_photos(); i++){
            cout << (i + 1) << ". ";

            (*(collection.get_photo(i))).shortPrint();

            cout << endl;
        }
    }

    display_page_options('V');
}

void view_details(PhotoCollection & collection, int index){
    cout << endl << "View Photo Details" << endl;

    (*(collection.get_photo(index-1))).longPrint();

    cout << endl;

    display_page_options((char) index);
}

void help(){
    cout << "-----------------------------------------------------------------" << endl;
    cout << "Help Page" << endl << endl;

    cout << "To add a photo:" <<endl;
    cout << "> Enter A to access the Add Photo Page" << endl;
    cout << "> Enter the photo information" << endl;
    cout << "> When reviewing your photo information, you can ";
    cout << "change the information added by hitting E, and then ";
    cout << "hitting the letter corresponding to the field you'd like to edit" << endl;
    cout << "> You can also hit C during the review process to cancel adding your photo, ";
    cout << "but all data entered will be lost" << endl;
    cout << "Note: title and path are required fields, but all other fields can be skipped ";
    cout << "by hitting enter" << endl << endl;

    cout << "To view all photos: " << endl;
    cout << "> Enter V to access the View All Photos Page" << endl << endl;

    cout << "To view a specific photo: " << endl;
    cout << "> Start on the View All Photos Page" << endl;
    cout << "> Enter the number of the photo you'd like to view" << endl << endl;

    display_page_options('H');
}

std::vector<std::string>* convert_to_tags(string input){
    std::vector<std::string>* tags = new std::vector<std::string>;

    stringstream ss(input);

    string intermediate;

    while(getline(ss, intermediate, ';')){
        (*tags).push_back(intermediate);
    }

    return tags;
}

PhotoEntry* collect_photo_info(){
    bool valid;

    // collect title (check and reprompt for empty)
    
    std::string title;

    do {
        cout << "> Enter the photo title (required): ";

        getline(cin, title);

        valid = title != "";
        if(!valid){
            cout << "> Error: No photo title entered! This is a required field; please try again" << endl;
        } 

    } while(!valid);

    // collect path (check and reprompt for empty)
    std::string path;

    do {
        cout << "> Enter the photo path (required): ";

        getline(cin, path);

        valid = path != "";
        if(!valid){
            cout << "> Error: No photo path entered! This is a required field; please try again" << endl;
        } 

    } while(!valid);

    // collect date
    std::string date;
    cout << "> Enter date, or hit enter to skip: ";

    getline(cin, date);

    // collection location
    std::string location;
    cout << "> Enter location, or hit enter to skip: ";

    getline(cin, location);

    // collect notes
    std::string notes;
    cout << "> Enter notes, or hit enter to skip: ";

    getline(cin, notes);

    // collect and process tags
    std::string tags;
    cout << "> Enter tags, or hit enter to skip: ";

    getline(cin, tags);

    std::vector <std::string> * processed_tags = nullptr;

    if(tags != ""){
        processed_tags = convert_to_tags(tags);
    }

    PhotoEntry* photo = new PhotoEntry(title, path, date, location, processed_tags, notes);

    return photo;
}

void edit_photo_details(PhotoEntry* photo){
    cout << endl << "Options:" << endl;
    cout << "> Enter T to edit photo title" << endl;
    cout << "> Enter P to edit photo path" << endl;
    cout << "> Enter D to edit photo date" << endl;
    cout << "> Enter L to edit photo location" << endl;
    cout << "> Enter N to edit photo notes" << endl;
    cout << "> Enter G to edit tags" << endl;
    cout << "> Hit enter to confirm and add photo" << endl;

    bool valid;
    std::string input;

    cout << "Your choice: ";

    // get a user response
    do {
        getline(cin, input);

        valid = input == "" || \
        input[0] == 'T' || input[0] == 't' || \
        input[0] == 'P' || input[0] == 'p' || \
        input[0] == 'D' || input[0] == 'd' || \
        input[0] == 'L' || input[0] == 'l' || \
        input[0] == 'N' || input[0] == 'n' || \
        input[0] == 'G' || input[0] == 'g';

        if(!valid){
            cout << "Invalid input! Try again: ";
        }
    } while(!valid);

    if(input == ""){
        return;
    }


    if(input[0] == 'T' || input[0] == 't'){
        std::string title;

        do {
            cout << "> Enter the photo title (required): ";

            getline(cin, title);

            valid = title != "";
            if(!valid){
                cout << "> Error: No photo title entered! This is a required field; please try again" << endl;
            } 

        } while(!valid);

        (*photo).setTitle(title);

    } else if(input[0] == 'P' || input[0] == 'p'){

        // collect path (check and reprompt for empty)
        std::string path;

        do {
            cout << "> Enter the photo path (required): ";

            getline(cin, path);

            valid = path != "";
            if(!valid){
                cout << "> Error: No photo path entered! This is a required field; please try again" << endl;
            } 

        } while(!valid);

        (*photo).setPath(path);

    } else if(input[0] == 'D' || input[0] == 'd'){

        // collect date
        std::string date;
        cout << "> Enter date, or hit enter to skip: ";

        getline(cin, date);

        (*photo).setDate(date);

    } else if(input[0] == 'L' || input[0] == 'l'){

        // collection location
        std::string location;
        cout << "> Enter location, or hit enter to skip: ";

        getline(cin, location);

        (*photo).setLocation(location);


    } else if(input[0] == 'N' || input[0] == 'n'){

        // collect notes
        std::string notes;
        cout << "> Enter notes, or hit enter to skip: ";

        getline(cin, notes);

        (*photo).setNotes(notes);

    } else if(input[0] == 'G' || input[0] == 'g'){
        // collect and process tags
        std::string tags;
        cout << "> Enter tags separated by ; or hit enter to skip: ";

        getline(cin, tags);

        std::vector <std::string> * processed_tags = nullptr;

        if(tags != ""){
            processed_tags = convert_to_tags(tags);
        }
        
        (*photo).setTags(processed_tags);
    }

}

bool review_photo_details(PhotoCollection & collection, PhotoEntry * photo){
    std::string input;
    bool valid;

    // confirm choices
    do {
        cout << endl << "Review photo details:" << endl;

        (*photo).longPrint();

        cout << endl << "Is this correct?" << endl;

        cout << "> Hit enter to confirm and add your photo" << endl;
        cout << "> Enter E to edit photo details" << endl;
        cout << "> Enter C to cancel" << endl;

        cout << "Your choice: ";

        // get a user response
        do {
            getline(cin, input);

            valid = input == "" || input[0] == 'E' || input[0] == 'e' || input[0] == 'C' || input[0] == 'c';
            if(!valid){
                cout << "Invalid input! Try again: ";
            }
        } while(!valid);

        if(input[0] == 'E' || input[0] == 'e'){
            edit_photo_details(photo);
            continue;

        } else if(input[0] == 'C' || input[0] == 'c'){
            cout << endl << "Are you sure you want to cancel? All the photo information you entered will be lost." << endl;

            cout << "Options:" << endl;
            cout << "> Enter C to confirm cancellation" << endl;
            cout << "> Enter B to go back" << endl;
            cout << "Your choice: ";

            do {
                getline(cin, input);

                valid = input[0] == 'B' || input[0] == 'b' || input[0] == 'C' || input[0] == 'c';
                if(!valid){
                    cout << "Invalid input! Try again: ";
                }
            } while(!valid);

            if(input[0] == 'b' || input[0] == 'B'){
                continue;
            } else {
                return false;
            }

        } else {
            return true;
        }

    } while(true);

}

void add_photo(PhotoCollection & collection){
    cout << "-----------------------------------------------------------------" << endl;
    cout << "Add Photo Page" << endl;

    cout << "*Required information: Photo title and path" << endl << endl;

    cout << "> Enter C to cancel, or hit enter to continue: ";

    std::string input;
    bool valid = false;

    // get a user response
    do {
        getline(cin, input);

        valid = input == "" || input[0] == 'C' || input[0] == 'c';
        if(!valid){
            cout << "Invalid input! Try again: ";
        }
    } while(!valid);

    // cancel photo add operation
    if(input != ""){
        wayfinder('V', collection);
        return;
    }

    // steps: 

    PhotoEntry* photo = collect_photo_info();

    // confirm choices
    bool added_photo = review_photo_details(collection, photo);

    if(added_photo){
        collection.add_photo(photo);

        cout << endl << "Photo added successfully!" << endl << endl;

        display_page_options('R');

    } else {
        cout << endl << "Photo add cancelled." << endl << endl;

        delete photo;

        display_page_options('C');
    }
}
