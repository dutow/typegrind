//
// Created by dutow on 10/8/15.
//

#ifndef TYPEGRIND_LOG_COUT_H
#define TYPEGRIND_LOG_COUT_H

#include <iostream>
#include <memory>

#define TYPEGRIND_LOG_ALLOC(typeStr, locationStr, newExpression, typeSize) typegrind::log_cout::alloc(typeStr, locationStr, (newExpression), typeSize, 1, "Singular")
#define TYPEGRIND_LOG_ALLOC_ARRAY(typeStr, locationStr, newExpression, typeSize, arraySize) typegrind::log_cout::alloc(typeStr, locationStr, (newExpression), typeSize,  arraySize, "Array")
#define TYPEGRIND_LOG_OP_NEW(typeStr, locationStr, newExpression, typeSize, size) typegrind::log_cout::alloc(typeStr, locationStr, (newExpression), typeSize, size/typeSize, "OpNew")
#define TYPEGRIND_LOG_DEALLOC(pointerAddr, locationStr, deleteExpression) deleteExpression; std::cout << "Deleting singular memory at " << pointerAddr << " from " << locationStr << std::endl;
#define TYPEGRIND_LOG_DEALLOC_ARRAY(pointerAddr, locationStr, deleteExpression) deleteExpression; std::cout << "Deleting array memory at " << pointerAddr << " from " << locationStr << std::endl;
#define TYPEGRIND_LOG_OP_DELETE(pointerAddr, locationStr, deleteExpression) deleteExpression; std::cout << "Deleting memory with operator delete at " << pointerAddr << " from " << locationStr << std::endl;

namespace typegrind
{
    namespace log_cout
    {
        template<typename T>
        T* alloc(const char* typeStr, const char* locationStr, T* ptr, size_t typeSize, size_t count, const char* allocType)
        {
            std::cout << allocType << " allocation '" << typeStr << "' from " << locationStr << " at " << ptr << " with size " << typeSize << "*" << count << std::endl;
            return ptr;
        }
    }
}

#endif //TYPEGRIND_LOG_COUT_H
