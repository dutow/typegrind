/*
FAILING:
========

This example currently fails, because typegrind inserts it's decoration before and after the macro - encapsulating the entire expression, not just new.

To handle this, we have to generate sources with new-related macros expanded, and decorating new expressions in that source. 

(Okay, it could be hacked to work in this case, but what if the statement consists of two idiotically placed macros, or other strange constructs?)

Fortunately, this isn't a typical usecase, so it's a known limitation for now.

Important AST snippet:
======================

TODO
*/

#define NAMEDVAR_NEW(n, v) int* n = new int(v);

int main(void) {
    NAMEDVAR_NEW(i5, 13);
    // expected:
    // TODO
    return 0;
}
