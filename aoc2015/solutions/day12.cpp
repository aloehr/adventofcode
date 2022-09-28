#include <iostream>
#include <string>
#include <vector>

#include "aoch/AOCSolutionTypes.hpp"

enum class ObjectType {Array, Object, Number};

struct AbstractObject {
    ObjectType m_type;

    AbstractObject(ObjectType t) : m_type(t) {}

    virtual int count() const = 0;
    virtual int count2() const = 0;

    virtual ~AbstractObject() {}
};

struct Container : public AbstractObject {

    Container *m_parent;
    std::vector<AbstractObject *> m_items;
    bool m_has_red = false;

    Container(Container *p, ObjectType t) : AbstractObject(t), m_parent(p) {}

    void set_has_red(bool r) {
        m_has_red = r;
    }

    void add(AbstractObject *o) {
        m_items.push_back(o);
    }

    int count() const {
        int total = 0;

        for (const auto& o : m_items) {
            total += o->count();
        }

        return total;
    }

    int count2() const {
        int total = 0;

        if (m_type == ObjectType::Object && m_has_red) {
            return 0;
        } else {
            for (const auto& o : m_items) {
                total += o->count2();
            }
        }

        return total;
    }

    ~Container() {
        for (auto p : m_items) {
            delete p;
        }
    }
};

struct Number : public AbstractObject {
    int m_value;

    Number(int v) : AbstractObject(ObjectType::Number), m_value(v) {}

    int count() const {
        return m_value;
    }

    int count2() const {
        return count();
    }
};

aoch::Result solve_day12(aoch::Input& in) {
    aoch::Result r;

    Container *root = new Container(nullptr, ObjectType::Array);
    Container *current = root;

    std::string data = in[0];

    size_t i = 0;

    while (i < data.size()) {
        if (data[i] == '[' || data[i] == '{') {
            Container *next = new Container(current, data[i] == '[' ? ObjectType::Array : ObjectType::Object);

            current->add(next);
            current = next;

            i++;
        } else if (data[i] == ']' || data[i] == '}') {
            current = current->m_parent;
            i++;
        } else if (data[i] == ',') {
            i++;
        } else {
            if (current->m_type == ObjectType::Object) {
                size_t key_end = data.find(':', i);
                i = key_end + 1;
            }

            if (data[i] != '[' && data[i] != '{') {
                if (data[i] == '"' && current->m_type == ObjectType::Object) {
                    if (i+3 < data.size() && data.substr(i+1, 3) == "red") {
                        current->set_has_red(true);
                    }
                }

                if (data[i] == '"') {
                    size_t val_end = data.find('"', i+1);
                    i = val_end + 1;
                } else {
                    size_t after_number = data.find_first_of(",]}", i);
                    int val = std::stoi(data.substr(i, i - after_number));
                    current->add(new Number(val));

                    i = after_number;
                }
            }
        }
    }

    r.part1 = std::to_string(root->count());
    r.part2 = std::to_string(root->count2());

    delete root;

    return r;
}
