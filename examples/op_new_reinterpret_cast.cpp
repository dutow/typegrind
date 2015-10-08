/*
Important ast snippet:
======================

|   |   `-CXXReinterpretCastExpr 0x18a2120 <col:15, col:57> 'int *' reinterpret_cast<int *> <BitCast>
|   |     `-CallExpr 0x18a20c0 <col:38, col:56> 'void *'
|   |       |-ImplicitCastExpr 0x18a20a8 <col:38, col:49> 'void *(*)(unsigned long) throw(class std::bad_alloc)' <FunctionToPointerDecay>
|   |       | `-DeclRefExpr 0x18a1ff0 <col:38, col:49> 'void *(unsigned long) throw(class std::bad_alloc)' lvalue Function 0x18a1af0 'operator new' 'void *(unsigned long) throw(class std::bad_alloc)'
|   |       `-ImplicitCastExpr 0x18a20f0 <col:53> 'unsigned long' <IntegralCast>
|   |         `-IntegerLiteral 0x18a2028 <col:53> 'int' 100
*/
int main(void) {
    int* pT = reinterpret_cast<int*>(::operator new(100));
    // expected:
    // int* pT = TYPEGRIND_LOG_ALLOC("int*", "op_new.cpp:13:15", reinterpret_cast<int*>(::operator new(100)), 100);
    return 0;
}
