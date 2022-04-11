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

    LEX_NUM, LEX_STR, LEX_ID, /*39*/
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
    if ( ( k = find ( TID.begin (), TID.end (), buf ) ) != TID.end () )
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
                        buf.push_back (c);
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
                    return Lex ( LEX_NUM, d );
                }
                break;
            case STR:
                if ( c == '"' ) {
                    return Lex ( LEX_STR, 0 );
                }
                else if ( c == '@' )
                    throw c;
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

ostream & operator<< ( ostream &out, Lex l ){
  int type = l.get_type();
  if (type < LEX_FIN) 
    out << Scanner::TW[type];
  else if (type < LEX_NUM)
    out << Scanner::TD[type - LEX_FIN];
  if (type == LEX_NUM)
    out << "NUMCONST";
  else if (type == LEX_STR)
    out << "STRCONST";
  else if (type == LEX_ID)
    out << "PID";
  out << "<->" << l.get_value();
  return out;
}

int main () {
    try {
      Scanner sigur("input.txt"); Lex temp;
      while (temp.get_type() != LEX_FIN) {
        temp = sigur.get_lex();
        cout << temp << endl;
      }
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