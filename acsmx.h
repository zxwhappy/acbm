/* $Id$ */
/*
 ** Copyright (C) 2002-2008 Sourcefire, Inc.
 ** Copyright (C) 2002 Martin Roesch <roesch@sourcefire.com>
 **
 ** This program is free software; you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License Version 2 as
 ** published by the Free Software Foundation.  You may not use, modify or
 ** distribute this program under any other version of the GNU General
 ** Public License.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software
 ** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


/*
 **   ACSMX.H 
 **
 **
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _ACSMX_H
#define _ACSMX_H


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 *   Prototypes
 */


#define ALPHABET_SIZE    256     

#define ACSM_FAIL_STATE   -1     


typedef struct _acsm_pattern {      
	struct  _acsm_pattern *next;
	unsigned char         *patrn;
	unsigned char         *casepatrn;
	int      n;
	int      nocase;
	int      offset;
	int      depth;
	void   * id;
	int      iid;
#ifdef DETECTION_OPTION_TREE
	void   * rule_option_tree;
#endif

} ACSM_PATTERN;


typedef struct  {    
	/* Next state - based on input character */
	int      NextState[ ALPHABET_SIZE ];  

	/* Failure state - used while building NFA & DFA  */
	int      FailState;   

	/* List of patterns that end here, if any */
	ACSM_PATTERN *MatchList;   

}ACSM_STATETABLE; 


/*
 * State machine Struct
 */
typedef struct {

	int acsmMaxStates;  
	int acsmNumStates;  

	ACSM_PATTERN    * acsmPatterns;
	ACSM_STATETABLE * acsmStateTable;

	int   bcSize;
	short bcShift[256];

}ACSM_STRUCT;

/*
 *   Prototypes
 */
ACSM_STRUCT * acsmNew (void);

//acsmAddPattern函数中倒数第二个参数就是 Match 函数中的 id 参数
//所以 Match函数 中 id 的实际数据类型就是 acsmAddPattern 的倒数第二个参数的实际数据类型
//其中 Match 函数原型为 int Match(void *id, int index, void *data);
// Match 函数的 index 参数表示匹配的特征内容（即 id 参数）在被查找的内容中所在的位置，从0开始编号
//例如：被查找的内容为 "abcdefg" 匹配的特征内容为 "de" 则此时 index 为 3
int acsmAddPattern( ACSM_STRUCT * p, unsigned char * pat, int n, int nocase, int offset, int depth, void *  id, int iid );

int acsmCompile ( ACSM_STRUCT * acsm
#ifdef DETECTION_OPTION_TREE
		, int (*build_tree)(void * id, void **existing_tree)
#endif
		);

//acsmSearch函数的倒数第二个参数就是 Match 函数中的 data 参数
//所以 Match函数 中 data 的实际数据类型就是 acsmSearch 的倒数第二个参数的实际数据类型
//Match函数 返回值显示的指定为0时可以匹配字符串中所有的关键词，否则，当匹配一个关键词后就返回了
int acsmSearch ( ACSM_STRUCT * acsm,unsigned char * T, int n, 
#ifdef DETECTION_OPTION_TREE
		int (*Match)(void * id, void *tree, int index, void *data),
#else
		int (*Match)( void * id, int index, void * data ),
#endif
		void * data, int* current_state );

void acsmFree ( ACSM_STRUCT * acsm );

int acsmPrintDetailInfo(ACSM_STRUCT *);

int acsmPrintSummaryInfo(void);

#endif
