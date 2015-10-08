/*
Important AST snippet:
======================

    |     `-CXXNewExpr 0x236fc28 <col:20, col:35> 'class ClName *'
    |       `-CXXConstructExpr 0x236fbe8 <col:24, col:35> 'class ClName' 'void (int, short)'
    |         |-IntegerLiteral 0x236f838 <col:31> 'int' 5
    |         `-ImplicitCastExpr 0x236fbd0 <col:34> 'short' <IntegralCast>
    |           `-IntegerLiteral 0x236f858 <col:34> 'int' 2
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
    ClName(int a, short c) : a(a), b('b'), c(c) {}
    ~ClName() { g++; }
};

int main(void) {
    ClParent* pT = new ClName(5, 2);
    // expected:
    // ClParent* pT = TYPEGRIND_LOG_ALLOC("ClName*", "default_new_class.cpp:30:20", new ClName(5, 2), sizeof(ClName));
    return 0;
}

