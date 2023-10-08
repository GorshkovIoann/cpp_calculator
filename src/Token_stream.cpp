#include "Token_stream.h"

constexpr char quit = 'q';
constexpr char print = ';';
constexpr char number = '8';
constexpr char name = 'a';
constexpr char let = 'L';

const string prompt = "> ";
const string result = "= ";
const string declkey = "let";

void Token_stream::putback(Token t)
{
  if (full)
    error("putback() into a full buffer");

  buffer = t;
  full = true;
}

Token Token_stream::get()  // обрабатывает полученный символ как токен
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
    cin.putback(ch);  // возвращаем в поток и считываем все число
    double val;
    cin >> val;
    return Token{number, val};
  }

  default:  // проверка коректности имени введенной переменной
    if (isalpha(ch))  // первая буква?
    {
      string s;
      s += ch;
      while (cin.get(ch) && (isalpha(ch) || isdigit(ch)))  // считали слово
        s += ch;
      cin.putback(ch);

      if (s == declkey)
        return Token{let};

      return Token{name, s};
    }
    error("Bad token");
  }
}

void Token_stream::ignore(char c)  // c represents the kind of Token
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
