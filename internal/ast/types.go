package ast

type Type interface {
	TypeName() string
	String() string
}

type PrimitiveType string

func (p PrimitiveType) TypeName() string {
	return string(p)
}

func (p PrimitiveType) String() string {
	return p.TypeName()
}

const (
	IntType    PrimitiveType = "Int"
	StringType PrimitiveType = "String"
	FloatType  PrimitiveType = "Float"
	BoolType   PrimitiveType = "Bool"
	AnyType    PrimitiveType = "Any"
)

type ArrayType struct {
	ElementType Type
}

func (a ArrayType) TypeName() string {
	return "[]" + a.ElementType.TypeName()
}

func (a *ArrayType) String() string {
	return a.TypeName()
}
