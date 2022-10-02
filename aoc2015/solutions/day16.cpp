#include <iostream>
#include <optional>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"

struct Aunt {
    size_t id;
    std::optional<int> children;
    std::optional<int> cats;
    std::optional<int> samoyeds;
    std::optional<int> pomeranians;
    std::optional<int> akitas;
    std::optional<int> vizslas;
    std::optional<int> goldfish;
    std::optional<int> trees;
    std::optional<int> cars;
    std::optional<int> perfumes;

    bool match(const Aunt& o) const {
        if (children) {
            if (children != o.children) return false;
        }
        if (cats) {
            if (cats != o.cats) return false;
        }
        if (samoyeds) {
            if (samoyeds != o.samoyeds) return false;
        }
        if (pomeranians) {
            if (pomeranians != o.pomeranians) return false;
        }
        if (akitas) {
            if (akitas != o.akitas) return false;
        }
        if (vizslas) {
            if (vizslas != o.vizslas) return false;
        }
        if (goldfish) {
            if (goldfish != o.goldfish) return false;
        }
        if (trees) {
            if (trees != o.trees) return false;
        }
        if (cars) {
            if (cars != o.cars) return false;
        }
        if (perfumes) {
            if (perfumes != o.perfumes) return false;
        }

        return true;
    }

    bool match2(const Aunt& o) const {
        if (children) {
            if (children != o.children) return false;
        }
        if (cats) {
            if (cats <= o.cats) return false;
        }
        if (samoyeds) {
            if (samoyeds != o.samoyeds) return false;
        }
        if (pomeranians) {
            if (pomeranians >= o.pomeranians) return false;
        }
        if (akitas) {
            if (akitas != o.akitas) return false;
        }
        if (vizslas) {
            if (vizslas != o.vizslas) return false;
        }
        if (goldfish) {
            if (goldfish >= o.goldfish) return false;
        }
        if (trees) {
            if (trees <= o.trees) return false;
        }
        if (cars) {
            if (cars != o.cars) return false;
        }
        if (perfumes) {
            if (perfumes != o.perfumes) return false;
        }

        return true;
    }
};

aoch::Result solve_day16(aoch::Input& in) {
    aoch::Result r;

    std::vector<Aunt> aunts;
    aunts.reserve(in.size());

    for (auto& l : in) {
        auto tokens = aoch::split(l);

        Aunt a;
        a.id = std::stoul(tokens[1]);

        for (size_t i = 2; i < tokens.size(); ++i) {
            std::string str = tokens[i].substr(0, tokens[i].size() - 1);

            if (str == "children") {
                a.children = std::stoi(tokens[i+1]);
            } else if (str == "cats") {
                a.cats = std::stoi(tokens[i+1]);
            } else if (str == "samoyeds") {
                a.samoyeds = std::stoi(tokens[i+1]);
            } else if (str == "pomeranians") {
                a.pomeranians = std::stoi(tokens[i+1]);
            } else if (str == "akitas") {
                a.akitas = std::stoi(tokens[i+1]);
            } else if (str == "vizslas") {
                a.vizslas = std::stoi(tokens[i+1]);
            } else if (str == "goldfish") {
                a.goldfish = std::stoi(tokens[i+1]);
            } else if (str == "trees") {
                a.trees = std::stoi(tokens[i+1]);
            } else if (str == "cars") {
                a.cars = std::stoi(tokens[i+1]);
            } else if (str == "perfumes") {
                a.perfumes = std::stoi(tokens[i+1]);
            }
        }

        aunts.push_back(a);
    }

    Aunt pattern;
    pattern.children = 3;
    pattern.cats = 7;
    pattern.samoyeds = 2;
    pattern.pomeranians = 3;
    pattern.akitas = 0;
    pattern.vizslas = 0;
    pattern.goldfish = 5;
    pattern.trees = 3;
    pattern.cars = 2;
    pattern.perfumes = 1;

    bool found_match1 = false;
    bool found_match2 = false;

    for (const auto& a : aunts) {
        if (!found_match1 && a.match(pattern)) {
            r.part1 = std::to_string(a.id);
            found_match1 = true;
        }

        if (!found_match2 && a.match2(pattern)) {
            r.part2 = std::to_string(a.id);
            found_match2 = true;
        }

        if (found_match1 && found_match2) break;
    }

    return r;
}
