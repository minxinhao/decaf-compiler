Program ::= Decl+
ident ::= ID
type_ident ::= TYPE_ID
Decl ::= VariableDecl | FunctionDecl | ClassDecl | InterfaceDecl
VariableDecl ::= Variable ;
Variable ::= Type ident
TypeBase ::= T_int | T_double | T_bool | T_string 
Type ::= TypeBase | type_ident | Type [ ]
FunctionDecl ::= Type ident ( Formals ) StmtBlock | \
 T_void ident ( Formals ) StmtBlock
Formals ::= Variable,*
ClassDecl ::= T_class type_ident Extender? Implementor? { Field* }
Extender ::= T_extends type_ident
Implementor ::= T_implements type_ident+
Field ::= VariableDecl | FunctionDecl

InterfaceDecl ::= T_interface type_ident { Prototype* }
Prototype ::= Type ident ( Formals ) ; | T_void ident ( Formals ) ;
StmtBlock ::= { Stmt* }
Stmt ::= Expr? ; | IfStmt | WhileStmt | ForStmt | \
 BreakStmt | ReturnStmt | PrintStmt | StmtBlock | VariableDecl

IfStmt ::=  T_if ( Expr ) Stmt ElseTail?
ElseTail ::= T_else Stmt
WhileStmt ::= T_while ( Expr ) Stmt
ForStmt ::= T_for ( Expr? ; Expr ; Expr? ) Stmt
ReturnStmt ::= T_return Expr? ;
BreakStmt ::= T_break ;
PrintStmt ::= T_Print ( Expr,+ ) ;

Expr ::= LValue = Expr | Constant | LValue | T_this | Call | ( Expr ) | \
 Expr + Expr | Expr - Expr | Expr * Expr | Expr / Expr | \
 Expr % Expr | - Expr | Expr < Expr | Expr T_less_eq Expr | \
 Expr > Expr | Expr T_greater_eq Expr | Expr T_eq Expr | Expr T_not_eq Expr | \
 Expr T_and Expr | Expr T_or Expr | ! Expr | \
 T_ReadInteger ( ) | T_ReadLine ( ) | T_New ident_type  | \
 T_NewArray ( Expr , Type ) 

LValue ::= ident | Expr . ident | Expr [ Expr ]
Call ::= ident ( Actuals ) | Expr . ident ( Actuals )
Actuals ::= Expr,*
Constant ::= NUM_int | NUM_double | T_null
