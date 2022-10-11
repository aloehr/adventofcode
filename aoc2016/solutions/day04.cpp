#include <algorithm>
#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


struct room {
    std::string encrypted_name;
    std::string name;
    std::string checksum;

    int sector_id;
    bool real;

    room(std::string en, int s_id, std::string cs) :
        encrypted_name(en),
        checksum(cs),
        sector_id(s_id),
        real(false)
    {
        real = check_real_room();
        if (real) decrypt_name();
    }

    void decrypt_name() {
        for (auto c : encrypted_name) {
            if (c == '-') name += " ";
            else name += ((c - 'a' + sector_id) % 26) + 'a';
        }
    }

    bool check_real_room() {
        std::array<int, 26> letter_count;
        letter_count.fill(0);

        for (auto c : encrypted_name) {
            if (c >= 'a' && c <= 'z') {
                letter_count[c - 'a']++;
            }
        }

        std::vector<std::pair<int, char>> sorted_letter_count;

        for (size_t i = 0; i < letter_count.size(); ++i) {
            sorted_letter_count.push_back(std::make_pair(letter_count[i], i + 'a'));
        }

        std::sort(
            sorted_letter_count.begin(),
            sorted_letter_count.end(),
            [] (std::pair<int, char> a, std::pair<int, char> b) {
                if (a.first != b.first) return a.first > b.first;
                else return a.second < b.second;
            }
        );

        std::string cs;

        for (size_t i = 0; i < 5; ++i) {
            cs += sorted_letter_count[i].second;
        }

        return cs == checksum;
    }
};

aoch::Result solve_day04(aoch::Input& in) {
    aoch::Result r;

    std::vector<room> rooms;
    rooms.reserve(in.size());

    for (auto& l : in) {
        auto tokens = aoch::split(l, '[');

        size_t last_dash_pos = tokens[0].find_last_of('-');

        std::string en = tokens[0].substr(0, last_dash_pos);
        int s_id = std::stoi(tokens[0].substr(last_dash_pos + 1));
        std::string cs = tokens[1].substr(0, 5);

        rooms.emplace_back(en, s_id, cs);
    }

    int sum = 0;
    for (const auto& room : rooms) {
        if (room.real) {
            if (room.name == "northpole object storage") {
                r.part2 = std::to_string(room.sector_id);
            }

            sum += room.sector_id;
        }
    }

    r.part1 = std::to_string(sum);

    return r;
}
