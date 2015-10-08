/*
Important AST snippet:
======================

|-FunctionTemplateDecl 0x2979880 </home/dutow/tphp/typegrind/examples/default_new_in_template.cpp:9:1, line:12:1> line:10:4 allocateSomething
| |-TemplateTypeParmDecl 0x2979650 <line:9:11, col:20> col:20 referenced typename T
| |-FunctionDecl 0x29797e0 <line:10:1, line:12:1> line:10:4 allocateSomething 'T *(void)'
| | `-CompoundStmt 0x2979978 <col:24, line:12:1>
| |   `-ReturnStmt 0x2979958 <line:11:5, col:18>
| |     `-CXXNewExpr 0x2979900 <col:12, col:18> 'T *'
| |       `-ParenListExpr 0x29798c8 <col:17, col:18> 'NULL TYPE'
| `-FunctionDecl 0x29b91a0 <line:10:1, line:12:1> line:10:4 used allocateSomething 'double *(void)'
|   |-TemplateArgument type 'double'
|   `-CompoundStmt 0x29b9b88 <col:24, line:12:1>
|     `-ReturnStmt 0x29b9b68 <line:11:5, col:18>
|       `-CXXNewExpr 0x29b9b10 <col:12, col:18> 'double *'
|         `-ImplicitValueInitExpr 0x29b9b00 <<invalid sloc>> 'double':'double'

Notes:
======

Be careful with the (automatic) specialization!!!
*/

template <typename T>
T* allocateSomething() {
    return new T();
    // expected:
    // return TYPEGRIND_LOG_ALLOC("???", "default_new_in_template.cpp:X:Y", new T(), sizeof(T));
}


int main(void) {
    double* dp = allocateSomething<double>();
    return 0;
}

