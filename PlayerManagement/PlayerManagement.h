//
//  PlayerManagement.h
//  PlayerManagement
//
//  Copyright © 2018 Luke Yuan. All rights reserved.
//

#ifndef PlayerManagement_h
#define PlayerManagement_h

#include "PlayerMap.h"

const char clear_command[] = "cls"; // for Windows
//const char clear_command[] = "clear"; // for Linux and possibly Mac's

const char players_file[] = "players.txt";

class PlayerManagement {
public:
    void run();
    
private:
    void display_entry_and_menu() const;
    bool execute(char command, bool & done);
    void new_season(); // since this function could be called from two places, a function is implemented for convenience
    
    PlayerMap entry_list_;
};

#endif /* PlayerManagement_h */
