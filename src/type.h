/*************************************************************************
	> File Name: type.h
	> Author: 
	> Mail: 
	> Created Time: Mon 04 Mar 2019 08:44:36 AM UTC
 ************************************************************************/

#ifndef _TYPE_H
#define _TYPE_H

#include <stddef.h>
#include <stdint.h>


namespace SC
{
/**
 * type code 
 */
enum class TypeCode : uint8_t
{
    INT = 0,
    UINT = 1,
    FLOAT = 2,
    HANDLER = 3 // include pointer and reference
};


/**
 * base type, represent a internel type
 */
struct TypeBase {

    TypeBase(TypeCode typecode, uint8_t bits, uint16_t length=1):
        code(typecode), bits(bits), length(length)
    {}

    TypeBase():code(TypeCode::INT), bits(0), length(0){}

    size_t bytes() const { return (bits+7)/8; }

    bool operator==(const TypeBase& o) const 
    {
        return (code == o.code && bits == o.bits && length == o.length);
    }


    TypeCode code;
    uint8_t bits;
    uint16_t length;
};


/**
 * get TypeBase
 */
template <typename T>
struct TypeOf;

template <typename T>
struct TypeOf<T*>
{
    static TypeBase type;
};
template <typename T>
struct TypeOf<T&>
{
    static TypeBase type;
};
template <typename T>
struct TypeOf<T&&>
{
    static TypeBase type;
};


template <>
struct TypeOf<uint8_t>
{
    static TypeBase type;
};

template<>
struct TypeOf<uint16_t>
{
    static TypeBase type;
};
template<>
struct TypeOf<uint32_t>
{
    static TypeBase type;
};
template<>
struct TypeOf<uint64_t>
{
    static TypeBase type;
};
template<>
struct TypeOf<int8_t>
{
    static TypeBase type;
};
template<>
struct TypeOf<int16_t>
{
    static TypeBase type;
};
template<>
struct TypeOf<int32_t>
{
    static TypeBase type;
};
template<>
struct TypeOf<int64_t>
{
    static TypeBase type;
};
template<>
struct TypeOf<float>
{
    static TypeBase type;
};
template<>
struct TypeOf<double>
{
    static TypeBase type;
};

template <typename T>
TypeBase TypeOf<T*>::type{TypeCode::HANDLER, 64};
template <typename T>
TypeBase TypeOf<T&>::type{TypeCode::HANDLER, 64};
template <typename T>
TypeBase TypeOf<T&&>::type{TypeCode::HANDLER, 64};

TypeBase TypeOf<uint8_t>::type{TypeCode::UINT, 8};
TypeBase TypeOf<uint16_t>::type{TypeCode::UINT, 16};
TypeBase TypeOf<uint32_t>::type{TypeCode::UINT, 32};
TypeBase TypeOf<uint64_t>::type{TypeCode::UINT, 64};
TypeBase TypeOf<int8_t>::type{TypeCode::INT, 8};
TypeBase TypeOf<int16_t>::type{TypeCode::INT, 16};
TypeBase TypeOf<int32_t>::type{TypeCode::INT, 32};
TypeBase TypeOf<int64_t>::type{TypeCode::INT, 64};
TypeBase TypeOf<float>::type{TypeCode::FLOAT, 32};
TypeBase TypeOf<double>::type{TypeCode::FLOAT, 64};

class DataType
{
private:
    TypeBase type;

public:

    DataType(TypeCode code, uint8_t bits, uint16_t length)
        : type(code, bits, length)
    {}
    
    DataType(const DataType& o)
        : type(o.type)
    {}

    size_t bits() const { return type.bits; }
    size_t bytes() const { return type.bytes(); }
    TypeCode getTypeCode() const { return type.code; }

    bool isInt() const 
    { 
        return (type.code == TypeCode::INT) && 
            (bits() == 8 || bits() == 16 || bits() == 32 || bits() == 64); 
    }
    bool isUint() const 
    { 
        return (type.code == TypeCode::UINT) && 
            (bits() == 8 || bits() == 16 || bits() == 32 || bits() == 64); 
    }
    bool isFloat() const 
    { 
        return (type.code == TypeCode::FLOAT) && (bits() == 32 || bits() == 64);
    }
    bool isHandler() const { return (type.code == TypeCode::HANDLER && bits() == 64); }
    bool isScalar() const { return type.length == 1; }

    static DataType makeInt(uint8_t bits, uint16_t length=1) 
    { 
        return DataType(TypeCode::INT, bits, length); 
    }
    static DataType makeUInt(uint8_t bits, uint16_t length=1) 
    { 
        return DataType(TypeCode::UINT, bits, length); 
    }
    static DataType makeFloat(uint8_t bits, uint16_t length=1)
    {
        return DataType(TypeCode::FLOAT, bits, length);
    }

};

} // SC


#endif // _TYPE_H
