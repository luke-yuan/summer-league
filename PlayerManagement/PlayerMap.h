//
//  PlayerMap.h
//  PlayerManagement
//
//  Copyright © 2018 Luke Yuan. All rights reserved.
//

#ifndef PlayerMap_h
#define PlayerMap_h

#include "PlayerEntry.h"
#include <iostream>
#include <map>
#include <iterator>

class PlayerMap
{
public:
    bool read_file(const std::string & filename);
    bool write_file(const std::string & filename);
    
    void display_current_entry() const;
    void display_stats() const;
    void delete_current_entry();
    void move_to_next();
    void move_to_previous();
    
    void add(const std::string & first_name, const std::string & last_name, int birth_year, bool registration);
    void edit(const std::string & first_name, const std::string & last_name, int birth_year, int registration);
    bool search(const std::string & first_name, const std::string & last_name, int birth_year, int category, int registration);
    
    void new_season(int year);
    void exit_search();
    bool in_browse() const { return in_browse_; };
    bool is_empty() const { return all_players_.size() == 0; }
    
private:
    bool update_search_results();
    std::map<std::string, PlayerEntry> all_players_;
    std::map<std::string, PlayerEntry>::iterator current_entry_;
    // variables relevant to searching
    std::map<std::string, PlayerEntry> found_players_;
    std::string searched_first_name_;
    std::string searched_last_name_;
    int searched_birth_year_;
    int searched_category_;
    int searched_status_;
    
    bool in_browse_ = true;
    
    bool starts_with(std::string str, std::string begin) { return str.substr(0, begin.length()) == begin; }
};

inline void PlayerMap::display_current_entry() const {
    if (!all_players_.empty() && (in_browse_ || !found_players_.empty())) {
        std::cout << current_entry_->second;
        std::map<std::string, PlayerEntry>::const_iterator const_itr = current_entry_;
        
        if (in_browse_)
            std::cout << std::distance(all_players_.begin(), const_itr) + 1 << "/" << all_players_.size() << std::endl;
        else
            std::cout << std::distance(found_players_.begin(), const_itr) + 1 << "/" << found_players_.size() << std::endl;
        
    }
}

inline void PlayerMap::add(const std::string & first_name, const std::string & last_name, int birth_year, bool registration) {
    
    PlayerEntry temp = PlayerEntry(first_name, last_name, birth_year, registration);
    all_players_.insert({last_name + first_name, temp});
    if (!in_browse_)
        update_search_results();
    else
        current_entry_ = all_players_.find(last_name + first_name);
}

inline void PlayerMap::delete_current_entry() {
    all_players_.erase(current_entry_->first);
    if (!in_browse_)
        update_search_results();
    move_to_next();
}

inline bool PlayerMap::search(const std::string & first_name, const std::string & last_name, int birth_year, int category, int status) {
    searched_first_name_ = first_name;
    searched_last_name_ = last_name;
    searched_birth_year_ = birth_year;
    searched_category_ = category;
    searched_status_ = status;
    bool success = update_search_results();
    if (success)
        in_browse_ = false;
    else
        in_browse_ = true;
    return success;
}

inline void PlayerMap::exit_search() {
    in_browse_ = true;
    found_players_ = std::map<std::string, PlayerEntry>();
    current_entry_ = all_players_.begin();
}

#endif /* PlayerMap_h */
