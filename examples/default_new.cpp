/*
Important AST snippet:
======================

|   |   `-CXXNewExpr 0x139d0b0 <col:15, col:25> 'int *'
|   |     `-IntegerLiteral 0x135d8a8 <col:23> 'int' 53
*/


int main(void) {
    int* pT = new int(53);
    // expected:
    // int* pT = TYPEGRIND_LOG_ALLOC("int*", "default_new.cpp:9:15", new int(53), sizeof(int));
    return 0;
}
