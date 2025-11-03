#include <string>
#include "photoCollection.hpp"

bool is_int(std::string input);

bool is_valid_int(int x, PhotoCollection & collection);

char parse_option_input(char c);

void wayfinder(char c);

void display_page_options(char current_page);

void run_program();

void welcome();

void display_all(PhotoCollection &collection);

void view_details(PhotoCollection &collection, int index);

void edit_photo_details(PhotoEntry* photo);

void help();

std::vector<std::string>* convert_to_tags(std::string input);

PhotoEntry* collect_photo_info();

bool review_photo_details(PhotoCollection & collection, PhotoEntry * photo);

void add_photo(PhotoCollection & collection);
