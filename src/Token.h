#pragma once

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