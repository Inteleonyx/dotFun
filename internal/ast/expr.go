package ast

type Expr interface {
	exprNode()
}

type IntLiteral struct {
	Value int64
}

func (il *IntLiteral) exprNode() {}

type FloatLiteral struct {
	Value float64
}

func (fl *FloatLiteral) exprNode() {}

type StringLiteral struct {
	Value string
}

func (sl *StringLiteral) exprNode() {}

type BoolLiteral struct {
	Value bool
}

func (bl *BoolLiteral) exprNode() {}

type ArrayLiteral struct {
	Elements []Expr
}

func (al *ArrayLiteral) exprNode() {}

type AssignExpr struct {
	Name  *Identifier
	Value Expr
}

func (ase *AssignExpr) exprNode() {}

type BinaryExpr struct {
	Left     Expr
	Operator string
	Right    Expr
}

func (be *BinaryExpr) exprNode() {}

type CallExpr struct {
	Callee    Expr
	Arguments []Expr
}

func (ce *CallExpr) exprNode() {}

type GroupingExpr struct {
	Expression Expr
}

func (ge *GroupingExpr) exprNode() {}

type InstanceOfExpr struct {
	Object Expr
	Type   Type
}

func (ie *InstanceOfExpr) exprNode() {}

type LambdaExpr struct {
	Params []string
	Body   Expr
}

func (le *LambdaExpr) exprNode() {}

type LogicalExpr struct {
	Left     Expr
	Operator string
	Right    Expr
}

func (le *LogicalExpr) exprNode() {}

type NewExpr struct {
	ClassName string
	Args      []Expr
}

func (ne *NewExpr) exprNode() {}

type PostfixUnaryExpr struct {
	Operand  Expr
	Operator string
}

func (pu *PostfixUnaryExpr) exprNode() {}

type SuperExpr struct {
	Method Expr
}

func (se *SuperExpr) exprNode() {}

type ThisExpr struct{}

func (te *ThisExpr) exprNode() {}

type UnaryExpr struct {
	Operator string
	Right    Expr
}

func (ue *UnaryExpr) exprNode() {}

type VariableExpr struct {
	Name         *Identifier
	DeclaredType Type
}

func (ve *VariableExpr) exprNode() {}

type MemberExpr struct {
	Object   Expr
	Property *Identifier
}

func (me *MemberExpr) exprNode() {}

type Identifier struct {
	Name string
}

func (id *Identifier) exprNode() {}
