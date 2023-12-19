#include <stdexcept>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


struct Range {
    int start;
    int end;

    std::pair<std::optional<Range>, std::optional<Range>> split_at(int x) {
        if (x <= start) {
            return std::make_pair(std::nullopt, *this);
        } else if (x > this->end) {
            return std::make_pair(*this, std::nullopt);
        } else {
            return std::make_pair(Range{start, x-1}, Range{x, end});
        }
    }

    std::optional<Range> intersection(const Range& o) const {
        if (o.end < this->start) {
            return std::nullopt;
        } else if (o.start > this->end) {
            return std::nullopt;
        }

        return Range{std::max(this->start, o.start), std::min(this->end, o.end)};
    }

    std::vector<Range> complement(const Range& o) const {
        std::vector<Range> ret;
        if (o.start <= this->start && o.end >= this->end) {
            return ret;
        }

        auto intersect = this->intersection(o);

        if (!intersect) {
            ret.push_back(*this);
            return ret;
        }

        if (this->start < intersect->start) {
            ret.push_back({this->start, intersect->start - 1});
        }
        if (this->end > intersect->end) {
            ret.push_back({intersect->end + 1, this->end});
        }

        return ret;
    }

    operator std::string() const {
        return "Range{" + std::to_string(this->start) + ", " + std::to_string(this->end) + "}";
    }

    friend std::ostream& operator<<(std::ostream& os, const Range& r) {
        os << "Range{" << r.start << ", " << r.end << "}";
        return os;
    }
};


struct RangedPart {
    Range x {1, 4000};
    Range m {1, 4000};
    Range a {1, 4000};
    Range s {1, 4000};

    Range get(char c) const {
        switch (c) {
            case 'x': return this->x;
            case 'm': return this->m;
            case 'a': return this->a;
            case 's': return this->s;
            default: throw std::runtime_error("tried to access unknown Part category: " + std::string(1, c));
        }
    }

    unsigned long long combinations() const {
        unsigned long long ret = 1;

        ret *= this->x.end - this->x.start + 1;
        ret *= this->m.end - this->m.start + 1;
        ret *= this->a.end - this->a.start + 1;
        ret *= this->s.end - this->s.start + 1;

        return ret;
    }

    std::vector<RangedPart> complement(const RangedPart& o) const {
        std::vector<RangedPart> ret;

        auto x_intersection = this->x.intersection(o.x);
        auto m_intersection = this->m.intersection(o.m);
        auto a_intersection = this->a.intersection(o.a);
        auto s_intersection = this->s.intersection(o.s);

        if (!x_intersection || !m_intersection || !a_intersection || !s_intersection) {
            ret.push_back(*this);
            return ret;
        }

        auto x_complement = this->x.complement(o.x);
        auto m_complement = this->m.complement(o.m);
        auto a_complement = this->a.complement(o.a);
        auto s_complement = this->s.complement(o.s);

        for (auto& cur_c : x_complement) {
            ret.push_back(RangedPart{cur_c, this->m, this->a, this->s});
        }

        for (auto& cur_c : m_complement) {
            ret.push_back(RangedPart{this->x, cur_c, this->a, this->s});
        }

        for (auto& cur_c : a_complement) {
            ret.push_back(RangedPart{this->x, this->m, cur_c, this->s});
        }

        for (auto& cur_c : s_complement) {
            ret.push_back(RangedPart{this->x, this->m, this->a, cur_c});
        }

        return ret;
    }

    void set(char c, const Range& v) {
        switch (c) {
            case 'x': this->x = v; break;
            case 'm': this->m = v; break;
            case 'a': this->a = v; break;
            case 's': this->s = v; break;
            default: throw std::runtime_error("tried to set unknown Part category: " + std::string(1, c) + " with value " + std::string(v));
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const RangedPart& rp) {
        os << "RangedPart{x: " << rp.x << ", m: " << rp.m << ", a: " << rp.a << ", s: " << rp.s << "}";
        return os;
    }
};


struct Part {
    int x, m , a, s;

    Part(const std::string& str) {
        auto part_tokens = aoch::split(str.substr(1, str.size() - 2), ',');

        for (const auto& part_token : part_tokens) {
            auto tokens = aoch::split(part_token, '=');

            this->set(tokens[0][0], std::stoi(tokens[1]));
        }
    }

    int sum() const {
        return this->x + this->m + this->a + this->s;
    }

    int get(char c) const {
        switch (c) {
            case 'x': return this->x;
            case 'm': return this->m;
            case 'a': return this->a;
            case 's': return this->s;
            default: throw std::runtime_error("tried to access unknown Part category: " + c);
        }
    }

    void set(char c, int v) {
        switch (c) {
            case 'x': this->x = v; break;
            case 'm': this->m = v; break;
            case 'a': this->a = v; break;
            case 's': this->s = v; break;
            default: throw std::runtime_error("tried to set unknown Part category: " + std::string(1, c) + " with value " + std::to_string(v));
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Part& p) {
        os << "Part{x: " << p.x << ", m: " << p.m << ", a: " << p.a << ", s: " << p.s << "}";
        return os;
    }
};

struct Rule {
    char x;
    char op;
    int value;
    std::string dest;

    Rule(const std::string& str) {
        this->x = str[0];
        this->op = str[1];

        auto x = aoch::split(str.substr(2), ':');

        this->value = std::stoi(x[0]);
        this->dest = x[1];
    }

    bool check_condition(const Part& p) const {
        switch (this->op) {
            case '<': return p.get(this->x) < this->value;
            case '>': return p.get(this->x) > this->value;
            default: throw std::runtime_error("unknown op");
        }
    }

    std::pair<std::optional<RangedPart>, std::optional<RangedPart>> check_condition(const RangedPart& p) const {
        switch (this->op) {
            case '<': {
                auto splitted_category_range = p.get(this->x).split_at(this->value);
                std::optional<RangedPart> less;

                if (splitted_category_range.first) {
                    less = p;
                    less->set(this->x, *splitted_category_range.first);
                }

                std::optional<RangedPart> equal_or_more;
                if (splitted_category_range.second) {
                    equal_or_more = p;
                    equal_or_more->set(this->x, *splitted_category_range.second);
                }

                return std::make_pair(less, equal_or_more);
            }
            case '>': {
                auto splitted_category_range = p.get(this->x).split_at(this->value+1);
                std::optional<RangedPart> less_or_equal;

                if (splitted_category_range.first) {
                    less_or_equal = p;
                    less_or_equal->set(this->x, *splitted_category_range.first);
                }

                std::optional<RangedPart> more;
                if (splitted_category_range.second) {
                    more = p;
                    more->set(this->x, *splitted_category_range.second);
                }

                return std::make_pair(more, less_or_equal);
            }
            default: throw std::runtime_error("unknown op");
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Rule& r) {
        os << "Rule{" << r.x << r.op << r.value << " -> " << r.dest << "}";
        return os;
    }
};

struct Workflow {
    std::string name;
    std::string dest_if_no_rule_applies;
    std::vector<Rule> rules;

    Workflow(const std::string str) {
        size_t idx = str.find_first_of('{');
        this->name = str.substr(0, idx);

        auto rule_tokens = aoch::split(str.substr(idx+1, str.size() - idx - 2), ',');
        this->dest_if_no_rule_applies = rule_tokens.back();
        rule_tokens.pop_back();

        for (const auto& r_str : rule_tokens) {
            this->rules.emplace_back(r_str);
        }
    }
    void add_rule(Rule& r) {
        this->rules.push_back(r);
    }

    const std::string& get_destination(const Part& p) const {
        for (const auto& r : this->rules) {
            if (r.check_condition(p)) {
                return r.dest;
            }
        }

        return this->dest_if_no_rule_applies;
    }

    std::vector<std::pair<RangedPart, std::string>> get_destination(RangedPart p) const {
        std::vector<std::pair<RangedPart, std::string>> ret;

        for (const auto& r : this->rules) {
            auto x = r.check_condition(p);

            if (x.first) {
                ret.push_back(std::make_pair(*x.first, r.dest));
            }

            if (!x.second) return ret;
            else {
                p = *x.second;
            }
        }

        ret.push_back(std::make_pair(p, this->dest_if_no_rule_applies));
        return ret;
    }

    friend std::ostream& operator<<(std::ostream& os, const Workflow& wf);
};

std::ostream& operator<<(std::ostream& os, const Workflow& wf) {
    os << "Workflow{name: " << wf.name << ", rules: (";
    for (auto r : wf.rules) {
        os << r << ", ";
    }
    os << "), dest if no rule applied: " << wf.dest_if_no_rule_applies << "}";
    return os;
}

aoch::Result solve_day19(aoch::Input& in) {
    aoch::Result a;

    std::map<std::string, Workflow> workflows;
    std::vector<Part> parts;
    std::vector<Part> accepted;

    // parsing
    {
        size_t i = 0;

        // parsing workflows
        while (in[i] != "") {
            Workflow wf(in[i++]);
            workflows.insert({wf.name, wf});
        }

        i++;

        // parsing parts
        while (i < in.size()) {
            parts.emplace_back(in[i++]);
        }

        for (const auto& p : parts) {
            std::string dest = "in";

            while (dest != "A" && dest != "R") {
                const Workflow& cur_wf = workflows.at(dest);
                dest = cur_wf.get_destination(p);
            }

            if (dest == "A") accepted.push_back(p);
        }
    }

    int sum = 0;
    for (const auto& p : accepted) {
        sum += p.sum();
    }

    a.part1 = std::to_string(sum);


    // part 2
    RangedPart rp;
    std::vector<std::pair<RangedPart, std::string>> work {std::make_pair(rp, "in")};
    std::vector<RangedPart> accepted_ranges;

    while (work.size()) {
        auto cur_rp = work.back();
        work.pop_back();

        const Workflow& cur_wf = workflows.at(cur_rp.second);
        auto results = cur_wf.get_destination(cur_rp.first);

        for (auto& result : results) {
            if (result.second == "A") {
                accepted_ranges.push_back(result.first);
            } else if (result.second != "R") {
                work.push_back(result);
            }
        }
    }

    std::vector<RangedPart> accepted_without_intersection {accepted_ranges.back()};
    accepted_ranges.pop_back();

    for (const auto& rp : accepted_ranges) {
        std::vector<RangedPart> next_stage {rp};
        std::vector<RangedPart> needs_intersection_check;

        for (const auto& test : accepted_without_intersection) {
            std::swap(next_stage, needs_intersection_check);
            while (needs_intersection_check.size()) {
                const auto cur = needs_intersection_check.back();
                needs_intersection_check.pop_back();

                auto result = cur.complement(test);
                next_stage.insert(next_stage.end(), result.begin(), result.end());
            }
        }

        accepted_without_intersection.insert(accepted_without_intersection.end(), next_stage.begin(), next_stage.end());
    }

    unsigned long long total_combinations = 0;
    for (const auto& rp : accepted_without_intersection) {
        total_combinations += rp.combinations();
    }

    a.part2 = std::to_string(total_combinations);

    return a;
}
