#ifndef LAB1_TRIT_H
#define LAB1_TRIT_H

enum Trit{
    Unknown,
    False,
    True
};

Trit operator&(Trit t1, Trit t2);
Trit operator|(Trit t1, Trit t2);
Trit operator~(Trit t);

#endif //LAB1_TRIT_H
