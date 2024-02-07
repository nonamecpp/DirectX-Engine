#pragma once
#include<windows.h>
#include<string.h>
#include<math.h>
#include<algorithm>
#include<stdio.h>
#include<iostream>
using namespace std;

template<typename IntType>string writeint_str(IntType x);
string writefloat_str(float x, int _dot);
string writedouble_str(double x, int _dot);