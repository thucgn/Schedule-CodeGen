/*************************************************************************
	> File Name: ConstantFoldPass.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri 29 Mar 2019 08:31:01 AM UTC
 ************************************************************************/

#include "constantfold_pass.h"
#include "util.h"
#include <algorithm>

namespace 
{

inline bool is_const_number(const SC::Expr& e)
{
    return e.is_type<SC::IntImm>() || e.is_type<SC::UIntImm>()
        || e.is_type<SC::FloatImm>();
}



} // namespace 

namespace SC
{

Expr ConstantFoldPass::mutateNode(const Add* n)
{
    Expr new_a = mutate(n->a);
    Expr new_b = mutate(n->b);
    if(is_const_number(new_a) && is_const_number(new_b))
    {
        if(new_a.is_type<SC::IntImm>())
        {
            int64_t v = new_a.cast_to<IntImm>()->value + 
                new_b.cast_to<IntImm>()->value;
            return IntImm::make(new_a->data_type, v);
        }
        else if(new_a.is_type<SC::UIntImm>())
        {
            uint64_t v = new_a.cast_to<UIntImm>()->value + 
                new_b.cast_to<UIntImm>()->value;
            return UIntImm::make(new_a->data_type, v);
        }
        else
        {
            CHECK_IF(new_a.is_type<SC::FloatImm>(), "not constant");
            double v = new_a.cast_to<FloatImm>()->value + 
                new_b.cast_to<FloatImm>()->value;
            return FloatImm::make(n->a->data_type, v);
        }
    }
    else if(new_a.sameAs(n->a) && new_b.sameAs(n->b))
    {
        return n;
    }
    else
    {
        return Add::make(new_a, new_b);
    }
}
Expr ConstantFoldPass::mutateNode(const Sub* n)
{
    Expr new_a = mutate(n->a);
    Expr new_b = mutate(n->b);
    if(is_const_number(new_a) && is_const_number(new_b))
    {
        if(new_a.is_type<SC::IntImm>())
        {
            int64_t v = new_a.cast_to<IntImm>()->value -
                new_b.cast_to<IntImm>()->value;
            return IntImm::make(new_a->data_type, v);
        }
        else if(new_a.is_type<SC::UIntImm>())
        {
            uint64_t v = new_a.cast_to<UIntImm>()->value -
                new_b.cast_to<UIntImm>()->value;
            return UIntImm::make(new_a->data_type, v);
        }
        else
        {
            CHECK_IF(new_a.is_type<SC::FloatImm>(), "not constant");
            double v = new_a.cast_to<FloatImm>()->value -
                new_b.cast_to<FloatImm>()->value;
            return FloatImm::make(n->a->data_type, v);
        }
    }
    else if(new_a.sameAs(n->a) && new_b.sameAs(n->b))
    {
        return n;
    }
    else
    {
        return Sub::make(new_a, new_b);
    }
}
Expr ConstantFoldPass::mutateNode(const Mul* n)
{
    Expr new_a = mutate(n->a);
    Expr new_b = mutate(n->b);
    if(is_const_number(new_a) && is_const_number(new_b))
    {
        if(new_a.is_type<SC::IntImm>())
        {
            int64_t v = new_a.cast_to<IntImm>()->value *
                new_b.cast_to<IntImm>()->value;
            return IntImm::make(new_a->data_type, v);
        }
        else if(new_a.is_type<SC::UIntImm>())
        {
            uint64_t v = new_a.cast_to<UIntImm>()->value *
                new_b.cast_to<UIntImm>()->value;
            return UIntImm::make(new_a->data_type, v);
        }
        else
        {
            CHECK_IF(new_a.is_type<SC::FloatImm>(), "not constant");
            double v = new_a.cast_to<FloatImm>()->value *
                new_b.cast_to<FloatImm>()->value;
            return FloatImm::make(n->a->data_type, v);
        }
    }
    else if(new_a.sameAs(n->a) && new_b.sameAs(n->b))
    {
        return n;
    }
    else
    {
        return Mul::make(new_a, new_b);
    }
}

Expr ConstantFoldPass::mutateNode(const Div* n)
{
    Expr new_a = mutate(n->a);
    Expr new_b = mutate(n->b);
    if(is_const_number(new_a) && is_const_number(new_b))
    {
        if(new_a.is_type<SC::IntImm>())
        {
            int64_t v = new_a.cast_to<IntImm>()->value /
                new_b.cast_to<IntImm>()->value;
            return IntImm::make(new_a->data_type, v);
        }
        else if(new_a.is_type<SC::UIntImm>())
        {
            uint64_t v = new_a.cast_to<UIntImm>()->value /
                new_b.cast_to<UIntImm>()->value;
            return UIntImm::make(new_a->data_type, v);
        }
        else
        {
            CHECK_IF(new_a.is_type<SC::FloatImm>(), "not constant");
            double v = new_a.cast_to<FloatImm>()->value /
                new_b.cast_to<FloatImm>()->value;
            return FloatImm::make(n->a->data_type, v);
        }
    }
    else if(new_a.sameAs(n->a) && new_b.sameAs(n->b))
    {
        return n;
    }
    else
    {
        return Div::make(new_a, new_b);
    }
}
Expr ConstantFoldPass::mutateNode(const Mod* n)
{
    Expr new_a = mutate(n->a);
    Expr new_b = mutate(n->b);
    if(is_const_number(new_a) && is_const_number(new_b))
    {
        if(new_a.is_type<SC::IntImm>())
        {
            int64_t v = new_a.cast_to<IntImm>()->value %
                new_b.cast_to<IntImm>()->value;
            return IntImm::make(new_a->data_type, v);
        }
        else if(new_a.is_type<SC::UIntImm>())
        {
            uint64_t v = new_a.cast_to<UIntImm>()->value %
                new_b.cast_to<UIntImm>()->value;
            return UIntImm::make(new_a->data_type, v);
        }
        else
        {
            CHECK_IF(false, "float can not MOD");
        }
    }
    else if(new_a.sameAs(n->a) && new_b.sameAs(n->b))
    {
        return n;
    }
    else
    {
        return Mod::make(new_a, new_b);
    }
}
Expr ConstantFoldPass::mutateNode(const Min* n)
{
    Expr new_a = mutate(n->a);
    Expr new_b = mutate(n->b);
    if(is_const_number(new_a) && is_const_number(new_b))
    {
        if(new_a.is_type<SC::IntImm>())
        {
            int64_t v = std::min(new_a.cast_to<IntImm>()->value ,
                new_b.cast_to<IntImm>()->value);
            return IntImm::make(new_a->data_type, v);
        }
        else if(new_a.is_type<SC::UIntImm>())
        {
            uint64_t v = std::min(new_a.cast_to<UIntImm>()->value,
                new_b.cast_to<UIntImm>()->value);
            return UIntImm::make(new_a->data_type, v);
        }
        else
        {
            CHECK_IF(new_a.is_type<SC::FloatImm>(), "not constant");
            double v = std::min(new_a.cast_to<FloatImm>()->value ,
                new_b.cast_to<FloatImm>()->value);
            return FloatImm::make(n->a->data_type, v);
        }
    }
    else if(new_a.sameAs(n->a) && new_b.sameAs(n->b))
    {
        return n;
    }
    else
    {
        return Min::make(new_a, new_b);
    }
}
Expr ConstantFoldPass::mutateNode(const Max* n)
{
    Expr new_a = mutate(n->a);
    Expr new_b = mutate(n->b);
    if(is_const_number(new_a) && is_const_number(new_b))
    {
        if(new_a.is_type<SC::IntImm>())
        {
            int64_t v = std::max(new_a.cast_to<IntImm>()->value ,
                new_b.cast_to<IntImm>()->value);
            return IntImm::make(new_a->data_type, v);
        }
        else if(new_a.is_type<SC::UIntImm>())
        {
            uint64_t v = std::max(new_a.cast_to<UIntImm>()->value,
                new_b.cast_to<UIntImm>()->value);
            return UIntImm::make(new_a->data_type, v);
        }
        else
        {
            CHECK_IF(new_a.is_type<SC::FloatImm>(), "not constant");
            double v = std::max(new_a.cast_to<FloatImm>()->value ,
                new_b.cast_to<FloatImm>()->value);
            return FloatImm::make(n->a->data_type, v);
        }
    }
    else if(new_a.sameAs(n->a) && new_b.sameAs(n->b))
    {
        return n;
    }
    else
    {
        return Max::make(new_a, new_b);
    }
}

bool check_divided(Expr a, Expr b)
{
    
    SC::ConstantFoldPass cf;   
    SC::Expr new_a = a->mutate_expr(&cf);
    SC::Expr new_b = b->mutate_expr(&cf);
    if((new_a.is_type<SC::IntImm>() || new_a.is_type<SC::UIntImm>())
            &&(new_b.is_type<SC::IntImm>() || new_b.is_type<SC::UIntImm>()))
    {
        if(new_a.is_type<SC::IntImm>() && new_b.is_type<SC::IntImm>())
        {
            return (new_a.cast_to<SC::IntImm>()->value % 
                    new_b.cast_to<SC::IntImm>()->value == 0);
        }
        else if(new_a.is_type<SC::IntImm>() && new_b.is_type<SC::UIntImm>())
        {
            return (new_a.cast_to<SC::IntImm>()->value % 
                    new_b.cast_to<SC::UIntImm>()->value == 0);
        }
        else if(new_a.is_type<SC::UIntImm>() && new_b.is_type<SC::IntImm>())
        {
            return (new_a.cast_to<SC::UIntImm>()->value % 
                    new_b.cast_to<SC::IntImm>()->value == 0);
        }
        else if(new_a.is_type<SC::UIntImm>() && new_b.is_type<SC::UIntImm>())
        {
            return (new_a.cast_to<SC::UIntImm>()->value % 
                    new_b.cast_to<SC::UIntImm>()->value == 0);
        }
    }

    return false;

}


} // namespace SC
