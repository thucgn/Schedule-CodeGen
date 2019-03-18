/*************************************************************************
	> File Name: simple_codegen.cpp
	> Author: 
	> Mail: 
	> Created Time: Thu 14 Mar 2019 09:12:18 AM UTC
 ************************************************************************/

#include "simple_codegen.h"
#include "util.h"

namespace 
{
const std::string typeToString(::SC::DataType t)
{
    if(t.isBool()) 
        return "bool";
    else if(t.isFloat())
    {
        if(t.bytes() == 4)
            return "float";
        else
            return "double";
    }
    else if(t.isInt())
    {
        return std::string("int")
            .append(std::to_string(t.bits()))
            .append("_t");
    }
    else if(t.isUint())
    {
        return std::string("uint")
            .append(std::to_string(t.bits()))
            .append("_t");
    }
    else
    {
        CHECK_IF(false, "can not transfer the datatype to string");
    }
}

} //namespace

namespace SC
{

void SimpleCodegenC::visit(const IntImm* n) { os << n->value; }
void SimpleCodegenC::visit(const UIntImm* n) { os << n->value; }
void SimpleCodegenC::visit(const FloatImm* n) { os << n->value; }
void SimpleCodegenC::visit(const StringImm* n) { os << n->value; }

void SimpleCodegenC::visit(const Cast* n)
{
    os << "((" << typeToString(n->data_type) << ')';
    n->value.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Add* n)
{
    os << '(';
    n->a.accept(this);
    os << '+';
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Sub* n)
{
    os << '(';
    n->a.accept(this);
    os << '-';
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Mul* n)
{
    os << '(';
    n->a.accept(this);
    os << '*';
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Div* n)
{
    os << '(';
    n->a.accept(this);
    os << '/';
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Mod* n)
{
    os << '(';
    n->a.accept(this);
    os << '%';
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Min* n)
{
    os << "min(";
    n->a.accept(this);
    os << ',';
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Max* n)
{
    os << "max(";
    n->a.accept(this);
    os << ",";
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Eq* n)
{
    os << '(';
    n->a.accept(this);
    os << "==";
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Ne* n)
{
    os << '(';
    n->a.accept(this);
    os << "!=";
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Lt* n)
{
    os << '(';
    n->a.accept(this);
    os << "<";
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Gt* n)
{
    os << '(';
    n->a.accept(this);
    os << ">";
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Le* n)
{
    os << '(';
    n->a.accept(this);
    os << "<=";
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Ge* n)
{
    os << '(';
    n->a.accept(this);
    os << ">=";
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const And* n)
{
    os << '(';
    n->a.accept(this);
    os << "&&";
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Or* n)
{
    os << '(';
    n->a.accept(this);
    os << "||";
    n->b.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Not* n)
{
    os << '!';
    n->value.accept(this);
}

void SimpleCodegenC::visit(const LetStmt* n)
{
    os << this->indent;
    n->var.accept(this);
    os << " = ";
    n->value.accept(this);
    os << ";\n";
    n->body.accept(this);
}
void SimpleCodegenC::visit(const Select* n)
{
    os << '(';
    n->cond.accept(this);
    os << " ? ";
    n->true_case.accept(this);
    os << " : ";
    n->false_case.accept(this);
    os << ')';
}
void SimpleCodegenC::visit(const Call* n)
{
    if(n->call_type == CallType::TENSOR_ACCESS)
    {
        os << n->name << "[";
        for(auto& arg : n->args)
        {
            arg.accept(this);
            os << ",";
        }
        os << ')';
    }
    else
    {
        os << n->name << "(";
        for(auto& arg : n->args)
        {
            arg.accept(this);
            os << ", ";
        }
        os << ')';
    
    }
}
void SimpleCodegenC::visit(const Block* n)
{
    n->first.accept(this);
    if(n->rest.notNull())
        n->rest.accept(this);
}
void SimpleCodegenC::visit(const IfThenElse* n)
{
    os << indent;
    os << "if( ";
    n->cond.accept(this);
    os << " )\n";
    os << indent << "{\n";
    increIndent();
    n->then_case.accept(this);
    decreIndent();
    os << indent << "}\n";
    if(n->else_case.notNull())
    {
        os << indent << "else\n";
        os << indent << "{\n";
        increIndent();
        n->else_case.accept(this);
        decreIndent();
        os << indent << "}\n";
    }
}

void SimpleCodegenC::visit(const For* n)
{
    switch(n->for_type)
    {
        case ForType::SERIAL:
            break;
        case ForType::PARALLEL:
            os << indent << "#pragma omp parallel for\n";
            break;
        case ForType::VECTORIZED:
            break;
        case ForType::UNROLL:
            break;
    }
    os << indent << "for( ";
    n->var.accept(this);
    os << '=';
    n->min.accept(this);
    os << ';';
    n->var.accept(this);
    os << '<';
    Add::make(n->min, n->extent).accept(this);
    os << ';';
    n->var.accept(this);
    os << "++ )\n";
    os << indent << "{\n";
    increIndent();
    n->body.accept(this);
    decreIndent();
    os << indent << "}\n";
}

void SimpleCodegenC::visit(const Variable* n)
{
    os << n->label;
}


} //namespace SC
