#pragma once

#include <algorithm>
#include <cassert>
#include <vector>


template <typename T>
class BucketQueue {
    std::vector<std::vector<T>> buckets;

    // if items are in the queue, top_idx should always point to the bucket
    // of the item with the highest prio
    size_t top_idx;

    // if items are in the queue, min_idx should always point to the bucket
    // of the item with the lowest prio
    size_t min_idx;

    size_t item_count;

    public:

    BucketQueue(size_t bucket_count = 1) {
        this->buckets.resize(std::max(1ul, bucket_count));

        this->top_idx = 0;
        this->min_idx = this->buckets.size() - 1;

        this->item_count = 0;
    }

    size_t size() const {
        return this->item_count;
    }

    void clear() {
        for (size_t i = this->min_idx; i <= this->top_idx; ++i) {
            this->buckets[i].clear();
        }

        this->top_idx = 0;
        this->min_idx = this->buckets.size() - 1;

        this->item_count = 0;
    }

    void resize(size_t bucket_count) {
        if (bucket_count <= this->buckets.size()) return;
        this->buckets.resize(bucket_count);

        if (!this->size()) {
            this->min_idx = this->buckets.size() - 1;
        }
    }

    void insert(const T& item, size_t prio) {
        if (prio >= this->buckets.size()) {
            this->resize(this->buckets.size() * 2);
        }

        this->buckets[prio].push_back(item);
        this->item_count++;

        if (prio > this->top_idx) {
            this->top_idx = prio;
        }

        if (prio < this->min_idx) {
            this->min_idx = prio;
        }
    }

    T top() const {
        assert(item_count && "tried to get top from an empty queue");
        return this->buckets[this->top_idx].back();
    }

    T min() const {
        assert(item_count && "tried to get min from an empty queue");
        return this->buckets[this->min_idx].back();
    }


    void pop_top() {
        this->pop(this->top_idx);
    }

    void pop_min() {
        this->pop(this->min_idx);
    }

    private:

    void pop(const size_t idx) {
        assert(item_count && "queue popped without any items in it");

        this->buckets[idx].pop_back();
        this->item_count--;

        if (this->buckets[idx].size()) return;

        if (!item_count) {
            this->top_idx = 0;
            this->min_idx = this->buckets.size() - 1;
            return;
        }

        while (this->top_idx && !this->buckets[this->top_idx].size()) {
            this->top_idx--;
        }

        while (this->min_idx < (this->buckets.size() - 1) && !this->buckets[this->min_idx].size()) {
            this->min_idx++;
        }
    }
};
