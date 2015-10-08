/*
Important AST snippet:
======================

    |   `-CXXNewExpr 0x320bfc0 <col:18, col:31> 'class ClName *'
    |     |-IntegerLiteral 0x320bc48 <col:29> 'int' 53
    |     `-CXXConstructExpr 0x320bf90 <col:22> 'class ClName' 'void (void)'

Notes
=====

CXXNewExpr::isArray == true
*/

int g;

class ClParent {
public:
    virtual ~ClParent(){}
};

class ClName : public ClParent {
    int a;
    char b;
    short c;

public: 
    ClName() : a(0), b('b'), c(1) {}
    ~ClName() { g++; }
};

int main(void) {
    ClName* pT = new ClName[53];
    // expected
    // int* pT = TYPEGRIND_LOG_ARRAY_ALLOC("ClName*", "default_new_array.cpp:X:Y", new ClName[53], sizeof(ClName), 53);
    return 0;
}
