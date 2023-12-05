#include <algorithm>
#include <array>
#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"

class Range {
    public:

    unsigned long start;
    unsigned long end;

    Range(unsigned long start, unsigned long end) : start(start), end(end) {}

    bool is_inside_range(unsigned long x) const {
        return x >= this->start && x <= this->end;
    }

    bool has_intersection(const Range& o) const {
        return ((this->start >= o.start || this->end >= o.start) && (this->start <= o.end || this->end <= o.end));
    }

    Range intersection(const Range& o) const {
        if (!this->has_intersection(o)) return Range(1, 0);

        return Range(std::max(this->start, o.start), std::min(this->end, o.end));
    }

    std::vector<Range> exclusion(const Range& exclusor) const {
        if (!this->has_intersection(exclusor)) return { Range(this->start, this->end) };

        std::vector<Range> ret;

        if (this->start < exclusor.start) {
            ret.push_back( Range(this->start, exclusor.start - 1) );
        }

        if (this->end > exclusor.end) {
            ret.push_back( Range(exclusor.end + 1, this->end) );
        }

        return ret;
    }
};

class Map {
    public:

    std::string name;
    std::vector<std::array<Range, 2>> ranges;

    Map(const std::string& name) : name(name) {}

    void add_range(unsigned long dest_start, unsigned long source_start, unsigned long length) {
        this->ranges.push_back({Range(source_start, source_start + length - 1), Range(dest_start, dest_start + length - 1)});
    }

    unsigned long convert(unsigned long x) const {
        for (const auto& r : this->ranges) {
            if (r[0].is_inside_range(x)) {
                unsigned long offset = x - r[0].start;

                return r[1].start + offset;
            }
        }

        return x;
    }

    std::vector<Range> convert(Range s) const {
        std::vector<Range> ret;
        std::vector<Range> sources {s};

        while (sources.size()) {
            auto cur_s = sources.back();
            sources.pop_back();

            bool found_intersection = false;

            for (const auto& r : this->ranges) {
                if (cur_s.has_intersection(r[0])) {
                    found_intersection = true;
                    auto is = cur_s.intersection(r[0]);

                    unsigned long offset_start = is.start - r[0].start;
                    unsigned long offset_end = is.end - r[0].start;

                    ret.push_back( Range(r[1].start + offset_start, r[1].start + offset_end) );

                    auto ex = cur_s.exclusion(is);

                    sources.insert(sources.end(), ex.begin(), ex.end());

                    break;
                }
            }

            if (!found_intersection) {
                ret.push_back(cur_s);
            }
        }

        return ret;
    }
};

aoch::Result solve_day05(aoch::Input& in) {
    aoch::Result a;

    std::vector<unsigned long> seeds;
    std::vector<Map> maps;

    auto seed_tokens = aoch::split(in[0]);

    for (uint i = 1; i < seed_tokens.size(); ++i) {
        seeds.push_back(std::stoul(seed_tokens[i]));
    }

    uint i = 2;

    while (i < in.size()) {
        Map m(in[i++]);

        while(i < in.size() && in[i] != "") {
            auto range_tokens = aoch::split(in[i++]);

            unsigned long d_start= std::stoul(range_tokens[0]);
            unsigned long s_start = std::stoul(range_tokens[1]);
            unsigned long length = std::stoul(range_tokens[2]);

            m.add_range(d_start, s_start, length);
        }

        maps.push_back(m);
        i++;
    }

    unsigned long min_loc = ~0;

    for (auto s : seeds) {
        for (const auto& m : maps) {
            s = m.convert(s);
        }

        min_loc = std::min(min_loc, s);
    }

    a.part1 = std::to_string(min_loc);


    // part 2
    std::vector<Range> sources;
    std::vector<Range> destinations;

    for (uint i = 0; i < seeds.size(); i += 2) {
        sources.push_back(Range(seeds[i], seeds[i] + seeds[i+1] - 1));
    }

    for (const auto& m : maps) {
        for (const auto& s : sources) {
            auto tmp = m.convert(s);
            destinations.insert(destinations.end(), tmp.begin(), tmp.end());
        }

        std::swap(sources, destinations);
        destinations.clear();
    }

    min_loc = ~0;
    for (const auto& r : sources) {
        min_loc = std::min(min_loc, r.start);
    }

    a.part2 = std::to_string(min_loc);

    return a;
}
