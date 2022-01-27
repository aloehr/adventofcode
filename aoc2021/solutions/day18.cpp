#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

#include "aoch/AOCSolutionTypes.hpp"


struct SnailNumber {

    long l = 0;
    long r = 0;
    SnailNumber* lp = nullptr;
    SnailNumber* rp = nullptr;
    SnailNumber* p = nullptr;

    SnailNumber() = default;
    SnailNumber(long l, long r) : l(l), r(r) {};

    SnailNumber& operator=(const SnailNumber& o) {
        this->l = o.l;
        this->r = o.r;

        if (o.lp) {
            this->lp = new SnailNumber(*(o.lp));
            this->lp->p = this;

        }
        if (o.rp) {
            this->rp = new SnailNumber(*(o.rp));
            this->rp->p = this;
        }

        this->p = o.p;

        return *this;
    }

    SnailNumber& operator=(SnailNumber&& o) {
        if (o.p) {
            if (o.p->lp && o.p->lp == &o) {
                o.p->lp = this;
            } else  {
                o.p->rp = this;
            }
            this->p = o.p;
        }

        this->l = o.l;
        this->r = o.r;

        if (o.lp) {
            this->lp = o.lp;
            this->lp->p = this;
        }

        if (o.rp) {
            this->rp = o.rp;
            this->rp->p = this;
        }

        o.p = nullptr;
        o.lp = nullptr;
        o.rp = nullptr;

        o.l = 0;
        o.r = 0;

        return *this;
    }

    SnailNumber(SnailNumber&& o) {
        (*this) = o;
    }

    SnailNumber(const SnailNumber& o) {
        (*this) = o;
    }

    SnailNumber(const std::string& str, size_t& idx) {
        idx += 1;

        if (idx == 0) idx = 1;

        if (str[idx] == '[') {

            this->lp = new SnailNumber(str, idx);
            this->lp->p = this;
        }
        else {
            size_t t;

            this->l = std::stol(str.substr(idx), &t);
            idx += t+1;
        }

        if (str[idx] == '[') {
            this->rp = new SnailNumber(str, idx);
            this->rp->p = this;
            idx++;
        }
        else {
            size_t t;
            this->r = std::stol(str.substr(idx), &t);
            idx += t+2;
        }

    }

    ~SnailNumber() {
        if (this->rp) {
            delete this->rp;
        }

        if (this->lp) {
            delete this->lp;
        }
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

    long magnitude() const {
        long mag = 0;

        if (lp) mag += 3 * lp->magnitude();
        else mag += 3*l;

        if (rp) mag += 2 * rp->magnitude();
        else mag += 2*r;

        return mag;
    }

    void add(const SnailNumber& o) {
        SnailNumber* sn = new SnailNumber(o);
        SnailNumber* n = new SnailNumber(std::move(*this));


        this->lp = n;
        n->p = this;

        this->rp = sn;
        sn->p = this;

        this->reduce();
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

aoch::Result solve_day18(aoch::Input& in) {

    aoch::Result a;

    std::vector<SnailNumber> sns;
    sns.reserve(in.size());

    for (auto& l : in) {
        size_t idx = 0;
        sns.emplace_back(l, idx);
    }

    SnailNumber sum(sns[0]);

    for (size_t i = 1; i < sns.size(); ++i) {
        sum.add(sns[i]);
    }

    a.part1 = std::to_string(sum.magnitude());


    // part 2
    long best_mag = 0;
    for (size_t i = 0; i < sns.size(); ++i) {
        for (size_t j = 0; j < sns.size(); ++j) {
            if (i == j) continue;

            SnailNumber x(sns[i]);
            x.add(sns[j]);

            best_mag = std::max(best_mag, x.magnitude());
        }
    }

    a.part2 = std::to_string(best_mag);

    return a;
}
