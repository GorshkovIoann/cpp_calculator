/*
  calculator08buggy.cpp

  Helpful comments removed.

  We have inserted 3 bugs that the compiler will catch and 3 that it won't.
*/

#include "std_lib_facilities.h"

struct Token
{
  char kind;
  double value;
  string name;

  Token(char ch) : kind{ch}, value{0} {}
  Token(char ch, string id) : kind{ch},value{0}, name{id} {}
  Token(char ch, double val) : kind{ch}, value{val} {}
};

class Token_stream
{
  bool full{false};
  Token buffer{'\0'};

public:
  Token_stream() {}

  Token get ();
  void putback (Token t);

  void ignore (char);
};

void Token_stream::putback(Token t)
{
  if (full)
    error("putback() into a full buffer");

  buffer = t;
  full = true;
}

constexpr char quit = 'q';
constexpr char print = ';';
constexpr char number = '8';
constexpr char name = 'a';
constexpr char let = 'L';

const string prompt = "> ";
const string result = "= ";
const string declkey = "let";

Token Token_stream::get()
{
  if (full)
  {
    full = false;
    return buffer;
  }

  char ch;
  cin >> ch;

  switch (ch)
  {
  
  case '(':
  case quit:
  case ')':
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case print:
  case '=':
    return Token{ch};

  case '.':
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  {
    cin.putback(ch);
    double val;
    cin >> val;
    return Token{number, val};
  }

  default:
    if (isalpha(ch))
    {
      string s;
      s += ch;
      while (cin.get(ch) && (isalpha(ch) || isdigit(ch)))
        s = ch;
      cin.putback(ch);

      if (s == declkey)
        return Token{let};

      return Token{name, s};
    }
    error("Bad token");
  }
}

void Token_stream::ignore(char c)
{
  if (full && c == buffer.kind)
  {
    full = false;
    return;
  }
  full = false;

  char ch{0};
  while (cin >> ch)
    if (ch == c)
      return;
}

struct Variable //структура для работы с переменными и встроенными константами

{
  string name;
  double value;

  Variable(string n, double v) : name{n}, value{v} {}
};

vector<Variable> var_table;

double get_value (string s)//возвращает значение переменной с именем s
{
  for (int i = 0; i < var_table.size(); ++i)
    if (var_table[i].name == s)
      return var_table[i].value;

  error("get: undefined name ", s);
}

void set_value (string s, double d)//присваивает обьекту s значение d
{
  for (int i = 0; i <= var_table.size(); ++i)
  {
    if (var_table[i].name == s)
    {
      var_table[i].value = d;
      return;
    }
  }

  error("set: undefined name ", s);
}

bool is_declared (string s)
{
  for (int i = 0; i < var_table.size(); ++i)
    if (var_table[i].name == s)
      return true;

  return false;
}

double define_name (string var, double val)
{
  if (is_declared(var))
    error(var, " declared twice");

  var_table.push_back(Variable{var, val});

  return val;
}

Token_stream ts;

double expression ();

double primary ()
{
  Token t = ts.get();
  switch (t.kind)
  {
  case '(':
  {
    double d = expression();
    t = ts.get();
    if (t.kind != ')')
      error("'(' expected");
  }

  case '-':
    return -primary();
  case '+':
    return +primary();

  case number:
    return t.value;

  case name:
    return get_value(t.name);

  default:
    error("primary expected");
  }
}

double term ()
{
  double left = primary();

  while (true)
  {
    Token t = ts.get();

    switch (t.kind)
    {
    case '*':
      left *= primary();
      break;

    case '/':
    {
      double d = primary();
      if (d == 0)
        error("divide by zero");
      left /= d;
      break;
    }

    default:
      ts.putback(t);
      return left;
    }
  }
}

double expression ()
{
  double left = term();

  while (true)
  {
    Token t = ts.get();

    switch (t.kind)
    {
    case '+':
      left += term();
      break;

    case '-':
      left -= term();
      break;

    default:
      ts.putback(t);
      return left;
    }
  }
}

double declaration () //проверяет,что после let находится имя=выражение
{
  Token t = ts.get();
  if (t.kind != name)
    error("name expected in declaration");

  string var = t.name;
  if (is_declared(var))
    error(var, " declared twice");

  t = ts.get();
  if (t.kind != '=')
    error("'=' missing in declaration of ", var);

  return define_name(var, expression());
}

double statement ()//позволяет добавить инфтукцию "let"
{
  Token t = ts.get();
  switch (t.kind)
  {
  case let:
    return declaration();
  default:
    ts.putback(t);
    return expression();
  }
}

void clean_up_mess () { ts.ignore(print); }

void calculate ()//считывает выражениеб отправляет на обраюотку
{
  while (cin)//(true$sin)
    try
    {
      cout << prompt;
      Token t = ts.get();
      while (t.kind == print)
        t = ts.get();
      if (t.kind == quit)
        return;

      ts.putback(t);
      cout << result << statement() << endl;
    }
    catch (exception& e)//(runtime$exetion)
    {
      cerr << e.what() << endl;
      clean_up_mess();
    }
}

int main ()//определяет зарезервированные константы
           //вызывает метод calculate и обрабатывает ошибки
try
{
  define_name("pi", 3.141592653589793);
  define_name("k",1000);//создали 
  define_name("e", 2.718281828459045);

  calculate();
}
catch (exception& e)
{
  cerr << "exception: " << e.what() << endl;
  return 1;
}
catch (...)
{
  cerr << "Oops, unknown exception" << endl;
  return 2;
}