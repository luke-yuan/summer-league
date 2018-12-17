//
//  PlayerEntry.cpp
//  PlayerManagement
//
//  Copyright © 2018 Luke Yuan. All rights reserved.
//

#include "PlayerEntry.h"

std::istream & operator>>(std::istream & input, PlayerEntry & p) {
    input >> p.first_name >> p.last_name;
    input.get(); // \n character
    input >> p.year;
    input.get(); // \n character
    input.get(); // U character
    int category_junk;
    input >> category_junk;
    input.get(); // \n character
    std::string temp;
    getline(input, temp);
    p.paid = temp == "paid";
    
    return input;
}

std::ostream & operator<<(std::ostream & output, const PlayerEntry & p) {
    output << p.first_name << " " << p.last_name << std::endl;
    output << p.year << std::endl;
    output << "U" << p.category() << std::endl;
    if (!p.paid)
        output << "not ";
    
    output << "paid" << std::endl;
    
    return output;
}
