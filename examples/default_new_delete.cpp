/*
Important AST snippet:
======================

*/


int main(void) {
    int* pT = new int(53);
    // expected:
    // int* pT = TYPEGRIND_LOG_ALLOC("int*", "default_new.cpp:9:15", new int(53), sizeof(int));
    delete pT;
    return 0;
}
