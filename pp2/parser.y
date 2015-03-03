/* File: parser.y
 * --------------
 * Yacc input file to generate the parser for the compiler.
 *
 * pp2: your job is to write a parser that will construct the parse tree
 *      and if no parse errors were found, print it.  The parser should
 *      accept the language as described in specification, and as augmented
 *      in the pp2 handout.
 */

%{

/* Just like lex, the text within this first region delimited by %{ and %}
 * is assumed to be C/C++ code and will be copied verbatim to the y.tab.c
 * file ahead of the definitions of the yyparse() function. Add other header
 * file inclusions or C++ variable declarations/prototypes that are needed
 * by your code here.
 */
#include "scanner.h" // for yylex
#include "parser.h"
#include "errors.h"

void yyerror(const char *msg); // standard error-handling routine

%}

/* The section before the first %% is the Definitions section of the yacc
 * input file. Here is where you declare tokens and types, add precedence
 * and associativity options, and so on.
 */

/* yylval
 * ------
 * Here we define the type of the yylval global variable that is used by
 * the scanner to store attibute information about the token just scanned
 * and thus communicate that information to the parser.
 *
 * pp2: You will need to add new fields to this union as you add different
 *      attributes to your non-terminal symbols.
 */
%union {
    int integerConstant;
    bool boolConstant;
    char *stringConstant;
    double doubleConstant;
    char identifier[MaxIdentLen+1]; // +1 for terminating null
    Decl *decl;
    ClassDecl *classDecl;
    InterfaceDecl *interDecl;
    NamedType *namedType;
    VarDecl *varDecl;
    FnDecl *fnDecl;
    Type *type;
    StmtBlock *stmtBlock;
    Stmt *stmt;
    CaseStmt *caseStmt;
    DefaultStmt *defaultStmt;
    Expr *expr;
    LValue *lValue;
    Call *call;
    List<Decl*> *declList;
    List<VarDecl*> *formals;
    List<Stmt*> *stmts;
    List<CaseStmt*> *cases;
    List<Expr*> *exprs;
    List<NamedType*> *implements;
}

/* Tokens
 * ------
 * Here we tell yacc about all the token types that we are using.
 * Yacc will assign unique numbers to these and export the #define
 * in the generated y.tab.h header file.
 */
%token   T_Void T_Bool T_Int T_Double T_String T_Class
%token   T_LessEqual T_GreaterEqual T_Equal T_NotEqual T_Dims
%token   T_And T_Or T_Null T_Extends T_This T_Interface T_Implements
%token   T_While T_For T_If T_Else T_Return T_Break
%token   T_New T_NewArray T_Print T_ReadInteger T_ReadLine

%token   <identifier> T_Identifier
%token   <stringConstant> T_StringConstant
%token   <integerConstant> T_IntConstant
%token   <doubleConstant> T_DoubleConstant
%token   <boolConstant> T_BoolConstant

%token   T_Increm T_Decrem T_Switch T_Case T_Default

%left '='
%left T_Or
%left T_And
%nonassoc '|' '&' '^'
%nonassoc T_NotEqual T_Equal
%nonassoc '<' '>' T_LessEqual T_GreaterEqual
%left '+' '-'
%left '*' '/' '%'
%left T_Decrem T_Increm
%right '!'
%nonassoc '.' '['
%nonassoc P_Else
%nonassoc T_Else
%nonassoc P_Empty
%nonassoc P_EmptyDefault
%nonassoc P_NonEmpty
%nonassoc P_NonEmptyDefault


/* Non-terminal types
 * ------------------
 * In order for yacc to assign/access the correct field of $$, $1, we
 * must to declare which field is appropriate for the non-terminal.
 * As an example, this first type declaration establishes that the DeclList
 * non-terminal uses the field named "declList" in the yylval union. This
 * means that when we are setting $$ for a reduction for DeclList ore reading
 * $n which corresponds to a DeclList nonterminal we are accessing the field
 * of the union named "declList" which is of type List<Decl*>.
 * pp2: You'll need to add many of these of your own.
 */
%type <declList>   DeclList Field Prototypes
%type <decl>       Decl
%type <classDecl>  ClassDecl
%type <implements> Implements CImplement
%type <namedType>  CExtend
%type <interDecl>  InterDecl
%type <formals>    Vars VarDecls Formals
%type <varDecl>    Var VarDecl
%type <fnDecl>     Prototype FnDecl
%type <type>       Type
%type <stmtBlock>  StmtBlock
%type <stmts>      Stmts
%type <stmt>       Stmt
%type <cases>      Cases
%type <caseStmt>   CaseStmt
%type <defaultStmt>DefaultStmt
%type <expr>       Expr Constant OutExpr
%type <exprs>      Exprs Actuals
%type <lValue>     LValue
%type <call>       Call

%%
/* Rules
 * -----
 * All productions and actions should be placed between the start and stop
 * %% markers which delimit the Rules section.

 */
Program     : DeclList                  {
                                        @1;
                                        /* pp2: The @1 is needed to convince
                                        * yacc to set up yylloc. You can remove
                                        * it once you have other uses of @n*/
                                        Program *program = new Program($1);
                                        // if no errors, advance to next phase
                                        if (ReportError::NumErrors() == 0)
                                            program->Print(0);
                                        }
            ;
DeclList    : DeclList Decl             { ($$=$1)->Append($2); }
            | Decl                      { ($$ = new List<Decl*>)->Append($1); }
            ;
Decl        : VarDecl                   { $$ = $1; }
            | FnDecl                    { $$ = $1; }
            | ClassDecl                 { $$ = $1; }
            | InterDecl                 { $$ = $1; }
            ;
ClassDecl   : T_Class T_Identifier CExtend CImplement '{' Field '}'  { $$ = new ClassDecl(new Identifier(@2, $2), $3, $4, $6); }
            | T_Class T_Identifier CExtend CImplement '{' '}'        { $$ = new ClassDecl(new Identifier(@2, $2), $3, $4, new List<Decl*>); }
            ;
CExtend     : T_Extends T_Identifier    { $$ = new NamedType(new Identifier(@2, $2)); }
            | /* empty */               { $$ = nullptr; }
            ;
CImplement  : T_Implements Implements   { $$ = $2; }
            | /* empty */               { $$ = new List<NamedType*>; }
            ;
Implements  : Implements ',' T_Identifier { ($$=$1)->Append(new NamedType(new Identifier(@3, $3))); }
            | T_Identifier                { ($$ = new List<NamedType*>)->Append(new NamedType(new Identifier(@1, $1))); }
            ;
Field       : Field VarDecl             { ($$=$1)->Append($2); }
            | Field FnDecl              { ($$=$1)->Append($2); }
            | VarDecl                   { ($$=new List<Decl*>)->Append($1); }
            | FnDecl                    { ($$=new List<Decl*>)->Append($1); }
            ;
InterDecl   : T_Interface T_Identifier '{' Prototypes '}' { $$ = new InterfaceDecl(new Identifier(@2, $2), $4); }
            | T_Interface T_Identifier '{' '}'            { $$ = new InterfaceDecl(new Identifier(@2, $2), new List<Decl*>); }
            ;
VarDecl     : Var ';'                   { $$ = $1; }
            ;
Var         : Type T_Identifier         { $$ = new VarDecl(new Identifier(@2, $2), $1); }
            ;
Type        : T_Int                     { $$ = Type::intType; }
            | T_Double                  { $$ = Type::doubleType; }
            | T_Bool                    { $$ = Type::boolType; }
            | T_String                  { $$ = Type::stringType; }
            | T_Identifier              { $$ = new NamedType(new Identifier(@1, $1)); }
            | Type T_Dims               { $$ = new ArrayType(Join(@1, @2), $1); }
            ;
FnDecl      : Type T_Identifier   '(' Formals ')' StmtBlock { ($$ = new FnDecl(new Identifier(@2, $2), $1, $4))->SetFunctionBody($6); }
            | T_Void T_Identifier '(' Formals ')' StmtBlock { ($$ = new FnDecl(new Identifier(@2, $2), Type::voidType, $4))->SetFunctionBody($6); }
            ;
Prototypes  : Prototypes Prototype      { ($$ = $1)->Append($2);}
            | Prototype                 { ($$ = new List<Decl*>)->Append($1);}
Prototype   : Type T_Identifier   '(' Formals ')' ';' { $$ = new FnDecl(new Identifier(@2, $2), $1, $4); }
            | T_Void T_Identifier '(' Formals ')' ';' { $$ = new FnDecl(new Identifier(@2, $2), Type::voidType, $4); }
            ;
Formals     : Vars                      { $$ = $1;}
            | /*epsilon */              { $$ = new List<VarDecl*>; }
            ;
Vars        : Vars ',' Var              { ($$=$1)->Append($3); }
            | Var                       { ($$ = new List<VarDecl*>)->Append($1); }
            ;
StmtBlock   : '{' VarDecls Stmts '}'    { $$ = new StmtBlock($2, $3); }
            | '{' VarDecls       '}'    { $$ = new StmtBlock($2, new List<Stmt*>); }
            | '{' Stmts          '}'    { $$ = new StmtBlock(new List<VarDecl*>, $2); }
            | '{' /* epsilon */  '}'    { $$ = new StmtBlock(new List<VarDecl*>, new List<Stmt*>); }
            ;
VarDecls    : VarDecls VarDecl          { ($$ = $1)->Append($2); }
            | VarDecl                   { ($$ = new List<VarDecl*>)->Append($1); }
            ;
Stmts       : Stmts Stmt                { ($$ = $1)->Append($2); }
            | Stmt                      { ($$ = new List<Stmt*>)->Append($1); }
            ;
Stmt        : OutExpr ';'               { $$ = $1; }
            | StmtBlock                 { $$ = $1; }
            | T_Break ';'               { $$ = new BreakStmt(@1); }
            | T_Return ';'              { $$ = new ReturnStmt(@1, new EmptyExpr); }
            | T_Return Expr ';'         { $$ = new ReturnStmt(@2, $2); }
            | T_Print '(' Exprs ')' ';' { $$ = new PrintStmt($3); }
            | T_Switch '(' Expr ')' '{' Cases DefaultStmt '}' { $$ = new SwitchStmt($3, $6, $7); }
            | T_While '(' Expr ')' Stmt { $$ = new WhileStmt($3, $5); }
            | T_For '(' OutExpr ';' Expr ';' OutExpr ')' Stmt { $$ = new ForStmt($3, $5, $7, $9); }
            | T_If '(' Expr ')' Stmt %prec P_Else { $$ = new IfStmt($3, $5, nullptr); }
            | T_If '(' Expr ')' Stmt T_Else Stmt  { $$ = new IfStmt($3, $5, $7); }
            ;
Cases       : Cases CaseStmt            { ($$ = $1)->Append($2); }
            | CaseStmt                  { ($$ = new List<CaseStmt*>)->Append($1); }
            ;
CaseStmt    : T_Case T_IntConstant ':' Stmts %prec P_NonEmpty { $$ = new CaseStmt(new IntConstant(@2, $2), $4); }
            | T_Case T_IntConstant ':' %prec P_Empty          { $$ = new CaseStmt(new IntConstant(@2, $2), new List<Stmt*>); }
            ;
DefaultStmt : T_Default ':' Stmts %prec P_NonEmptyDefault { $$ = new DefaultStmt($3); }
            | T_Default ':' %prec P_EmptyDefault          { $$ = new DefaultStmt(new List<Stmt*>); }
            ;
Exprs       : Exprs ',' Expr            { ($$ = $1)->Append($3); }
            | Expr                      { ($$ = new List<Expr*>)->Append($1); }
            ;
OutExpr     : Expr                      { $$ = $1; }
            | /* empty */               { $$ = new EmptyExpr(); }
            ;
Expr        : Constant                  { $$ = $1; }
            | LValue                    { $$ = $1; }
            | T_This                    { $$ = new This(@1); }
            | Call                      { $$ = $1; }
            | '(' Expr ')'              { $$ = $2; }
            | LValue '=' Expr           { $$ = new AssignExpr($1, new Operator(@2, "="), $3); }
            | Expr '+' Expr             { $$ = new ArithmeticExpr($1, new Operator(@2, "+"), $3); }
            | Expr '-' Expr             { $$ = new ArithmeticExpr($1, new Operator(@2, "-"), $3); }
            | Expr '*' Expr             { $$ = new ArithmeticExpr($1, new Operator(@2, "*"), $3); }
            | Expr '/' Expr             { $$ = new ArithmeticExpr($1, new Operator(@2, "/"), $3); }
            | Expr '%' Expr             { $$ = new ArithmeticExpr($1, new Operator(@2, "%"), $3); }
            | '-' Expr                  { $$ = new ArithmeticExpr(new Operator(@1, "-"), $2); }
            | Expr '<' Expr             { $$ = new RelationalExpr($1, new Operator(@2, "<"), $3); }
            | Expr '>' Expr             { $$ = new RelationalExpr($1, new Operator(@2, ">"), $3); }
            | Expr T_LessEqual Expr     { $$ = new RelationalExpr($1, new Operator(@2, "<="), $3); }
            | Expr T_GreaterEqual Expr  { $$ = new RelationalExpr($1, new Operator(@2, ">="), $3); }
            | Expr T_Equal Expr         { $$ = new EqualityExpr($1, new Operator(@2, "=="), $3); }
            | Expr T_NotEqual Expr      { $$ = new EqualityExpr($1, new Operator(@2, "!="), $3); }
            | Expr T_Or Expr            { $$ = new LogicalExpr($1, new Operator(@2, "||"), $3); }
            | Expr T_And Expr           { $$ = new LogicalExpr($1, new Operator(@2, "&&"), $3); }
            | '!' Expr                  { $$ = new LogicalExpr(new Operator(@1, "!"), $2); }
            | T_ReadInteger '(' ')'     { $$ = new ReadIntegerExpr(Join(@1, @3)); }
            | T_ReadLine '(' ')'        { $$ = new ReadLineExpr(Join(@1, @3)); }
            | T_New '(' T_Identifier ')'{ $$ = new NewExpr(Join(@1, @4), new NamedType(new Identifier(@3, $3))); }
            | T_NewArray '(' Expr ',' Type ')' { $$ = new NewArrayExpr(Join(@1, @6), $3, $5); }
            | LValue T_Increm           { $$ = new PostFixExpr($1, new Operator(@2, "++")); }
            | LValue T_Decrem           { $$ = new PostFixExpr($1, new Operator(@2, "--")); }
            ;
LValue      : T_Identifier              { $$ = new FieldAccess(nullptr, new Identifier(@1, $1)); }
            | Expr '.' T_Identifier     { $$ = new FieldAccess($1, new Identifier(@3, $3)); }
            | Expr '[' Expr ']'         { $$ = new ArrayAccess(Join(@1, @4), $1, $3);}
            ;
Call        : T_Identifier '(' Actuals ')'          { $$ = new Call(Join(@1, @4), nullptr, new Identifier(@1, $1), $3); }
            | Expr '.' T_Identifier '(' Actuals ')' { $$ = new Call(Join(@1, @6), $1, new Identifier(@3, $3), $5); }
            ;
Actuals     : Exprs                     { $$ = $1; }
            | /* empty */               { $$ = new List<Expr*>; }
            ;
Constant    : T_IntConstant             { $$ = new IntConstant(@1, $1); }
            | T_DoubleConstant          { $$ = new DoubleConstant(@1, $1); }
            | T_BoolConstant            { $$ = new BoolConstant(@1, $1); }
            | T_StringConstant          { $$ = new StringConstant(@1, $1); }
            | T_Null                    { $$ = new NullConstant(@1); }
            ;
%%

/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated y.tab.c file.
 * This section is where you put definitions of helper functions.
 */

/* Function: InitParser
 * --------------------
 * This function will be called before any calls to yyparse().  It is designed
 * to give you an opportunity to do anything that must be done to initialize
 * the parser (set global variables, configure starting state, etc.). One
 * thing it already does for you is assign the value of the global variable
 * yydebug that controls whether yacc prints debugging information about
 * parser actions (shift/reduce) and contents of state stack during parser.
 * If set to false, no information is printed. Setting it to true will give
 * you a running trail that might be helpful when debugging your parser.
 * Please be sure the variable is set to false when submitting your final
 * version.
 */
void InitParser()
{
   PrintDebug("parser", "Initializing parser");
   yydebug = false;
}
