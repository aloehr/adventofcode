#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <cassert>

#include "../aoc.hpp"


struct food {
    std::vector<const std::string *> ingredients;
    std::vector<const std::string *> allergens;

    std::string to_string() const {
        std::stringstream ss;

        for (auto i : ingredients) {
            ss << *i << " ";    
        }

        ss << '(';

        for (auto a : allergens) {
            ss << *a << " ";
        }
        ss << ')';

        return ss.str();
    }
};

struct sort_obj {

    std::map<const std::string *, const std::string *>& map;

    sort_obj(std::map<const std::string *, const std::string *>& m) : map(m) {}

    bool operator() (const std::string *a, const std::string *b) {
        return (*map[a] < *map[b]);
    }
};

answer solve_day21(input& in) {

    answer ans;
    in.pop_back();

    std::set<std::string> ingredients;
    std::set<std::string> allergens;
    std::vector<food> foods;


    for (auto& l : in) {
        food f;

        while (l[0] != '(')  {
            size_t ptr = l.find(' ');

            auto p = ingredients.insert(l.substr(0, ptr));
            f.ingredients.push_back(&(*p.first));

            l = l.substr(ptr+1);
        }

        l = l.substr(10);
        assert(l[0] != ' ');

        while (true)  {
            size_t ptr = l.find(',');

            if (ptr == std::string::npos) {
                auto p = allergens.insert(l.substr(0, l.size() - 1));
                f.allergens.push_back(&(*p.first));
                break;
            }
            else {
                auto p = allergens.insert(l.substr(0, ptr));
                f.allergens.push_back(&(*p.first));

                l = l.substr(ptr+2);
            }
        }

        foods.push_back(f);
    }


    //for (const food& f: foods) {
    //    std::cout << f.to_string() << std::endl;
    //}

    std::map<const std::string *, const std::string *> allergen_translations; 
    std::map<const std::string *, const std::string *> ingredient_contains; 

    bool progress = true;

    while (allergen_translations.size() != allergens.size() && progress) {

        progress = false;
        for (const auto& a : allergens) {
            if (allergen_translations.count(&a)) continue;

            std::vector<std::vector<const std::string *>> contains_allergen;

            for (const auto& f : foods) {
                if (std::count(f.allergens.begin(), f.allergens.end(), &a)) {

                    //std::cout << "found food with allergens YAY" << std::endl;

                    contains_allergen.push_back(f.ingredients);
                }
            }


            // try to reduce
            std::vector<const std::string *> candidates;

            for (auto i : contains_allergen[0]) {
                if (ingredient_contains.count(i)) continue;
                bool not_the_one = false;
                for (auto& iv : contains_allergen) {
                    if (!std::count(iv.begin(), iv.end(), i)) {
                        not_the_one = true;
                        break;
                    }
                }
                if (!not_the_one) {
                    candidates.push_back(i);
                }
            }

            //std::cout << "testing " << a << std::endl; 
            //std::cout << "candidates: "; 
            //for (auto c : candidates)
            //    std::cout << *c << " ";
            //std::cout << std::endl;

            //assert(candidates.size() == 1);
            if (candidates.size() == 1) {
                progress = true;
                allergen_translations[&a] = candidates[0];
                ingredient_contains[candidates[0]] = &a;
            }
        }
    }

    if (!progress) {
        std::cout << "sad story" << std::endl;
    }

    //for (auto p : allergen_translations) {
    //    std::cout << *p.first  << " is in " << *p.second << std::endl;
    //}

    int c = 0;

    for (auto& f : foods) {
        for (auto i : f.ingredients) {
            if (!ingredient_contains.count(i)) c++;
        }
    }

    std::vector<const std::string *> a;
    for (auto p : ingredient_contains) {
        a.push_back(p.first);
    }

    sort_obj sor(ingredient_contains);
    std::sort(a.begin(), a.end(), sor);

    std::stringstream ss;

    for (size_t i = 0; i < a.size(); ++i) {
        ss << *a[i];

        if (i != a.size() - 1)
            ss << ",";
    }

    ans.part1 = std::to_string(c);
    ans.part2 = ss.str();

    return ans;
}
