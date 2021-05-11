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
    LEX_ID,                                                                                     /*40*/
    LEX_UPLUS, LEX_UMINUS,                                                                      /*42*/
    POLIZ_LABEL,                                                                                /*43*/
    POLIZ_ADDRESS,                                                                              /*44*/
    POLIZ_GO,                                                                                   /*45*/
    POLIZ_FGO                                                                                   /*46*/
};
 
/* ----------------------------------- Class Lex ----------------------------------- */

template <class T>

class Error {
private:
    string err_name;
    int err_str;
    T err_obj;
public:
    Error(string s, int n, T o) {
        err_name = s;
        err_str = n;
        err_obj = o;
    }
    
    string get_name() const {
        return err_name;
    }
    
    int get_str_num() const {
        return err_str;
    }
    
    T get_obj() const {
        return err_obj;
    }
};

struct number {
    bool is_int;
    union num {
        int i;
        double r;
    } storage;
};
    
class Lex {
private:
    type_of_lex t_lex = LEX_NULL;
    number v_lex;
    int str_lex;
public:
    Lex () {
         t_lex = LEX_NULL;
         str_lex = 0;
         v_lex = {true, {0}};
    }
    
    Lex (type_of_lex t) {
         t_lex = t;
         str_lex = 0;
         v_lex = {true, {0}};
    }
    
    Lex (type_of_lex t, number n) {
         t_lex = t;
         str_lex = 0;
         v_lex = n;
    }
    
    Lex (type_of_lex t, int v) {
         t_lex = t;
         str_lex = 0;
         v_lex = {true, {v}};  
    }
    
    Lex (type_of_lex t, int v, int s) {
         t_lex = t;
         str_lex = s;
         v_lex = {true, {v}}; 
    }
    
    Lex (type_of_lex t, double v, int s) {
        t_lex = t;
        str_lex = s;
        v_lex = {false, {.r = v}}; 
    }
    
    type_of_lex  get_type() const { 
    	  return t_lex; 
    }
    
    number get_value() const { 
    	  return v_lex; 
    }
    
    int get_string() const { 
    	  return str_lex; 
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
    number value;
public:
    Ident() { 
        declare = false; 
        assign  = false; 
    }
    
    Ident(const string n) {
        name = n;
        declare = false; 
        assign  = false;
    }
    
    string get_name() const { 
      return name; 
    }
    
    bool get_declare() const { 
      return declare; 
    }
    
    bool get_assign() const { 
      return assign; 
    }
    
    number get_value() const { 
      return value; 
    }
    
    type_of_lex get_type() const { 
      return type; 
    }
    
    void put_declare() { 
      declare = true; 
    }
    
    void put_type(type_of_lex t) { 
      type = t; 
    }
    
    void put_assign() { 
      assign = true; 
    }
    
    void put_value(number v) { 
      value = v;
    }
    
    bool operator== (const string& s) const { 
        return name == s; 
    }
};

/* ------------------------------- Table of Identifiers ---------------------------- */
 
vector<Ident> TID;
 
int put(const string& buf) {
    vector<Ident>::iterator k;
 
    if ((k = find(TID.begin(), TID.end(), buf)) != TID.end()) {
        return k - TID.begin();
    }
    TID.push_back(Ident(buf));
    return TID.size() - 1;
}
 
/* --------------------------------- Class Scanner  -------------------------------- */
 
class Scanner {
private:
    FILE * fp;
    char c;
    int str_num = 1;
    int look(const string buf, const char ** list) {
        int i = 0;
        while (list[i]) {
            if (buf == list[i])
                return i;
            ++i;
        }
        return 0;
    }

    void scan_char() {
        c = fgetc(fp);
    }
    
public:
    static const char * TW [];
    static const char * TD [];
    
    Scanner(const char * program) {
        if (!(fp = fopen(program, "r" ))) {
            throw "can’t open file";
        }
    }
    
    FILE* get_fp() {
        return fp;
    }
    
    Lex get_lex();
};
 
/* ----------------------- Tables of Service Words & Delimiters -------------------- */

const char * Scanner::TW [] = {
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
 
const char * Scanner::TD [] = {
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
 
Lex Scanner::get_lex() {
    enum state {H, IDENT, NUMB, COM, ALE, NEQ};
    int j, d = 0;
    double f = 0.0;
    int count = 0;
    string buf;
    state CS = H;
    do {
        scan_char();
        switch (CS) {
            case H:
                if (c == ' ' || c== '\r' || c == '\t') {
                } else if (c == '\n') {
                    str_num++;
                } else if (isalpha(c)) {
                    buf.push_back(c);
                    CS = IDENT;
                } else if (isdigit(c)) {
                    d = c - '0';
                    CS = NUMB;
                } else if (c == '{') { 
                    CS = COM;
                } else if (c == ':' || c == '<' || c == '>') { 
                    buf.push_back(c);
                    CS = ALE; 
                } else if (c == '@') {
                    j = look("@", TD);
                    scan_char();
                    return Lex(LEX_FIN, j, str_num);
                } else if (c == '!') {
                    buf.push_back(c);
                    CS = NEQ;
                } else {
                    buf.push_back(c);
                    if ((j = look(buf, TD))) {
                        return Lex((type_of_lex)(j + (int) LEX_FIN), j, str_num);
                    } else {
                        throw Error<char>("unexpected symbol", str_num, c);
                    }
                }
                break;
            case IDENT:
                if (isalpha(c) || isdigit(c)) {
                    buf.push_back(c); 
                } else {
                    ungetc(c, fp);
                    if ((j = look(buf, TW))) {
                        return Lex((type_of_lex) j, j, str_num);
                    } else {
                        j = put(buf);
                        return Lex(LEX_ID, j, str_num);
                    }
                }
                break;
            case NUMB:
                if (isdigit(c)) {
                    if (count == 0) {
                        d = d * 10 + (c - '0');
                    } else {
                        f = f + (double)(c - '0') / count;
                        count *= 10;
                    }
                } else if (c == '.') {
                    count = 10;
                } else {
                    ungetc(c, fp);
                    if (count == 0) {
                        return Lex(LEX_INT, d, str_num);
                    }
                    return Lex(LEX_REAL, d + f, str_num);
                }
                break;
            case COM:
                if (c == '}') {
                    CS = H;
                } else if (c == '@' || c == '{') {
                    throw Error<char>("unexpected symbol", str_num, c);;
                }
                break;
            case ALE:
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex((type_of_lex) (j + (int) LEX_FIN), j, str_num);
                } else {
                    ungetc(c, fp);
                    j = look(buf, TD);
                    return Lex((type_of_lex) (j + (int) LEX_FIN ), j, str_num);
                }
                break;
            case NEQ:
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex(LEX_NEQ, j, str_num);
                } else {
                    throw Error<char>("unexpected symbol", str_num, '!');
                }
                break;
    }
  } while (true);
}

/* ---------------------------------- Operator << ---------------------------------- */

ostream & operator<< (ostream &s, Lex l) {
    string t;
    if (l.t_lex == LEX_INT) {
        s << '(' << "NUMB" << ',' << l.v_lex.storage.i << ");" << endl;
        return s;
    } else if (l.t_lex == LEX_REAL) {
        s << '(' << "NUMB" << ',' << l.v_lex.storage.r << ");" << endl;
        return s;
    } else if (l.t_lex <= LEX_BREAK) {
            t = Scanner::TW[l.t_lex];
    } else if (l.t_lex >= LEX_FIN && l.t_lex <= LEX_GEQ) {
            t = Scanner::TD[l.t_lex - LEX_FIN];
    } else if (l.t_lex == LEX_ID) {
        t = TID[l.v_lex.storage.i].get_name();
    } else if (l.t_lex == LEX_UPLUS) {
        t = "Unary+";
    } else if (l.t_lex == LEX_UMINUS) {
        t = "Unary-";
    } else if (l.t_lex == POLIZ_LABEL) {
        t = "Label";
    } else if (l.t_lex == POLIZ_ADDRESS) {
        t = "Addr";
    } else if (l.t_lex == POLIZ_GO) {
        t = "!";
    } else if (l.t_lex == POLIZ_FGO) {
        t = "!F";
    } else {
        throw Error<Lex>("unexpected lexeme", l.str_lex, l);
    }
    s << '(' << t << ',' << l.v_lex.storage.i << ");" << endl;
    return s;
}
 

/* ---------------------------------- Class Parser --------------------------------- */

template <class T, class T_EL>
void from_st(T & st, T_EL & i) {
    i = st.top(); 
    st.pop();
}

class Parser {
private:
    Lex curr_lex;
    type_of_lex c_type;
    number c_val;
    Scanner scan;
    stack <number> st_number;
    stack <type_of_lex> st_lex;
    int loop_flag = 0;
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
    void check_op(int j);
    void check_not();
    void eq_type();
    void eq_bool();
    void check_id_in_read();
    void check_loop();
    void check_unary();
    void read_lex() {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }
public:
    vector <Lex> poliz;
    Parser(const char *program): scan(program) { }
    void  analyze();
};
 
void Parser::analyze() {
    read_lex();
    P();
    if (c_type != LEX_FIN) {
        throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
    }
    
    cout << endl << "POLIZ:" << endl;
    int i = 0;
    for (Lex l: poliz) {
        cout << i << ": ";
        cout << l;
        i++;
    }
    cout << endl << " << Finish of cheking! >> " << endl;
}
 
void Parser::P() {
    if (c_type == LEX_PROGRAM) {
        read_lex();
    } else {
        throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
    }
    D1(); 
    if (c_type == LEX_SEMICOLON) {
        read_lex();
    } else {
        throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
    }
    B();
}
 
void Parser::D1() {
    if (c_type == LEX_VAR) {
        read_lex();
        D();
        while (c_type == LEX_COMMA) {
            read_lex();
            D();
        }
    } else {
        throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
    }
}
 
void Parser::D() {
    if (c_type != LEX_ID) {
        throw Error <Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
    } else {
        st_number.push(c_val);
        read_lex();
        while (c_type == LEX_COMMA) {
            read_lex();
            if (c_type != LEX_ID) {
                throw Error <Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
            } else {
                st_number.push(c_val);
                read_lex();
            }
        }
        if (c_type != LEX_COLON) {
            throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
        } else {
            read_lex();
            if (c_type == LEX_INT) {
                dec(LEX_INT);
                read_lex();
            } else if (c_type == LEX_REAL) {
                dec(LEX_REAL);
                read_lex();
            } else if (c_type == LEX_BOOL) {
                dec(LEX_BOOL);
                read_lex();
            } else {
                throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
            }
        } 
    }
}
 
void Parser::B() {
    if (c_type == LEX_BEGIN) {
        read_lex();
        S();
        while (c_type == LEX_SEMICOLON) {
            read_lex();
            S();
        }
        if (c_type == LEX_END) {
            read_lex();
        } else {
            throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
        }
    } else {
        throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
    }
}
 
void Parser::S() {
    int pl0 = 0, pl1 = 0, pl2 = 0, pl3 = 0, pl4 = 0;
    number pl5;
     
    // IF-THEN operator:
    if (c_type == LEX_IF) {
        read_lex();
        E();
        eq_bool();
        pl2 = poliz.size();
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_FGO));
        
        if (c_type == LEX_THEN) {
            read_lex();
            S();
            pl3 = poliz.size();
            poliz.push_back(Lex());
            poliz.push_back(Lex(POLIZ_GO));
            poliz[pl2] = Lex(POLIZ_LABEL, poliz.size());
            if (c_type == LEX_ELSE) {
                read_lex();
                S();
                poliz[pl3] = Lex(POLIZ_LABEL, poliz.size());
            } else {
                poliz.pop_back();
                poliz.pop_back();
                poliz[pl2] = Lex(POLIZ_LABEL, poliz.size());
            }
        } else {
            throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
        }
    
    // WHILE-DO operator:
    } else if (c_type == LEX_WHILE) { 
        ++loop_flag;
        pl0 = poliz.size();
        read_lex();
        E();
        eq_bool();
        pl1 = poliz.size(); 
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_FGO));
        if (c_type == LEX_DO) {
            read_lex();
            S();
            poliz.push_back(Lex(POLIZ_LABEL, pl0));
            poliz.push_back(Lex(POLIZ_GO));
            poliz[pl1] = Lex(POLIZ_LABEL, poliz.size());
        } else {
            throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
        }
        if (!st_number.empty()) {
            from_st(st_number, pl5);
            poliz[pl5.storage.i] = Lex(POLIZ_LABEL, poliz.size());
        }
        --loop_flag;
    
    // READ operator:
    } else if (c_type == LEX_READ) {
        int j = c_val.storage.i;
        read_lex();
        if (c_type == LEX_LPAREN) {
            read_lex();
            if (c_type == LEX_ID) {
                check_id_in_read();
                poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
                read_lex();
            } else {
                throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
            }
            if (c_type == LEX_RPAREN) {
                read_lex();
                poliz.push_back(Lex(LEX_READ, j));
            } else {
                throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
            }
        } else {
            throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
        }
    
    //WRITE operator:
    } else if (c_type == LEX_WRITE) {
        int j = c_val.storage.i;
        read_lex();
        if (c_type == LEX_LPAREN) {
            read_lex();
            E();
            st_lex.pop(); // clearing steck
            if (c_type == LEX_RPAREN) {
                read_lex();
                poliz.push_back(Lex(LEX_WRITE, j));
            } else {
                throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
            }
        } else {
            throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
        }
    
    //ASSIGH operator:
    } else if (c_type == LEX_ID) { 
        check_id();
        poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
        read_lex();
        if (c_type == LEX_ASSIGN) {
            int j = c_val.storage.i;
            read_lex();
            E();
            eq_type();
            poliz.push_back(Lex(LEX_ASSIGN, j));
        } else {
            throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
        }
    
    //REPEAT-UNTIL operator:
    } else if (c_type == LEX_REPEAT) {
        ++loop_flag;
        pl4 = poliz.size();
        read_lex();
        S();
        if (c_type == LEX_UNTIL) {
            read_lex();
            E();
            eq_bool();
            poliz.push_back(Lex(POLIZ_LABEL, pl4));
            poliz.push_back(Lex(POLIZ_FGO));
        } else {
            throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
        }
        if (!st_number.empty()) {
            from_st(st_number, pl5);
            poliz[pl5.storage.i] = Lex(POLIZ_LABEL, poliz.size());
        }
        --loop_flag;
    
    } else if (c_type == LEX_BREAK) {
        check_loop();
        st_number.push({true, {(int)poliz.size()}});
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_GO));
        read_lex();
    
    } else {
        B();
    }
} // end procedure S()
 
void Parser::E() {
    E1();
    if (c_type == LEX_EQ  || c_type == LEX_LSS || c_type == LEX_GTR ||
        c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ) {
        int j = c_val.storage.i;
        st_lex.push(c_type);
        read_lex(); 
        E1(); 
        check_op(j);
    }
}
 
void Parser::E1() {
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
        int j = c_val.storage.i;
        st_lex.push(c_type);
        read_lex();
        T();
        check_op(j);
    }
}
 
void Parser::T() {
    F();
    while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND) {
        int j = c_val.storage.i;
        st_lex.push(c_type);
        read_lex();
        F();
        check_op(j);
    }
}
 
void Parser::F() {
    if (c_type == LEX_ID) {
        check_id();
        poliz.push_back(Lex(LEX_ID, c_val));
        read_lex();
    } else if (c_type == LEX_INT) {
        st_lex.push(LEX_INT);
        poliz.push_back(curr_lex);
        read_lex();
    } else if (c_type == LEX_REAL) {
        st_lex.push(LEX_REAL);
        poliz.push_back(curr_lex);
        read_lex();
    } else if (c_type == LEX_TRUE) {
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lex(LEX_TRUE, 1));
        read_lex();
    } else if (c_type == LEX_FALSE) {
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lex(LEX_FALSE, 0));
        read_lex();
    } else if (c_type == LEX_NOT) {
        read_lex(); 
        F();
        check_not();
    } else if (c_type == LEX_PLUS) {
        int j = c_val.storage.i;
        read_lex();
        F();
        check_unary();
        poliz.push_back(Lex(LEX_UPLUS, j));
    } else if (c_type == LEX_MINUS) {
        int j = c_val.storage.i;
        read_lex();
        F();
        check_unary();
        poliz.push_back(Lex(LEX_UMINUS, j));
    } else if (c_type == LEX_LPAREN) {
        read_lex(); 
        E();
        if (c_type == LEX_RPAREN) {
            read_lex();
        } else { 
            throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
        }
    } else {
        throw Error<Lex>("unexpected lexeme", curr_lex.get_string(), curr_lex);
    }
}
 

void Parser::dec(type_of_lex type) {
    number n;
    while (!st_number.empty()) {
        from_st(st_number, n);
        if (TID[n.storage.i].get_declare()) {
            throw Error<Lex>("identifier was twice declared", curr_lex.get_string(), curr_lex);
        } else {
            TID[n.storage.i].put_declare();
            TID[n.storage.i].put_type(type);
        }
    }
}

  
void Parser::check_id() {
    if (TID[c_val.storage.i].get_declare()) {     
        st_lex.push(TID[c_val.storage.i].get_type());
    } else {
        throw Error<Lex>("identifier not declared", curr_lex.get_string(), curr_lex);
    }
}

void Parser::check_op(int j) {
    type_of_lex t1, t2, op, res = LEX_BOOL;
 
    from_st(st_lex, t2);
    from_st(st_lex, op);
    from_st(st_lex, t1);
 
    if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH) {
        if (t1 == LEX_BOOL || t2 == LEX_BOOL) {
            throw Error<Lex>("wrong types are in operation", curr_lex.get_string(), curr_lex);
        } else if (t1 == LEX_REAL || t2 == LEX_REAL) {
            res = LEX_REAL;
        } else {
            res = LEX_INT;
        }
    }
    if (op == LEX_OR || op == LEX_AND) {
        if (t1 != LEX_BOOL || t2 != LEX_BOOL) {
            throw Error<Lex>("wrong types are in operation", curr_lex.get_string(), curr_lex);
        }  
    }
    
    if (t1 == LEX_BOOL || t2 == LEX_BOOL) { 
        throw Error<Lex>("wrong types are in operation", curr_lex.get_string(), curr_lex);
    }
    st_lex.push(res);
    poliz.push_back(Lex(op, j));
}

void Parser::check_not() {
    if (st_lex.top() != LEX_BOOL) {
        throw Error<Lex>("wrong type is in NOT operation", curr_lex.get_string(), curr_lex);
    } else {
        poliz.push_back(Lex(LEX_NOT));
    }
}

void Parser::check_unary() {
    if (st_lex.top() == LEX_BOOL) {
        throw Error<Lex>("wrong type is in unary operation", curr_lex.get_string(), curr_lex);
    }
}
 
void Parser::check_loop() {
    if (loop_flag == 0) {
        throw Error<Lex>("operator break outside of loop", curr_lex.get_string(), curr_lex);
    }
}
 
void Parser::eq_type () {
    type_of_lex t;
    from_st(st_lex, t);
    if (t == LEX_BOOL && t != st_lex.top()) {
        throw Error<Lex>("wrong types are in :=", curr_lex.get_string(), curr_lex);
    }
    st_lex.pop();
}
 
void Parser::eq_bool() {
    if (st_lex.top() != LEX_BOOL) {
        throw Error<Lex>("expression is not boolean", curr_lex.get_string(), curr_lex);
    }
    st_lex.pop ();
  }
  
void Parser::check_id_in_read () {
    if (!TID[c_val.storage.i].get_declare()) {
        throw Error<Lex>("identifier not declared", curr_lex.get_string(), curr_lex);
    }
}
 
class Executer {
public:
    void execute(vector<Lex> & poliz);
};
 
void Executer::execute(vector<Lex> & poliz) {
    Lex pc_el;
    stack <number> args;
    int index = 0;
    int size = poliz.size();
    number n, m;
    
    cout << endl << " << Results of executing >> " << endl;
    
    while (index < size) {
        pc_el = poliz[index];
        switch (pc_el.get_type()) {
            case LEX_TRUE:
            case LEX_FALSE:
            case LEX_REAL:
            case LEX_INT:
            case POLIZ_ADDRESS:
            case POLIZ_LABEL:
                args.push(pc_el.get_value());
                break;
                
            case LEX_ID:
                n = pc_el.get_value();
                if (TID[n.storage.i].get_assign()) {
                  args.push(TID[n.storage.i].get_value());
                  break;
                } else {
                  throw "POLIZ: indefinite identifier";
                }
              
            case LEX_NOT:
                from_st(args, n);
                args.push({true, {!n.storage.i}});
                break;
                
            case LEX_UPLUS:
                break;
                    
            case LEX_UMINUS:
                from_st(args, n);
                if (n.is_int) {
                    if (n.storage.i == 0) {
                        args.push(n);
                    } else {
                        args.push({true, {-n.storage.i}});
                    }
                } else {
                    if (n.storage.r == 0) {
                        args.push(n);
                    } else {
                        args.push({false, {.r = -n.storage.r}});
                    }
                }
                break;
 
            case LEX_OR:
                from_st(args, n); 
                from_st(args, m);
                args.push({true, {m.storage.i || n.storage.i}});
                break;
 
            case LEX_AND:
                from_st(args, n);
                from_st(args, m);
                args.push({true, {m.storage.i && n.storage.i}});
                break;
 
            case POLIZ_GO:
                from_st(args, n);
                index = n.storage.i - 1;
                break;
 
            case POLIZ_FGO:
                from_st(args, n);
                from_st(args, m);
                if (!m.storage.i) {
                    index = n.storage.i - 1;
                }
                break;
 
            case LEX_WRITE:
                from_st(args, m);
                if (m.is_int) {
                    cout << m.storage.i << endl;
                } else {
                    cout << m.storage.r << endl;
                }
                break;
 
            case LEX_READ:
                number k;
                from_st(args, n);
                if (TID[n.storage.i].get_type() == LEX_INT) {
                    cout << "Input int value for " << TID[n.storage.i].get_name() << endl;
                    int x;
                    cin >> x;
                    k = {true, {x}};
                } else if (TID[n.storage.i].get_type() == LEX_REAL) {
                    cout << "Input real value for " << TID[n.storage.i].get_name() << endl;
                    double x;
                    cin >> x;
                    k = {false, {.r = x}};
                } else {
                    string j;
                    int x;
                    while (1) {
                        cout << "Input boolean value (true or false) for " << TID[n.storage.i].get_name() << endl;
                        cin >> j;
                        if (j != "true" && j != "false") {
                            cout << "Error in input:true/false" << endl;
                            continue;
                        }
                        x = (j == "true") ? 1 : 0;
                        k = {true, {x}};
                        break;
                    }
                }
                TID[n.storage.i].put_value(k);
                TID[n.storage.i].put_assign();
                break;
 
            case LEX_PLUS:
                from_st(args, n);
                from_st(args, m);
                if (n.is_int && m.is_int) {
                    args.push({true, {m.storage.i + n.storage.i}});
                } else if (n.is_int && !m.is_int) {
                    args.push({false, {.r = m.storage.r + n.storage.i}});
                } else if (m.is_int && !n.is_int) {
                    args.push({false, {.r = m.storage.i + n.storage.r}});
                } else {
                    args.push({false, {.r = m.storage.r + n.storage.r}});
                }
                break;
 
            case LEX_TIMES:
                from_st(args, n);
                from_st(args, m);
                if (n.is_int && m.is_int) {
                    args.push({true, {m.storage.i * n.storage.i}});
                } else if (n.is_int && !m.is_int) {
                    args.push({false, {.r = m.storage.r * n.storage.i}});
                } else if (m.is_int && !n.is_int) {
                    args.push({false, {.r = m.storage.i * n.storage.r}});
                } else {
                    args.push({false, {.r = m.storage.r * n.storage.r}});
                }
                break;
 
            case LEX_MINUS:
                from_st(args, n);
                from_st(args, m);
                if (n.is_int && m.is_int) {
                    args.push({true, {m.storage.i - n.storage.i}});
                } else if (n.is_int && !m.is_int) {
                    args.push({false, {.r = m.storage.r - n.storage.i}});
                } else if (m.is_int && !n.is_int) {
                    args.push({false, {.r = m.storage.i - n.storage.r}});
                } else {
                    args.push({false, {.r = m.storage.r - n.storage.r}});
                }
                break;
 
            case LEX_SLASH:
                from_st(args, n);
                from_st(args, m);
                if (n.is_int && m.is_int) {
                    if (n.storage.i == 0) {
                        throw "POLIZ:divide by zero";
                    } else {
                        args.push({true, {m.storage.i / n.storage.i}});
                    }
                } else if (n.is_int && !m.is_int) {
                    if (n.storage.i == 0) {
                       throw "POLIZ:divide by zero"; 
                    } else {
                        args.push({false, {.r = m.storage.r / n.storage.i}});
                    }
                } else if (m.is_int && !n.is_int) {
                    if (n.storage.r == 0) {
                        throw "POLIZ:divide by zero"; 
                    } else {
                        args.push({false, {.r = m.storage.i / n.storage.r}});
                    }
                } else {
                    if (n.storage.r == 0) {
                        throw "POLIZ:divide by zero"; 
                    } else {
                        args.push({false, {.r = m.storage.r / n.storage.r}});
                    }
                }
                break;
 
            case LEX_EQ:
                from_st(args, n);
                from_st(args, m);
                if (n.is_int && m.is_int) {
                    args.push({true, {m.storage.i == n.storage.i}});
                } else if (n.is_int && !m.is_int) {
                    args.push({true, {m.storage.r == n.storage.i}});
                } else if (m.is_int && !n.is_int) {
                    args.push({true, {m.storage.i == n.storage.r}});
                } else {
                    args.push({true, {m.storage.r == n.storage.r}});
                }
                break;
 
            case LEX_LSS:
                from_st(args, n);
                from_st(args, m);
                if (n.is_int && m.is_int) {
                    args.push({true, {m.storage.i < n.storage.i}});
                } else if (n.is_int && !m.is_int) {
                    args.push({true, {m.storage.r < n.storage.i}});
                } else if (m.is_int && !n.is_int) {
                    args.push({true, {m.storage.i < n.storage.r}});
                } else {
                    args.push({true, {m.storage.r < n.storage.r}});
                }
                break;
 
            case LEX_GTR:
                from_st(args, n);
                from_st(args, m);
                if (n.is_int && m.is_int) {
                    args.push({true, {m.storage.i > n.storage.i}});
                } else if (n.is_int && !m.is_int) {
                    args.push({true, {m.storage.r > n.storage.i}});
                } else if (m.is_int && !n.is_int) {
                    args.push({true, {m.storage.i > n.storage.r}});
                } else {
                    args.push({true, {m.storage.r > n.storage.r}});
                }
                break;
 
            case LEX_LEQ:
                from_st(args, n);
                from_st(args, m);
                if (n.is_int && m.is_int) {
                    args.push({true, {m.storage.i <= n.storage.i}});
                } else if (n.is_int && !m.is_int) {
                    args.push({true, {m.storage.r <= n.storage.i}});
                } else if (m.is_int && !n.is_int) {
                    args.push({true, {m.storage.i <= n.storage.r}});
                } else {
                    args.push({true, {m.storage.r <= n.storage.r}});
                }
                break;
            
            case LEX_GEQ:
                from_st(args, n);
                from_st(args, m);
                if (n.is_int && m.is_int) {
                    args.push({true, {m.storage.i >= n.storage.i}});
                } else if (n.is_int && !m.is_int) {
                    args.push({true, {m.storage.r >= n.storage.i}});
                } else if (m.is_int && !n.is_int) {
                    args.push({true, {m.storage.i >= n.storage.r}});
                } else {
                    args.push({true, {m.storage.r >= n.storage.r}});
                }
                break;
 
            case LEX_NEQ:
                from_st(args, n);
                from_st(args, m);
                if (n.is_int && m.is_int) {
                    args.push({true, {m.storage.i != n.storage.i}});
                } else if (n.is_int && !m.is_int) {
                    args.push({true, {m.storage.r != n.storage.i}});
                } else if (m.is_int && !n.is_int) {
                    args.push({true, {m.storage.i != n.storage.r}});
                } else {
                    args.push({true, {m.storage.r != n.storage.r}});
                }
                break;
 
            case LEX_ASSIGN:
                from_st(args, n);
                from_st(args, m);
                if (TID[m.storage.i].get_type() == LEX_INT && n.is_int) {
                    TID[m.storage.i].put_value(n);
                } else if ((TID[m.storage.i].get_type() == LEX_INT && !n.is_int)) {
                    TID[m.storage.i].put_value({true, {(int)n.storage.r}});
                } else if ((TID[m.storage.i].get_type() == LEX_REAL && n.is_int)) {
                    TID[m.storage.i].put_value({false, {.r = (double)n.storage.i}});
                } else if ((TID[m.storage.i].get_type() == LEX_REAL && !n.is_int)) {
                    TID[m.storage.i].put_value(n);
                } else {
                    throw "POLIZ: unexpected elem";
                }
                TID[m.storage.i].put_assign(); 
                break;
 
            default:
                throw "POLIZ: unexpected elem";
        }
        ++index;
    };
    cout << endl << " << Finish of executing! >> " << endl;
}
 
class Interpretator {
private:
    Parser pars;
    Executer E;
public:
    Interpretator (const char* program): pars (program) {}
    void interpretation();
};
 
void Interpretator::interpretation() {
    pars.analyze();
    E.execute(pars.poliz);
}

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> MAIN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
int main () {
    try {
        cout << "Input file name:" << endl;
        string testfile = string();
        cin >> testfile;
        Interpretator I(testfile.c_str());
        I.interpretation();
        return 0;
        
    } catch (Error <char> E) {
        cout << E.get_name() << ": " << E.get_obj() << " in " << E.get_str_num() << " string" << endl;
        return 1;
    } catch (Error <Lex> E) {
         if (E.get_name() == "unexpected lexeme") {
             cout << E.get_name() << ": " << E.get_obj() << "in " << E.get_str_num() << " string" << endl;
         } else {
             cout << E.get_name() << " in " << E.get_str_num() << " string" << endl;
         }
        return 1;
    } catch (const char *source) {
        cout << source << endl;
        return 1;
    }
}