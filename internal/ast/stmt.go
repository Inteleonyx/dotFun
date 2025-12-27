package ast

type Stmt interface {
	stmtNode()
}

type Modifier int

const (
	ModifierNone Modifier = iota
	ModifierPublic
	ModifierProtected
	ModifierPrivate
)

func ModifierFromString(s string) Modifier {
	switch s {
	case "public":
		return ModifierPublic
	case "protected":
		return ModifierProtected
	case "private":
		return ModifierPrivate
	default:
		return ModifierNone
	}
}

type BlockStmt struct {
	Statements []Stmt
}

func (bs *BlockStmt) stmtNode() {}

type BreakStmt struct{}

func (bs *BreakStmt) stmtNode() {}

type ContinueStmt struct{}

func (cs *ContinueStmt) stmtNode() {}

type ExpressionStmt struct {
	Expression Expr
}

func (es *ExpressionStmt) stmtNode() {}

type ReturnStmt struct {
	Value Expr
}

func (rs *ReturnStmt) stmtNode() {}

type ThrowStmt struct {
	Value Expr
}

func (ts *ThrowStmt) stmtNode() {}

type TryStmt struct {
	TryBlock     *BlockStmt
	CatchVarName string
	CatchBlock   *BlockStmt
	FinallyBlock *BlockStmt
}

func (ts *TryStmt) stmtNode() {}

type IfStmt struct {
	Condition Expr
	ThenBlock *BlockStmt
	ElseBlock Stmt
}

func (ifs *IfStmt) stmtNode() {}

type WhileStmt struct {
	Condition Expr
	Body      *BlockStmt
}

func (ws *WhileStmt) stmtNode() {}

type ForStmt struct {
	Init      Stmt
	Condition Expr
	Post      Stmt
	Body      *BlockStmt
}

func (fs *ForStmt) stmtNode() {}

type SwitchCase struct {
	CaseExprs []Expr
	Body      *BlockStmt
}

func (sc *SwitchCase) stmtNode() {}

type SwitchStmt struct {
	Expr    Expr
	Cases   []*SwitchCase
	Default *BlockStmt
}

func (ss *SwitchStmt) stmtNode() {}

type ValStmt struct {
	Name         string
	DeclaredType Type
	Initializer  Expr
	Modifiers    Modifier
}

func (vs *ValStmt) stmtNode() {}

type LetStmt struct {
	Name         string
	DeclaredType Type
	Initializer  Expr
	Modifiers    Modifier
}

func (ls *LetStmt) stmtNode() {}

type GlobalStmt struct {
	Name         string
	DeclaredType Type
	Initializer  Expr
}

func (gs *GlobalStmt) stmtNode() {}

type FunctionStmt struct {
	Name       string
	Parameters []Parameter
	ReturnType Type
	Body       *BlockStmt
	Modifiers  Modifier
	Async      bool
	Override   bool
}

func (fs *FunctionStmt) stmtNode() {}

type Parameter struct {
	Name string
	Type Type
}

type ClassStmt struct {
	Name       string
	SuperClass string
	Modifiers  Modifier
	Members    []Stmt
}

func (cs *ClassStmt) stmtNode() {}

type ConstructorStmt struct {
	Parameters []Parameter
	Body       *BlockStmt
}

func (cs *ConstructorStmt) stmtNode() {}

type InterfaceStmt struct {
	Name      string
	Modifiers Modifier
	Members   []Stmt
}

func (is *InterfaceStmt) stmtNode() {}

type StructStmt struct {
	Name      string
	Modifiers Modifier
	Members   []Stmt
}

func (ss *StructStmt) stmtNode() {}

type EnumStmt struct {
	Name      string
	Modifiers Modifier
	Elements  []string
}

func (es *EnumStmt) stmtNode() {}

type DataStmt struct {
	Name      string
	Modifiers Modifier
	Fields    []Parameter
}

func (ds *DataStmt) stmtNode() {}

type ImportStmt struct {
	Module string
}

func (is *ImportStmt) stmtNode() {}

type ExportStmt struct {
	ExportedName string
}

func (es *ExportStmt) stmtNode() {}
