#include <iostream>
#include <map>
#include <algorithm>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


struct Dir;

struct Entry {
    std::string name;
    std::string type;
    Dir* parent;

    virtual int size() = 0;

    Entry(std::string name, std::string type, Dir* parent) : name(name), type(type), parent(parent) {}
};

struct Dir : public Entry {
    std::map<std::string, Entry*> entries;

    Dir(std::string name, Dir* parent) : Entry(name, "dir", parent) {}

    int size() {
        int dir_size = 0;
        for (const auto& e : entries) {
            dir_size += e.second->size();
        }

        return dir_size;
    }
};

struct File : public Entry {
    int s;

    File(std::string name, int size, Dir* parent) : Entry(name, "file", parent), s(size) {}

    int size() {
        return this->s;
    }
};

int sum_it(Dir* d) {
    int sum = 0;

    int x = d->size();

    if (x <= 100000) {
        sum += x;
    }

    for (auto& e : d->entries) {
        if (e.second->type == "dir") {
            sum += sum_it(static_cast<Dir *>(e.second));
        }
    }

    return sum;
}

aoch::Result solve_day07(aoch::Input& in) {

    aoch::Result a;

    Dir root("/", nullptr);
    std::vector<Dir*> dirs;
    dirs.push_back(&root);

    Dir* cur_dir = &root;


    for (auto l : in ) {
        auto tokens = aoch::split(l);

        if (tokens[0] == "$") {
            if (tokens[1] == "cd") {
                if (tokens[2] == "..") {
                    cur_dir = cur_dir->parent;
                } else if (cur_dir->entries.count(tokens[2])){
                    if (cur_dir->entries[tokens[2]]->type == "dir") {
                        cur_dir = static_cast<Dir*>(cur_dir->entries[tokens[2]]);
                    } else {
                        std::cout << "tried to change dir to non dir entry: " << tokens[2] << std::endl;
                    }
                }
            }
        } else if (tokens[0] == "dir") {
            if (!cur_dir->entries.count(tokens[1])) {
                Dir *d = new Dir(tokens[1], cur_dir);
                cur_dir->entries[tokens[1]] = d;
                dirs.push_back(d);
            } else {
                std::cout << "tried to add a dir that already exists: " << tokens[1] << std::endl;
            }
        } else {
            if (!cur_dir->entries.count(tokens[1])) {
                cur_dir->entries[tokens[1]] = new File(tokens[1], std::stoi(tokens[0]), cur_dir);
            } else {
                std::cout << "tried to add a file that already exists: " << tokens[1] << std::endl;
            }
        }
    }

    a.part1 = std::to_string(sum_it(&root));


    // part 2
    std::sort(dirs.begin(), dirs.end(), [](Dir* a, Dir* b) {
        return a->size() < b->size();
    });

    int unused_space = 70000000 - root.size();
    int min_dir_size_to_delete = 30000000 - unused_space;

    int i = 0;

    while (dirs[i]->size() < min_dir_size_to_delete) {
        i++;
    }

    a.part2 = std::to_string(dirs[i]->size());

    return a;
}
