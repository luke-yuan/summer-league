//
//  PlayerManagement.cpp
//  PlayerManagement
//
//  Copyright © 2018 Luke Yuan. All rights reserved.
//

#include "PlayerManagement.h"

using namespace std;

void PlayerManagement::run() {
    entry_list_ = PlayerMap();
    bool read_success = entry_list_.read_file(players_file);
    bool done = false;
    
    if (!read_success) {
        cout << "Input File could not be read! A new season will be started." << endl;
        new_season();
    }
    
    while (!done) {
        display_entry_and_menu();
        char command;
        do {
            cout << "choice: ";
            cin >> command;
            cin.ignore(256, '\n');
        } while (!execute(command, done)); // if user inputs an invalid command, ask again
        cout << endl;
    }
    
    if (!entry_list_.write_file(players_file))
        cout << "File could not be written!" << endl;
}

void PlayerManagement::display_entry_and_menu() const {
    system(clear_command);
    
    string long_separator(50, '-');
    
    entry_list_.display_current_entry();
    
    cout << long_separator << endl;
    cout << "edit      next    previous    output    search" << endl;
    if (entry_list_.in_browse())
        cout << "add     delete    Newseason     Stats     quit" << endl;
    else
        cout << "search    add     delete      Exit      quit" << endl;
    cout << long_separator << endl;
}

void PlayerManagement::new_season() {
    cout << "Please enter the year of the new season: ";
    
    int year;
    while (!(cin >> year) || year <= 0) {
        cout << "Please enter a valid year! ";
        cin.clear();
        cin.ignore(256, '\n');
    }
    
    entry_list_.new_season(year);
    
}

bool PlayerManagement::execute(char command, bool & done) {
    switch (command) {
        case 'e': {
            if (!entry_list_.is_empty()) {
                string first, last;
                int birth_year, registration;
                
                cout << "First name? (Leave empty to not change) ";
                getline(cin, first);
                cout << "Last name? ";
                getline(cin, last);
                
                cout << "Birth year? (Enter 0 to not change) ";
                while (!(cin >> birth_year) || birth_year < 0) {
                    cout << "Please enter a valid birth year! ";
                    cin.clear();
                    cin.ignore(256, '\n');
                }
                
                if (!is_valid_age(birth_year) && birth_year != 0) {
                    cout << "Players younger than 4 or older than 16 cannot play in the summer league! Please delete this player." << endl;
                    break;
                }
                
                cout << "Registration status? (0: no change 1: unpaid 2: paid) ";
                while (!(cin >> registration) || !(registration >= 0 && registration <= 2)) {
                    cout << "Please enter a valid registration status! ";
                    cin.clear();
                    cin.ignore(256, '\n');
                }
                
                entry_list_.edit(first, last, birth_year, registration);
            } else {
                cout << "No player's selected!" << endl;
                return false;
            }
            break;
        } case 'n': {
            if (!entry_list_.is_empty())
                entry_list_.move_to_next();
            else {
                cout << "There aren't any players!" << endl;
                return false;
            }
            break;
        } case 'p': {
            if (!entry_list_.is_empty())
                entry_list_.move_to_previous();
            else {
                cout << "There aren't any players!" << endl;
                return false;
            }
            break;
        } case 'o': {
            if (!entry_list_.is_empty()) {
                cout << "Please enter the file to be outputted to: ";
                string file;
                getline(cin, file);
                bool success = entry_list_.write_file(file);
                if (!success) {
                    cout << "File could not be opened!" << endl;
                    return false;
                }
            } else {
                cout << "There aren't any players!" << endl;
                return false;
            }
            break;
        } case 's': {
            if (!entry_list_.is_empty()) {
                string first, last;
                int birth_year, category, registration;
                
                cout << "First name? (Leave empty to ignore) ";
                getline(cin, first);
                cout << "Last name? ";
                getline(cin, last);
                
                cout << "Birth year? (Enter 0 to ignore) ";
                while (!(cin >> birth_year) || birth_year < 0) {
                    cout << "Please enter a valid birth year! ";
                    cin.clear();
                    cin.ignore(256, '\n');
                }
                cout << "Category? (Enter 0 to ignore) U";
                while (!(cin >> category) || !(is_valid_category(category) || category == 0)) {
                    cout << "Please enter a valid category! U";
                    cin.clear();
                    cin.ignore(256, '\n');
                }
                
                cout << "Registration status? (0: ignore 1: unpaid 2: paid) ";
                while (!(cin >> registration) || !(registration >= 0 && registration <= 2)) {
                    cout << "Please enter a valid registration status! ";
                    cin.clear();
                    cin.ignore(256, '\n');
                }
                
                bool found = entry_list_.search(first, last, birth_year, category, registration);
                if (!found)
                    cout << "No players were found!" << endl;
            } else {
                cout << "There aren't any players!" << endl;
                return false;
            }
            break;
        } case 'a': {
            string first, last;
            int birth_year, registration;
            
            cout << "First name? ";
            getline(cin, first);
            cout << "Last name? ";
            getline(cin, last);
            cout << "Birth year? ";
            
            while (!(cin >> birth_year) || birth_year <= 0) {
                cout << "Please enter a valid birth year! ";
                cin.clear();
                cin.ignore(256, '\n');
            }
            
            if (!is_valid_age(birth_year)) {
                cout << "Players younger than 4 or older than 16 cannot play in the summer league!" << endl;
                break;
            }
            
            cout << "Registration status? (1: unpaid 2: paid) ";
            while (!(cin >> registration) || (registration != 1 && registration != 2)) {
                cout << "Please enter a valid registration status! ";
                cin.clear(); // reset cin status
                cin.ignore(256, '\n'); // ignore all characters from the same line
            }
            
            entry_list_.add(first, last, birth_year, registration);
            break;
        } case 'd': {
            if (!entry_list_.is_empty()) {
                char response;
                cout << "Are you sure you want to delete this player? y/n ";
                
                while (cin >> response && response != 'y' && response != 'n') {
                    cout << "Please enter a valid response! ";
                    cin.ignore(256, '\n');
                }
                if (response == 'y')
                    entry_list_.delete_current_entry();
            } else {
                cout << "No player's selected!" << endl;
                return false;
            }
            break;
        } case 'N': {
            char response;
            cout << "This will delete all players. Are you sure? y/n ";
            while (cin >> response && response != 'y' && response != 'n') {
                cout << "Please enter a valid response! ";
                cin.ignore(256, '\n');
            }
            
            if (response == 'y')
                new_season();
            
            break;
        } case 'S': {
            if (entry_list_.in_browse())
                entry_list_.display_stats();
            else {
                cout << "Stats can only be displayed in the browsing view!" << endl;
            }
            return false; // the user needs to see the data
            break;
        } case 'E': {
            if (!entry_list_.in_browse())
                entry_list_.exit_search();
            else {
                cout << "Invalid command!" << endl;
                return false;
            }
            break;
        } case 'q': {
            done = true;
            break;
        } default: {
            cout << "Command not found!" << endl;
            return false;
        }
    }
    return true;
}
