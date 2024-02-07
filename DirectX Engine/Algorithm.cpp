#include"EngineAlgorithm.h"

template<typename IntType>string writeint_str(IntType x) {
	string out = "";
	bool f = (x < 0);
	if (x < 0)x *= -1;
	while (x > 0) {
		out = char(x % 10 + 48) + out;
		x /= 10;
	}
	return f ? ('-' + out) : out;
}
string writefloat_str(float x, int _dot) {
	string out = "";
	bool f = (x < 0);
	if (x < 0)x *= -1;
	int itx = int(x);
	x -= itx;
	while (itx > 0) {
		out = char(itx % 10 + 48) + out;
		itx /= 10;
	}
	out += '.';
	if (_dot == 0)
		return f ? ('-' + out) : out;
	for (int i = 1; i <= _dot; i++) {
		out = out + char(48 + int(x * 10));
		x = x * 10 - int(x * 10);
	}
	return f ? ('-' + out) : out;
}
string writedouble_str(double x, int _dot) {
	string out = "";
	bool f = (x < 0);
	if (x < 0)x *= -1;
	long long itx = (long long)(x);
	x -= itx;
	while (itx > 0) {
		out = char(itx % 10 + 48) + out;
		itx /= 10;
	}
	out += '.';
	if (_dot == 0)
		return f ? ('-' + out) : out;
	for (int i = 1; i <= _dot; i++) {
		out = out + char(48 + (long long)(x * 10));
		x = x * 10 - (long long)(x * 10);
	}
	return f ? ('-' + out) : out;
}
