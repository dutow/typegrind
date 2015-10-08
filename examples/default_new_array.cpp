/*
Important AST snippet:
======================

|   |   `-CXXNewExpr 0x1f811c0 <col:15, col:25> 'int *'
|   |     `-IntegerLiteral 0x1f418a8 <col:23> 'int' 53

Notes
=====

CXXNewExpr::isArray == true
*/

int main(void) {
    int* pT = new int[53];
    // expected
    // int* pT = TYPEGRIND_LOG_ARRAY_ALLOC("int*", "default_new_array.cpp:X:Y", new int[53], sizeof(int), 53);
    return 0;
}
