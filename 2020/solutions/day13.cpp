#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

#include "../aoc.hpp"


answer solve_day13(input& in) {

    answer a;
    in.pop_back();

    unsigned int time_at_stop = std::stoul(in[0]); 
    std::vector<std::pair<unsigned int, unsigned int>> busses;
    size_t idx = 0;

    unsigned long long i = 0;
    while (idx != std::string::npos) {
        while (in[1][0] == 'x') {
            in[1] = in[1].substr(2);
            i++;
        }
        unsigned int id = std::stoul(in[1]);
        busses.push_back(std::make_pair(id, i++ % id));
        idx = in[1].find(',');

        if (idx != std::string::npos) in[1] = in[1].substr(idx+1);
    }
    unsigned int best_waiting_time = 99999999;
    unsigned int best_bus_id = 0;

    for (auto p : busses) {

        unsigned int bus_id = p.first;

        unsigned int div = time_at_stop / bus_id;
        if (time_at_stop % bus_id) div++;

        unsigned int waiting_time = bus_id * div - time_at_stop;

        if (waiting_time < best_waiting_time) {
            best_waiting_time = waiting_time;
            best_bus_id = bus_id;
        }
    }

    std::sort(busses.rbegin(), busses.rend(), [](const std::pair<unsigned int, unsigned int>& a, const std::pair<unsigned int, unsigned int>& b) {
        return a.first < b.first;         
    });
    
    //for (auto p : busses) std::cout << p.first << " " << p.second << std::endl;

    unsigned long long current_incr = busses[0].first;


    // chinese remainder theorem - sieving alogrithm
    unsigned long long j = 1;
    unsigned long long add = (busses[0].first - busses[0].second)%busses[0].first;
    for (size_t ix = 1; ix < busses.size(); ++ix) {
        //std::cout << "id: " << busses[ix].first << " 2nd: " << busses[ix].second << " time: " << current_incr << " add: " << add << std::endl;
        while ((add + j * current_incr) % busses[ix].first != (busses[ix].first - busses[ix].second) % busses[ix].first) {
            j++;
        }

        add = add + j * current_incr;
        current_incr *= busses[ix].first;
        //std::cout << " j: " << j << " time: " << current_incr << " add: " << add << std::endl;
        j = 1;
    }
    
    a.part1 = std::to_string(best_bus_id * best_waiting_time);
    a.part2 = std::to_string(add);

    return a;
}

    //unsigned long long current_incr = 1; 


    //// chinese remainder theorem - sieving alogrithm
    //unsigned long long j = 1;
    ////unsigned long long add = (busses[0].first - busses[0].second)%busses[0].first;
    //unsigned long long add = 0; 

    //for (size_t ix = 0; ix < busses.size(); ++ix) {
    //    //std::cout << "id: " << busses[ix].first << " 2nd: " << busses[ix].second << " time: " << current_incr << " add: " << add << std::endl;
    //    while ((add + busses[ix].second) % busses[ix].first != (0)) {
    //        add += current_incr;
    //    }

    //    current_incr *= busses[ix].first;
    //    //std::cout << " j: " << j << " time: " << current_incr << " add: " << add << std::endl;
    //    j = 1;
    //}
