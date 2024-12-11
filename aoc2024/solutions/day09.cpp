#include <array>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>

#include <aoch/AOCSolutionTypes.hpp>


enum class BlockType : char{
    FREE_SPACE,
    FILE
};

struct DiskBlock {
    int start_idx;
    int file_id;
    char size;
    BlockType type;

    DiskBlock(size_t idx, size_t s, size_t f_id, BlockType t) :
        start_idx(idx), file_id(f_id), size(s),  type(t) {}

    std::string to_string() const {

        std::string ret = "{type: ";

        if (type == BlockType::FREE_SPACE) ret += "FREE_SPACE";
        else ret += "FILE";

        ret += " start_idx: " + std::to_string(start_idx);

        if (type == BlockType::FILE)
            ret += " file_id: " + std::to_string(file_id);

        ret += " size: " + std::to_string(size) + "}";

        return ret;
    }
};

inline int sum_to_n(int n) {
    return (n * n + n) / 2;
}

long long calc_checksum(std::vector<DiskBlock>& file_blocks) {
    long long checksum = 0;

    for (size_t i = 0; i < file_blocks.size(); ++i) {
        const DiskBlock& cur = file_blocks[i];
        checksum += (static_cast<long long>(cur.size) * cur.start_idx + sum_to_n(cur.size - 1)) * cur.file_id;
    }

    return checksum;
}

long long calc_checksum_p1(const std::string& mem) {
    long long checksum = 0;

    size_t back_idx = (mem.size() - 1) & (~1);
    int back_size = mem[back_idx] - '0';
    size_t cur_disk_start_idx = 0;

    for (size_t i = 0; i < mem.size(); ++i) {
        if (i > back_idx) break;
        if (mem[i] == '0') continue;

        if (i == back_idx) {
            if (back_size) checksum += i / 2 * (back_size  * cur_disk_start_idx + sum_to_n(back_size - 1));
            break;
        }
        if (i % 2 == 0) {
            long long size = mem[i] - '0';
            checksum += i / 2 * (size  * cur_disk_start_idx + sum_to_n(size - 1));
            cur_disk_start_idx += size;
        } else {
            int space = mem[i] - '0';

            while (space) {
                if (back_size == 0) {
                    back_idx -= 2;
                    back_size = mem[back_idx] - '0';
                }

                if (back_idx < i) {
                    break;
                }

                if (back_size < space) {
                    checksum += back_idx / 2 * (back_size  * cur_disk_start_idx + sum_to_n(back_size - 1));
                    cur_disk_start_idx += back_size;
                    space -= back_size;
                    back_size = 0;
                } else if (back_size > space){
                    checksum += back_idx / 2 * (space  * cur_disk_start_idx + sum_to_n(space - 1));
                    cur_disk_start_idx += space;
                    back_size -= space;
                    space = 0;
                } else {
                    checksum += back_idx / 2 * (back_size  * cur_disk_start_idx + sum_to_n(back_size - 1));
                    cur_disk_start_idx += space;
                    space = 0;
                    back_size = 0;
                }
            }
        }
    }

    return checksum;
}

aoch::Result solve_day09(aoch::Input& in) {
    aoch::Result a;

    a.part1 = std::to_string(calc_checksum_p1(in[0]));


    // part 2
    std::vector<DiskBlock> free_space_blocks;
    free_space_blocks.reserve((in[0].size() + 1 ) / 2);

    std::vector<DiskBlock> file_blocks;
    file_blocks.reserve((in[0].size() + 1) / 2);

    size_t cur_disk_start_idx = 0;

    for (size_t i = 0; i < in[0].size(); i++) {

        if (in[0][i] < '1' || in[0][i] > '9') continue;

        const int block_size = in[0][i] - '0';
        const bool is_file_block = i % 2 == 0;

        if (is_file_block) {
            file_blocks.emplace_back(
                cur_disk_start_idx,
                block_size,
                i >> 1,
                BlockType::FILE
            );
        } else {
            free_space_blocks.emplace_back(
                cur_disk_start_idx,
                block_size,
                0,
                BlockType::FREE_SPACE
            );
        }

        cur_disk_start_idx += block_size;
    }

    using pq_item = std::pair<int, int>;
    std::array<std::priority_queue<pq_item, std::vector<pq_item>, std::greater<>>, 10> free_space_blocks_segments;

    for (size_t i = 0; i < free_space_blocks.size(); ++i) {
        free_space_blocks_segments[free_space_blocks[i].size].emplace(free_space_blocks[i].start_idx, i);
    }

    for (auto cur_fb = file_blocks.rbegin(); cur_fb != file_blocks.rend(); ++cur_fb) {
        size_t fsbs_idx = free_space_blocks_segments.size();
        size_t selected_fsb_idx = 0;
        for (size_t i = cur_fb->size; i < free_space_blocks_segments.size(); ++i) {
            if (free_space_blocks_segments[i].empty()) continue;

            if (fsbs_idx == free_space_blocks_segments.size() ||
                free_space_blocks_segments[i].top().first < free_space_blocks_segments[fsbs_idx].top().first
            ) {
                fsbs_idx = i;
                selected_fsb_idx = free_space_blocks_segments[i].top().second;
            }

        }

        if (fsbs_idx == free_space_blocks_segments.size() || free_space_blocks[selected_fsb_idx].start_idx > cur_fb->start_idx) {
            continue;
        }

        free_space_blocks_segments[fsbs_idx].pop();

        DiskBlock& cur_fsb = free_space_blocks[selected_fsb_idx];

        if (cur_fb->size == cur_fsb.size) {
            cur_fb->start_idx = cur_fsb.start_idx;
        } else {
            cur_fb->start_idx = cur_fsb.start_idx;
            cur_fsb.start_idx += cur_fb->size;
            cur_fsb.size -= cur_fb->size;

            free_space_blocks_segments[cur_fsb.size].emplace(cur_fsb.start_idx, selected_fsb_idx);
        }
    }

    a.part2 = std::to_string(calc_checksum(file_blocks));

    return a;
}
