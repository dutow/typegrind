/*
Important AST snippet:
======================

|-FunctionTemplateDecl 0x3191880 </home/dutow/tphp/typegrind/examples/default_new_in_template_spec.cpp:25:1, line:30:1> line:26:4 allocateSomething
| |-TemplateTypeParmDecl 0x3191650 <line:25:11, col:20> col:20 referenced typename T
| |-FunctionDecl 0x31917e0 <line:26:1, line:30:1> line:26:4 allocateSomething 'T *(void)'
| | `-CompoundStmt 0x3191978 <col:24, line:30:1>
| |   `-ReturnStmt 0x3191958 <line:27:5, col:18>
| |     `-CXXNewExpr 0x3191900 <col:12, col:18> 'T *'
| |       `-ParenListExpr 0x31918c8 <col:17, col:18> 'NULL TYPE'
| `-Function 0x31d1090 'allocateSomething' 'double *(void)'
|-FunctionDecl 0x31d1090 prev 0x31d12e0 <line:32:1, line:37:1> line:33:9 used allocateSomething 'double *(void)'
| |-TemplateArgument type 'double'
| `-CompoundStmt 0x31d1b88 <col:29, line:37:1>
|   `-ReturnStmt 0x31d1b68 <line:34:5, col:27>
|     `-CXXNewExpr 0x31d1b10 <col:12, col:27> 'double *'
|       `-FloatingLiteral 0x31d1420 <col:23> 'double' 3.660000e+00

Notes:
======

Be careful with the (non-automatic) specialization!!!
*/

template <typename T>
T* allocateSomething() {
    return new T();
    // expected:
    // return TYPEGRIND_LOG_ALLOC("???", "default_new_in_template.cpp:X:Y", new T(), sizeof(T));
}

template <>
double* allocateSomething() {
    return new double(3.66);
    // expected:
    // return TYPEGRIND_LOG_ALLOC("???", "default_new_in_template.cpp:X:Y", new T(), sizeof(T));
}


int main(void) {
    double* dp = allocateSomething<double>();
    return 0;
}

