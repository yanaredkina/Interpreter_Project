#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
 
using namespace std;
 
enum type_of_lex {
    LEX_NULL,                                                                                   /* 0*/
    LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_INT,        /* 9*/
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE,  /*18*/
    LEX_REAL, LEX_REPEAT, LEX_UNTIL, LEX_BREAK,                                                 /*22*/
    LEX_FIN,                                                                                    /*23*/
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS,   /*31*/
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ,              /*39*/
    LEX_NUM,                                                                                    /*40*/
    LEX_ID,                                                                                     /*41*/
    POLIZ_LABEL,                                                                                /*42*/
    POLIZ_ADDRESS,                                                                              /*43*/
    POLIZ_GO,                                                                                   /*44*/
    POLIZ_FGO                                                                                   /*45*/
};
 
/* ----------------------------------- Class Lex ----------------------------------- */
 
class Lex {
private:
    type_of_lex t_lex;
    double v_lex;
public:
    Lex (type_of_lex t = LEX_NULL, double v = 0): t_lex (t), v_lex (v)  {  
    }
    
    type_of_lex  get_type () const { 
    	  return t_lex; 
    }
    
    int get_value () const { 
    	  return v_lex; 
    }
    
  friend ostream & operator<< (ostream &s, Lex l);
};
 
/* ---------------------------------- Class Ident ---------------------------------- */
 
class Ident {
private:
    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    double value;
public:
    Ident() { 
        declare = false; 
        assign  = false; 
    }
    Ident (const string n) {
        name = n;
        declare = false; 
        assign  = false;
    }
    
    string get_name () const { 
      return name; 
    }
    bool get_declare () const { 
      return declare; 
    }
    bool get_assign () const { 
      return assign; 
    }
    int  get_value () const { 
      return value; 
    }
    type_of_lex get_type () const { 
      return type; 
    }
    
    void put_declare () { 
      declare = true; 
    }
    void put_type (type_of_lex t) { 
      type = t; 
    }
    void put_assign () { 
      assign = true; 
    }
    void put_value (double v) { 
      value = v; 
    }
    bool operator== (const string& s) const { 
        return name == s; 
    }
};

/* ------------------------------- Table of Identifiers ---------------------------- */
 
vector<Ident> TID;
 
int put (const string & buf) {
    vector<Ident>::iterator k;
 
    if ((k = find(TID.begin(), TID.end(), buf)) != TID.end())
        return k - TID.begin();
    TID.push_back (Ident(buf));
    return TID.size() - 1;
}
 
/* --------------------------------- Class Scanner  -------------------------------- */
 
class Scanner {
private:
    FILE * fp;
    char c;
    int look (const string buf, const char ** list) {
        int i = 0;
        while (list[i]) {
            if (buf == list[i])
                return i;
            ++i;
        }
        return 0;
    }
    void gc() {
        c = fgetc(fp);
    }
    
public:
    static const char * TW [], * TD [];
    
    Scanner (const char * program) {
        if (!(fp = fopen(program, "r" ))) 
            throw "can’t open file";
    }
    
    FILE * get_fp () {
        return fp;
    }
    
    Lex get_lex ();
};
 
/* ----------------------- Tables of Service Words & Delimiters -------------------- */

const char * Scanner::TW    [] = {
    "",         // 0
    "and",      // 1
    "begin",    // 2
    "bool",     // 3
    "do",       // 4
    "else",     // 5
    "end",      // 6
    "if",       // 7
    "false",    // 8
    "int",      // 9
    "not",      // 10
    "or",       // 11
    "program",  // 12
     "read",    // 13
     "then",    // 14
     "true",    // 15
     "var",     // 16
     "while",   // 17
     "write",   // 18
     "real",    // 19
     "repeat",  // 20
     "until",   // 21
     "break",   // 22
     NULL
 };
 
const char * Scanner::TD    [] = {
    "@",  // 0
    ";",  // 1
    ",",  // 2
    ":",  // 3
    ":=", // 4
    "(",  // 5
    ")",  // 6
    "=",  // 7
    "<",  // 8
    ">",  // 9
    "+",  // 10
    "-",  // 11
    "*",  // 12
    "/",  // 13
    "<=", // 14
    "!=", // 15
    ">=", // 16
};

/* ---------------------- get_lex method from Scanner class ------------------------ */
 
Lex Scanner::get_lex () {
    enum state {H, IDENT, NUMB, COM, ALE, NEQ};
    int j, d = 0;
    double f = 0.0;
    int count = 0;
    string buf;
    state CS = H;
    do {
        gc ();
        switch (CS) {
            case H:
                if (c==' ' || c == '\n' || c== '\r' || c == '\t');
                else if (isalpha(c)) {
                    buf.push_back(c);
                    CS = IDENT;
                } else if (isdigit(c)) {
                    d = c - '0';
                    CS = NUMB;
                } else if (c== '{') { 
                    CS = COM;
                } else if (c == ':' || c == '<' || c == '>') { 
                    buf.push_back (c);
                    CS = ALE; 
                } else if (c == '@') {
                    gc();
                    return Lex(LEX_FIN);
                } else if (c == '!') {
                    buf.push_back(c);
                    CS = NEQ;
                } else {
                    buf.push_back(c);
                    if ((j = look(buf, TD))) {
                        return Lex((type_of_lex)(j + (int) LEX_FIN), j);
                    } else {
                        throw c;
                    }
                }
                break;
            case IDENT:
                if (isalpha(c) || isdigit(c)) {
                    buf.push_back(c); 
                } else {
                    ungetc (c, fp);
                    if ((j = look(buf, TW))) {
                        return Lex((type_of_lex) j, j );
                    } else {
                        j = put(buf);
                        return Lex(LEX_ID, j);
                    }
                }
                break;
            case NUMB:
                if (isdigit(c)) {
                    if (count == 0) {
                        d = d * 10 + (c - '0');
                    } else {
                        f = f + (double)(c - '0') / count ;
                        count *= 10;
                    }
                } else if (c == '.') {
                    count = 10;
                } else {
                    ungetc (c, fp);
                    if (f == 0) {
                        return Lex(LEX_INT, d);
                    }
                    return Lex(LEX_REAL, d + f);
                }
                break;
            case COM:
                if (c == '}') {
                    CS = H;
                } else if (c == '@' || c == '{') {
                    throw c;
                }
                break;
            case ALE:
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex((type_of_lex) (j + (int) LEX_FIN), j);
                } else {
                    ungetc (c, fp);
                    j = look(buf, TD);
                    return Lex((type_of_lex) (j + (int) LEX_FIN ), j);
                }
                break;
            case NEQ:
                if ( c == '=' ) {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex(LEX_NEQ, j);
                } else {
                    throw '!';
                }
                break;
    }
  } while (true);
}

/* ---------------------------------- Operator << ---------------------------------- */

ostream & operator<< (ostream &s, Lex l) {
    string t;
    if (l.t_lex <= LEX_BREAK)
        t = Scanner::TW[l.t_lex];
    else if (l.t_lex >= LEX_FIN && l.t_lex <= LEX_GEQ)
        t = Scanner::TD[l.t_lex - LEX_FIN];
    else if (l.t_lex == LEX_INT || l.t_lex == LEX_REAL)
        t = "NUMB";
    else if (l.t_lex == LEX_ID)
        t = TID[l.v_lex].get_name();
    else if (l.t_lex == POLIZ_LABEL)
        t = "Label";
    else if (l.t_lex == POLIZ_ADDRESS)
        t = "Addr";
    else if (l.t_lex == POLIZ_GO)
        t = "!";
    else if (l.t_lex == POLIZ_FGO) 
        t = "!F";
    else
        throw l;
    s << '(' << t << ',' << l.v_lex << ");" << endl;
    return s;
}
 

/* ---------------------------------- Class Parser --------------------------------- */

template <class T, class T_EL>
void from_st (T & st, T_EL & i) {
    i = st.top(); st.pop();
}

class Parser {
private:
    Lex curr_lex;
    type_of_lex c_type;
    double c_val;
    Scanner scan;
    stack <double> st_double;
    stack <type_of_lex> st_lex;
    void P();
    void D1();
    void D();
    void B();
    void S();
    void E();
    void E1();
    void T();
    void F();
    void dec(type_of_lex type);
    void check_id();
    void check_op();
    void check_not();
    void eq_type();
    void eq_bool();
    void check_id_in_read();
    void check_loop();
    void check_unary();
    void gl() {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }
public:
    Parser (const char *program): scan(program) { }
    void  analyze();
};
 
void Parser::analyze() {
    gl();
    P();
    if (c_type != LEX_FIN) {
        throw curr_lex;
    }
    cout << endl << "Yes!!!" << endl;
}
 
void Parser::P() {
    if (c_type == LEX_PROGRAM) {
        gl();
    } else {
        throw curr_lex;
    }
    D1(); 
    if (c_type == LEX_SEMICOLON) {
        gl();
    } else {
        throw curr_lex;
    }
    B();
}
 
void Parser::D1() {
    if (c_type == LEX_VAR) {
        gl();
        D();
        while (c_type == LEX_COMMA) {
            gl();
            D();
        }
    } else {
        throw curr_lex;
    }
}
 
void Parser::D() {
    if (c_type != LEX_ID) {
        throw curr_lex;
    } else {
        st_double.push(c_val);
        gl();
        while (c_type == LEX_COMMA) {
            gl();
            if (c_type != LEX_ID) {
                throw curr_lex;
            } else {
                st_double.push(c_val);
                gl();
            }
        }
        if (c_type != LEX_COLON) {
            throw curr_lex;
        } else {
            gl();
            if (c_type == LEX_INT) {
                dec(LEX_INT);
                gl();
            } else if (c_type == LEX_REAL) {
                    dec(LEX_REAL);
                    gl();
            } else if (c_type == LEX_BOOL) {
                    dec(LEX_BOOL);
                    gl();
            } else {
                throw curr_lex;
            }
        } 
    }
}
 
void Parser::B () {
    if (c_type == LEX_BEGIN) {
        gl();
        S();
        while (c_type == LEX_SEMICOLON) {
            gl();
            S();
        }
        if (c_type == LEX_END) {
            gl ();
        } else {
            throw curr_lex;
        }
    } else {
        throw curr_lex;
    }
}
 
void Parser::S () {
    int pl0, pl1, pl2, pl3;
     
    // IF-THEN operator:
    if (c_type == LEX_IF) {
        gl();
        E();
        eq_bool();
        if (c_type == LEX_THEN) {
            gl();
            S();
            if (c_type == LEX_ELSE) {
                gl();
                S();
            }
        } else {
            throw curr_lex;
        }
    }
    // WHILE-DO operator:
    else if (c_type == LEX_WHILE) {
        st_lex.push(c_type);
        gl();
        E();
        eq_bool();
        if (c_type == LEX_DO) {
            gl();
            S();
        } else {
            throw curr_lex;
        }
        st_lex.pop();
    }
    
    // READ operator:
    else if (c_type == LEX_READ) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            if (c_type == LEX_ID) {
                check_id_in_read();
                gl();
            } else {
                throw curr_lex;
            }
            if (c_type == LEX_RPAREN) {
                gl();
            } else {
                throw curr_lex;
            }
        } else {
            throw curr_lex;
        }
    }
    
    //WRITE operator:
    else if (c_type == LEX_WRITE) {
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            E();
            st_lex.pop();
            if (c_type == LEX_RPAREN) {
                gl();
            } else {
                throw curr_lex;
            }
        } else {
            throw curr_lex;
        }
    }
    
    //ASSIGH operator:
    else if (c_type == LEX_ID) { 
        check_id ();
        gl();
        if (c_type == LEX_ASSIGN) {
            gl();
            E();
            eq_type();
        } else {
            throw curr_lex;
        }
    }
    
    //REPEAT-UNTIL operator:
    else if (c_type == LEX_REPEAT) {
        st_lex.push(c_type);
        gl();
        S();
        if (c_type == LEX_UNTIL) {
            gl();
            E();
            eq_bool();
        } else {
            throw curr_lex;
        }
        st_lex.pop();
    }
    
    else if (c_type == LEX_BREAK) {
        check_loop();
        gl();
    }
        
    else {
        B();
    }
} // end procedure S()
 
void Parser::E () {
    E1();
    if (c_type == LEX_EQ  || c_type == LEX_LSS || c_type == LEX_GTR ||
        c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ ) {
        st_lex.push(c_type);
        gl(); 
        E1(); 
        check_op();
    }
}
 
void Parser::E1 () {
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
        st_lex.push(c_type);
        gl();
        T();
        check_op();
    }
}
 
void Parser::T () {
    F();
    while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND) {
        st_lex.push(c_type);
        gl();
        F();
        check_op();
    }
}
 
void Parser::F () {
    if (c_type == LEX_ID) {
        check_id();
        gl();
    } else if (c_type == LEX_INT) {
        st_lex.push(LEX_INT);
        gl();
    } else if (c_type == LEX_REAL) {
        st_lex.push(LEX_REAL);
        gl();
    } else if (c_type == LEX_TRUE) {
        st_lex.push(LEX_BOOL);
        gl();
    } else if (c_type == LEX_FALSE) {
        st_lex.push(LEX_BOOL);
        gl();
    } else if (c_type == LEX_NOT) {
        gl(); 
        F(); 
        check_not();
    } else if (c_type == LEX_PLUS) {
        gl();
        F();
        check_unary();
    } else if (c_type == LEX_MINUS) {
        gl();
        F();
        check_unary();
    } else if (c_type == LEX_LPAREN) {
        gl(); 
        E();
        if (c_type == LEX_RPAREN) {
            gl ();
        } else { 
            throw curr_lex;
        }
    } else {
        throw curr_lex;
    }
}
 

void Parser::dec (type_of_lex type) {
    int i;
    while (!st_double.empty()) {
        from_st(st_double, i);
        if (TID[i].get_declare()) {
            throw "twice";
        } else {
            TID[i].put_declare();
            TID[i].put_type(type);
        }
    }
}

  
void Parser::check_id() {
    if (TID[(int)c_val].get_declare()) {     
        st_lex.push(TID[(int)c_val].get_type());
    } else {
        throw "not declared";
    }
}

void Parser::check_op () {
    type_of_lex t1, t2, op, res = LEX_BOOL;
 
    from_st(st_lex, t2);
    from_st(st_lex, op);
    from_st(st_lex, t1);
 
    if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH) {
        if (t1 == LEX_BOOL || t2 == LEX_BOOL) {
            throw "wrong types are in operation";
        } else if (t1 == LEX_REAL || t2 == LEX_REAL) {
            res = LEX_REAL;
        } else {
            res = LEX_INT;
        }
    }
    if (op == LEX_OR || op == LEX_AND) {
        if (t1 != LEX_BOOL || t2 != LEX_BOOL) {
            throw "wrong types are in operation";
        }  
    }
    
    if (t1 == LEX_BOOL || t2 == LEX_BOOL) { 
        throw "wrong types are in operation";
    }
    st_lex.push(res);
}

void Parser::check_not() {
    if (st_lex.top() != LEX_BOOL) {
        throw "wrong type is in not";
    } else {
    }
}

void Parser::check_unary() {
    if (st_lex.top() != LEX_INT && st_lex.top() != LEX_REAL) {
        throw "wrong type is in unary operation";
    } else {
    }
}
 
void Parser::check_loop() {
    if (st_lex.empty()) {
        throw "operator break - outside of loop";
    }
    if (st_lex.top() == LEX_WHILE || st_lex.top() == LEX_REPEAT) {
        st_lex.pop ();
    } else {
        throw "operator break - outside of loop";
    }
}
 
void Parser::eq_type () {
    type_of_lex t;
    from_st(st_lex, t);
    if (t == LEX_BOOL && t != st_lex.top()) {
        throw "wrong types are in :=";
    }
    st_lex.pop();
}
 
void Parser::eq_bool () {
    if (st_lex.top() != LEX_BOOL) {
        throw "expression is not boolean";
    }
    st_lex.pop ();
  }
  
void Parser::check_id_in_read () {
    if (!TID[(int)c_val].get_declare()) {
        throw "not declared";
    }
}

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> MAIN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
int main () {
    try {
        cout << "Input file name:" << endl;
        char * testfile;
        cin >> testfile;
        Parser P (testfile);
        P.analyze();
        return 0;
        
    }
    catch (char c) {
        cout << "unexpected symbol " << c << endl;
        return 1;
    }
    catch (Lex l) {
        cout << "unexpected lexeme" << l << endl;
        return 1;
    }
    catch (const char *source) {
        cout << source << endl;
        return 1;
    }
}