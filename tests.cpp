#include "tritSet.h"
#include <gtest/gtest.h>

TEST(General, DefaultTest) {

    TritSet set(1000);

    size_t allocLength = set.capacity();
    ASSERT_GE(allocLength, 1000*2 / 8 / sizeof(uint));

    set[1000000000] = Unknown;
    ASSERT_EQ(allocLength, set.capacity());

    if(set[2000000000]==True){}
    ASSERT_EQ(allocLength, set.capacity());

    set[1000000000] = True;
    ASSERT_LT(allocLength, set.capacity());


    allocLength = set.capacity();
    set[1000000000] = Unknown;
    set[1000000] = False;
    ASSERT_EQ(allocLength, set.capacity());

    set.shrink();
    ASSERT_GT(allocLength, set.capacity());

}

TritSet foo(TritSet tritSet) {
    tritSet[0] = True;
    return tritSet;
}

TEST(Constructor, Move) {
    TritSet setA(1000);
    TritSet setB = foo(setA);
    ASSERT_EQ (setA.capacity(), setB.capacity());
}

TEST(ExtremePoints, ZeroCapacity) {
    TritSet set(0);
    ASSERT_EQ (set.capacity(), 0);
}

TEST(Equation, EquationTritSet) {
    TritSet setA(1000);
    TritSet setB(setA);
    ASSERT_EQ(setA.capacity(), setB.capacity());

    size_t allocLength = setB.capacity();
    setA[1000] = setB[1500] = True;
    ASSERT_EQ (setA[1000], True);
    ASSERT_EQ (setB[1500], True);
    ASSERT_LT(allocLength, setB.capacity());
}

TEST(Equation, EquationTrits) {
    TritSet setA(1000);
    TritSet setB(setA);

    setA[10] = True;
    Trit a = setA[10];
    ASSERT_EQ (a, True);

    setB[5] = True;
    setA[5] = False;
    setB[1500] = setA[5];
    ASSERT_EQ (setA[5], False);
    ASSERT_EQ (setB[1500], False);
}

TEST(Logic, And) {
    TritSet A(1000);
    for (size_t i = 0; i < 1000; i++)
        A[i] = True;

    TritSet B(2000);
    for (size_t i = 0; i < 2000; i++)
        B[i] = False;

    TritSet C = A & B;
    ASSERT_EQ (C.cardinality(False), 2000);
    ASSERT_EQ (C.capacity(), B.capacity());
}

TEST(Logic, Or) {
    TritSet A(1000);
    for (size_t i = 0; i < 1000; i++)
        A[i] = False;

    TritSet B(2000);
    for (size_t i = 0; i < 2000; i++)
        B[i] = True;

    TritSet C = A | B;
    ASSERT_EQ (C.cardinality(True), 2000);
    ASSERT_EQ (C.capacity(), A.capacity());
}

TEST(Logic, Neg) {
    TritSet A(1000);
    for (size_t i = 0; i < 1000; i++)
        A[i] = False;

    TritSet B = ~A;
    for (size_t i = 0; i < 1000; i++)
        A[i] = True;

    ASSERT_EQ (B.cardinality(True), 1000);
    ASSERT_EQ (B[10], True);
}

TEST(IteratorTest, Iterator) {
    TritSet set(1000);

    for(auto i : set)
        i = True;

    for (auto i : set)
        ASSERT_EQ (i, True);

    ASSERT_EQ (set[0], True);
    ASSERT_EQ (set[999], True);
}

TEST(Method, Shrink) {
    TritSet set(1000);
    int allocLength = set.capacity();

    set.shrink();
    ASSERT_EQ (allocLength, set.capacity());

    set[1007] = False;
    set[10] = False;
    ASSERT_LT (allocLength, set.capacity());

    set.shrink();

    ASSERT_EQ (allocLength, set.capacity());
    ASSERT_EQ (set[10], False);

    set[10000] = Unknown;
    ASSERT_EQ (allocLength, set.capacity());
}

TEST(Method, AdditionalMemory) {
    TritSet set(1000);
    set[50] = set[100] = False;
    ASSERT_EQ (set.cardinality(False), 2);
    ASSERT_EQ (set.cardinality(Unknown), 998);

    set.trim(100);
    ASSERT_EQ (set[100], Unknown);
    ASSERT_EQ (set[50], False);

    ASSERT_EQ (set.length(), 51);

    set[1000] = True;
    ASSERT_EQ (set.length(), 1001);

    set[10000] = Unknown;
    ASSERT_EQ (set.length(), 1001);
}

