#include <iostream>
#include <string>

#include "../aoc.hpp"


bool is_digit(const char c) {
    return c >= '0' && c <= '9';
}

bool is_digits(const std::string& data) {
    for (auto c : data) {
        if (!is_digit(c)) return false;
    }

    return true;
}

bool is_hex(const std::string& data) {
    for (auto c : data) {
        if (!((c >= 'a' && c <= 'f') || is_digit(c))) return false;
    }

    return true;
}

answer solve_day04(input& in) {

    answer a;
    in.pop_back();

    int passport_valid_count = 0;
    int passport_valid_count2 = 0;

    for (size_t i = 0; i < in.size(); ++i) {

        std::string data;
        while (in[i] != "") {
            if (data.size()) data += " " + in[i++];
            else data += in[i++];
        }

        std::string data_copy = data;

        bool byr = false;
        bool iyr = false;
        bool eyr = false;
        bool hgt = false;
        bool hcl = false;
        bool ecl = false;
        bool pid = false;


        while (true) {
            std::string field = data.substr(0,3);

            if (field == "byr") byr = true;
            else if (field == "iyr") iyr = true;
            else if (field == "eyr") eyr = true;
            else if (field == "hgt") hgt = true;
            else if (field == "hcl") hcl = true;
            else if (field == "ecl") ecl = true;
            else if (field == "pid") pid = true;

            size_t pos = data.find(' ');
            if (pos == std::string::npos) {
                if (byr && iyr && eyr && hgt && hcl && ecl && pid) passport_valid_count++; 
                break;
            }

            data = data.substr(pos+1);
        }

        data = data_copy;
        byr = false;
        iyr = false;
        eyr = false;
        hgt = false;
        hcl = false;
        ecl = false;
        pid = false;

        while (true) {
            std::string field = data.substr(0,3);
            size_t pos = data.find(' ');
            std::string value = data.substr(4,pos-4);

            if (field == "byr") { 
                if (value.size() == 4 && is_digits(value) && std::stoi(value) >= 1920 && std::stoi(value) <= 2002)
                    byr = true;
            }
            else if (field == "iyr") {
                if (value.size() == 4 && is_digits(value) && std::stoi(value) >= 2010 && std::stoi(value) <= 2020)
                    iyr = true;
            }
            else if (field == "eyr") { 
                if (value.size() == 4 && is_digits(value) && std::stoi(value) >= 2020 && std::stoi(value) <= 2030)
                    eyr = true;
            }
            else if (field == "hgt") {
                size_t past;
                int h = std::stoi(value, &past);
                std::string unit = value.substr(past);
                if ((unit == "cm" && h >= 150 && h <= 193) || (unit == "in" && h >= 59 && h <= 76))
                    hgt = true;
            }
            else if (field == "hcl") {
                if (value[0] == '#' && is_hex(value.substr(1)))
                    hcl = true;
            }
            else if (field == "ecl") {
                if (value == "amb" || value == "blu" || value == "brn" || value == "gry" || value == "grn" || value == "hzl" || value == "oth")
                    ecl = true;
            }
            else if (field == "pid") {
                if (value.size() == 9 && is_digits(value)) {
                    pid = true;
                }
            }

            if (pos == std::string::npos) {
                if (byr && iyr && eyr && hgt && hcl && ecl && pid) passport_valid_count2++; 
                break;
            }

            data = data.substr(pos+1);
        }
        
        data.clear();
    }

    a.part1 = std::to_string(passport_valid_count);
    a.part2 = std::to_string(passport_valid_count2);

    return a;
}
