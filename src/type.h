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


class DataType
{
private:
    TypeBase type;

public:

    DataType() : 
        type(TypeCode::INT, 0, 0)
    {}

    DataType(TypeCode code, uint8_t bits, uint16_t length) : 
        type(code, bits, length)
    {}
    
    DataType(const DataType& o) : 
        type(o.type)
    {}

    bool operator==(const DataType& o) const
    {
        return (this->type == o.type);
    }

    size_t bits() const { return type.bits; }
    size_t bytes() const { return type.bytes(); }
    size_t length() const { return type.length; }

    TypeCode getTypeCode() const { return type.code; }

    bool defined() const { 
        return (type.bits != 0 && type.length != 0);
    }

    bool isBool() const
    {
        return (type.code == TypeCode::UINT) && (bits() == 1);
    }
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


};


inline DataType Int(uint8_t bits, uint16_t length=1)
{
    return DataType(TypeCode::INT, bits, length);
}
inline DataType UInt(uint8_t bits, uint16_t length=1)
{
    return DataType(TypeCode::UINT, bits, length);
}
inline DataType Float(uint8_t bits, uint16_t length=1)
{
    return DataType(TypeCode::FLOAT, bits, length);
}
inline DataType Handler(uint8_t bits, uint16_t length=1)
{
    return DataType(TypeCode::HANDLER, bits, length);
}
inline DataType Bool(uint16_t length=1)
{
    return DataType(TypeCode::UINT, 1, length);
}


} // SC


#endif // _TYPE_H
