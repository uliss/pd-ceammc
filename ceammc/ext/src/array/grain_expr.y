%name parse_grain_expr
%token_prefix TK_
%extra_argument {ceammc::GrainExprParser *p}

%include {
union grain_expr_t {
    double val;
    t_symbol* sym;

    grain_expr_t() = default;
    grain_expr_t(double v) : val(v) {}
    grain_expr_t(t_symbol* s) : sym(s) {}
};
}

%token_type {grain_expr_t}

%left PLUS.
%left DIVIDE TIMES POW MOD.
%right MINUS.

%syntax_error {
    p->setErrorMsg("syntax error");
    const int N = sizeof(yyTokenName) / sizeof(yyTokenName[0]);
    for (int i = 0; i < N; i++) {
        int a = yy_find_shift_action((YYCODETYPE)i, yypParser->yytos->stateno);
        if (a < YYNSTATE + YYNRULE) {
            std::cerr << "possible token: " << yyTokenName[i] << "\n";
        }
    }
}

%parse_accept { }

%parse_failure {
    p->setErrorMsg("parse failure");
}

%stack_overflow {
    p->setErrorMsg("stack overflow");
}

%stack_size 16

program ::= prop_list.

prop    ::= PROP_AMP.         { p->startProp(ceammc::GRAIN_PROP_AMP); }
prop    ::= PROP_AT.          { p->startProp(ceammc::GRAIN_PROP_AT); }
prop    ::= PROP_LEN.         { p->startProp(ceammc::GRAIN_PROP_LENGTH); }
prop    ::= PROP_SPEED.       { p->startProp(ceammc::GRAIN_PROP_SPEED); }
prop    ::= PROP_TIME_BEFORE. { p->startProp(ceammc::GRAIN_PROP_TIME_BEFORE); }
prop    ::= PROP_TIME_AFTER.  { p->startProp(ceammc::GRAIN_PROP_TIME_AFTER); }
prop    ::= PROP_PAN.         { p->startProp(ceammc::GRAIN_PROP_PAN); }
prop    ::= PROP_REPEATS.     { p->startProp(ceammc::GRAIN_PROP_REPEATS); }

ondone  ::= ONDONE.           { p->setCalcMoment(ceammc::GRAIN_CALC_ONDONE); }
ondone  ::= .

wintype(A) ::= HANN.  { A.val = ceammc::GRAIN_WIN_HANN; }
wintype(A) ::= RECT.  { A.val = ceammc::GRAIN_WIN_RECT; }
wintype(A) ::= TRI.   { A.val = ceammc::GRAIN_WIN_TRI; }
wintype(A) ::= TRPZ.  { A.val = ceammc::GRAIN_WIN_TRPZ; }
wintype(A) ::= TRPZ OPENP DOUBLE CLOSEP.
                      { A.val = ceammc::GRAIN_WIN_TRPZ; }

prop_pan  ::= PROP_PAN overflow.
prop_pan  ::= PROP_PAN mode.

prop_tag ::= PROP_TAG SYMBOL(A). { p->startProp(ceammc::GRAIN_PROP_TAG); p->saveGrainTag(A.sym); }


prop_expr ::= prop ondone assign expr. {
    switch (p->calcMoment()) {
    case ceammc::GRAIN_CALC_ONDONE:
        p->setOnDone();
    break;
    case ceammc::GRAIN_CALC_ONCE:
    default:
        if (!p->byteCodeEval()) {
            std::cerr << "eval failed: " << p->byteCode() << std::endl;
        }
    break;
    }
}
prop_expr ::= prop_pan.
prop_expr ::= prop_tag.
prop_expr ::= PROP_INTERP interp.
prop_expr ::= PROP_WINDOW wintype(T).          { p->setWinType(static_cast<ceammc::GrainWindowType>(T.val)); }
prop_expr ::= prop MODULATE modulate.

prop_list ::= prop_list prop_expr.
prop_list ::= prop_expr.

overflow ::= OVERFLOW CLIP.    { p->setPanOverflow(ceammc::GRAIN_PROP_OVERFLOW_CLIP); }
overflow ::= OVERFLOW FOLD.    { p->setPanOverflow(ceammc::GRAIN_PROP_OVERFLOW_FOLD); }
overflow ::= OVERFLOW WRAP.    { p->setPanOverflow(ceammc::GRAIN_PROP_OVERFLOW_WRAP); }

interp   ::= NONE.             { p->setInterp(ceammc::GRAIN_INTERP_NONE); }
interp   ::= LINEAR.           { p->setInterp(ceammc::GRAIN_INTERP_LINEAR); }
interp   ::= CUBIC.            { p->setInterp(ceammc::GRAIN_INTERP_CUBIC); }

mode ::= MODE NONE.            { p->setPanMode(ceammc::GRAIN_PAN_NONE); }
mode ::= MODE LINEAR.          { p->setPanMode(ceammc::GRAIN_PAN_LINEAR); }
mode ::= MODE FN_SQRT.         { p->setPanMode(ceammc::GRAIN_PAN_SQRT); }

waveform1(A) ::= SIN.          { A = ceammc::GRAIN_MOD_SIN; }
waveform1(A) ::= SAW.          { A = ceammc::GRAIN_MOD_SAW; }
waveform1(A) ::= TRI.          { A = ceammc::GRAIN_MOD_TRI; }
waveform1(A) ::= SQR.          { A = ceammc::GRAIN_MOD_SQR; }

modulate ::= expr(A) DOTS expr(B) waveform1(W) OPENP expr(FREQ) CLOSEP.
                               { p->setModulation(A.val, B.val, (ceammc::GrainModulation)W.val, FREQ.val); }
modulate ::= NONE.             { p->setModulation(0, 0, ceammc::GRAIN_MOD_NONE, 0); }

// NOTE: keep in sync with grain.cpp
vars    ::= CONST_AMP.         { p->bcPushCode(ceammc::OP_CONST0); }
vars    ::= CONST_AT.          { p->bcPushCode(ceammc::OP_CONST1); }
vars    ::= CONST_LEN.         { p->bcPushCode(ceammc::OP_CONST2); }
vars    ::= CONST_PAN.         { p->bcPushCode(ceammc::OP_CONST3); }
vars    ::= CONST_SPEED.       { p->bcPushCode(ceammc::OP_CONST4); }
vars    ::= CONST_TIME_BEFORE. { p->bcPushCode(ceammc::OP_CONST5); }
vars    ::= CONST_TIME_AFTER.  { p->bcPushCode(ceammc::OP_CONST13); }

vars    ::= CONST_SR.     { p->bcPushCode(ceammc::OP_CONST6); }
vars    ::= CONST_BS.     { p->bcPushCode(ceammc::OP_CONST7); }
vars    ::= CONST_ASIZE.  { p->bcPushCode(ceammc::OP_CONST8); }
vars    ::= CONST_NITER.  { p->bcPushCode(ceammc::OP_CONST9); }
vars    ::= CONST_NBLOCK. { p->bcPushCode(ceammc::OP_CONST10); }
vars    ::= CONST_NSAMP.  { p->bcPushCode(ceammc::OP_CONST11); }
vars    ::= CONST_GIDX.   { p->bcPushCode(ceammc::OP_CONST12); }

assign  ::= ASSIGN_ADD.   { p->setAssignType(ceammc::BC_ASSIGN_ADD); }
assign  ::= ASSIGN_SUB.   { p->setAssignType(ceammc::BC_ASSIGN_SUB); }
assign  ::= ASSIGN_MUL.   { p->setAssignType(ceammc::BC_ASSIGN_MUL); }
assign  ::= ASSIGN_SET.   { p->setAssignType(ceammc::BC_ASSIGN_SET); }
assign  ::= .

expr    ::= vars.
expr    ::= MINUS vars.

expr    ::= FN_ABS   OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_ABS); }
expr    ::= FN_SQRT  OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_SQRT); }
expr    ::= SIN      OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_SIN); }
expr    ::= FN_COS   OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_COS); }
expr    ::= FN_TAN   OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_TAN); }
expr    ::= FN_ACOS  OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_ACOS); }
expr    ::= FN_ASIN  OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_ASIN); }
expr    ::= FN_ATAN  OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_ATAN); }
expr    ::= FN_CEIL  OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_CEIL); }
expr    ::= FN_FLOOR OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_FLOOR); }
expr    ::= FN_ROUND OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_ROUND); }
expr    ::= FN_LOG   OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_LOG); }
expr    ::= FN_LOG10 OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_LOG10); }
expr    ::= FN_LOG2  OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_LOG2); }
expr    ::= FN_FRAC  OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_FRAC); }

expr    ::= WRAP  OPENP expr CLOSEP.      { p->bcPushCode(ceammc::OP_WRAP0); }
expr    ::= WRAP  OPENP expr COMMA expr CLOSEP.
                                          { p->bcPushCode(ceammc::OP_WRAP1); }
expr    ::= WRAP  OPENP expr COMMA expr COMMA expr CLOSEP.
                                          { p->bcPushCode(ceammc::OP_WRAP2); }

expr    ::= FN_URAND OPENP CLOSEP.        { p->bcPushCode(ceammc::OP_URAND0); }
expr    ::= FN_URAND OPENP expr CLOSEP.   { p->bcPushCode(ceammc::OP_URAND1); }
expr    ::= FN_URAND OPENP expr COMMA expr CLOSEP.
                                          { p->bcPushCode(ceammc::OP_URAND2); }

number(A) ::= DOUBLE(B).                  { A = B; p->bcPushValue(B.val); }
number(A) ::= MINUS DOUBLE(B).            { A.val = -B.val; p->bcPushValue(-B.val); }

num_vargs ::= num_vargs COMMA number.     { p->bcVarArg(); }
num_vargs ::= number.                     { p->bcVarArg(); }

expr    ::= CHOICE OPENP num_vargs CLOSEP.{ p->bcPushVarArgs(); p->bcPushCode(ceammc::OP_CHOICE); }

expr    ::= FOLD  OPENP expr CLOSEP.      { p->bcPushCode(ceammc::OP_FOLD0); }
expr    ::= FOLD  OPENP expr COMMA expr CLOSEP.
                                          { p->bcPushCode(ceammc::OP_FOLD1); }


expr    ::= CLIP  OPENP expr COMMA expr COMMA expr CLOSEP.
                                          { p->bcPushCode(ceammc::OP_CLIP); }

expr    ::= OPENP expr CLOSEP.
expr    ::= expr PLUS expr.               { p->bcPushCode(ceammc::OP_ADD); }
expr    ::= expr MINUS expr.              { p->bcPushCode(ceammc::OP_SUB); }
expr    ::= expr TIMES expr.              { p->bcPushCode(ceammc::OP_MUL); }
expr    ::= expr DIVIDE expr.             { p->bcPushCode(ceammc::OP_DIV); }
expr    ::= expr POW expr.                { p->bcPushCode(ceammc::OP_POW); }
expr    ::= expr MOD expr.                { p->bcPushCode(ceammc::OP_MOD); }
expr(A) ::= DOUBLE(B).                    { A = B; p->bcPushValue(B.val); }
expr(A) ::= MINUS DOUBLE(B).              { A.val = -B.val; p->bcPushValue(-B.val); }
expr    ::= DOUBLE(A) SAMP.               { p->bcPushValue(A.val); }
expr    ::= DOUBLE(A) MSEC.               { p->bcPushValue((sys_getsr() / 1000.0) * A.val); }
expr    ::= DOUBLE(A) SEC.                { p->bcPushValue(sys_getsr() * A.val); }
