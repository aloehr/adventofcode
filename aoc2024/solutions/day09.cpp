#include <list>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

#include <aoch/AOCSolutionTypes.hpp>
#include <aoch/string.hpp>


aoch::Result solve_day09(aoch::Input& in) {
    aoch::Result a;

    // in = { "2333133121414131402"};


    std::vector<int> mem_layout;
    std::list<std::pair<int, size_t>> free_space_idxes;

    int cur_file_id = 0;
    for (size_t i = 0; i < in[0].size(); i++) {
        if (i%2 != 0) cur_file_id++;

        if (i%2 != 0 && in[0][i] != '0') {
            free_space_idxes.push_back({mem_layout.size(), in[0][i] - '0'});
        }

        for (int j = 0; j < in[0][i] - '0'; ++j) {
            if (i%2 && in[0][i] == '0') std::cout << "0 sized file!" << std::endl;
            mem_layout.push_back(i%2 == 0 ? cur_file_id : -1);

        }
    }

    std::vector<int> mem_layout2 = mem_layout;
    size_t empty_block = 0;

    size_t file_block = mem_layout.size() - 1;


    while (empty_block < file_block) {
        while (empty_block < mem_layout.size() && mem_layout[empty_block] != -1) empty_block++;

        if (empty_block == mem_layout.size()) break;

        while (mem_layout[file_block] == -1) file_block--;

        if (empty_block < file_block) std::swap(mem_layout[empty_block], mem_layout[file_block]);
        empty_block++;
        file_block--;
    }

    // for (auto x : mem_layout) std::cout << x;
    // std::cout << std::endl;

    long long sum = 0;
    for (size_t i = 0; i < mem_layout.size(); ++i) {
        sum += mem_layout[i] != -1 ? i * mem_layout[i] : 0;
    }

    a.part1 = std::to_string(sum);


    for (size_t i = mem_layout2.size() - 1; i < mem_layout2.size(); --i) {
        if (mem_layout2[i] == -1) {
            continue;
        }

        size_t j = i-1;

        while (j < mem_layout2.size() && mem_layout2[i] == mem_layout2[j]) j--;

        // std::cout << "f id: " << mem_layout2[i] << " i: " << i << " j: " << j << std::endl;


        size_t free_space_idx = mem_layout2.size();
        for (auto it = free_space_idxes.begin(); it != free_space_idxes.end(); ++it) {
            if ((*it).second < i - j) continue;

            free_space_idx = (*it).first;
            if ((*it).second > i - j) {
                int diff = (*it).second - i + j;

                (*it).first = free_space_idx + (i - j);
                (*it).second = diff;
            } else {
                free_space_idxes.erase(it);
            }

            break;
        }

        if (free_space_idx == mem_layout2.size() || free_space_idx > j+1) {
            i = j + 1;
            continue;
        }

        int file_id = mem_layout2[i];
        // std::cout << "file id: " << file_id << " fsi: " << free_space_idx << " i: " << i << " j: " <<j << std::endl;
        for (size_t k = 0; k < i - j; ++k) {
            mem_layout2[free_space_idx + k] = file_id;
            mem_layout2[k + j + 1] = -1;
        }

        i = j + 1;
    }

    // for (auto x : mem_layout2) std::cout << x;
    // std::cout << std::endl;

    sum = 0;
    for (size_t i = 0; i < mem_layout.size(); ++i) {
        sum += mem_layout2[i] != -1 ? i * mem_layout2[i] : 0;
    }

    a.part2 = std::to_string(sum);

    return a;
}
