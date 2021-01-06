#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <cassert>

#include "../aoc.hpp"


struct ticket {
    std::vector<int> fields;
    std::vector<bool> valid_fields;

    void add_field(int n) {
        this->fields.push_back(n);
        valid_fields.push_back(true);
    }

    std::string to_string() const {
        std::string r = "";

        for (size_t i = 0; i < this->fields.size(); ++i) {
            if (this->valid_fields[i]) {
                r += std::to_string(this->fields[i]) + "; ";
            }  
            else {
                r += "XXX; ";
            }      
        }

        return r;
    }

};

struct range {
    int start;
    int end;

    bool in_range(int n) const {
        return n >= start && n <= end;
    }
};

struct ticket_field {
    std::string name;
    std::vector<range> ranges;
    
    bool in_range(int n) const {
        bool ret = false;
        for (const auto& r : this->ranges) {
            if (r.in_range(n)) {
                ret = true;
                break;
            }
        }

        return ret;
    }

    std::string to_string() const {
        std::string r = this->name + ": ";

        for (const auto& ra : this->ranges) {
            r += std::to_string(ra.start) + " - " + std::to_string(ra.end) + ", ";
        }

        return r;
    }
};

ticket parse_ticket(std::string in) {
    ticket t;

    size_t ptr = 0;

    while (true) {
        t.add_field(std::stoi(in));

        ptr = in.find(',');
        if (ptr == std::string::npos) break;
        in = in.substr(ptr+1);
    }

    return t;
}

bool can_reduce(const std::set<size_t>& locked_fields, const std::vector<std::vector<ticket_field>>& tf_order, size_t& field) {
    for (size_t i = 0; i < tf_order.size(); ++i) {
        if (tf_order[i].size() == 1 && locked_fields.count(i) == 0) {
            field = i;
            return true;
        }
    }

    return false;
}

size_t get_idx(const std::vector<std::vector<ticket_field>>& tf_order, const std::string& name) {
    for (size_t i = 0; i < tf_order.size(); ++i) {
        if (tf_order[i][0].name == name) {
            return i;
        }
    }

    return tf_order.size();
}

answer solve_day16(input& in) {

    answer a;
    in.pop_back();

    std::vector<ticket_field> ticket_fields;

    // parsing valid ticket fields
    for (const auto& l : in) {
        if (l == "") {
            break;
        }

        std::string line = l;
        size_t ptr;
        ticket_field f;

        // ticket name
        ptr = line.find(':'); 
        f.name = line.substr(0, ptr);
        line = line.substr(ptr+2);
        
        // first range
        range r;
        r.start = std::stoi(line, &ptr);
        line = line.substr(ptr+1);
        r.end = std::stoi(line, &ptr);
        line = line.substr(ptr+4);
        f.ranges.push_back(r);

        //second range
        r.start = std::stoi(line, &ptr);
        line = line.substr(ptr+1);
        r.end = std::stoi(line, &ptr);
        f.ranges.push_back(r);

        ticket_fields.push_back(f);
    }

    // my ticket after one blank line and "my ticket" line
    ticket my_ticket = parse_ticket(in[ticket_fields.size() + 2]);

    std::vector<ticket> nearby_tickets;
    // plus 5 lines because  of 2 blank lines, "my ticket", my ticket values and "nearby tickets"
    for (size_t i = ticket_fields.size() + 5; i < in.size(); ++i) {
        nearby_tickets.push_back(parse_ticket(in[i]));
    }

    int sum = 0;

    std::vector<ticket> valid_tickets;

    for (auto& t : nearby_tickets) {
        for (size_t i = 0; i < t.fields.size(); ++i) {
            bool valid = false;
            for (const auto& tf : ticket_fields) {
                if (tf.in_range(t.fields[i])) {
                    valid = true;
                    break;
                }
            }

            if (!valid) {
                t.valid_fields[i] = valid;
                sum += t.fields[i];
            }
        }
    }

    for (const auto& t : nearby_tickets) {
        bool v = true;

        for (const auto b : t.valid_fields) {
            if (!b) {
                v = false;
                break;
            }
        }

        if (v) {
            valid_tickets.push_back(t);
        }
    }
    
    std::vector<std::vector<ticket_field>> tf_order(my_ticket.fields.size(), ticket_fields);

    for (size_t i = 0; i < tf_order.size(); ++i) {
        for (size_t j = tf_order[i].size(); j > 0; --j) {
            bool valid_field = true;

            for (const auto& t : valid_tickets) {
                if (!tf_order[i][j-1].in_range(t.fields[i])) {
                    valid_field = false;
                    break;
                }
            }

            if (!valid_field) {
                tf_order[i].erase(tf_order[i].begin()+j-1);
            }
        }
    }

    std::set<size_t> locked_fields;

    size_t idx = 0;
    while (can_reduce(locked_fields, tf_order, idx)) {
        locked_fields.insert(idx);

        for (size_t i = 0; i < tf_order.size(); ++i) {
            if (locked_fields.count(i)) continue;

            for (size_t j = 0; j < tf_order[i].size(); ++j) {
                if (tf_order[i][j].name == tf_order[idx][0].name) {
                    tf_order[i].erase(tf_order[i].begin() + j);
                    break;
                }
            }
        }
    }

    for (const auto& x : tf_order) {
        assert(x.size() == 1);
    }

    unsigned long long res = my_ticket.fields[get_idx(tf_order, "departure location")];
    res *= my_ticket.fields[get_idx(tf_order, "departure station")];
    res *= my_ticket.fields[get_idx(tf_order, "departure platform")];
    res *= my_ticket.fields[get_idx(tf_order, "departure track")];
    res *= my_ticket.fields[get_idx(tf_order, "departure date")];
    res *= my_ticket.fields[get_idx(tf_order, "departure time")];

    a.part1 = std::to_string(sum);
    a.part2 = std::to_string(res);

    return a;
}
