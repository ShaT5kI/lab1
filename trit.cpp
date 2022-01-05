#include "trit.h"

Trit operator|(Trit t1, Trit t2) {
    if (t1 == False && t2 == False)
        return False;
    if (t1 == True || t2 == True)
        return True;
    return Unknown;
}

Trit operator&(Trit t1, Trit t2){
    if (t1 == False || t2 == False)
        return False;
    if (t1 == True && t2 == True)
        return True;
    return Unknown;
}

Trit operator~(Trit t) {
    if (t == False)
        return True;
    if (t == True)
        return False;
    return Unknown;
}

