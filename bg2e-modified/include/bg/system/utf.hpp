/*
 *	bg2 engine license
 *	Copyright (c) 2016 Fernando Serrano <ferserc1@gmail.com>
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *	of the Software, and to permit persons to whom the Software is furnished to do
 *	so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 *	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _bg2e_system_utf_hpp_
#define _bg2e_system_utf_hpp_

#include <bg/export.hpp>

#include <string>
#include <functional>

namespace bg {
namespace system {

typedef unsigned long UTFCode;

namespace utf {

enum HexCodes {
	kCodeTab				= 0x09, //		09	<control>	\t
	kCodeReturn				= 0x0A,	//  	0a	<control>	\r
	kCodeNewLine			= 0x0D,	//  	0d	<control>	\n
	kCodeSpace				= 0x20,	// 	20	SPACE
	kCodeExclamation		= 0x21,	// !	21	EXCLAMATION MARK
	kCodeQuotation			= 0x22,	// "	22	QUOTATION MARK
	kCodeNumberSign			= 0x23,	// #	23	NUMBER SIGN
	kCodeDollar				= 0x24,	// $	24	DOLLAR SIGN
	kCodePercent			= 0x25,	// %	25	PERCENT SIGN
	kCodeAmpersand			= 0x26,	// &	26	AMPERSAND
	kCodeAposthrophe		= 0x27,	// '	27	APOSTROPHE
	kCodeLeftParenthesis	= 0x28,	// (	28	LEFT PARENTHESIS
	kCodeRightParenthesis	= 0x29,	// )	29	RIGHT PARENTHESIS
	kCodeAsterisk			= 0x2A,	// *	2a	ASTERISK
	kCodePlusSign			= 0x2B,	// +	2b	PLUS SIGN
	kCodeComma				= 0x2C,	// ,	2c	COMMA
	kCodeMinusSign			= 0x2D,	// -	2d	HYPHEN-MINUS
	kCodeFullStop			= 0x2E,	// .	2e	FULL STOP
	kCodeSolidus			= 0x2F,	// /	2f	SOLIDUS
	kCode0					= 0x30,	// 0	30	DIGIT ZERO
	kCode1					= 0x31,	// 1	31	DIGIT ONE
	kCode2					= 0x32,	// 2	32	DIGIT TWO
	kCode3					= 0x33,	// 3	33	DIGIT THREE
	kCode4					= 0x34,	// 4	34	DIGIT FOUR
	kCode5					= 0x35,	// 5	35	DIGIT FIVE
	kCode6					= 0x36,	// 6	36	DIGIT SIX
	kCode7					= 0x37,	// 7	37	DIGIT SEVEN
	kCode8					= 0x38,	// 8	38	DIGIT EIGHT
	kCode9					= 0x39,	// 9	39	DIGIT NINE
	kCodeColon				= 0x3A,	// :	3a	COLON
	kCodeSemicolon			= 0x3B,	// ;	3b	SEMICOLON
	kCodeLesThan			= 0x3C,	// <	3c	LESS-THAN SIGN
	kCodeEquals				= 0x3D,	// =	3d	EQUALS SIGN
	kCodeGreaterThan		= 0x3E,	// >	3e	GREATER-THAN SIGN
	kCodeQuestionMark		= 0x3F,	// ?	3f	QUESTION MARK
	kCodeComercialAt		= 0x40,	// @	40	COMMERCIAL AT
	kCodeCapitalA			= 0x41,	// A	41	LATIN CAPITAL LETTER A
	kCodeCapitalB			= 0x42,	// B	42	LATIN CAPITAL LETTER B
	kCodeCapitalC			= 0x43,	// C	43	LATIN CAPITAL LETTER C
	kCodeCapitalD			= 0x44,	// D	44	LATIN CAPITAL LETTER D
	kCodeCapitalE			= 0x45,	// E	45	LATIN CAPITAL LETTER E
	kCodeCapitalF			= 0x46,	// F	46	LATIN CAPITAL LETTER F
	kCodeCapitalG			= 0x47,	// G	47	LATIN CAPITAL LETTER G
	kCodeCapitalH			= 0x48,	// H	48	LATIN CAPITAL LETTER H
	kCodeCapitalI			= 0x49,	// I	49	LATIN CAPITAL LETTER I
	kCodeCapitalJ			= 0x4A,	// J	4a	LATIN CAPITAL LETTER J
	kCodeCapitalK			= 0x4B,	// K	4b	LATIN CAPITAL LETTER K
	kCodeCapitalL			= 0x4C,	// L	4c	LATIN CAPITAL LETTER L
	kCodeCapitalM			= 0x4D,	// M	4d	LATIN CAPITAL LETTER M
	kCodeCapitalN			= 0x4E,	// N	4e	LATIN CAPITAL LETTER N
	kCodeCapitalO			= 0x4F,	// O	4f	LATIN CAPITAL LETTER O
	kCodeCapitalP			= 0x50,	// P	50	LATIN CAPITAL LETTER P
	kCodeCapitalQ			= 0x51,	// Q	51	LATIN CAPITAL LETTER Q
	kCodeCapitalR			= 0x52,	// R	52	LATIN CAPITAL LETTER R
	kCodeCapitalS			= 0x53,	// S	53	LATIN CAPITAL LETTER S
	kCodeCapitalT			= 0x54,	// T	54	LATIN CAPITAL LETTER T
	kCodeCapitalU			= 0x55,	// U	55	LATIN CAPITAL LETTER U
	kCodeCapitalV			= 0x56,	// V	56	LATIN CAPITAL LETTER V
	kCodeCapitalW			= 0x57,	// W	57	LATIN CAPITAL LETTER W
	kCodeCapitalX			= 0x58,	// X	58	LATIN CAPITAL LETTER X
	kCodeCapitalY			= 0x59,	// Y	59	LATIN CAPITAL LETTER Y
	kCodeCapitalZ			= 0x5A,	// Z	5a	LATIN CAPITAL LETTER Z
	kCodeLeftSquareBracket	= 0x5B,	// [	5b	LEFT SQUARE BRACKET
	kCodeReverseSolidus		= 0x5C,	// \	5c	REVERSE SOLIDUS
	kCodeRightSquareBracket	= 0x5D,	// ]	5d	RIGHT SQUARE BRACKET
	kCodeCircumflexAccent	= 0x5E,	// ^	5e	CIRCUMFLEX ACCENT
	kCodeLowLine			= 0x5F,	// _	5f	LOW LINE
	kCodeGraveAccent		= 0x60,	// `	60	GRAVE ACCENT
	kCodeSmallA				= 0x61,	// a	61	LATIN SMALL LETTER A
	kCodeSmallB				= 0x62,	// b	62	LATIN SMALL LETTER B
	kCodeSmallC				= 0x63,	// c	63	LATIN SMALL LETTER C
	kCodeSmallD				= 0x64,	// d	64	LATIN SMALL LETTER D
	kCodeSmallE				= 0x65,	// e	65	LATIN SMALL LETTER E
	kCodeSmallF				= 0x66,	// f	66	LATIN SMALL LETTER F
	kCodeSmallG				= 0x67,	// g	67	LATIN SMALL LETTER G
	kCodeSmallH				= 0x68,	// h	68	LATIN SMALL LETTER H
	kCodeSmallI				= 0x69,	// i	69	LATIN SMALL LETTER I
	kCodeSmallJ				= 0x6A,	// j	6a	LATIN SMALL LETTER J
	kCodeSmallK				= 0x6B,	// k	6b	LATIN SMALL LETTER K
	kCodeSmallL				= 0x6C,	// l	6c	LATIN SMALL LETTER L
	kCodeSmallM				= 0x6D,	// m	6d	LATIN SMALL LETTER M
	kCodeSmallN				= 0x6E,	// n	6e	LATIN SMALL LETTER N
	kCodeSmallO				= 0x6F,	// o	6f	LATIN SMALL LETTER O
	kCodeSmallP				= 0x70,	// p	70	LATIN SMALL LETTER P
	kCodeSmallQ				= 0x71,	// q	71	LATIN SMALL LETTER Q
	kCodeSmallR				= 0x72,	// r	72	LATIN SMALL LETTER R
	kCodeSmallS				= 0x73,	// s	73	LATIN SMALL LETTER S
	kCodeSmallT				= 0x74,	// t	74	LATIN SMALL LETTER T
	kCodeSmallU				= 0x75,	// u	75	LATIN SMALL LETTER U
	kCodeSmallV				= 0x76,	// v	76	LATIN SMALL LETTER V
	kCodeSmallW				= 0x77,	// w	77	LATIN SMALL LETTER W
	kCodeSmallX				= 0x78,	// x	78	LATIN SMALL LETTER X
	kCodeSmallY				= 0x79,	// y	79	LATIN SMALL LETTER Y
	kCodeSmallZ				= 0x7A,	// z	7a	LATIN SMALL LETTER Z
	kCodeLeftCurlyBracket	= 0x7B,	// {	7b	LEFT CURLY BRACKET
	kCodeVerticalLine		= 0x7C,	// |	7c	VERTICAL LINE
	kCodeRightCurlyBracket	= 0x7D,	// }	7d	RIGHT CURLY BRACKET
	kCodeTilde				= 0x7E	// ~	7e	TILDE
};

BG2E_EXPORT extern bool isControlCode(UTFCode hex);
BG2E_EXPORT extern size_t stringLenght(const std::string & input);
BG2E_EXPORT extern void iterateString(const std::string & input, std::function<void(std::string & multichar, UTFCode codepoint, UTFCode hex)> cb);
// Similar to JavaScript Array.some(): checks if some character meets the condition defined in the lambda closure
BG2E_EXPORT extern bool checkSome(const std::string & input, std::function<bool(std::string & multichar, UTFCode codepoint, UTFCode hex)> cb);
// Similar to JavaScript Array.every(): checks if every character meets the condition defined in the lambda closure
BG2E_EXPORT extern bool checkEvery(const std::string & input, std::function<bool(std::string & multichar, UTFCode codepoint, UTFCode hex)> cb);
BG2E_EXPORT extern std::string multibyteFromCodepoint(UTFCode cp);
BG2E_EXPORT extern UTFCode codepointFromMultibyte(const std::string & multibyte);

}

}
}

#endif	// _bg2e_export_hpp_


