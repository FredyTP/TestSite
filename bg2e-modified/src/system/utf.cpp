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


#include <bg/system/utf.hpp>

#include <map>
#include <sstream>
#include <iostream>

namespace bg {
namespace system {
namespace utf {

/*
static UTFCode UTFTable[] = {
	0x00,	//  	00	<control>
	0x01,	//  	01	<control>
	0x02,	//  	02	<control>
	0x03,	//  	03	<control>
	0x04,	//  	04	<control>
	0x05,	//  	05	<control>
	0x06,	//  	06	<control>
	0x07,	//  	07	<control>
	0x08,	//  	08	<control>
	0x09,	//  	09	<control>	\t
	0x0A,	//  	0a	<control>	\r
	0x0B,	//  	0b	<control>
	0x0C,	//  	0c	<control>
	0x0D,	//  	0d	<control>	\n
	0x0E,	//  	0e	<control>
	0x0F,	//  	0f	<control>
	0x10,	//  	10	<control>
	0x11,	//  	11	<control>
	0x12,	//  	12	<control>
	0x13,	//  	13	<control>
	0x14,	//  	14	<control>
	0x15,	//  	15	<control>
	0x16,	//  	16	<control>
	0x17,	//  	17	<control>
	0x18,	//  	18	<control>
	0x19,	//  	19	<control>
	0x1A,	//  	1a	<control>
	0x1B,	//  	1b	<control>
	0x1C,	//  	1c	<control>
	0x1D,	//  	1d	<control>
	0x1E,	//  	1e	<control>
	0x1F,	//  	1f	<control>
	0x20,	// 	20	SPACE
	0x21,	// !	21	EXCLAMATION MARK
	0x22,	// "	22	QUOTATION MARK
	0x23,	// #	23	NUMBER SIGN
	0x24,	// $	24	DOLLAR SIGN
	0x25,	// %	25	PERCENT SIGN
	0x26,	// &	26	AMPERSAND
	0x27,	// '	27	APOSTROPHE
	0x28,	// (	28	LEFT PARENTHESIS
	0x29,	// )	29	RIGHT PARENTHESIS
	0x2A,	// *	2a	ASTERISK
	0x2B,	// +	2b	PLUS SIGN
	0x2C,	// ,	2c	COMMA
	0x2D,	// -	2d	HYPHEN-MINUS
	0x2E,	// .	2e	FULL STOP
	0x2F,	// /	2f	SOLIDUS
	0x30,	// 0	30	DIGIT ZERO
	0x31,	// 1	31	DIGIT ONE
	0x32,	// 2	32	DIGIT TWO
	0x33,	// 3	33	DIGIT THREE
	0x34,	// 4	34	DIGIT FOUR
	0x35,	// 5	35	DIGIT FIVE
	0x36,	// 6	36	DIGIT SIX
	0x37,	// 7	37	DIGIT SEVEN
	0x38,	// 8	38	DIGIT EIGHT
	0x39,	// 9	39	DIGIT NINE
	0x3A,	// :	3a	COLON
	0x3B,	// ;	3b	SEMICOLON
	0x3C,	// <	3c	LESS-THAN SIGN
	0x3D,	// =	3d	EQUALS SIGN
	0x3E,	// >	3e	GREATER-THAN SIGN
	0x3F,	// ?	3f	QUESTION MARK
	0x40,	// @	40	COMMERCIAL AT
	0x41,	// A	41	LATIN CAPITAL LETTER A
	0x42,	// B	42	LATIN CAPITAL LETTER B
	0x43,	// C	43	LATIN CAPITAL LETTER C
	0x44,	// D	44	LATIN CAPITAL LETTER D
	0x45,	// E	45	LATIN CAPITAL LETTER E
	0x46,	// F	46	LATIN CAPITAL LETTER F
	0x47,	// G	47	LATIN CAPITAL LETTER G
	0x48,	// H	48	LATIN CAPITAL LETTER H
	0x49,	// I	49	LATIN CAPITAL LETTER I
	0x4A,	// J	4a	LATIN CAPITAL LETTER J
	0x4B,	// K	4b	LATIN CAPITAL LETTER K
	0x4C,	// L	4c	LATIN CAPITAL LETTER L
	0x4D,	// M	4d	LATIN CAPITAL LETTER M
	0x4E,	// N	4e	LATIN CAPITAL LETTER N
	0x4F,	// O	4f	LATIN CAPITAL LETTER O
	0x50,	// P	50	LATIN CAPITAL LETTER P
	0x51,	// Q	51	LATIN CAPITAL LETTER Q
	0x52,	// R	52	LATIN CAPITAL LETTER R
	0x53,	// S	53	LATIN CAPITAL LETTER S
	0x54,	// T	54	LATIN CAPITAL LETTER T
	0x55,	// U	55	LATIN CAPITAL LETTER U
	0x56,	// V	56	LATIN CAPITAL LETTER V
	0x57,	// W	57	LATIN CAPITAL LETTER W
	0x58,	// X	58	LATIN CAPITAL LETTER X
	0x59,	// Y	59	LATIN CAPITAL LETTER Y
	0x5A,	// Z	5a	LATIN CAPITAL LETTER Z
	0x5B,	// [	5b	LEFT SQUARE BRACKET
	0x5C,	// \	5c	REVERSE SOLIDUS
	0x5D,	// ]	5d	RIGHT SQUARE BRACKET
	0x5E,	// ^	5e	CIRCUMFLEX ACCENT
	0x5F,	// _	5f	LOW LINE
	0x60,	// `	60	GRAVE ACCENT
	0x61,	// a	61	LATIN SMALL LETTER A
	0x62,	// b	62	LATIN SMALL LETTER B
	0x63,	// c	63	LATIN SMALL LETTER C
	0x64,	// d	64	LATIN SMALL LETTER D
	0x65,	// e	65	LATIN SMALL LETTER E
	0x66,	// f	66	LATIN SMALL LETTER F
	0x67,	// g	67	LATIN SMALL LETTER G
	0x68,	// h	68	LATIN SMALL LETTER H
	0x69,	// i	69	LATIN SMALL LETTER I
	0x6A,	// j	6a	LATIN SMALL LETTER J
	0x6B,	// k	6b	LATIN SMALL LETTER K
	0x6C,	// l	6c	LATIN SMALL LETTER L
	0x6D,	// m	6d	LATIN SMALL LETTER M
	0x6E,	// n	6e	LATIN SMALL LETTER N
	0x6F,	// o	6f	LATIN SMALL LETTER O
	0x70,	// p	70	LATIN SMALL LETTER P
	0x71,	// q	71	LATIN SMALL LETTER Q
	0x72,	// r	72	LATIN SMALL LETTER R
	0x73,	// s	73	LATIN SMALL LETTER S
	0x74,	// t	74	LATIN SMALL LETTER T
	0x75,	// u	75	LATIN SMALL LETTER U
	0x76,	// v	76	LATIN SMALL LETTER V
	0x77,	// w	77	LATIN SMALL LETTER W
	0x78,	// x	78	LATIN SMALL LETTER X
	0x79,	// y	79	LATIN SMALL LETTER Y
	0x7A,	// z	7a	LATIN SMALL LETTER Z
	0x7B,	// {	7b	LEFT CURLY BRACKET
	0x7C,	// |	7c	VERTICAL LINE
	0x7D,	// }	7d	RIGHT CURLY BRACKET
	0x7E,	// ~	7e	TILDE
	0x7F,	//  	7f	<control>
	0x80,	//  	c2 80	<control>
	0x81,	//  	c2 81	<control>
	0x82,	//  	c2 82	<control>
	0x83,	//  	c2 83	<control>
	0x84,	//  	c2 84	<control>
	0x85,	//  	c2 85	<control>
	0x86,	//  	c2 86	<control>
	0x87,	//  	c2 87	<control>
	0x88,	//  	c2 88	<control>
	0x89,	//  	c2 89	<control>
	0x8A,	//  	c2 8a	<control>
	0x8B,	//  	c2 8b	<control>
	0x8C,	//  	c2 8c	<control>
	0x8D,	//  	c2 8d	<control>
	0x8E,	//  	c2 8e	<control>
	0x8F,	//  	c2 8f	<control>
	0x90,	//  	c2 90	<control>
	0x91,	//  	c2 91	<control>
	0x92,	//  	c2 92	<control>
	0x93,	//  	c2 93	<control>
	0x94,	//  	c2 94	<control>
	0x95,	//  	c2 95	<control>
	0x96,	//  	c2 96	<control>
	0x97,	//  	c2 97	<control>
	0x98,	//  	c2 98	<control>
	0x99,	//  	c2 99	<control>
	0x9A,	//  	c2 9a	<control>
	0x9B,	//  	c2 9b	<control>
	0x9C,	//  	c2 9c	<control>
	0x9D,	//  	c2 9d	<control>
	0x9E,	//  	c2 9e	<control>
	0x9F,	//  	c2 9f	<control>
	0xA0,	//  	c2 a0	NO-BREAK SPACE
	0xA1,	// ¡	c2 a1	INVERTED EXCLAMATION MARK
	0xA2,	// ¢	c2 a2	CENT SIGN
	0xA3,	// £	c2 a3	POUND SIGN
	0xA4,	// ¤	c2 a4	CURRENCY SIGN
	0xA5,	// ¥	c2 a5	YEN SIGN
	0xA6,	// ¦	c2 a6	BROKEN BAR
	0xA7,	// §	c2 a7	SECTION SIGN
	0xA8,	// ¨	c2 a8	DIAERESIS
	0xA9,	// ©	c2 a9	COPYRIGHT SIGN
	0xAA,	// ª	c2 aa	FEMININE ORDINAL INDICATOR
	0xAB,	// «	c2 ab	LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
	0xAC,	// ¬	c2 ac	NOT SIGN
	0xAD,	// 	c2 ad	SOFT HYPHEN
	0xAE,	// ®	c2 ae	REGISTERED SIGN
	0xAF,	// ¯	c2 af	MACRON
	0xB0,	// °	c2 b0	DEGREE SIGN
	0xB1,	// ±	c2 b1	PLUS-MINUS SIGN
	0xB2,	// ²	c2 b2	SUPERSCRIPT TWO
	0xB3,	// ³	c2 b3	SUPERSCRIPT THREE
	0xB4,	// ´	c2 b4	ACUTE ACCENT
	0xB5,	// µ	c2 b5	MICRO SIGN
	0xB6,	// ¶	c2 b6	PILCROW SIGN
	0xB7,	// ·	c2 b7	MIDDLE DOT
	0xB8,	// ¸	c2 b8	CEDILLA
	0xB9,	// ¹	c2 b9	SUPERSCRIPT ONE
	0xBA,	// º	c2 ba	MASCULINE ORDINAL INDICATOR
	0xBB,	// »	c2 bb	RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
	0xBC,	// ¼	c2 bc	VULGAR FRACTION ONE QUARTER
	0xBD,	// ½	c2 bd	VULGAR FRACTION ONE HALF
	0xBE,	// ¾	c2 be	VULGAR FRACTION THREE QUARTERS
	0xBF,	// ¿	c2 bf	INVERTED QUESTION MARK
	0xC0,	// À	c3 80	LATIN CAPITAL LETTER A WITH GRAVE
	0xC1,	// Á	c3 81	LATIN CAPITAL LETTER A WITH ACUTE
	0xC2,	// Â	c3 82	LATIN CAPITAL LETTER A WITH CIRCUMFLEX
	0xC3,	// Ã	c3 83	LATIN CAPITAL LETTER A WITH TILDE
	0xC4,	// Ä	c3 84	LATIN CAPITAL LETTER A WITH DIAERESIS
	0xC5,	// Å	c3 85	LATIN CAPITAL LETTER A WITH RING ABOVE
	0xC6,	// Æ	c3 86	LATIN CAPITAL LETTER AE
	0xC7,	// Ç	c3 87	LATIN CAPITAL LETTER C WITH CEDILLA
	0xC8,	// È	c3 88	LATIN CAPITAL LETTER E WITH GRAVE
	0xC9,	// É	c3 89	LATIN CAPITAL LETTER E WITH ACUTE
	0xCA,	// Ê	c3 8a	LATIN CAPITAL LETTER E WITH CIRCUMFLEX
	0xCB,	// Ë	c3 8b	LATIN CAPITAL LETTER E WITH DIAERESIS
	0xCC,	// Ì	c3 8c	LATIN CAPITAL LETTER I WITH GRAVE
	0xCD,	// Í	c3 8d	LATIN CAPITAL LETTER I WITH ACUTE
	0xCE,	// Î	c3 8e	LATIN CAPITAL LETTER I WITH CIRCUMFLEX
	0xCF,	// Ï	c3 8f	LATIN CAPITAL LETTER I WITH DIAERESIS
	0xD0,	// Ð	c3 90	LATIN CAPITAL LETTER ETH
	0xD1,	// Ñ	c3 91	LATIN CAPITAL LETTER N WITH TILDE
	0xD2,	// Ò	c3 92	LATIN CAPITAL LETTER O WITH GRAVE
	0xD3,	// Ó	c3 93	LATIN CAPITAL LETTER O WITH ACUTE
	0xD4,	// Ô	c3 94	LATIN CAPITAL LETTER O WITH CIRCUMFLEX
	0xD5,	// Õ	c3 95	LATIN CAPITAL LETTER O WITH TILDE
	0xD6,	// Ö	c3 96	LATIN CAPITAL LETTER O WITH DIAERESIS
	0xD7,	// ×	c3 97	MULTIPLICATION SIGN
	0xD8,	// Ø	c3 98	LATIN CAPITAL LETTER O WITH STROKE
	0xD9,	// Ù	c3 99	LATIN CAPITAL LETTER U WITH GRAVE
	0xDA,	// Ú	c3 9a	LATIN CAPITAL LETTER U WITH ACUTE
	0xDB,	// Û	c3 9b	LATIN CAPITAL LETTER U WITH CIRCUMFLEX
	0xDC,	// Ü	c3 9c	LATIN CAPITAL LETTER U WITH DIAERESIS
	0xDD,	// Ý	c3 9d	LATIN CAPITAL LETTER Y WITH ACUTE
	0xDE,	// Þ	c3 9e	LATIN CAPITAL LETTER THORN
	0xDF,	// ß	c3 9f	LATIN SMALL LETTER SHARP S
	0xE0,	// à	c3 a0	LATIN SMALL LETTER A WITH GRAVE
	0xE1,	// á	c3 a1	LATIN SMALL LETTER A WITH ACUTE
	0xE2,	// â	c3 a2	LATIN SMALL LETTER A WITH CIRCUMFLEX
	0xE3,	// ã	c3 a3	LATIN SMALL LETTER A WITH TILDE
	0xE4,	// ä	c3 a4	LATIN SMALL LETTER A WITH DIAERESIS
	0xE5,	// å	c3 a5	LATIN SMALL LETTER A WITH RING ABOVE
	0xE6,	// æ	c3 a6	LATIN SMALL LETTER AE
	0xE7,	// ç	c3 a7	LATIN SMALL LETTER C WITH CEDILLA
	0xE8,	// è	c3 a8	LATIN SMALL LETTER E WITH GRAVE
	0xE9,	// é	c3 a9	LATIN SMALL LETTER E WITH ACUTE
	0xEA,	// ê	c3 aa	LATIN SMALL LETTER E WITH CIRCUMFLEX
	0xEB,	// ë	c3 ab	LATIN SMALL LETTER E WITH DIAERESIS
	0xEC,	// ì	c3 ac	LATIN SMALL LETTER I WITH GRAVE
	0xED,	// í	c3 ad	LATIN SMALL LETTER I WITH ACUTE
	0xEE,	// î	c3 ae	LATIN SMALL LETTER I WITH CIRCUMFLEX
	0xEF,	// ï	c3 af	LATIN SMALL LETTER I WITH DIAERESIS
	0xF0,	// ð	c3 b0	LATIN SMALL LETTER ETH
	0xF1,	// ñ	c3 b1	LATIN SMALL LETTER N WITH TILDE
	0xF2,	// ò	c3 b2	LATIN SMALL LETTER O WITH GRAVE
	0xF3,	// ó	c3 b3	LATIN SMALL LETTER O WITH ACUTE
	0xF4,	// ô	c3 b4	LATIN SMALL LETTER O WITH CIRCUMFLEX
	0xF5,	// õ	c3 b5	LATIN SMALL LETTER O WITH TILDE
	0xF6,	// ö	c3 b6	LATIN SMALL LETTER O WITH DIAERESIS
	0xF7,	// ÷	c3 b7	DIVISION SIGN
	0xF8,	// ø	c3 b8	LATIN SMALL LETTER O WITH STROKE
	0xF9,	// ù	c3 b9	LATIN SMALL LETTER U WITH GRAVE
	0xFA,	// ú	c3 ba	LATIN SMALL LETTER U WITH ACUTE
	0xFB,	// û	c3 bb	LATIN SMALL LETTER U WITH CIRCUMFLEX
	0xFC,	// ü	c3 bc	LATIN SMALL LETTER U WITH DIAERESIS
	0xFD,	// ý	c3 bd	LATIN SMALL LETTER Y WITH ACUTE
	0xFE,	// þ	c3 be	LATIN SMALL LETTER THORN
	0xFF	// ÿ	c3 bf	LATIN SMALL LETTER Y WITH DIAERESIS
};
*/

static std::map<UTFCode, UTFCode> CodepointToUtf = {
	{ 0x00, 0x00 },		//	<control>
	{ 0x01, 0x01 },		//	<control>
	{ 0x02, 0x02 },		//	<control>
	{ 0x03, 0x03 },		//	<control>
	{ 0x04, 0x04 },		//	<control>
	{ 0x05, 0x05 },		//	<control>
	{ 0x06, 0x06 },		//	<control>
	{ 0x07, 0x07 },		//	<control>
	{ 0x08, 0x08 },		//	<control>
	{ 0x09, 0x09 },		//	<control>
	{ 0x0A, 0x0a },		//	<control>
	{ 0x0B, 0x0b },		//	<control>
	{ 0x0C, 0x0c },		//	<control>
	{ 0x0D, 0x0d },		//	<control>
	{ 0x0E, 0x0e },		//	<control>
	{ 0x0F, 0x0f },		//	<control>
	{ 0x10, 0x10 },		//	<control>
	{ 0x11, 0x11 },		//	<control>
	{ 0x12, 0x12 },		//	<control>
	{ 0x13, 0x13 },		//	<control>
	{ 0x14, 0x14 },		//	<control>
	{ 0x15, 0x15 },		//	<control>
	{ 0x16, 0x16 },		//	<control>
	{ 0x17, 0x17 },		//	<control>
	{ 0x18, 0x18 },		//	<control>
	{ 0x19, 0x19 },		//	<control>
	{ 0x1A, 0x1a },		//	<control>
	{ 0x1B, 0x1b },		//	<control>
	{ 0x1C, 0x1c },		//	<control>
	{ 0x1D, 0x1d },		//	<control>
	{ 0x1E, 0x1e },		//	<control>
	{ 0x1F, 0x1f },		//	<control>
	{ 0x20, 0x20 },		//
	{ 0x21, 0x21 },		//	EXCLAMATION MARK
	{ 0x22, 0x22 },		//	QUOTATION MARK
	{ 0x23, 0x23 },		//	NUMBER SIGN
	{ 0x24, 0x24 },		//	DOLLAR SIGN
	{ 0x25, 0x25 },		//	PERCENT SIGN
	{ 0x26, 0x26 },		//	AMPERSAND
	{ 0x27, 0x27 },		//	APOSTROPHE
	{ 0x28, 0x28 },		//	LEFT PARENTHESIS
	{ 0x29, 0x29 },		//	RIGHT PARENTHESIS
	{ 0x2A, 0x2a },		//	ASTERISK
	{ 0x2B, 0x2b },		//	PLUS SIGN
	{ 0x2C, 0x2c },		//	COMMA
	{ 0x2D, 0x2d },		//	HYPHEN-MINUS
	{ 0x2E, 0x2e },		//	FULL STOP
	{ 0x2F, 0x2f },		//	SOLIDUS
	{ 0x30, 0x30 },		//	DIGIT ZERO
	{ 0x31, 0x31 },		//	DIGIT ONE
	{ 0x32, 0x32 },		//	DIGIT TWO
	{ 0x33, 0x33 },		//	DIGIT THREE
	{ 0x34, 0x34 },		//	DIGIT FOUR
	{ 0x35, 0x35 },		//	DIGIT FIVE
	{ 0x36, 0x36 },		//	DIGIT SIX
	{ 0x37, 0x37 },		//	DIGIT SEVEN
	{ 0x38, 0x38 },		//	DIGIT EIGHT
	{ 0x39, 0x39 },		//	DIGIT NINE
	{ 0x3A, 0x3a },		//	COLON
	{ 0x3B, 0x3b },		//	SEMICOLON
	{ 0x3C, 0x3c },		//	LESS-THAN SIGN
	{ 0x3D, 0x3d },		//	EQUALS SIGN
	{ 0x3E, 0x3e },		//	GREATER-THAN SIGN
	{ 0x3F, 0x3f },		//	QUESTION MARK
	{ 0x40, 0x40 },		//	COMMERCIAL AT
	{ 0x41, 0x41 },		//	LATIN CAPITAL LETTER A
	{ 0x42, 0x42 },		//	LATIN CAPITAL LETTER B
	{ 0x43, 0x43 },		//	LATIN CAPITAL LETTER C
	{ 0x44, 0x44 },		//	LATIN CAPITAL LETTER D
	{ 0x45, 0x45 },		//	LATIN CAPITAL LETTER E
	{ 0x46, 0x46 },		//	LATIN CAPITAL LETTER F
	{ 0x47, 0x47 },		//	LATIN CAPITAL LETTER G
	{ 0x48, 0x48 },		//	LATIN CAPITAL LETTER H
	{ 0x49, 0x49 },		//	LATIN CAPITAL LETTER I
	{ 0x4A, 0x4a },		//	LATIN CAPITAL LETTER J
	{ 0x4B, 0x4b },		//	LATIN CAPITAL LETTER K
	{ 0x4C, 0x4c },		//	LATIN CAPITAL LETTER L
	{ 0x4D, 0x4d },		//	LATIN CAPITAL LETTER M
	{ 0x4E, 0x4e },		//	LATIN CAPITAL LETTER N
	{ 0x4F, 0x4f },		//	LATIN CAPITAL LETTER O
	{ 0x50, 0x50 },		//	LATIN CAPITAL LETTER P
	{ 0x51, 0x51 },		//	LATIN CAPITAL LETTER Q
	{ 0x52, 0x52 },		//	LATIN CAPITAL LETTER R
	{ 0x53, 0x53 },		//	LATIN CAPITAL LETTER S
	{ 0x54, 0x54 },		//	LATIN CAPITAL LETTER T
	{ 0x55, 0x55 },		//	LATIN CAPITAL LETTER U
	{ 0x56, 0x56 },		//	LATIN CAPITAL LETTER V
	{ 0x57, 0x57 },		//	LATIN CAPITAL LETTER W
	{ 0x58, 0x58 },		//	LATIN CAPITAL LETTER X
	{ 0x59, 0x59 },		//	LATIN CAPITAL LETTER Y
	{ 0x5A, 0x5a },		//	LATIN CAPITAL LETTER Z
	{ 0x5B, 0x5b },		//	LEFT SQUARE BRACKET
	{ 0x5C, 0x5c },		//	REVERSE SOLIDUS
	{ 0x5D, 0x5d },		//	RIGHT SQUARE BRACKET
	{ 0x5E, 0x5e },		//	CIRCUMFLEX ACCENT
	{ 0x5F, 0x5f },		//	LOW LINE
	{ 0x60, 0x60 },		//	GRAVE ACCENT
	{ 0x61, 0x61 },		//	LATIN SMALL LETTER A
	{ 0x62, 0x62 },		//	LATIN SMALL LETTER B
	{ 0x63, 0x63 },		//	LATIN SMALL LETTER C
	{ 0x64, 0x64 },		//	LATIN SMALL LETTER D
	{ 0x65, 0x65 },		//	LATIN SMALL LETTER E
	{ 0x66, 0x66 },		//	LATIN SMALL LETTER F
	{ 0x67, 0x67 },		//	LATIN SMALL LETTER G
	{ 0x68, 0x68 },		//	LATIN SMALL LETTER H
	{ 0x69, 0x69 },		//	LATIN SMALL LETTER I
	{ 0x6A, 0x6a },		//	LATIN SMALL LETTER J
	{ 0x6B, 0x6b },		//	LATIN SMALL LETTER K
	{ 0x6C, 0x6c },		//	LATIN SMALL LETTER L
	{ 0x6D, 0x6d },		//	LATIN SMALL LETTER M
	{ 0x6E, 0x6e },		//	LATIN SMALL LETTER N
	{ 0x6F, 0x6f },		//	LATIN SMALL LETTER O
	{ 0x70, 0x70 },		//	LATIN SMALL LETTER P
	{ 0x71, 0x71 },		//	LATIN SMALL LETTER Q
	{ 0x72, 0x72 },		//	LATIN SMALL LETTER R
	{ 0x73, 0x73 },		//	LATIN SMALL LETTER S
	{ 0x74, 0x74 },		//	LATIN SMALL LETTER T
	{ 0x75, 0x75 },		//	LATIN SMALL LETTER U
	{ 0x76, 0x76 },		//	LATIN SMALL LETTER V
	{ 0x77, 0x77 },		//	LATIN SMALL LETTER W
	{ 0x78, 0x78 },		//	LATIN SMALL LETTER X
	{ 0x79, 0x79 },		//	LATIN SMALL LETTER Y
	{ 0x7A, 0x7a },		//	LATIN SMALL LETTER Z
	{ 0x7B, 0x7b },		//	LEFT CURLY BRACKET
	{ 0x7C, 0x7c },		//	VERTICAL LINE
	{ 0x7D, 0x7d },		//	RIGHT CURLY BRACKET
	{ 0x7E, 0x7e },		//	TILDE
	{ 0x7F, 0x7f },		//	<control>
	{ 0x80, 0xc280 },	//	<control>
	{ 0x81, 0xc281 },	//	<control>
	{ 0x82, 0xc282 },	//	<control>
	{ 0x83, 0xc283 },	//	<control>
	{ 0x84, 0xc284 },	//	<control>
	{ 0x85, 0xc285 },	//	<control>
	{ 0x86, 0xc286 },	//	<control>
	{ 0x87, 0xc287 },	//	<control>
	{ 0x88, 0xc288 },	//	<control>
	{ 0x89, 0xc289 },	//	<control>
	{ 0x8A, 0xc28a },	//	<control>
	{ 0x8B, 0xc28b },	//	<control>
	{ 0x8C, 0xc28c },	//	<control>
	{ 0x8D, 0xc28d },	//	<control>
	{ 0x8E, 0xc28e },	//	<control>
	{ 0x8F, 0xc28f },	//	<control>
	{ 0x90, 0xc290 },	//	<control>
	{ 0x91, 0xc291 },	//	<control>
	{ 0x92, 0xc292 },	//	<control>
	{ 0x93, 0xc293 },	//	<control>
	{ 0x94, 0xc294 },	//	<control>
	{ 0x95, 0xc295 },	//	<control>
	{ 0x96, 0xc296 },	//	<control>
	{ 0x97, 0xc297 },	//	<control>
	{ 0x98, 0xc298 },	//	<control>
	{ 0x99, 0xc299 },	//	<control>
	{ 0x9A, 0xc29a },	//	<control>
	{ 0x9B, 0xc29b },	//	<control>
	{ 0x9C, 0xc29c },	//	<control>
	{ 0x9D, 0xc29d },	//	<control>
	{ 0x9E, 0xc29e },	//	<control>
	{ 0x9F, 0xc29f },	//	<control>
	{ 0xA0, 0xc2a0 },	//	NO-BREAK SPACE
	{ 0xA1, 0xc2a1 },	//	INVERTED EXCLAMATION MARK
	{ 0xA2, 0xc2a2 },	//	CENT SIGN
	{ 0xA3, 0xc2a3 },	//	POUND SIGN
	{ 0xA4, 0xc2a4 },	//	CURRENCY SIGN
	{ 0xA5, 0xc2a5 },	//	YEN SIGN
	{ 0xA6, 0xc2a6 },	//	BROKEN BAR
	{ 0xA7, 0xc2a7 },	//	SECTION SIGN
	{ 0xA8, 0xc2a8 },	//	DIAERESIS
	{ 0xA9, 0xc2a9 },	//	COPYRIGHT SIGN
	{ 0xAA, 0xc2aa },	//	FEMININE ORDINAL INDICATOR
	{ 0xAB, 0xc2ab },	//	LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
	{ 0xAC, 0xc2ac },	//	NOT SIGN
	{ 0xAD, 0xc2ad },	//	SOFT HYPHEN
	{ 0xAE, 0xc2ae },	//	REGISTERED SIGN
	{ 0xAF, 0xc2af },	//	MACRON
	{ 0xB0, 0xc2b0 },	//	DEGREE SIGN
	{ 0xB1, 0xc2b1 },	//	PLUS-MINUS SIGN
	{ 0xB2, 0xc2b2 },	//	SUPERSCRIPT TWO
	{ 0xB3, 0xc2b3 },	//	SUPERSCRIPT THREE
	{ 0xB4, 0xc2b4 },	//	ACUTE ACCENT
	{ 0xB5, 0xc2b5 },	//	MICRO SIGN
	{ 0xB6, 0xc2b6 },	//	PILCROW SIGN
	{ 0xB7, 0xc2b7 },	//	MIDDLE DOT
	{ 0xB8, 0xc2b8 },	//	CEDILLA
	{ 0xB9, 0xc2b9 },	//	SUPERSCRIPT ONE
	{ 0xBA, 0xc2ba },	//	MASCULINE ORDINAL INDICATOR
	{ 0xBB, 0xc2bb },	//	RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
	{ 0xBC, 0xc2bc },	//	VULGAR FRACTION ONE QUARTER
	{ 0xBD, 0xc2bd },	//	VULGAR FRACTION ONE HALF
	{ 0xBE, 0xc2be },	//	VULGAR FRACTION THREE QUARTERS
	{ 0xBF, 0xc2bf },	//	INVERTED QUESTION MARK
	{ 0xC0, 0xc380 },	//	LATIN CAPITAL LETTER A WITH GRAVE
	{ 0xC1, 0xc381 },	//	LATIN CAPITAL LETTER A WITH ACUTE
	{ 0xC2, 0xc382 },	//	LATIN CAPITAL LETTER A WITH CIRCUMFLEX
	{ 0xC3, 0xc383 },	//	LATIN CAPITAL LETTER A WITH TILDE
	{ 0xC4, 0xc384 },	//	LATIN CAPITAL LETTER A WITH DIAERESIS
	{ 0xC5, 0xc385 },	//	LATIN CAPITAL LETTER A WITH RING ABOVE
	{ 0xC6, 0xc386 },	//	LATIN CAPITAL LETTER AE
	{ 0xC7, 0xc387 },	//	LATIN CAPITAL LETTER C WITH CEDILLA
	{ 0xC8, 0xc388 },	//	LATIN CAPITAL LETTER E WITH GRAVE
	{ 0xC9, 0xc389 },	//	LATIN CAPITAL LETTER E WITH ACUTE
	{ 0xCA, 0xc38a },	//	LATIN CAPITAL LETTER E WITH CIRCUMFLEX
	{ 0xCB, 0xc38b },	//	LATIN CAPITAL LETTER E WITH DIAERESIS
	{ 0xCC, 0xc38c },	//	LATIN CAPITAL LETTER I WITH GRAVE
	{ 0xCD, 0xc38d },	//	LATIN CAPITAL LETTER I WITH ACUTE
	{ 0xCE, 0xc38e },	//	LATIN CAPITAL LETTER I WITH CIRCUMFLEX
	{ 0xCF, 0xc38f },	//	LATIN CAPITAL LETTER I WITH DIAERESIS
	{ 0xD0, 0xc390 },	//	LATIN CAPITAL LETTER ETH
	{ 0xD1, 0xc391 },	//	LATIN CAPITAL LETTER N WITH TILDE
	{ 0xD2, 0xc392 },	//	LATIN CAPITAL LETTER O WITH GRAVE
	{ 0xD3, 0xc393 },	//	LATIN CAPITAL LETTER O WITH ACUTE
	{ 0xD4, 0xc394 },	//	LATIN CAPITAL LETTER O WITH CIRCUMFLEX
	{ 0xD5, 0xc395 },	//	LATIN CAPITAL LETTER O WITH TILDE
	{ 0xD6, 0xc396 },	//	LATIN CAPITAL LETTER O WITH DIAERESIS
	{ 0xD7, 0xc397 },	//	MULTIPLICATION SIGN
	{ 0xD8, 0xc398 },	//	LATIN CAPITAL LETTER O WITH STROKE
	{ 0xD9, 0xc399 },	//	LATIN CAPITAL LETTER U WITH GRAVE
	{ 0xDA, 0xc39a },	//	LATIN CAPITAL LETTER U WITH ACUTE
	{ 0xDB, 0xc39b },	//	LATIN CAPITAL LETTER U WITH CIRCUMFLEX
	{ 0xDC, 0xc39c },	//	LATIN CAPITAL LETTER U WITH DIAERESIS
	{ 0xDD, 0xc39d },	//	LATIN CAPITAL LETTER Y WITH ACUTE
	{ 0xDE, 0xc39e },	//	LATIN CAPITAL LETTER THORN
	{ 0xDF, 0xc39f },	//	LATIN SMALL LETTER SHARP S
	{ 0xE0, 0xc3a0 },	//	LATIN SMALL LETTER A WITH GRAVE
	{ 0xE1, 0xc3a1 },	//	LATIN SMALL LETTER A WITH ACUTE
	{ 0xE2, 0xc3a2 },	//	LATIN SMALL LETTER A WITH CIRCUMFLEX
	{ 0xE3, 0xc3a3 },	//	LATIN SMALL LETTER A WITH TILDE
	{ 0xE4, 0xc3a4 },	//	LATIN SMALL LETTER A WITH DIAERESIS
	{ 0xE5, 0xc3a5 },	//	LATIN SMALL LETTER A WITH RING ABOVE
	{ 0xE6, 0xc3a6 },	//	LATIN SMALL LETTER AE
	{ 0xE7, 0xc3a7 },	//	LATIN SMALL LETTER C WITH CEDILLA
	{ 0xE8, 0xc3a8 },	//	LATIN SMALL LETTER E WITH GRAVE
	{ 0xE9, 0xc3a9 },	//	LATIN SMALL LETTER E WITH ACUTE
	{ 0xEA, 0xc3aa },	//	LATIN SMALL LETTER E WITH CIRCUMFLEX
	{ 0xEB, 0xc3ab },	//	LATIN SMALL LETTER E WITH DIAERESIS
	{ 0xEC, 0xc3ac },	//	LATIN SMALL LETTER I WITH GRAVE
	{ 0xED, 0xc3ad },	//	LATIN SMALL LETTER I WITH ACUTE
	{ 0xEE, 0xc3ae },	//	LATIN SMALL LETTER I WITH CIRCUMFLEX
	{ 0xEF, 0xc3af },	//	LATIN SMALL LETTER I WITH DIAERESIS
	{ 0xF0, 0xc3b0 },	//	LATIN SMALL LETTER ETH
	{ 0xF1, 0xc3b1 },	//	LATIN SMALL LETTER N WITH TILDE
	{ 0xF2, 0xc3b2 },	//	LATIN SMALL LETTER O WITH GRAVE
	{ 0xF3, 0xc3b3 },	//	LATIN SMALL LETTER O WITH ACUTE
	{ 0xF4, 0xc3b4 },	//	LATIN SMALL LETTER O WITH CIRCUMFLEX
	{ 0xF5, 0xc3b5 },	//	LATIN SMALL LETTER O WITH TILDE
	{ 0xF6, 0xc3b6 },	//	LATIN SMALL LETTER O WITH DIAERESIS
	{ 0xF7, 0xc3b7 },	//	DIVISION SIGN
	{ 0xF8, 0xc3b8 },	//	LATIN SMALL LETTER O WITH STROKE
	{ 0xF9, 0xc3b9 },	//	LATIN SMALL LETTER U WITH GRAVE
	{ 0xFA, 0xc3ba },	//	LATIN SMALL LETTER U WITH ACUTE
	{ 0xFB, 0xc3bb },	//	LATIN SMALL LETTER U WITH CIRCUMFLEX
	{ 0xFC, 0xc3bc },	//	LATIN SMALL LETTER U WITH DIAERESIS
	{ 0xFD, 0xc3bd },	//	LATIN SMALL LETTER Y WITH ACUTE
	{ 0xFE, 0xc3be },	//	LATIN SMALL LETTER THORN
	{ 0xFF, 0xc3bf }	//	LATIN SMALL LETTER Y WITH DIAERESIS
};

static std::map<UTFCode, UTFCode> UtfToCodepoint = {
	{ 0x00, 0x00 },		//	<control>
	{ 0x01, 0x01 },		//	<control>
	{ 0x02, 0x02 },		//	<control>
	{ 0x03, 0x03 },		//	<control>
	{ 0x04, 0x04 },		//	<control>
	{ 0x05, 0x05 },		//	<control>
	{ 0x06, 0x06 },		//	<control>
	{ 0x07, 0x07 },		//	<control>
	{ 0x08, 0x08 },		//	<control>
	{ 0x09, 0x09 },		//	<control>
	{ 0x0A, 0x0a },		//	<control>
	{ 0x0B, 0x0b },		//	<control>
	{ 0x0C, 0x0c },		//	<control>
	{ 0x0D, 0x0d },		//	<control>
	{ 0x0E, 0x0e },		//	<control>
	{ 0x0F, 0x0f },		//	<control>
	{ 0x10, 0x10 },		//	<control>
	{ 0x11, 0x11 },		//	<control>
	{ 0x12, 0x12 },		//	<control>
	{ 0x13, 0x13 },		//	<control>
	{ 0x14, 0x14 },		//	<control>
	{ 0x15, 0x15 },		//	<control>
	{ 0x16, 0x16 },		//	<control>
	{ 0x17, 0x17 },		//	<control>
	{ 0x18, 0x18 },		//	<control>
	{ 0x19, 0x19 },		//	<control>
	{ 0x1A, 0x1a },		//	<control>
	{ 0x1B, 0x1b },		//	<control>
	{ 0x1C, 0x1c },		//	<control>
	{ 0x1D, 0x1d },		//	<control>
	{ 0x1E, 0x1e },		//	<control>
	{ 0x1F, 0x1f },		//	<control>
	{ 0x20, 0x20 },		//
	{ 0x21, 0x21 },		//	EXCLAMATION MARK
	{ 0x22, 0x22 },		//	QUOTATION MARK
	{ 0x23, 0x23 },		//	NUMBER SIGN
	{ 0x24, 0x24 },		//	DOLLAR SIGN
	{ 0x25, 0x25 },		//	PERCENT SIGN
	{ 0x26, 0x26 },		//	AMPERSAND
	{ 0x27, 0x27 },		//	APOSTROPHE
	{ 0x28, 0x28 },		//	LEFT PARENTHESIS
	{ 0x29, 0x29 },		//	RIGHT PARENTHESIS
	{ 0x2A, 0x2a },		//	ASTERISK
	{ 0x2B, 0x2b },		//	PLUS SIGN
	{ 0x2C, 0x2c },		//	COMMA
	{ 0x2D, 0x2d },		//	HYPHEN-MINUS
	{ 0x2E, 0x2e },		//	FULL STOP
	{ 0x2F, 0x2f },		//	SOLIDUS
	{ 0x30, 0x30 },		//	DIGIT ZERO
	{ 0x31, 0x31 },		//	DIGIT ONE
	{ 0x32, 0x32 },		//	DIGIT TWO
	{ 0x33, 0x33 },		//	DIGIT THREE
	{ 0x34, 0x34 },		//	DIGIT FOUR
	{ 0x35, 0x35 },		//	DIGIT FIVE
	{ 0x36, 0x36 },		//	DIGIT SIX
	{ 0x37, 0x37 },		//	DIGIT SEVEN
	{ 0x38, 0x38 },		//	DIGIT EIGHT
	{ 0x39, 0x39 },		//	DIGIT NINE
	{ 0x3A, 0x3a },		//	COLON
	{ 0x3B, 0x3b },		//	SEMICOLON
	{ 0x3C, 0x3c },		//	LESS-THAN SIGN
	{ 0x3D, 0x3d },		//	EQUALS SIGN
	{ 0x3E, 0x3e },		//	GREATER-THAN SIGN
	{ 0x3F, 0x3f },		//	QUESTION MARK
	{ 0x40, 0x40 },		//	COMMERCIAL AT
	{ 0x41, 0x41 },		//	LATIN CAPITAL LETTER A
	{ 0x42, 0x42 },		//	LATIN CAPITAL LETTER B
	{ 0x43, 0x43 },		//	LATIN CAPITAL LETTER C
	{ 0x44, 0x44 },		//	LATIN CAPITAL LETTER D
	{ 0x45, 0x45 },		//	LATIN CAPITAL LETTER E
	{ 0x46, 0x46 },		//	LATIN CAPITAL LETTER F
	{ 0x47, 0x47 },		//	LATIN CAPITAL LETTER G
	{ 0x48, 0x48 },		//	LATIN CAPITAL LETTER H
	{ 0x49, 0x49 },		//	LATIN CAPITAL LETTER I
	{ 0x4A, 0x4a },		//	LATIN CAPITAL LETTER J
	{ 0x4B, 0x4b },		//	LATIN CAPITAL LETTER K
	{ 0x4C, 0x4c },		//	LATIN CAPITAL LETTER L
	{ 0x4D, 0x4d },		//	LATIN CAPITAL LETTER M
	{ 0x4E, 0x4e },		//	LATIN CAPITAL LETTER N
	{ 0x4F, 0x4f },		//	LATIN CAPITAL LETTER O
	{ 0x50, 0x50 },		//	LATIN CAPITAL LETTER P
	{ 0x51, 0x51 },		//	LATIN CAPITAL LETTER Q
	{ 0x52, 0x52 },		//	LATIN CAPITAL LETTER R
	{ 0x53, 0x53 },		//	LATIN CAPITAL LETTER S
	{ 0x54, 0x54 },		//	LATIN CAPITAL LETTER T
	{ 0x55, 0x55 },		//	LATIN CAPITAL LETTER U
	{ 0x56, 0x56 },		//	LATIN CAPITAL LETTER V
	{ 0x57, 0x57 },		//	LATIN CAPITAL LETTER W
	{ 0x58, 0x58 },		//	LATIN CAPITAL LETTER X
	{ 0x59, 0x59 },		//	LATIN CAPITAL LETTER Y
	{ 0x5A, 0x5a },		//	LATIN CAPITAL LETTER Z
	{ 0x5B, 0x5b },		//	LEFT SQUARE BRACKET
	{ 0x5C, 0x5c },		//	REVERSE SOLIDUS
	{ 0x5D, 0x5d },		//	RIGHT SQUARE BRACKET
	{ 0x5E, 0x5e },		//	CIRCUMFLEX ACCENT
	{ 0x5F, 0x5f },		//	LOW LINE
	{ 0x60, 0x60 },		//	GRAVE ACCENT
	{ 0x61, 0x61 },		//	LATIN SMALL LETTER A
	{ 0x62, 0x62 },		//	LATIN SMALL LETTER B
	{ 0x63, 0x63 },		//	LATIN SMALL LETTER C
	{ 0x64, 0x64 },		//	LATIN SMALL LETTER D
	{ 0x65, 0x65 },		//	LATIN SMALL LETTER E
	{ 0x66, 0x66 },		//	LATIN SMALL LETTER F
	{ 0x67, 0x67 },		//	LATIN SMALL LETTER G
	{ 0x68, 0x68 },		//	LATIN SMALL LETTER H
	{ 0x69, 0x69 },		//	LATIN SMALL LETTER I
	{ 0x6A, 0x6a },		//	LATIN SMALL LETTER J
	{ 0x6B, 0x6b },		//	LATIN SMALL LETTER K
	{ 0x6C, 0x6c },		//	LATIN SMALL LETTER L
	{ 0x6D, 0x6d },		//	LATIN SMALL LETTER M
	{ 0x6E, 0x6e },		//	LATIN SMALL LETTER N
	{ 0x6F, 0x6f },		//	LATIN SMALL LETTER O
	{ 0x70, 0x70 },		//	LATIN SMALL LETTER P
	{ 0x71, 0x71 },		//	LATIN SMALL LETTER Q
	{ 0x72, 0x72 },		//	LATIN SMALL LETTER R
	{ 0x73, 0x73 },		//	LATIN SMALL LETTER S
	{ 0x74, 0x74 },		//	LATIN SMALL LETTER T
	{ 0x75, 0x75 },		//	LATIN SMALL LETTER U
	{ 0x76, 0x76 },		//	LATIN SMALL LETTER V
	{ 0x77, 0x77 },		//	LATIN SMALL LETTER W
	{ 0x78, 0x78 },		//	LATIN SMALL LETTER X
	{ 0x79, 0x79 },		//	LATIN SMALL LETTER Y
	{ 0x7A, 0x7a },		//	LATIN SMALL LETTER Z
	{ 0x7B, 0x7b },		//	LEFT CURLY BRACKET
	{ 0x7C, 0x7c },		//	VERTICAL LINE
	{ 0x7D, 0x7d },		//	RIGHT CURLY BRACKET
	{ 0x7E, 0x7e },		//	TILDE
	{ 0x7F, 0x7f },		//	<control>
	{ 0xc280, 0x80 },	//	<control>
	{ 0xc281, 0x81 },	//	<control>
	{ 0xc282, 0x82 },	//	<control>
	{ 0xc283, 0x83 },	//	<control>
	{ 0xc284, 0x84 },	//	<control>
	{ 0xc285, 0x85 },	//	<control>
	{ 0xc286, 0x86 },	//	<control>
	{ 0xc287, 0x87 },	//	<control>
	{ 0xc288, 0x88 },	//	<control>
	{ 0xc289, 0x89 },	//	<control>
	{ 0xc28a, 0x8A },	//	<control>
	{ 0xc28b, 0x8B },	//	<control>
	{ 0xc28c, 0x8C },	//	<control>
	{ 0xc28d, 0x8D },	//	<control>
	{ 0xc28e, 0x8E },	//	<control>
	{ 0xc28f, 0x8F },	//	<control>
	{ 0xc290, 0x90 },	//	<control>
	{ 0xc291, 0x91 },	//	<control>
	{ 0xc292, 0x92 },	//	<control>
	{ 0xc293, 0x93 },	//	<control>
	{ 0xc294, 0x94 },	//	<control>
	{ 0xc295, 0x95 },	//	<control>
	{ 0xc296, 0x96 },	//	<control>
	{ 0xc297, 0x97 },	//	<control>
	{ 0xc298, 0x98 },	//	<control>
	{ 0xc299, 0x99 },	//	<control>
	{ 0xc29a, 0x9A },	//	<control>
	{ 0xc29b, 0x9B },	//	<control>
	{ 0xc29c, 0x9C },	//	<control>
	{ 0xc29d, 0x9D },	//	<control>
	{ 0xc29e, 0x9E },	//	<control>
	{ 0xc29f, 0x9F },	//	<control>
	{ 0xc2a0, 0xA0 },	//	NO-BREAK SPACE
	{ 0xc2a1, 0xA1 },	//	INVERTED EXCLAMATION MARK
	{ 0xc2a2, 0xA2 },	//	CENT SIGN
	{ 0xc2a3, 0xA3 },	//	POUND SIGN
	{ 0xc2a4, 0xA4 },	//	CURRENCY SIGN
	{ 0xc2a5, 0xA5 },	//	YEN SIGN
	{ 0xc2a6, 0xA6 },	//	BROKEN BAR
	{ 0xc2a7, 0xA7 },	//	SECTION SIGN
	{ 0xc2a8, 0xA8 },	//	DIAERESIS
	{ 0xc2a9, 0xA9 },	//	COPYRIGHT SIGN
	{ 0xc2aa, 0xAA },	//	FEMININE ORDINAL INDICATOR
	{ 0xc2ab, 0xAB },	//	LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
	{ 0xc2ac, 0xAC },	//	NOT SIGN
	{ 0xc2ad, 0xAD },	//	SOFT HYPHEN
	{ 0xc2ae, 0xAE },	//	REGISTERED SIGN
	{ 0xc2af, 0xAF },	//	MACRON
	{ 0xc2b0, 0xB0 },	//	DEGREE SIGN
	{ 0xc2b1, 0xB1 },	//	PLUS-MINUS SIGN
	{ 0xc2b2, 0xB2 },	//	SUPERSCRIPT TWO
	{ 0xc2b3, 0xB3 },	//	SUPERSCRIPT THREE
	{ 0xc2b4, 0xB4 },	//	ACUTE ACCENT
	{ 0xc2b5, 0xB5 },	//	MICRO SIGN
	{ 0xc2b6, 0xB6 },	//	PILCROW SIGN
	{ 0xc2b7, 0xB7 },	//	MIDDLE DOT
	{ 0xc2b8, 0xB8 },	//	CEDILLA
	{ 0xc2b9, 0xB9 },	//	SUPERSCRIPT ONE
	{ 0xc2ba, 0xBA },	//	MASCULINE ORDINAL INDICATOR
	{ 0xc2bb, 0xBB },	//	RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
	{ 0xc2bc, 0xBC },	//	VULGAR FRACTION ONE QUARTER
	{ 0xc2bd, 0xBD },	//	VULGAR FRACTION ONE HALF
	{ 0xc2be, 0xBE },	//	VULGAR FRACTION THREE QUARTERS
	{ 0xc2bf, 0xBF },	//	INVERTED QUESTION MARK
	{ 0xc380, 0xC0 },	//	LATIN CAPITAL LETTER A WITH GRAVE
	{ 0xc381, 0xC1 },	//	LATIN CAPITAL LETTER A WITH ACUTE
	{ 0xc382, 0xC2 },	//	LATIN CAPITAL LETTER A WITH CIRCUMFLEX
	{ 0xc383, 0xC3 },	//	LATIN CAPITAL LETTER A WITH TILDE
	{ 0xc384, 0xC4 },	//	LATIN CAPITAL LETTER A WITH DIAERESIS
	{ 0xc385, 0xC5 },	//	LATIN CAPITAL LETTER A WITH RING ABOVE
	{ 0xc386, 0xC6 },	//	LATIN CAPITAL LETTER AE
	{ 0xc387, 0xC7 },	//	LATIN CAPITAL LETTER C WITH CEDILLA
	{ 0xc388, 0xC8 },	//	LATIN CAPITAL LETTER E WITH GRAVE
	{ 0xc389, 0xC9 },	//	LATIN CAPITAL LETTER E WITH ACUTE
	{ 0xc38a, 0xCA },	//	LATIN CAPITAL LETTER E WITH CIRCUMFLEX
	{ 0xc38b, 0xCB },	//	LATIN CAPITAL LETTER E WITH DIAERESIS
	{ 0xc38c, 0xCC },	//	LATIN CAPITAL LETTER I WITH GRAVE
	{ 0xc38d, 0xCD },	//	LATIN CAPITAL LETTER I WITH ACUTE
	{ 0xc38e, 0xCE },	//	LATIN CAPITAL LETTER I WITH CIRCUMFLEX
	{ 0xc38f, 0xCF },	//	LATIN CAPITAL LETTER I WITH DIAERESIS
	{ 0xc390, 0xD0 },	//	LATIN CAPITAL LETTER ETH
	{ 0xc391, 0xD1 },	//	LATIN CAPITAL LETTER N WITH TILDE
	{ 0xc392, 0xD2 },	//	LATIN CAPITAL LETTER O WITH GRAVE
	{ 0xc393, 0xD3 },	//	LATIN CAPITAL LETTER O WITH ACUTE
	{ 0xc394, 0xD4 },	//	LATIN CAPITAL LETTER O WITH CIRCUMFLEX
	{ 0xc395, 0xD5 },	//	LATIN CAPITAL LETTER O WITH TILDE
	{ 0xc396, 0xD6 },	//	LATIN CAPITAL LETTER O WITH DIAERESIS
	{ 0xc397, 0xD7 },	//	MULTIPLICATION SIGN
	{ 0xc398, 0xD8 },	//	LATIN CAPITAL LETTER O WITH STROKE
	{ 0xc399, 0xD9 },	//	LATIN CAPITAL LETTER U WITH GRAVE
	{ 0xc39a, 0xDA },	//	LATIN CAPITAL LETTER U WITH ACUTE
	{ 0xc39b, 0xDB },	//	LATIN CAPITAL LETTER U WITH CIRCUMFLEX
	{ 0xc39c, 0xDC },	//	LATIN CAPITAL LETTER U WITH DIAERESIS
	{ 0xc39d, 0xDD },	//	LATIN CAPITAL LETTER Y WITH ACUTE
	{ 0xc39e, 0xDE },	//	LATIN CAPITAL LETTER THORN
	{ 0xc39f, 0xDF },	//	LATIN SMALL LETTER SHARP S
	{ 0xc3a0, 0xE0 },	//	LATIN SMALL LETTER A WITH GRAVE
	{ 0xc3a1, 0xE1 },	//	LATIN SMALL LETTER A WITH ACUTE
	{ 0xc3a2, 0xE2 },	//	LATIN SMALL LETTER A WITH CIRCUMFLEX
	{ 0xc3a3, 0xE3 },	//	LATIN SMALL LETTER A WITH TILDE
	{ 0xc3a4, 0xE4 },	//	LATIN SMALL LETTER A WITH DIAERESIS
	{ 0xc3a5, 0xE5 },	//	LATIN SMALL LETTER A WITH RING ABOVE
	{ 0xc3a6, 0xE6 },	//	LATIN SMALL LETTER AE
	{ 0xc3a7, 0xE7 },	//	LATIN SMALL LETTER C WITH CEDILLA
	{ 0xc3a8, 0xE8 },	//	LATIN SMALL LETTER E WITH GRAVE
	{ 0xc3a9, 0xE9 },	//	LATIN SMALL LETTER E WITH ACUTE
	{ 0xc3aa, 0xEA },	//	LATIN SMALL LETTER E WITH CIRCUMFLEX
	{ 0xc3ab, 0xEB },	//	LATIN SMALL LETTER E WITH DIAERESIS
	{ 0xc3ac, 0xEC },	//	LATIN SMALL LETTER I WITH GRAVE
	{ 0xc3ad, 0xED },	//	LATIN SMALL LETTER I WITH ACUTE
	{ 0xc3ae, 0xEE },	//	LATIN SMALL LETTER I WITH CIRCUMFLEX
	{ 0xc3af, 0xEF },	//	LATIN SMALL LETTER I WITH DIAERESIS
	{ 0xc3b0, 0xF0 },	//	LATIN SMALL LETTER ETH
	{ 0xc3b1, 0xF1 },	//	LATIN SMALL LETTER N WITH TILDE
	{ 0xc3b2, 0xF2 },	//	LATIN SMALL LETTER O WITH GRAVE
	{ 0xc3b3, 0xF3 },	//	LATIN SMALL LETTER O WITH ACUTE
	{ 0xc3b4, 0xF4 },	//	LATIN SMALL LETTER O WITH CIRCUMFLEX
	{ 0xc3b5, 0xF5 },	//	LATIN SMALL LETTER O WITH TILDE
	{ 0xc3b6, 0xF6 },	//	LATIN SMALL LETTER O WITH DIAERESIS
	{ 0xc3b7, 0xF7 },	//	DIVISION SIGN
	{ 0xc3b8, 0xF8 },	//	LATIN SMALL LETTER O WITH STROKE
	{ 0xc3b9, 0xF9 },	//	LATIN SMALL LETTER U WITH GRAVE
	{ 0xc3ba, 0xFA },	//	LATIN SMALL LETTER U WITH ACUTE
	{ 0xc3bb, 0xFB },	//	LATIN SMALL LETTER U WITH CIRCUMFLEX
	{ 0xc3bc, 0xFC },	//	LATIN SMALL LETTER U WITH DIAERESIS
	{ 0xc3bd, 0xFD },	//	LATIN SMALL LETTER Y WITH ACUTE
	{ 0xc3be, 0xFE },	//	LATIN SMALL LETTER THORN
	{ 0xc3bf, 0xFF }	//	LATIN SMALL LETTER Y WITH DIAERESIS
};

bool isControlCode(UTFCode hex) {
	return hex<0x20;
}

size_t stringLenght(const std::string & input) {
	size_t result = 0;
	std::string::const_iterator it;
	for(it = input.cbegin(); it != input.cend(); ++it) {
		UTFCode code = static_cast<unsigned char>(*it);
		if(code>0x7f && (it + 1) != input.cend()) {
			++it;
			// TODO: analyze strings with more than two UTF codepoints
			//UTFCode lowCode = static_cast<unsigned char>(*it);
			//code = (code << 8) | lowCode;			
		}
		++result;
	}
	return result;
}

void iterateString(const std::string & input, std::function<void(std::string & multichar, UTFCode codepoint, UTFCode hex)> cb) {
	std::string::const_iterator it;
	std::string mbStr;
	for (it=input.cbegin(); it!=input.cend(); ++it) {
		UTFCode code = static_cast<unsigned char>(*it);
		mbStr = "";
		mbStr.push_back(*it);
		if (code>0x7f && (it+1)!=input.cend()) {
			++it;
			UTFCode lowCode = static_cast<unsigned char>(*it);
			code = (code << 8) | lowCode;
			mbStr.push_back(static_cast<char>(lowCode));
			cb(mbStr,UtfToCodepoint[code],code);
		}
		else {
			cb(mbStr, code, code);
		}
	}
}

bool checkSome(const std::string & input, std::function<bool(std::string & multichar, UTFCode codepoint, UTFCode hex)> cb) {
	std::string::const_iterator it;
	std::string mbStr;
	bool result = false;
	for (it = input.cbegin(); it != input.cend(); ++it) {
		UTFCode code = static_cast<unsigned char>(*it);
		mbStr = "";
		mbStr.push_back(*it);
		if (code>0x7f && (it + 1) != input.cend()) {
			++it;
			UTFCode lowCode = static_cast<unsigned char>(*it);
			code = (code << 8) | lowCode;
			mbStr.push_back(static_cast<char>(lowCode));
			result = cb(mbStr, UtfToCodepoint[code], code);
		}
		else {
			result = cb(mbStr, code, code);
		}

		if (result) {
			break;
		}
	}
	return result;
}

bool checkEvery(const std::string & input, std::function<bool(std::string & multichar, UTFCode codepoint, UTFCode hex)> cb) {
	std::string::const_iterator it;
	std::string mbStr;
	bool result = true;
	for (it = input.cbegin(); it != input.cend(); ++it) {
		UTFCode code = static_cast<unsigned char>(*it);
		mbStr = "";
		mbStr.push_back(*it);
		if (code>0x7f && (it + 1) != input.cend()) {
			++it;
			UTFCode lowCode = static_cast<unsigned char>(*it);
			code = (code << 8) | lowCode;
			mbStr.push_back(static_cast<char>(lowCode));
			result = cb(mbStr, UtfToCodepoint[code], code);
		}
		else {
			result = cb(mbStr, code, code);
		}

		if (!result) {
			break;
		}
	}
	return result;
}

std::string multibyteFromCodepoint(UTFCode cp) {
	std::stringstream sstream;
	UTFCode hex = CodepointToUtf[cp];
	unsigned char hi = static_cast<unsigned char>(hex >> 8);
	unsigned char low = static_cast<unsigned char>(hex & 0xFF);
	if (hi) {
		sstream << hi;
	}
	sstream << low;
	return sstream.str();
}

UTFCode codepointFromMultibyte(const std::string & multibyte) {
	UTFCode result = 0x00;
	iterateString(multibyte, [&](std::string & multichar, UTFCode codepoint, UTFCode hex) {
		result = codepoint;
	});
	return result;
}

}

}
}
