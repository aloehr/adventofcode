#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"


std::string fill(const std::string& data, size_t length) {
    std::string res(length, 'X');

    size_t data_end_ptr = data.size() - 1;
    size_t write_ptr = data.size();

    for (size_t i = 0; i < data.size(); ++i) {
        res[i] = data[i];
    }

    while(write_ptr < res.size()) {
        res[write_ptr++] = '0';

        for (size_t i = 0; i <= data_end_ptr; ++i) {
            if (write_ptr >= res.size()) break;
            if (res[data_end_ptr - i] == '0') {
                res[write_ptr++] = '1';
            } else {
                res[write_ptr++] = '0';
            }
        }

        data_end_ptr = write_ptr - 1;
    }

    return res;
}

std::string checksum(const std::string& data) {
    size_t chunk_size = 1;
    size_t checksum_size = data.size();

    while (checksum_size % 2 == 0) {
        chunk_size *= 2;
        checksum_size /= 2;
    }

    std::string ret(checksum_size, 'X');

    for (size_t i = 0; i < ret.size(); ++i) {
        int c = 0;

        for (size_t j = i * chunk_size; j < (i + 1) * chunk_size; ++j) {
            if (data[j] == '1') c++;
        }

        if (c % 2 == 0) {
            ret[i] = '1';
        } else {
            ret[i] = '0';
        }
    }

    return ret;
}

aoch::Result solve_day16(aoch::Input& in) {
    aoch::Result r;

    size_t disk_size = 272;
    std::string disk = fill(in[0], disk_size);

    r.part1 = checksum(disk);

    disk_size = 35651584;
    disk = fill(in[0], disk_size);

    r.part2 = checksum(disk);

    return r;
}
