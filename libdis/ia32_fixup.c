#include <stdio.h>

static const char * mem_fixup[256] = {
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 00 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 08 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 10 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 18 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 20 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 28 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 30 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 38 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 40 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 48 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 50 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 58 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 60 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 68 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 70 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 78 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 80 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 88 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 90 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* 98 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* A0 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* A8 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* B0 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* B8 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* C0 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* C8 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* D0 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* D8 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* E0 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* E8 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* F0 */
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL /* F8 */
};
