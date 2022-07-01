
#pragma once

#include "AFX_EXT_MISC.h"

#pragma pack(push, 8)	// default align. (8 bytes)


//=============================================================================
//
// double :        7       6       5       4       3       2       1
//          7654321076543210765432107654321076543210765432107654321076543210
//             6         5         4         3         2         1
//          3210987654321098765432109876543210987654321098765432109876543210
//          ||         ||                                                  |
//          ||         |+--------------------------------------------------+- mantisa (52 bits)
//          ||         |
//          |+---------+- exp (11 bits)
//          |
//          +- sign (1 bit)
//
// float :         3       2       1
//          76543210765432107654321076543210
//           3         2         1
//          10987654321098765432109876543210
//          ||      ||                     |
//          ||      |+---------------------+- mantisa (23 bits)
//          ||      |
//          |+------+- exp (8 bits)
//          |
//          +- sign (1 bit)
//
//
// PASCAL REAL
// real48 :        5       4       3       2       1
//          765432107654321076543210765432107654321076543210
//                 4         3         2         1
//          765432109876543210987654321098765432109876543210
//          ||                                     ||      |
//          ||                                     |+------+- exp (8 bits)
//          ||                                     |
//          |+-------------------------------------+- mantisa (39 bits)
//          |
//          + sign (1 bit)
//
//

#pragma pack(push, 1)	// default align. (8 bytes)
typedef unsigned char T_REAL48[6];
#pragma pack(pop)

AFX_EXT_API_MISC double Real48ToDouble(T_REAL48 real48);
AFX_EXT_API_MISC void DoubleToReal48(double value, T_REAL48 real48);
AFX_EXT_API_MISC void FloatToReal48(float value, T_REAL48 real48);

#pragma pack(pop)

