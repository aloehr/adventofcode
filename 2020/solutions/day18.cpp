#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <cassert>

#include "../aoc.hpp"


struct expr {
    char op = 'X';
    expr *expr_left = nullptr;
    expr *expr_right = nullptr;
    long long value = -999;
};

expr *parse_expression(std::string in, size_t& ptr) {
    expr *res = new expr;

    if (in[ptr] == '(') {
        res->expr_left = parse_expression(in, ++ptr);

        if (ptr == in.size()) {
            return res;
        }
        if (in[ptr] == ')') {
            ptr += 1;
            if (ptr != in.size()) {
                if (in[ptr] == ' ') ptr += 1;
            }
            return res;
        }

        res->op = in[ptr];
        ptr += 2;

        res->expr_right = parse_expression(in, ptr);
    }
    else {
        size_t offset;
        res->value = std::stoi(in.substr(ptr), &offset);
        ptr += offset;
    
        if (ptr == in.size()) {
            return res;
        } 
        if (in[ptr] == ')') {
            ptr += 1;
            if (ptr != in.size()) {
                if (in[ptr] == ' ') ptr += 1;
            }
            return res;
        }

        ptr += 1;
        res->op = in[ptr];
        ptr += 2;

        res->expr_right = parse_expression(in, ptr);
    }

    return res;
}

std::string to_string(const expr *expr) {
    std::string res = "";
    if (expr->expr_left != nullptr) {
        res += '(';
        res += to_string(expr->expr_left);
        res += ')';
    }
    else {
        res += std::to_string(expr->value);
    }

    if (expr->expr_right != nullptr) {
        res += " ";
        res += expr->op;
        res += " ";
        res += to_string(expr->expr_right);
    }

    return res;
}

long long evaluate(expr *e) {
    long long res = e->value;

    if (e->expr_left) {
        res = evaluate(e->expr_left);
    }

    while (e->expr_right) {
        
        long long value_expr_right = e->expr_right->value;
        if (e->expr_right->expr_left) {
            value_expr_right = evaluate(e->expr_right->expr_left);
        }
        
        if (e->op == '+') {
            res += value_expr_right;
        }
        else {
            res *= value_expr_right;
        }

        e = e->expr_right;
    }

    return res;
}

long long evaluate2(expr *e) {
    expr *tmp = e;

    // evaluate op + first
    while (tmp->expr_right) {
        if (tmp->op == '+') {
            long long r = tmp->value;
            if (tmp->expr_left) {
                r = evaluate2(tmp->expr_left);
            }
            
            long long l = tmp->expr_right->value;
            if (tmp->expr_right->expr_left) {
               l = evaluate2(tmp->expr_right->expr_left); 
            }

            tmp->value = r + l;
            tmp->op = tmp->expr_right->op;
            tmp->expr_left = nullptr;
            tmp->expr_right = tmp->expr_right->expr_right;
        }
        else {
            tmp = tmp->expr_right;
        }
    }

    long long res = e->value;

    if (e->expr_left) {
        res = evaluate2(e->expr_left);
    }


    while (e->expr_right) {
        
        long long value_expr_right = e->expr_right->value;
        if (e->expr_right->expr_left) {
            value_expr_right = evaluate2(e->expr_right->expr_left);
        }
        
        if (e->op == '+') {
            res += value_expr_right;
        }
        else {
            res *= value_expr_right;
        }

        e = e->expr_right;
    }

    return res;
}

answer solve_day18(input& in) {

    answer a;
    in.pop_back();

    long long sum = 0;
    long long sum2 = 0;
    for (auto& l : in) {
        size_t tmp = 0;
        expr *e = parse_expression(l, tmp);
        assert(to_string(e) == l);
        
        long long r = evaluate(e); 
        sum += r; 
        //std::cout << to_string(e) << " = " << std::to_string(r) << std::endl;
        
        long long r2 = evaluate2(e); 
        sum2 += r2;
        //std::cout << to_string(e) << " = " << std::to_string(r2) << std::endl;
    }

    a.part1 = std::to_string(sum);
    a.part2 = std::to_string(sum2);

    return a;
}
