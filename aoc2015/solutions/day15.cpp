#include <array>
#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


struct Ingredient {
    int capacity = 0;
    int durability = 0;
    int flavor = 0;
    int texture = 0;
    int calories = 0;
    std::string name = "";
};

int total_score(const std::vector<int>& mix, const std::vector<Ingredient>& ingredients) {

    int capacity_score = 0;
    int durability_score = 0;
    int flavor_score = 0;
    int texture_score = 0;

    for (size_t i = 0; i < ingredients.size(); ++i) {
        capacity_score += mix[i] * ingredients[i].capacity;
        durability_score += mix[i] * ingredients[i].durability;
        flavor_score += mix[i] * ingredients[i].flavor;
        texture_score += mix[i] * ingredients[i].texture;
    }

    capacity_score = std::max(0, capacity_score);
    durability_score = std::max(0, durability_score);
    flavor_score = std::max(0, flavor_score);
    texture_score = std::max(0, texture_score);


    return capacity_score * durability_score * flavor_score * texture_score;
}

int total_calories(const std::vector<int>& mix, const std::vector<Ingredient>& ingredients) {
    int tc = 0;

    for (size_t i = 0; i < mix.size(); ++i) {
        tc += mix[i] * ingredients[i].calories;
    }

    return tc;
}

aoch::Result solve_day15(aoch::Input& in) {
    aoch::Result r;

    std::vector<Ingredient> ingredients;
    ingredients.reserve(in.size());

    for (auto& l : in) {
        auto tokens = aoch::split(l);

        std::string name = tokens[0].substr(0, tokens[0].size() - 1);
        int capacity = std::stoi(tokens[2].substr(0, tokens[2].size() - 1));
        int durability = std::stoi(tokens[4].substr(0, tokens[4].size() - 1));
        int flavor = std::stoi(tokens[6].substr(0, tokens[6].size() - 1));
        int texture = std::stoi(tokens[8].substr(0, tokens[8].size() - 1));
        int calories = std::stoi(tokens[10].substr(0, tokens[10].size()));

        ingredients.push_back({capacity, durability, flavor, texture, calories, name});
    }

    std::vector<int> mix(ingredients.size(), 0);

    const int max_total = 100;
    mix[0] = max_total;

    int best_score = total_score(mix, ingredients);
    int best_score2 = 0;

    if (total_calories(mix, ingredients) == 500) {
        best_score2 = best_score;
    }

    while (mix.back() != max_total) {
        size_t i = mix.size() - 2;

        while(mix[i] == 0) --i;
        mix[i++]--;

        int t = 0;
        mix.back() = 0;
        for (size_t j = 0; j < i; ++j) {
            t += mix[j];
        }

        mix[i] = max_total - t;

        int s = total_score(mix, ingredients);
        if (s > best_score) {
            best_score = s;
        }
        if (s > best_score2 && total_calories(mix, ingredients) == 500) {
            best_score2 = s;
        }
    }

    r.part1 = std::to_string(best_score);
    r.part2 = std::to_string(best_score2);

    return r;
}
