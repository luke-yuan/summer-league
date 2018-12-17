//
//  PlayerEntry.h
//  PlayerManagement
//
//  Copyright © 2018 Luke Yuan. All rights reserved.
//

#ifndef PlayerEntry_h
#define PlayerEntry_h

#include <string>
#include <fstream>

const int CATEGORIES[6] = {6, 8, 10, 12, 14, 17};
extern int current_year;

class PlayerEntry
{
public:

    PlayerEntry(): first_name(""), last_name(""), year(0), paid(0) {}
    PlayerEntry(const std::string & first, const std::string & last, int year, bool paid): first_name(first), last_name(last), year(year), paid(paid) {}
    
    int category() const;
    
    std::string first_name;
    std::string last_name;
    int year;
    bool paid;
    
};

std::istream & operator>>(std::istream & input, PlayerEntry & p);
std::ostream & operator<<(std::ostream & output, const PlayerEntry & p);

inline int PlayerEntry::category() const {
    for (int c: CATEGORIES) {
        if (current_year - year <= c)
            return c;
    }
    
    return -1; // if age is out of range
}

inline bool is_valid_category(int category) {
    for (int c: CATEGORIES) {
        if (category == c)
            return true;
    }
    return false;
}

inline bool is_valid_age(int birth_year) {
    return (current_year - birth_year) >= 4 && (current_year - birth_year) < 16;
}

#endif /* PlayerEntry_h */
