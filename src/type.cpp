/*************************************************************************
	> File Name: type.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri 15 Mar 2019 02:58:36 AM UTC
 ************************************************************************/

#include "type.h"

namespace SC
{
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
} // namespace SC
