import zmq
import json
import requests

'''
    photo = { title: "txt", path: "txt", date: "", location: "", tags: ["", "", ""], notes: "txt"}

'''

def welcome():
    print("-----------------------------------------------------------------")
    print("> Hello, and welcome to My Photo Library!")
    print("> Have all your favorite photos in one place and organize them to your heart's content!")
    print("> Additionally, store all the special details about your photo to make sure you remember", end=' ')
    print("everything that makes your favorite photos special.")

    display_page_options('W')


def help():
    print("-----------------------------------------------------------------")
    print("Help Page")
    print()

    print("To add a photo:")
    print("> Enter A to access the Add Photo Page")
    print("> Enter the photo information")
    print("> When reviewing your photo information, you can", end=' ')
    print("change the information added by hitting E, and then", end=' ')
    print("hitting the letter corresponding to the field you'd like to edit")
    print("> You can also hit C during the review process to cancel adding your photo", end=' ')
    print("but all data entered will be lost")
    print("Note: title and path are required fields, but all other fields can be skipped", end=' ')
    print("by hitting enter")

    print()
    print("To view all photos: ")
    print("> Enter V to access the View All Photos Page")

    print()
    print("To view a specific photo: ")
    print("> Start on the View All Photos Page")
    print("> Enter the number of the photo you'd like to view")
    print()

    display_page_options('H')


def valid_option_code(code, current_page):
    if current_page == 'W':     # welcome page
        return code == 'S' or code == 'X'
    elif current_page == 'R':   # review added photo page
        return code == 'R' or code == 'V' or code == 'H' or code == 'A' or code == 'X'
    elif current_page == 'V':   # view all photos page
        return code == 'A' or code == 'X' or code == 'H'
    elif current_page == 'H':   # help page
        return code == 'A' or code == 'V' or code == 'X'
    elif current_page == 'C':   # cancel photo add page
        return code == 'A' or code == 'V' or code == 'X' or code == 'H'
    elif current_page == 'A':   # add photo page
        return code == 'X' or code == 'H' or code == 'V'
    elif current_page == 'P':   # pre-review photo page
        return code == 'E' or code == 'C' or code == ""
    elif current_page == 'O':   # confirm cancellation page
        return code == 'C' or code == 'B'
    elif current_page == 'D':   # view photo details page
        return code == 'A' or code == 'X' or code == 'V' or code == 'H'
    elif current_page == 'E':
        return code == "" or code == 'T' or code == 'P' or code == 'L' or code == 'G' \
        or code == 'D' or code == 'N'


def get_option(current_page, photos):

    valid = False
    option = ''

    while not valid:
        option = input()

        if not option.isnumeric():
            valid = valid_option_code(option.upper(), current_page)
        else:
            valid = int(option) <= len(photos) and not int(option) == 0 
    
        if not valid:
            print()
            print(option, "is not a valid input, try again.")
            print("> Your choice:", end=' ')

    return option.upper()


def display_page_options(current_page):
    print()
    print("What would you like to do? ")
    print("> Options: ")

    if current_page == 'W':
        print("> Enter S to start")
    else:
        
        if current_page == 'R':
            print("> Enter R to view most recently added photo")
        
        if current_page != 'A':
            print("> Enter A to add a photo")
        
        if current_page != 'V':
            print("> Enter V to view all photos")
        else: 
            print("> Enter a number to view that photo's details")
        
        if current_page != 'H':
            print("> Enter H for help") 
        

    print("> Enter X to exit")

    print("> Your choice: ", end=' ')


def handle_path_input(response):
    if response == "\@":
        topic = input("> Enter the photo topic or hit enter for a random topic: ")

        if topic == "":
            topic = "random"

        response = retrieve_rand_photo_by_topic(topic)

        print("> Selected image path:", response)
        
    return response


def get_required_field(field):
    response = ""

    while True:
        if field == "path":
            response = input("> Enter the photo path (required), or enter \@ to pick a random image path instead: ")
        else: 
            response = input("> Enter the photo {} (required): ".format(field))

        if not response == "":
            if field == "path":
                response = handle_path_input(response)

            return response
        else:
            print(f"> Error: No photo {field} entered! This is a required field; please try again")


def get_non_req_field(field):
    response = ""

    response = input("> Enter {}, or hit enter to skip: ".format(field))

    return response


def process_tags(tags):
    tag_list = tags.split(';')

    edited_tags = []

    for t in tag_list:
        edited = "#" + t
        edited_tags.append(edited)
    
    return edited_tags

def get_tags(photo):
    tags = ""
    tags_list = ""

    tags = input("> Enter tags separated by ; or enter \@ to auto-generate tags or hit enter to skip: ")

    if tags == "\@":
        tags_list = get_generated_tags(photo)
        print("> Generated tags:", end=' ')
        for t in tags_list:
            print(t, end=' ')
        print()
    elif not tags == "":
        tags_list = process_tags(tags)
    
    return tags_list


def get_photo_details():
    photo = {}
    
    set_field('T', photo)
    set_field('P', photo)
    set_field('D', photo)
    set_field('L', photo)
    set_field('G', photo)
    set_field('N', photo)

    # MICROSERVICE FROM BIG POOL: json trimmer, to get rid of empty fields (e.g. "location": "")
    trimmed_photo = trim_json(photo)

    return trimmed_photo


def review_photo_details(photo):
    print("Review Photo Details: ")
    print()

    long_print(photo)

    print()

    print("Is this correct?")
    print("> Hit enter to confirm and add your photo")
    print("> Enter E to edit photo details")
    print("> Enter C to cancel")
    print("> Your choice:", end=' ')

    response = get_option('P', [])

    return response


def set_field(field, photo):
    if field == 'T':
        photo["title"] = get_required_field("title")
    elif field == 'P':
        photo["path"] = get_required_field("path")
    elif field == 'D':
        photo["date"] = get_non_req_field("date")
    elif field == 'L':
        photo["location"] = get_non_req_field("location")
    elif field == 'G':
        photo["tags"] = get_tags(photo)
    elif field == 'N':
        photo["notes"] = get_non_req_field("notes")
            

def edit_photo_details(photo):
    print("Edit Photo Details")
    print()

    print("Options:")
    print("> Enter T to edit photo title")
    print("> Enter P to edit photo path")
    print("> Enter D to edit photo date")
    print("> Enter L to edit photo location")
    print("> Enter N to edit photo notes")
    print("> Enter G to edit tags")
    print("> Hit enter to confirm and add photo")
    print("Your choice: ", end=' ')

    choice = get_option('E', [])

    set_field(choice, photo)


def confirm_cancel():
    print("Are you sure you want to cancel? All the photo information you entered will be lost.")
    print("Options:")
    print("> Enter C to confirm cancellation")
    print("> Enter B to go back")
    print("Your choice: ", end=' ')
    response = get_option('O', [])

    return response


def add_photo(photos):
    print("-----------------------------------------------------------------")
    print("Add Photo")
    print()
    photo = get_photo_details()

    while True:
        next_step = review_photo_details(photo)

        if next_step == "":
            photos.append(photo)
            print("Photo added successfully!")
            display_page_options('R')
            return 'R'
        
        elif next_step == 'E':
            edit_photo_details(photo)
        elif next_step == 'C':
            cancel = confirm_cancel()
            
            if cancel == 'C':
                print("Photo add cancelled.")
                display_page_options('C')
                return 'C'


def short_print(photo):
    print("Title:", photo["title"])
    print("Path:", photo["path"])


def long_print(photo):
    short_print(photo)
    
    fields = photo.keys()

    if "date" in fields:
        print("Date:", photo["date"])
    
    if "location" in fields:
        print("Location:", photo["location"])
    
    if "tags" in fields:
        print("Tags:", end=' ')
        for t in photo["tags"]:
            print(t, end=' ')
        print("")
    
    if "notes" in fields:
        print("Notes:", photo["notes"])


def zmq_client_code(path, data):
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.connect(path)

    data_to_send = data

    if not isinstance(data, str):
        data_to_send = json.dumps(data)
    
    socket.send_string(data_to_send)

    response = socket.recv()

    decoded = response.decode()

    context.destroy()

    return decoded
    

def get_fun_fact(topic):
    return zmq_client_code("tcp://localhost:7424", topic)


def get_generated_tags(photo):
    tags = zmq_client_code("tcp://localhost:8932", photo)
    return process_tags(tags)


def post_request_code(path, data):
    response = requests.post(path, json=data)
    return response


def retrieve_rand_photo_by_topic(topic):
    data = {"topic": topic}
    response = post_request_code("http://localhost:5209/requestImage", data)

    # grab the img path from the response
    return response.text


def trim_json(data):

    to_send = {"data": data}

    response = post_request_code("http://localhost:8244/api/json/trim", to_send)
    json_data = response.json()
    back_to_obj = json_data["data"]


    return back_to_obj


def display_all(photos):
    print("-----------------------------------------------------------------")
    print("View All Photos Page")
    print()

    # MICROSERVICE FROM BIG POOL: fun fact generator
    fun_fact = get_fun_fact("random")
    print("Fun fact:", fun_fact)
    print()

    index = 1

    if len(photos) == 0:
        print("You don't have any photos yet!")
        print()
    else:
        for p in photos:
            print(index, ":", end=' ')
            short_print(p)
            print()

            index = index + 1
        
    display_page_options('V')


def view_details(photo):
    print("-----------------------------------------------------------------")
    print("View Photo Details")
    print()

    long_print(photo)

    display_page_options('D')


def wayfinder(go_to, photos):
    if go_to == 'A':
         return add_photo(photos)
    elif go_to ==  'H':
        help()
        return 'H'
    elif go_to == 'V' or go_to == 'S':
        display_all(photos)
        return 'V'
    elif go_to == 'R':
        # show most recently added photo
        view_details(photos[len(photos) - 1])
        return 'R'
    else:
        # c must actually be an int; display details page for photo
        view_details(photos[int(go_to) - 1])
        return 'R'
    

def run():
    photos = []

    welcome()

    go_to = get_option('W', photos)

    while not go_to == 'X':
        redir = wayfinder(go_to, photos) 

        if go_to == 'S':
            go_to = 'V'

        go_to = get_option(redir, photos)
    
    print("Bye!")

if __name__ == "__main__":
    run()