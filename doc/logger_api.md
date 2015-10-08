Typegrind logger API
===

The Typegrind API is a set of well defined macros.


List of macros
---

 * `TYPEGRIND_LOG_ALLOC(typeStr, locationStr, newExpression, typeSize)`
 * `TYPEGRIND_LOG_ALLOC_ARRAY(typeStr, locationStr, newExpression, typeSize, arraySize)`
 * `TYPEGRIND_LOG_OP_NEW(typeStr, locationStr, newExpression, typeSize, size)`
 * `TYPEGRIND_LOG_DEALLOC(pointerAddr, deleteExpression)`
 * `TYPEGRIND_LOG_DEALLOC_ARRAY(pointerAddr, deleteExpression)`
 * `TYPEGRIND_LOG_OP_DELETE(pointerAddr, deleteExpression)`


### Soon TM:

 * `TYPEGRIND_IN_NEW_RETURN(returnExpr)` -- added inside global `operator new`, return TYPEG... style.
 * `TYPEGRIND_IN_MEMBER_NEW_RETURN(returnExpr, classType)` -- added inside member `operator new`, return TYPEG... style.
 * `TYPEGRIND_IN_DELETE_ADDR(pointerAddr)` -- added inside global `operator delete`, just somewhere in the function.
 * `TYPEGRIND_IN_MEMBER_DELETE_ADDR(pointerAddr, classType)` -- added inside member `operator delete`, just somewhere in the function.
 * `TYPEGRIND_OWNER(todo)` -- added into places where std::allocator is used, to log relations. (e.g. this large `int` allocation is for a `vector<int>`). This macro is also useful for manual instrumentation for similar cases.


Macro details
---

### `TYPEGRIND_LOG_ALLOC`

Decorates singular `new` expressions.

Expanded expression must return the result of newExpression.

### `TYPEGRIND_LOG_ALLOC_ARRAY`

Decorates array `new` expression. 

Expanded expression must return the result of newExpression.

### `TYPEGRIND_LOG_OP_NEW`

Decorates `::operator new` calls. If the call is directly inside a `static_cast` or `reinterpret_cast`, the `typeStr` is the type of the cast, otherwise `void*`.

Expanded expression must return the result of newExpression.

### `TYPEGRIND_LOG_DEALLOC`

Decorates singular `delete` expressios.

Expanded expression must call deleteExpression.

### `TYPEGRIND_LOG_DEALLOC_ARRAY`

Decorates array `delete[]` expressios.

Expanded expression must call deleteExpression.

###  `TYPEGRIND_LOG_OP_DELETE`

Decorates `::operator delete` calls.

Expanded expression must call deleteExpression.

Macro parameters
---


### typeStr

The type of the pointer as a string literal. 

For example: `"int*"`


### locationStr

The location of the underlining expression as a string literal.

For example: `"example.cpp:23"`


### newExpression

The original new expression.

For example: `new int(5)`


### typeSize

Size of the requested type.

For example: `sizeof(int)`


### arraySize

Size of the requested array.

For example: `43`


### pointerAddr

The address of the subject pointer

For example: `p` (where `p` is a pointer type variable)


### deleteExpression

The original delete expression.

For example: `delete[] a` 


### returnExpression

The expression after the `return` keyword.

For example: `malloc(size)`


### classType

Type of the class context.

For example: `ClName`


Creating loggers
---

To create a new logger, just implement the API macros in a header.

