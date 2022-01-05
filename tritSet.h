#ifndef LAB1_TRITSET_H
#define LAB1_TRITSET_H

#include <cmath>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "trit.h"
#include <unordered_map>

#define TRITS_IN_UINT 16

using uint = unsigned int;

class TritSet {
private:
    uint size = 0;
    uint *set;
    uint base_size = 0;
    uint original_size = 0;
    uint false_count = 0;
    uint true_count = 0;
    uint last_trit_ind = 0;

    static uint rightSize(size_t size);
    void changeSize(size_t new_size);
    static void compareSize(TritSet &first, TritSet &second);
    void countTrit(Trit tr, size_t ind_trit);
    void copyData(const TritSet &new_trit);



    class Proxy {
        TritSet *set_proxy;
        size_t ind_proxy;
        Trit trit_proxy;
    public:
        Proxy(size_t ind, TritSet *set);
        Trit operator=(Trit tr);
        operator Trit() const;

        Proxy &operator=(const Proxy &tr);
    };

public:

    class Iterator {
    private:
        TritSet *set_iterator;
        size_t ind_iterator;
    public:
        explicit Iterator(TritSet *set, size_t ind);

        bool operator==(const Iterator &i) const;
        bool operator!=(const Iterator &i) const;

        Iterator operator++();
        Iterator operator--();

        Proxy operator*();
    };

    Iterator begin();
    Iterator end();

    [[nodiscard]] size_t capacity() const;
    [[nodiscard]] uint getCountFalse () const;
    [[nodiscard]] uint getCountTrue () const;
    [[nodiscard]] size_t cardinality (Trit tr) const;
    [[nodiscard]] size_t length() const;

    void trim(size_t last_ind);
    void shrink();

    [[nodiscard]] std::unordered_map<Trit, int, std::hash<int>> cardinality() const;

    Proxy operator[] (size_t ind);
    Trit operator[] (size_t ind) const;
    TritSet operator& (TritSet& tr);
    TritSet operator| (TritSet& tr);
    TritSet operator~ ();


    explicit TritSet(size_t size);
    TritSet(const TritSet &tr);
    TritSet(TritSet &&tr) noexcept;
    ~TritSet();
};

#endif //LAB1_TRITSET_H
