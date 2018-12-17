//
//  PlayerMap.cpp
//  PlayerManagement
//
//  Copyright © 2018 Luke Yuan. All rights reserved.
//

#include "PlayerMap.h"

int current_year;

bool PlayerMap::read_file(const std::string & filename) {
    std::ifstream input(filename);
    input >> current_year;
    
    if (!input || current_year <= 0) {
        return false;
    }
    
    input.get(); // \n character
    
    PlayerEntry temp = PlayerEntry();
    
    if (!input)
        return false;
    
    while (input >> temp) {
        all_players_.insert({temp.last_name + temp.first_name, temp});
    }
    
    current_entry_ = all_players_.begin();
    return true;
}

bool PlayerMap::write_file(const std::string & filename) {
    std::ofstream output(filename);
    if (!output)
        return false;
    
    output << current_year << std::endl;
    if (in_browse_) {
        for (int c: CATEGORIES) {
            for (auto itr = all_players_.begin(); itr != all_players_.end(); ++itr) {
                if (itr->second.category() == c)
                    output << itr->second;
            }
        }
    } else {
        for (auto itr = found_players_.begin(); itr != found_players_.end(); ++itr) {
            output << itr->second;
        }
    }
    
    return true;
}

void PlayerMap::move_to_next() {
    if (all_players_.empty() || (!in_browse_ && found_players_.empty()))
        return;
    ++current_entry_;
    if (in_browse_ && current_entry_ == all_players_.end())
        current_entry_ = all_players_.begin();
    else if (!in_browse_ && current_entry_ == found_players_.end())
        current_entry_ = found_players_.begin();
}

void PlayerMap::move_to_previous() {
    if (all_players_.empty() || (!in_browse_ && found_players_.empty()))
        return;
    
    if (in_browse_ && current_entry_ == all_players_.begin())
        current_entry_ = all_players_.end();

    
    else if (!in_browse_ && current_entry_ == found_players_.begin())
        current_entry_ = found_players_.end();
    
    --current_entry_;
}

void PlayerMap::edit(const std::string & first_name, const std::string & last_name, int birth_year, int registration) {
    std::string new_first_name = (first_name == "") ? current_entry_->second.first_name : first_name;
    std::string new_last_name = (last_name == "") ? current_entry_->second.last_name : last_name;
    int new_birth_year = (birth_year == 0) ? current_entry_->second.year : birth_year;
    bool new_status = (registration == 0) ? current_entry_->second.paid : registration == 2;
    if (first_name != "" || last_name != "") { // the key of the entry needs to be changed
        delete_current_entry();
        all_players_.insert({new_last_name + new_first_name, PlayerEntry(new_first_name, new_last_name, new_birth_year, new_status)});
    } else {
        if (birth_year > 0)
            all_players_[current_entry_->first].year = new_birth_year;
        
        if (registration > 0)
            all_players_[current_entry_->first].paid = new_status;
    }
    
    if (in_browse_)
        current_entry_ = all_players_.find(new_last_name + new_first_name);
    else {
        update_search_results();
        auto found_player = found_players_.find(new_last_name + new_first_name);
        if (found_player != found_players_.end()) // found
            current_entry_ = found_player;
        else if (found_players_.size() > 0)
            current_entry_ = found_players_.begin();
        else
            current_entry_ = all_players_.begin();
    }
}

void PlayerMap::display_stats() const {
    std::cout << "Total Number of Players: " << all_players_.size() << std::endl;
    
    std::map<int, int> category_total;
    std::map<int, int> category_paid;
    
    for (int c: CATEGORIES) {
        category_total.insert(category_total.end(), {c, 0});
        category_paid.insert(category_paid.end(), {c, 0});
    }
    
    for (auto itr = all_players_.begin(); itr != all_players_.end(); ++itr) {
        category_total[itr->second.category()] += 1;
        category_paid[itr->second.category()] += itr->second.paid ? 1 : 0;
    }
    
    int total_paid = 0;
    for (auto itr = category_paid.begin(); itr != category_paid.end(); ++itr)
        total_paid += itr->second;
    
    std::cout << "Total Number of Paid Players: " << total_paid << std::endl;
    std::cout << "Total Number of Unpaid Players: " << all_players_.size() - total_paid << std::endl;
    
    for (int c: CATEGORIES) {
        std::cout << "Total number of Paid U" << c << " Players: " << category_paid[c] << std::endl;
        std::cout << "Total number of Unpaid U" << c << " Players: " << category_total[c] - category_paid[c] << std::endl;
    }
}

bool PlayerMap::update_search_results() {
    found_players_.clear();
    
    auto itr_begin = all_players_.begin();
    auto itr_end = all_players_.end();
    // if last name is specified, optimize search
    if (searched_last_name_ != "") {
        itr_begin = all_players_.lower_bound(searched_last_name_);
        std::string end_prefix = searched_last_name_;
        ++end_prefix[end_prefix.length() - 1]; // all strings greater or equal to this string should be discarded in the search
        itr_end = all_players_.lower_bound(end_prefix);
    }
    
    int count_found_players = 0;
    for (auto itr = itr_begin; itr != itr_end; ++itr) {
        if ((searched_first_name_ == "" || starts_with((itr->second).first_name, searched_first_name_))
                                           && (searched_last_name_ == "" || starts_with((itr->second).last_name, searched_last_name_))
                                           && (searched_birth_year_ == 0 || searched_birth_year_ == (itr->second).year)
                                           && (searched_category_ == 0 || searched_category_ == (itr->second).category())
                                           && (searched_status_ == 0 || (searched_status_ == 1 && !(itr->second).paid) || (searched_status_ == 2 && (itr->second).paid))) {
            found_players_.insert(*itr);
            count_found_players++;
        }
    }
    
    if (count_found_players > 0) {
        found_players_.clear();
        current_entry_ = found_players_.begin();
        return true;
    } else if (!in_browse_)
        current_entry_ = all_players_.begin();
    
    return false;
}

void PlayerMap::new_season(int year) {
    all_players_.clear();
    current_entry_ = all_players_.end();
    exit_search();
    current_year = year;
}
