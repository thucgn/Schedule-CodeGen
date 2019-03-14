/*************************************************************************
	> File Name: simple_codegen.h
	> Author: 
	> Mail: 
	> Created Time: Thu 14 Mar 2019 08:56:43 AM UTC
 ************************************************************************/

#ifndef _SIMPLE_CODEGEN_H
#define _SIMPLE_CODEGEN_H

#include <string>
#include <iostream>
#include "irvisitor.h"

namespace SC
{

class SimpleCodegenC : public IRVisitor
{
public:
    SimpleCodegenC(std::ostream& os=std::cout) 
        : os(os) 
    {}

private:
    std::ostream& os;

protected:
    const std::string INDENT_UNIT = "\t";
    std::string indent;

    const std::string& increIndent() { 
        indent.append(INDENT_UNIT); 
        return indent;
    }
    const std::string& decreIndent() {
        if(indent.length() > 0)
            indent.resize(indent.length()-1);
        return indent;
    }

    void visit(const IntImm* n) override;
    void visit(const UIntImm* n) override;
    void visit(const FloatImm* n) override;
    void visit(const StringImm* n) override;
    void visit(const Cast* n) override;
    void visit(const Variable* n) override;
    void visit(const Add* n) override;
    void visit(const Sub* n) override;
    void visit(const Mul* n) override;
    void visit(const Div* n) override;
    void visit(const Mod* n) override;
    void visit(const Min* n) override;
    void visit(const Max* n) override;
    void visit(const Eq* n) override;
    void visit(const Ne* n) override;
    void visit(const Lt* n) override;
    void visit(const Gt* n) override;
    void visit(const Le* n) override;
    void visit(const Ge* n) override;
    void visit(const And* n) override;
    void visit(const Or* n) override;
    void visit(const Not* n) override;
    void visit(const LetStmt* n) override;
    void visit(const Select* n) override;
    void visit(const Call* n) override;
    void visit(const For* n) override;
    void visit(const Block* n) override;
    void visit(const IfThenElse* n) override;

    
    template <typename T>
    friend class ExprNode;
    template <typename T>
    friend class StmtNode;
};

} // namespace SC

#endif
