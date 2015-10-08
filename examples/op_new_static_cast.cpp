/*
Important ast snippet:
======================

|   |   `-CXXStaticCastExpr 0x1a8d550 <col:15, col:52> 'int *' static_cast<int *> <BitCast>
|   |     `-CallExpr 0x1a8d4f0 <col:33, col:51> 'void *'
|   |       |-ImplicitCastExpr 0x1a8d4d8 <col:33, col:44> 'void *(*)(unsigned long) throw(class std::bad_alloc)' <FunctionToPointerDecay>
|   |       | `-DeclRefExpr 0x1a8d420 <col:33, col:44> 'void *(unsigned long) throw(class std::bad_alloc)' lvalue Function 0x1a8cf20 'operator new' 'void *(unsigned long) throw(class std::bad_alloc)'
|   |       `-ImplicitCastExpr 0x1a8d520 <col:48> 'unsigned long' <IntegralCast>
|   |         `-IntegerLiteral 0x1a8d458 <col:48> 'int' 100
*/
int main(void) {
    int* pT = static_cast<int*>(::operator new(100));
    // expected:
    // int* pT = TYPEGRIND_LOG_ALLOC("int*", "op_new.cpp:13:15", static_cast<int*>(::operator new(100)), 100);
    return 0;
}
