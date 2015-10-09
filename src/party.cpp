/**
	ESP Party.
*/

#include <stdio.h>
#include <math.h>
#include <vector>
#include "DataValue.hpp"

#include <hiredis/hiredis.h>

#include "party.h"
#include "hierarchie.h"

///DOXYS_OFF
///DOXYS_ON

using namespace std;

extern "C" {

	/**
	  add Party.
	 */
	int32_t h_addParty( int numargs, DataTypes::DataValue * top, DataTypes::DataValue * nextArgs, std::vector<void *>& arena) 
	{
		redisContext *pContext;
		redisReply *pReply;
		int i = 0;
		const char *pParty = NULL;
		const char *pRating = NULL;
		const char *pId = NULL;
		const char *pParent = NULL;
		const char *pUParent = NULL;

		DataTypes::DataValue *pCurr = (nextArgs != NULL) ? nextArgs : top;

		pParty = (nextArgs + i++)->val.stringv;
		pRating = (nextArgs + i++)->val.stringv;
		pId = (nextArgs + i++)->val.stringv;
		pParent = (nextArgs + i++)->val.stringv;
		pUParent = top->val.stringv;

		pContext = GetContext();

		/* multi .*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "MULTI");
		freeReplyObject( pReply);                                                                                                           

		/* add party.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "HMSET Hierarchie:Party:%s rating %s clientLegalId %s parentId %s ultimateParent %s", pParty, pRating, pId, pParent, pUParent);
		freeReplyObject( pReply);                                                                                                           

		/* add parent set.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "SADD Hierarchie:Set:%s %s", pParent, pId);
		freeReplyObject( pReply);                                                                                                           

		/* add  ultimate parent set.*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "SADD Hierarchie:Set:%s %s", pUParent, pParent);
		freeReplyObject( pReply);                                                                                                           

		/* add tree.*/
		pReply = (redisReply *) redisCommand( pContext, "ZADD Hierarchie:Tree 0 %s", pUParent);
		freeReplyObject( pReply);                                                                                                           

		pReply = (redisReply *) redisCommand( pContext, "ZADD Hierarchie:Tree 1 %s", pParent);
		freeReplyObject( pReply);                                                                                                           

		pReply = (redisReply *) redisCommand( pContext, "ZADD Hierarchie:Tree 2 %s", pId);
		freeReplyObject( pReply);                                                                                                           

		/* exec .*/                                                                                                                          
		pReply = (redisReply *) redisCommand( pContext, "EXEC");
		freeReplyObject( pReply);                                                                                                           

		return( 0);
	}

} 

