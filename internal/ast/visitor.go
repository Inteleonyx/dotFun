package ast

type ExprVisitor interface {
	VisitIntLiteral(*IntLiteral) any
	VisitFloatLiteral(*FloatLiteral) any
	VisitStringLiteral(*StringLiteral) any
	VisitBoolLiteral(*BoolLiteral) any
	VisitArrayLiteral(*ArrayLiteral) any
	VisitAssignExpr(*AssignExpr) any
	VisitBinaryExpr(*BinaryExpr) any
	VisitCallExpr(*CallExpr) any
	VisitGroupingExpr(*GroupingExpr) any
	VisitInstanceOfExpr(*InstanceOfExpr) any
	VisitLambdaExpr(*LambdaExpr) any
	VisitLogicalExpr(*LogicalExpr) any
	VisitNewExpr(*NewExpr) any
	VisitPostfixUnaryExpr(*PostfixUnaryExpr) any
	VisitSuperExpr(*SuperExpr) any
	VisitThisExpr(*ThisExpr) any
	VisitUnaryExpr(*UnaryExpr) any
	VisitVariableExpr(*VariableExpr) any
	VisitMemberExpr(*MemberExpr) any
}

type StmtVisitor interface {
	VisitBlockStmt(*BlockStmt) any
	VisitBreakStmt(*BreakStmt) any
	VisitContinueStmt(*ContinueStmt) any
	VisitExpressionStmt(*ExpressionStmt) any
	VisitReturnStmt(*ReturnStmt) any
	VisitThrowStmt(*ThrowStmt) any
	VisitTryStmt(*TryStmt) any
	VisitIfStmt(*IfStmt) any
	VisitWhileStmt(*WhileStmt) any
	VisitForStmt(*ForStmt) any
	VisitSwitchCase(*SwitchCase) any
	VisitSwitchStmt(*SwitchStmt) any
	VisitValStmt(*ValStmt) any
	VisitLetStmt(*LetStmt) any
	VisitGlobalStmt(*GlobalStmt) any
	VisitFunctionStmt(*FunctionStmt) any
	VisitClassStmt(*ClassStmt) any
	VisitConstructorStmt(*ConstructorStmt) any
	VisitInterfaceStmt(*InterfaceStmt) any
	VisitStructStmt(*StructStmt) any
	VisitEnumStmt(*EnumStmt) any
	VisitDataStmt(*DataStmt) any
	VisitImportStmt(*ImportStmt) any
	VisitExportStmt(*ExportStmt) any
}

func (il *IntLiteral) Accept(visitor ExprVisitor) any {
	return visitor.VisitIntLiteral(il)
}

func (fl *FloatLiteral) Accept(visitor ExprVisitor) any {
	return visitor.VisitFloatLiteral(fl)
}

func (sl *StringLiteral) Accept(visitor ExprVisitor) any {
	return visitor.VisitStringLiteral(sl)
}

func (bl *BoolLiteral) Accept(visitor ExprVisitor) any {
	return visitor.VisitBoolLiteral(bl)
}

func (al *ArrayLiteral) Accept(visitor ExprVisitor) any {
	return visitor.VisitArrayLiteral(al)
}

func (ase *AssignExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitAssignExpr(ase)
}

func (be *BinaryExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitBinaryExpr(be)
}

func (ce *CallExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitCallExpr(ce)
}

func (ge *GroupingExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitGroupingExpr(ge)
}

func (ie *InstanceOfExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitInstanceOfExpr(ie)
}

func (le *LambdaExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitLambdaExpr(le)
}

func (le *LogicalExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitLogicalExpr(le)
}

func (ne *NewExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitNewExpr(ne)
}

func (pue *PostfixUnaryExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitPostfixUnaryExpr(pue)
}

func (se *SuperExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitSuperExpr(se)
}

func (te *ThisExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitThisExpr(te)
}

func (ue *UnaryExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitUnaryExpr(ue)
}

func (ve *VariableExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitVariableExpr(ve)
}

func (me *MemberExpr) Accept(visitor ExprVisitor) any {
	return visitor.VisitMemberExpr(me)
}

func (bs *BlockStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitBlockStmt(bs)
}

func (brs *BreakStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitBreakStmt(brs)
}

func (cs *ContinueStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitContinueStmt(cs)
}

func (es *ExpressionStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitExpressionStmt(es)
}

func (rs *ReturnStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitReturnStmt(rs)
}

func (ts *ThrowStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitThrowStmt(ts)
}

func (trs *TryStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitTryStmt(trs)
}

func (ifs *IfStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitIfStmt(ifs)
}

func (ws *WhileStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitWhileStmt(ws)
}

func (fs *ForStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitForStmt(fs)
}

func (sc *SwitchCase) Accept(visitor StmtVisitor) any {
	return visitor.VisitSwitchCase(sc)
}

func (ss *SwitchStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitSwitchStmt(ss)
}

func (vs *ValStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitValStmt(vs)
}

func (ls *LetStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitLetStmt(ls)
}

func (gs *GlobalStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitGlobalStmt(gs)
}

func (fs *FunctionStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitFunctionStmt(fs)
}

func (cs *ClassStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitClassStmt(cs)
}

func (cons *ConstructorStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitConstructorStmt(cons)
}

func (is *InterfaceStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitInterfaceStmt(is)
}

func (ss *StructStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitStructStmt(ss)
}

func (es *EnumStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitEnumStmt(es)
}

func (ds *DataStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitDataStmt(ds)
}

func (is *ImportStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitImportStmt(is)
}

func (es *ExportStmt) Accept(visitor StmtVisitor) any {
	return visitor.VisitExportStmt(es)
}
