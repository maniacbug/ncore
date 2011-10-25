#include <string>
#include <algorithm>
#include <stdlib.h>
#include <string.h>
	
/**
	 * C++ version 0.4 std::string style "itoa":
	 * Contributions from Stuart Lowe, Ray-Yuan Sheu,
	 * Rodrigo de Salvo Braz, Luc Gallant, John Maloney
	 * and Brian Hunt
	 */
template <class T>
	std::string __toa(T value, int base) {
	
		std::string buf;
	
		// check that the base if valid
		if (base < 2 || base > 16) return buf;

		enum { kMaxDigits = 35 };
		buf.reserve( kMaxDigits ); // Pre-allocate enough space.
	
		int quotient = value;
	
		// Translating number to string with base:
		do {
			buf += "0123456789abcdef"[ std::abs( quotient % base ) ];
			quotient /= base;
		} while ( quotient );
	
		// Append the negative sign
		if ( value < 0) buf += '-';
	
		std::reverse( buf.begin(), buf.end() );
		return buf;
	}

template <class T>
	std::string __utoa(T value, int base) {
	
		std::string buf;
	
		// check that the base if valid
		if (base < 2 || base > 16) return buf;

		enum { kMaxDigits = 35 };
		buf.reserve( kMaxDigits ); // Pre-allocate enough space.
	
		int quotient = value;
	
		// Translating number to string with base:
		do {
			buf += "0123456789abcdef"[ std::abs( quotient % base ) ];
			quotient /= base;
		} while ( quotient );
	
		std::reverse( buf.begin(), buf.end() );
		return buf;
	}

char* ultoa( unsigned long __val, char* __s, int __radix )
{
	std::string result = __utoa(__val,__radix);
	strcpy(__s,result.c_str());

	return __s;
}
char* ltoa( long __val, char* __s, int __radix )
{
	std::string result = __toa(__val,__radix);
	strcpy(__s,result.c_str());

	return __s;
}
char* itoa( int __val, char* __s, int __radix )
{
	std::string result = __toa(__val,__radix);
	strcpy(__s,result.c_str());

	return __s;
}
char* utoa( unsigned int __val, char* __s, int __radix )
{
	std::string result = __utoa(__val,__radix);
	strcpy(__s,result.c_str());

	return __s;
}
