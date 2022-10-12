#include <array>
#include <iostream>
#include <set>
#include <string>
#include <utility>

#include "aoch/AOCSolutionTypes.hpp"


struct ip_address {
    std::vector<std::string> normal;
    std::vector<std::string> hypernet;
};

bool has_abba(const std::string& s) {
    for (size_t i = 0; i < s.size() - 3; ++i) {
        if (s[i + 0] == s[i + 3] && s[i + 1] == s[i + 2] && s[i + 0] != s[i + 1]) return true;
    }

    return false;
}

void find_all_aba(const std::string& s, std::set<std::array<char, 2>>& out) {
    for (size_t i = 0; i < s.size() - 2; ++i) {
        if (s[i + 0] == s[i + 2] && s[i + 0] != s[i + 1]) {
            out.insert({s[i + 0], s[i + 1]});
        }
    }

    return;
}

bool has_matching_bab(const std::string& s, const std::array<char, 2>& ab) {
    for (size_t i = 0; i < s.size() - 2; ++i) {
        if (s[i + 0] == s[i + 2] && s[i + 0] != s[i + 1] &&
            s[i + 0] == ab[1] && s[i + 1] == ab[0]
        ) {
            return true;
        }
    }

    return false;
}

aoch::Result solve_day07(aoch::Input& in) {
    aoch::Result r;

    std::vector<ip_address> ips;

    for (auto l : in) {
        ip_address ip;

        size_t left_p = 0;
        size_t right_p;

        while ((right_p = l.find('[', left_p)) != std::string::npos) {
            ip.normal.push_back(l.substr(left_p, right_p - left_p));

            left_p = right_p + 1;
            right_p = l.find(']', left_p);

            ip.hypernet.push_back(l.substr(left_p, right_p - left_p));

            left_p = right_p + 1;
        }

        if (left_p < l.size()) {
            ip.normal.push_back(l.substr(left_p));
        }

        ips.push_back(std::move(ip));
    }

    int ips_tls_support_count = 0;

    for (const auto& ip : ips) {
        bool found_abba = false;
        for (const auto& s : ip.normal) {
            if (has_abba(s)) {
                found_abba = true;
                break;
            }
        }

        if (!found_abba) continue;

        found_abba = false;
        for (const auto& s : ip.hypernet) {
            if (has_abba(s)) {
                found_abba = true;
                break;
            }
        }

        if (!found_abba) {
            ips_tls_support_count++;
        }
    }

    r.part1 = std::to_string(ips_tls_support_count);


    // part 2
    int ips_ssl_support_count = 0;

    for (const auto& ip : ips) {
        std::set<std::array<char, 2>> abs;

        for (const auto& s : ip.normal) {
            find_all_aba(s, abs);
        }

        bool found_bab = false;
        for (const auto& s : ip.hypernet) {
            for (const auto& ab : abs) {
                if (has_matching_bab(s, ab)) {
                    found_bab = true;
                    ips_ssl_support_count++;
                    break;
                }
            }

            if (found_bab) break;
        }
    }

    r.part2 = std::to_string(ips_ssl_support_count);

    return r;
}
