/***************************************************************************************************************
Subroutine:             													数据表格
Description:  															          								
Input:                                         								
Output:                                         						
***************************************************************************************************************/
#include <SN8F5702.h>
#include "Tab.h"																

//---------------------------100K的分压电阻
code uint16_t Temp_AD[135] = {220,	226,	233,	240,	246,	254,	261,	268,	276,	284,
														  293,	301,	310,	319,	329,	338,	348,	359,	369,	380,
														  392,	403,	416,	428,	441,	454,	468,	482,	497,	512,
														  527,	543,	560,	576,	594,	612,	630,	649,	669,	689,
														  710,	731,	753,	776,	799,	823,	847,	873,	898,	925,
														  952,	980,	1008,	1038,	1067,	1098,	1130,	1162,	1195,	1228,
														  1262,	1297,	1333,	1369,	1406,	1443,	1481,	1520,	1560,	1600,
														  1640,	1681,	1723,	1765,	1807,	1850,	1894,	1938,	1982,	2026,
														  2071,	2115,	2160,	2206,	2251,	2296,	2341,	2386,	2431,	2476,
														  2521,	2566,	2610,	2654,	2700,	2743,	2783,	2826,	2867,	2909,
														  2949,	2989,	3028,	3067,	3105,	3142,	3179,	3214,	3249,	3283,
														  3316,	3349,	3380,	3411,	3441,	3470,	3498,	3525,	3552,	3577,
														  3602,	3626,	3649,	3671,	3692,	3713,	3733,	3752,	3770,	3788,
														  3804,	3821,	3836,	3851,	3865};
												 
code uint8_t Temp_Num[136] = {185,	184,	183,	182,	181,	180,	179,	178,	177,	176,			//每个AD点对应的温度值										
															175,	174,	173,	172,	171,	170,	169,	168,	167,	166,														
															165,	164,	163,	162,	161,	160,	159,	158,	157,	156,													
															155,	154,	153,	152,	151,	150,	149,	148,	147,	146,													
															145,	144,	143,	142,	141,	140,	139,	138,	137,	136,
															135,	134,	133,	132,	131,	130,	129,	128,	127,	126,
															125,	124,	123,	122,	121,	120,	119,	118,	117,	116,
															115,	114,	113,	112,	111,	110,	109,	108,	107,	106,
															105,	104,	103,	102,	101,	100,	99,		98,		97,		96,
															95,		94,		93,		92,		91,		90,		89,		88,		87,		86,
															85,		84,		83,		82,		81,		80,		79,		78,		77,		76,
															75,		74,		73,		72,		71,		70,		69,		68,		67,		66,
															65,		64,		63,		62,		61,		60,		59,		58,		57,		56,
															55,		54,		53,		52,		51,		50};

