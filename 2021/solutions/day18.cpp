#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

#include "../aoc.hpp"


struct SnailNumber {

    long l = 0, r = 0;
    SnailNumber* lp = nullptr;
    SnailNumber* rp = nullptr;
    SnailNumber* p = nullptr;

    SnailNumber() = default;
    SnailNumber(long l, long r) : l(l), r(r) {};
    SnailNumber(SnailNumber* l, SnailNumber* r) : lp(l), rp(r) {};
    SnailNumber(SnailNumber* l, long r) : r(r), lp(l) {};
    SnailNumber(long l, SnailNumber* r) : l(l), rp(r) {};

    SnailNumber(const SnailNumber* c) {
        this->l = c->l;
        this->r = c->r;

        if (c->lp) {
            this->lp = new SnailNumber(c->lp);
            this->lp->p = this;

        }
        if (c->rp) {
            this->rp = new SnailNumber(c->rp);
            this->rp->p = this;
        }

        this->p = c->p;

    }

    long* find_first_right() {
        SnailNumber* c = this;

        while (c->p) {
            if (!c->p->rp) {
                return &(c->p->r);
            } else {
                if (c->p->rp == c) {
                    c = c->p;
                    continue;
                }
                else {
                    c = c->p->rp;

                    while (c->lp) {
                        c = c->lp;
                    }

                    return &(c->l);
                }
            }
        }

        return nullptr;
    }

    long* find_first_left() {
        SnailNumber* c = this;

        while (c->p) {
            if (!c->p->lp) {
                return &(c->p->l);
            } else {
                if (c->p->lp == c) {
                    c = c->p;
                    continue;
                }
                else {
                    c = c->p->lp;

                    while (c->rp) {
                        c = c->rp;
                    }

                    return &(c->r);
                }
            }
        }

        return nullptr;
    }

    void reduce() {
        bool reduced = true;
        while (reduced) {
            reduced = false;
            if (this->reduce_explode()) {
                reduced = true;
            } else if (this->reduce_split()) {
                reduced = true;
            }
        }

    }

    void explode() {
        assert(!this->lp && !this->rp);
        long* fl = this->find_first_left();
        long* fr = this->find_first_right();

        if (fl) *fl += this->l;
        if (fr) *fr += this->r;

        assert(p != nullptr);
        if (p->lp && p->lp == this) {
            p->lp = nullptr;
            p->l = 0;
        } else {
            p->rp = nullptr;
            p->r = 0;
        }

        delete this;
    }

    bool reduce_explode(int d = 0) {
        if (d>3) {
            this->explode();
            return true;
        }
        if (lp) {
            if (lp->reduce_explode(d+1))  {
                return true;
            }
        }

        if (rp) {
            if (rp->reduce_explode(d+1)) {
                return true;
            }
        }

        return false;
    }

    bool reduce_split() {
        if (lp) {
            if (lp->reduce_split()) {
                return true;
            }
        }
        else {
            if (l > 9) {
                // split l
                lp = new SnailNumber(l/2, l - l/2);
                lp->p = this;
                return true;
            }
        }

        if (rp) {
            if (rp->reduce_split()) {
                return true;
            }
        }
        else {
            if (r > 9) {
                // split r
                rp = new SnailNumber(r/2, r - r/2);
                rp->p = this;
                return true;
            }
        }

        return false;

    }

    long magnitude() {
        long mag = 0;

        if (lp) mag += 3 * lp->magnitude();
        else mag += 3*l;

        if (rp) mag += 2 * rp->magnitude();
        else mag += 2*r;

        return mag;
    }

    void add(SnailNumber* o) {
        SnailNumber* sn = new SnailNumber(o);
        SnailNumber* n = new SnailNumber;
        n->l = this->l;
        n->r = this->r;

        n->lp = this->lp;
        n->rp = this->rp;
        n->p = this;

        if (n->lp) n->lp->p = n;
        if (n->rp) n->rp->p = n;

        this->lp = n;

        this->rp = sn;
        sn->p = this;

        this->reduce();
    }

    static SnailNumber* parse_sn(const std::string& str, size_t& idx, SnailNumber* p = nullptr) {
        SnailNumber* ret = new SnailNumber;
        ret->p = p;
        idx++;

        if (str[idx] == '[') {
            ret->lp = parse_sn(str, idx, ret);
        }
        else {
            size_t t;
            ret->l = std::stol(str.substr(idx), &t);
            idx += t+1;
        }

        if (str[idx] == '[') {
            ret->rp = parse_sn(str, idx, ret);
            idx++;
        }
        else {
            size_t t;
            ret->r = std::stol(str.substr(idx), &t);
            idx += t+2;
        }


        return ret;
    }

    std::string to_string() const {
        std::stringstream ss;

        ss << '[';
        if (lp) {
            ss << lp->to_string();
        }
        else {
            ss << l;
        }

        ss << ",";

        if (rp) {
            ss << rp->to_string();
        }
        else {
            ss << r;
        }

        ss << ']';

        return ss.str();

    }

};

answer solve_day18(input& in) {

    answer a;

    std::vector<SnailNumber*> sns;
    sns.reserve(in.size());

    for (auto& l : in) {
        size_t idx = 0;
        sns.push_back(SnailNumber::parse_sn(l, idx));
    }

    SnailNumber* sum = new SnailNumber(sns[0]);

    for (size_t i = 1; i < sns.size(); ++i) {
        sum->add(sns[i]);
    }

    a.part1 = std::to_string(sum->magnitude());


    // part 2
    long best_mag = 0;
    for (size_t i = 0; i < sns.size(); ++i) {
        for (size_t j = 0; j < sns.size(); ++j) {
            if (i == j) continue;

            SnailNumber* x = new SnailNumber(sns[i]);
            x->add(sns[j]);


            best_mag = std::max(best_mag, x->magnitude());
        }
    }

    a.part2 = std::to_string(best_mag);

    return a;
}
