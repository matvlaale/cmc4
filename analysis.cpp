#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>

// I 1. III 3. IV 1. V 3. VI 2.
// if, -, break, boolean, %, вычисление по обычной схеме

//〈программа〉 → program { 〈описания〉 〈операторы〉 }
//〈описания〉 → { 〈описание〉; }
//〈описание〉 → 〈тип〉 〈переменная〉 { , 〈переменная〉 }
//〈тип〉 → int | string | bool
//〈переменная〉 → 〈идентификатор〉 |
//〈идентификатор〉 = 〈константа〉
//〈константа〉 → 〈целочисленная〉 | 〈строковая〉 | 〈логическая〉
//〈логическая〉 → true | false
//〈целочисленная〉 → [〈знак〉] 〈цифра〉 { 〈цифра〉 }
//〈знак〉 → + | −
//〈строковая〉 → " { 〈литера〉 } "
//〈операторы〉 → { 〈оператор〉 }
//〈оператор〉 → if (〈выражение〉) 〈оператор〉 else 〈оператор〉 |
//  while (〈выражение〉) 〈оператор〉|
//  read (〈идентификатор〉); |
//  write (〈выражение〉 { , 〈выражение〉 } ); |
//  〈составной оператор〉 | 〈оператор-выражение〉
//  if (〈выражение〉) 〈оператор〉
//  break;
//  %
//〈составной оператор〉 → { 〈операторы〉 }

using namespace std;

enum type_of_lex {
    LEX_NULL, /* 0*/
    LEX_IF, LEX_ELSE, LEX_AND, LEX_OR, LEX_NOT, /* 5*/
    LEX_BOOL, LEX_INT, LEX_STRING, /* 8*/
    LEX_PROGRAM, LEX_TRUE, LEX_FALSE, LEX_READ, LEX_WRITE, LEX_BREAK, /*14*/
    LEX_CONST, LEX_WHILE, LEX_FIN, /*17*/
    LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_ASSIGN, /*22*/
    LEX_SEMICOLON, LEX_COMMA, /*24*/
    LEX_EQ, LEX_NEQ, LEX_GTR, LEX_LSS, LEX_GEQ, LEX_LEQ, /*30*/
    LEX_PERC, LEX_LSK, LEX_RSK, LEX_LFG, LEX_RFG, LEX_LQUOT, LEX_RQUOT, /*37*/

    LEX_NUM, LEX_STR, LEX_ID, /*40*/
    POLIZ_LABEL,
    POLIZ_ADDRESS,
    POLIZ_GO,
    POLIZ_FGO
};

class Lex {
    type_of_lex   t_lex;
    int           v_lex;
public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0 ): t_lex (t), v_lex (v)  { }
    type_of_lex  get_type () const { 
    	  return t_lex; 
    }
    int get_value () const { 
    	  return v_lex; 
    }
  friend ostream & operator<< ( ostream &out, Lex l );
};

class Ident {
    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
public:
    Ident() { 
        declare = false; 
        assign = false; 
    }
    bool operator== ( const string& s ) const { 
        return name == s; 
    }
    Ident ( const string n ) {
        name = n;
        declare = false; 
        assign = false;
    }
    string get_name () const { 
      return name; 
    }
    bool get_declare () const { 
      return declare; 
    }
    void put_declare () { 
      declare = true; 
    }
    type_of_lex get_type () const { 
      return type; 
    }
    void put_type ( type_of_lex t ) { 
      type = t; 
    }
    bool get_assign () const { 
      return assign; 
    }
    void put_assign () { 
      assign = true; 
    }
    int  get_value () const { 
      return value; 
    }
    void put_value ( int v ) { 
      value = v; 
    }
};

vector<Ident> TID;
int put ( const string & buf ) {
    vector<Ident>::iterator k;
    if ( ( k = find ( TID.begin (), TID.end (), buf ) ) 
        != TID.end () )
        return k - TID.begin();
    TID.push_back ( Ident(buf) );
    return TID.size () - 1;
}
 
class Scanner {
    FILE * fp;
    char c;
    int look ( const string buf, const char ** list ) {
        int i = 0;
        while ( list[i] ) {
            if ( buf == list[i] )
                return i;
            ++i;
        }
        return 0;
    }
    void gc () {
        c = fgetc (fp);
    }
public:
    static const char * TW [], * TD [];
    Scanner ( const char * program ) {
        if ( !(fp = fopen ( program, "r" )) ) 
            throw  "can’t open file" ;
    }
    Lex get_lex ();
};
 
const char *
Scanner::TW    [] = { "", "if", "else", "and", "or", "not", "bool", "int", "string", "program", "true", "false", "read",
                      "write", "break", "const", "while", NULL };

const char *
Scanner::TD    [] = { "@", "+", "-", "*", "/", "=", ";", ",", "==", "!=", ">", "<", ">=", "<=", "%", "(", ")", "{", "}", "\"", "\"", NULL };

string strings [200]; int strcap = 200, strnum = 0;

Lex Scanner::get_lex () {
    enum state { H, IDENT, NUMB, STR, ALE, NEQ };
    int d, j;
    string buf;
    state CS = H;
    do {
        gc ();
        switch ( CS ) {
            case H:
                if ( c ==' ' || c == '\n' || c == '\r' || c == '\t' );
                    else if ( isalpha (c) ) {
                        buf.push_back (c);
                        CS  = IDENT;
                    }
                    else if ( isdigit (c) ) {
                        d   = c - '0';
                        CS  = NUMB;
                    } 
                    else if ( c == '"' ) { 
                        //buf.push_back (c);
                        CS  = STR;
                    }
                    else if ( c == '=' || c == '<' || c == '>' ) { 
                        buf.push_back (c);
                        CS  = ALE; 
                    }
                    else if (c == '@')
                        return Lex ( LEX_FIN );
                    else if (c == '!') {
                        buf.push_back (c);
                        CS  = NEQ;
                    }
                    else {
                        buf.push_back (c);
                        if ( ( j = look ( buf, TD) ) ){
                            return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j );
                        }
                        else
                            throw c;
                    }
                    break;
            case IDENT:
                if ( isalpha (c) || isdigit (c) ) {
                    buf.push_back (c); 
                }
                else {
                    ungetc ( c, fp );
                    if ( (j = look ( buf, TW) ) ) {
                        return Lex ( (type_of_lex) j, j );
                    }
                    else {
                        j   = put ( buf );
                        return Lex ( LEX_ID, j );
                    }
                }
                break;
            case NUMB:
                if ( isdigit (c) ) {
                    d = d * 10 + ( c - '0' );
                }
                else {
                    ungetc ( c, fp );
                    return Lex ( LEX_INT, d );
                }
                break;
            case STR:
                if ( c == '"' ) {
                    if (strnum >= strcap) {
                        throw ("Memory of strings is wasted");
                    }
                    strings[strnum] = buf;
                    return Lex ( LEX_STRING, strnum++ );
                }
                else if ( c == '@' )
                    throw c;
                buf.push_back (c);
                break;
            case ALE:
                if ( c == '=' ) {
                    buf.push_back ( c );
                    j   = look ( buf, TD );
                    return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
                }
                else {
                    ungetc ( c, fp );
                    j   = look ( buf, TD );
                    return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
                }
                break;
            case NEQ:
                if ( c == '=' ) {
                    buf.push_back(c);
                    j   = look ( buf, TD );
                    return Lex ( LEX_NEQ, j );
                }
                else
                    throw '!';
                break;
    } //end switch
  } while (true);
}

class End_Exception {};

template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
    i = st.top(); st.pop();
}

class Parser {
    Lex curr_lex;
    type_of_lex c_type;
    int c_val;
    Scanner scan;
    stack < int > st_string;
    stack < type_of_lex >   st_lex;
    void  P();
    void  B1();
    void  D();
    void  B();
    void  S();
    void  E();
    void  E1();
    void  T();
    void  F();
    void  dec (type_of_lex type);
    void  check_id ();
    void  check_op ();
    void  check_not ();
    void  eq_type ();
    void  eq_bool ();
    void  check_id_in_read ();
    void  gl () {
        curr_lex  = scan.get_lex ();
        c_type    = curr_lex.get_type ();
        c_val     = curr_lex.get_value ();
        if (c_type == LEX_FIN)
            throw End_Exception();
        cout << curr_lex << endl;
    }
public:
    vector <Lex> poliz;
    Parser (const char *program) : scan (program) { }
    void  analyze();
};

void Parser::analyze () {
    gl ();
    try {
        P ();
    }
    catch (End_Exception) { cout << endl << "Yes" << endl; }
    catch (...) { throw; }
    if (c_type != LEX_FIN)
        throw curr_lex;
    for (Lex l : poliz) 
        cout << l << endl;
    cout << endl << "Yes" << endl;
}

void Parser::P () {
    if (c_type == LEX_PROGRAM) {
        gl ();
    }
    else 
        throw curr_lex; 
    if (c_type == LEX_LFG) { 
        gl ();
        D();
        B1();
        if (c_type == LEX_RFG) {
            gl();
        }
        else {
            throw curr_lex;
        }
    }
    else
        throw curr_lex;
}

void Parser::D() {
    type_of_lex line_type;
    while (c_type == LEX_BOOL || c_type == LEX_INT || c_type == LEX_STRING) {
        if (c_type == LEX_BOOL){
            line_type = LEX_BOOL;
            gl();
        } else if (c_type == LEX_INT){
            line_type = LEX_INT;
            gl();
        } else {
            line_type = LEX_STRING;
            gl();
        }
        if (c_type != LEX_ID)
            throw curr_lex;
        else {
            st_string.push(c_val);

            dec(line_type);
            check_id();
            poliz.push_back (Lex(POLIZ_ADDRESS, c_val));
            gl();
            if (c_type == LEX_ASSIGN) {
                gl();
                F();
                eq_type();
                poliz.push_back(Lex(LEX_ASSIGN));
            } else {
                st_lex.top();
                poliz.pop_back();
            }

            while (c_type == LEX_COMMA) {
                gl();
                if (c_type != LEX_ID)
                    throw curr_lex;
                else {
                    st_string.push ( c_val );
                    dec (line_type);
                    check_id ();
                    poliz.push_back (Lex (POLIZ_ADDRESS, c_val));
                    gl();
                    if (c_type == LEX_ASSIGN) {
                        gl();
                        F();
                        eq_type();
                        poliz.push_back(Lex(LEX_ASSIGN));
                    } else {
                        st_lex.top();
                        poliz.pop_back();
                    }
                }
            }
            if (c_type != LEX_SEMICOLON)
                throw curr_lex;
            else {
                dec(line_type);
                gl();
            } 
        }
    }
}

void Parser::B1() {
    S ();
    while (c_type == LEX_SEMICOLON) {
        gl();
        if (c_type == LEX_RFG) break;
        S();
    } 
}

void Parser::B() {
    if (c_type == LEX_LFG) {
        gl();
        S();
        while (c_type == LEX_SEMICOLON) {
            gl();
            if (c_type == LEX_RFG) break;
            S();
        }
        if (c_type == LEX_RFG) {
            gl();
        }
        else {
            throw curr_lex;
        }
    }
    else
        throw curr_lex;
}

void Parser::S() {
    int pl0, pl1, pl2, pl3, pl4, pl5;
    vector <Lex> name_ptr;
    vector <int> addr_ptr;
    Lex point;
    if (c_type == LEX_IF) {
        //условие
        gl();
        E();
        eq_bool();
        pl2 = poliz.size();
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_FGO));
        //то
        S();
        pl3 = poliz.size ();
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_GO));
        poliz[pl2] = Lex(POLIZ_LABEL, poliz.size());
        //иначе
        if (c_type == LEX_ELSE) {
            gl();
            S();
            poliz[pl3] = Lex(POLIZ_LABEL, poliz.size());
        }
        else
            throw curr_lex;
    }//end if
    else if (c_type == LEX_WHILE) {
        pl0 = poliz.size();
        gl();
        E();
        eq_bool();
        pl1 = poliz.size(); 
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_FGO));

        S();
        poliz.push_back(Lex(POLIZ_LABEL, pl0));
        poliz.push_back(Lex ( POLIZ_GO));
        poliz[pl1] = Lex(POLIZ_LABEL, poliz.size());
    }//end while
    else if (c_type == LEX_READ) {
        gl();
        if (c_type == LEX_LSK) {
            gl();
            if (c_type == LEX_ID) {
                check_id_in_read();
                poliz.push_back(Lex( POLIZ_ADDRESS, c_val));
                gl();
            }
            else
                throw curr_lex;
            if (c_type == LEX_RSK) {
                gl();
                poliz.push_back(Lex(LEX_READ));
            }    
        else
            throw curr_lex;
        }
    else  
            throw curr_lex;
    }//end read
    else if (c_type == LEX_WRITE) {
        gl();
        if (c_type == LEX_LSK) {
            gl();
            E();
            while (c_type == LEX_COMMA) {
                gl();
                E();
            }
            if (c_type == LEX_RSK) {
                gl();
                poliz.push_back(Lex(LEX_WRITE));
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;
    }//end write
    else if (c_type == LEX_ID) { 
        check_id(); 

        poliz.push_back (Lex(POLIZ_ADDRESS, c_val));
        gl();
        if (c_type == LEX_ASSIGN) {
            gl();
            E();
            eq_type();
            poliz.push_back(Lex(LEX_ASSIGN));
        }
        else
            throw curr_lex;
    }//assign-end
    else if (c_type == LEX_BREAK) {
        poliz.push_back(curr_lex);
        gl();
    }
    else
        B();
}

void Parser::E() {
    E1();
    if (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
         c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ) {
        st_lex.push(c_type);
        gl(); 
        E1(); 
        check_op();
    }
}

void Parser::E1() {
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS 
          || c_type == LEX_OR || c_type == LEX_PERC) {
        st_lex.push(c_type);
        gl();
        T();
        check_op();
    }
}

void Parser::T() {
    F();
    while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_PERC
            || c_type == LEX_AND) {
        st_lex.push(c_type);
        gl();
        F();
        check_op();
    }
}

void Parser::F() {
    if (c_type == LEX_ID) {
        check_id();
        poliz.push_back(Lex(LEX_ID, c_val));
        gl();
    }
    else if (c_type == LEX_INT) {
        st_lex.push(LEX_INT);
        poliz.push_back(curr_lex);
        gl();
    }
    else if (c_type == LEX_BOOL) {
        st_lex.push(LEX_BOOL);
        poliz.push_back(curr_lex);
        gl();
    }
    else if (c_type == LEX_STRING) {
        st_lex.push(LEX_STRING);
        poliz.push_back(curr_lex);
        gl();
    }
    else if (c_type == LEX_TRUE) {
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lex(LEX_TRUE, 1));
        gl();
    }
    else if (c_type == LEX_FALSE) {
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lex(LEX_FALSE, 0));
        gl();
    }
    else if (c_type == LEX_NOT) {
        gl(); 
        F(); 
        check_not();
    }
    else if (c_type == LEX_MINUS) {
        st_lex.push(LEX_INT);
        poliz.push_back(Lex(LEX_INT, 0));
        st_lex.push(c_type);
        gl();
        F();
    }
    else if (c_type == LEX_PLUS) {
        gl(); 
        F();
    }
    else if (c_type == LEX_LSK) {
        gl(); 
        E();
        if (c_type == LEX_RSK)
            gl();
        else 
            throw curr_lex;
    }
    else 
        throw curr_lex;
}

////////////////////////////////////////////////////////////////

void Parser::dec(type_of_lex type) {
    int i;
    while (!st_string.empty()) {
        from_st(st_string, i);
        if (TID[i].get_declare()) 
            throw "twice";
        else {
            TID[i].put_declare();
            TID[i].put_type(type);
        }
    }
}

void Parser::check_id() {
    if (TID[c_val].get_declare())
        st_lex.push(TID[c_val].get_type());
    else 
        throw "not declared";
}

void Parser::check_op() {
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;
 
    from_st ( st_lex, t2 );
    from_st ( st_lex, op );
    from_st ( st_lex, t1 );
 
    if ( op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES ||
      op == LEX_PERC || op == LEX_SLASH )
        r = LEX_INT;
    if ( op == LEX_OR || op == LEX_AND )
        t = LEX_BOOL;
    if ( t1 == t2  &&  t1 == t ) 
        st_lex.push (r);
    else
        throw "wrong types are in operation";
    poliz.push_back (Lex (op) );
}

void Parser::check_not() {
    if (st_lex.top() != LEX_BOOL)
        throw "wrong type is in not";
    else  
        poliz.push_back(Lex(LEX_NOT));
}

void Parser::eq_type() {
    type_of_lex t;
    from_st(st_lex, t);
    if (t != st_lex.top())
        throw "wrong types are in :=";
    st_lex.pop();
}

void Parser::eq_bool() {
    if (st_lex.top() != LEX_BOOL)
        throw "expression is not boolean";
    st_lex.pop();
  }

void Parser::check_id_in_read() {
    if (!TID[c_val].get_declare())
        throw "not declared";
}

////////////////////////////////////////////////

ostream & operator<< ( ostream &out, Lex l ){
  int type = l.get_type();
  if (type < LEX_FIN) 
    out << Scanner::TW[type];
  else if (type < LEX_NUM)
    out << Scanner::TD[type - LEX_FIN];
  /*else if (type == LEX_INT)
    out << "NUMCONST";
  else if (type == LEX_STRING) {
    out << "STRCONST" << "<->" << strings[l.get_value()];
    return out;
  }*/ else if (type == LEX_ID)
    out << "PID";
  else if (type == POLIZ_LABEL) {
    cout << endl;
    return out;
  }
  out << "<->" << l.get_value();
  return out;
}

int main () {
    try {
      Parser pars("input.txt");
      pars.analyze();
      return 0;
    }
    catch ( char c ) {
        cout << "unapropriate char " << c << endl;
        return 1;
    }
    catch ( Lex l ) {
        cout << "unapropriate lexeme " << l << endl;
        return 1;
    }
    catch ( const char *source ) {
        cout << source << endl;
        return 1;
    }
}