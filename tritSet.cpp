#include "tritSet.h"

uint TritSet::rightSize(size_t size) {
    return (uint) ceil((double) size / TRITS_IN_UINT);
}

void TritSet::changeSize(size_t new_size) {
    this->size = new_size;
    uint now_size = rightSize(new_size);
    uint *tmp;

    if (this->base_size != 0)
        tmp = new uint[now_size + 1];
    else
        tmp = nullptr;

    memset(tmp, 0, now_size + 1);
    memmove(tmp, this->set, this->base_size * sizeof(uint));

    delete[] this->set;
    this->set = tmp;
    this->base_size = rightSize(new_size) + 1;
}

void TritSet::compareSize(TritSet& first, TritSet& second) {
    if (first.size > second.size)
        second.changeSize(first.size);
    else if (second.size > first.size)
        first.changeSize(second.size);

}

void TritSet::countTrit (Trit tr, size_t ind_trit) {
    if (tr == True) {
        this->true_count++;
        this->last_trit_ind = ind_trit;
    }
    if (tr == False) {
        this->false_count++;
        this->last_trit_ind = ind_trit;
    }
}

void TritSet::copyData (const TritSet& tr) {
    this->size = tr.size;
    this->base_size = tr.base_size;
    this->original_size = tr.original_size;
    this->false_count = tr.false_count;
    this->true_count = tr.true_count;
    this->last_trit_ind = tr.last_trit_ind;
}

size_t TritSet::capacity() const {
    return this->size;
}

uint TritSet::getCountFalse() const{
    return this->false_count;
}

uint TritSet::getCountTrue() const {
    return this->true_count;
}

size_t TritSet::length() const{
    return this->last_trit_ind + 1;
}

size_t TritSet::cardinality(Trit tr) const {
    if (tr == True)
        return this->true_count;
    if (tr == False)
        return this->false_count;
    return this->size - (this->true_count + this->false_count);
}

TritSet::Proxy::Proxy(size_t ind, TritSet *set) {
    this->set_proxy = set;
    this->ind_proxy = ind;

    uint set_byte = ind / TRITS_IN_UINT;

    if (ind <= set->capacity()) {
        size_t bit_ind = 2 * (TRITS_IN_UINT - (ind % TRITS_IN_UINT) - 1);

        uint bit1 = set_proxy->set[set_byte] & ((uint) 1 << bit_ind);
        uint bit2 = set_proxy->set[set_byte] & ((uint) 1 << (bit_ind + 1));

        this->trit_proxy = Trit((bit1 + bit2) >> bit_ind);
    } else
        this->trit_proxy = Unknown;

}

TritSet::Proxy TritSet::operator[](size_t ind) {
    return {ind, this};
}

Trit TritSet::operator[](size_t ind) const {
    uint set_byte = ind / TRITS_IN_UINT;

    size_t ind_bit = 2 * (TRITS_IN_UINT - (ind % TRITS_IN_UINT) - 1);
    uint bit1 = this->set[set_byte] & ((uint)1 << ind_bit);
    uint bit2 = this->set[set_byte] & ((uint)1 << (ind_bit + 1));

    return Trit((bit1 + bit2) >> ind_bit);
}

TritSet TritSet::operator&(TritSet &tr) {
    compareSize(tr, *this);

    TritSet result(tr.size);
    Trit tr1, tr2;

    for (uint i = 0; i < tr.size; i++) {
        tr1 = (*this)[i];
        tr2 = tr[i];

        result[i] = tr1 & tr2;
        countTrit(result[i], i);
    }

    return result;
}

TritSet TritSet::operator|(TritSet &tr) {

    compareSize(tr, *this);

    TritSet result(tr.size);
    Trit tr1, tr2;

    for (uint i = 0; i < tr.size; i++) {
        tr1 = (*this)[i];
        tr2 = tr[i];

        result[i] = tr1 | tr2;
        countTrit(result[i], i);
    }

    return result;
}

TritSet::Proxy::operator Trit() const {
    return this->trit_proxy;
}

Trit TritSet::Proxy::operator=(Trit tr) {
    if (this->ind_proxy > this->set_proxy->capacity() && tr != Unknown)
        this->set_proxy->changeSize(this->ind_proxy);


    if (this->ind_proxy <= this->set_proxy->capacity()) {
        uint trit_position = 2 * (TRITS_IN_UINT - (this->ind_proxy % TRITS_IN_UINT) - 1);

        //
        this->set_proxy->set[ind_proxy / TRITS_IN_UINT] =
                this->set_proxy->set[ind_proxy / TRITS_IN_UINT] & (~(3 << trit_position)) | ((uint)tr << trit_position);
    }

    this->set_proxy->countTrit(tr, ind_proxy);
    return tr;
}

TritSet TritSet::operator~() {
    TritSet result (this->size);
    Trit tr;

    for (uint i = 0; i < result.size; i++) {
        tr = (*this)[i];
        result[i] = ~tr;
        countTrit(result[i], i);
    }

    return result;
}

TritSet::Proxy &TritSet::Proxy::operator=(const TritSet::Proxy &tr) {
    if (this == &tr)
        return *this;

    if (this->ind_proxy > this->set_proxy->capacity() && tr.trit_proxy != Unknown)
        this->set_proxy->changeSize(this->ind_proxy);


    if (this->ind_proxy <= this->set_proxy->capacity()) {

        uint trit_position = 2 * (TRITS_IN_UINT - (this->ind_proxy % TRITS_IN_UINT) - 1);

        this->set_proxy->set[ind_proxy / TRITS_IN_UINT] = this->set_proxy->set[ind_proxy / TRITS_IN_UINT] &
                (~(3 << trit_position)) | ((uint) tr.trit_proxy << trit_position);
    }

    this->set_proxy->countTrit(tr.trit_proxy, ind_proxy);
    return *this;
}


TritSet::Iterator::Iterator(TritSet* set, size_t ind) {
    this->set_iterator = set;
    this->ind_iterator = ind;
}

TritSet::Iterator TritSet::begin() {
    return TritSet::Iterator(this, 0);
}

TritSet::Iterator TritSet::end() {
    return TritSet::Iterator(this, this->size);
}


TritSet::Iterator TritSet::Iterator::operator++() {
    this->ind_iterator ++;

    return TritSet::Iterator(this->set_iterator, ind_iterator - 1);
}

TritSet::Iterator TritSet::Iterator::operator--() {
    this->ind_iterator --;

    return TritSet::Iterator(this->set_iterator , ind_iterator + 1);
}

bool TritSet::Iterator::operator==(const TritSet::Iterator &i) const {
    if (this->set_iterator == i.set_iterator && this->ind_iterator == i.ind_iterator)
        return true;

    return false;
}

bool TritSet::Iterator::operator!=(const TritSet::Iterator &i) const {
    if (*this == i)
        return false;

    return true;
}

TritSet::Proxy TritSet::Iterator::operator*() {
    return (*this->set_iterator)[ind_iterator];
}


void TritSet::shrink() {
    this->base_size = rightSize(this->original_size);
    this->size = this->original_size;
    uint *tmp;

    if (this->base_size != 0)
        tmp = new uint[this->base_size];
    else
        tmp = nullptr;

    memset(tmp, 0, this->base_size);
    memmove(tmp, this->set, this->base_size * sizeof(uint));
    delete[] this->set;
    this->set = tmp;
}

void TritSet::trim(size_t last_ind) {
    for (size_t i = last_ind; i < this->capacity(); i++)
        (*this)[i] = Unknown;
}


std::unordered_map<Trit, int, std::hash<int>> TritSet::cardinality() const {
    std::unordered_map<Trit, int, std::hash<int>> res;

    res[True] = getCountTrue();
    res[False] = getCountFalse();
    res[Unknown] = cardinality(Unknown);

    return res;
}


TritSet::TritSet(size_t amount) {
    this->size = amount;
    this->original_size = amount;
    this->base_size = rightSize(amount);
    this->false_count = this->true_count = 0;
    this->last_trit_ind = -1;

    if (this->base_size == 0)
        this->set = nullptr;
    else {
        this->set = new uint[this->base_size];
        memset(this->set, 0, this->base_size);
    }
}

TritSet::TritSet(const TritSet &tr) {
    this->copyData(tr);
    this->set = new uint[base_size];
    memmove (this->set, tr.set, base_size * sizeof(uint));
}

TritSet::TritSet(TritSet &&tr) noexcept {
    this->copyData(tr);

    this->set = tr.set;
    tr.set = nullptr;
}

TritSet::~TritSet() {
    delete[] this->set;
}