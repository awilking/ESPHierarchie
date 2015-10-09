/**
	ESP Hierarchie.
*/

#ifndef INC_HIERARCHIE_H
#define INC_HIERARCHIE_H

///DOXYS_OFF
///DOXYS_ON

/**
	public interface.
*/

extern "C" {

int init();
int fini();

redisContext *GetContext();

int32_t h_getContext( int , DataTypes::DataValue * , DataTypes::DataValue * , std::vector<void *>& );
int32_t h_releaseContext( int , DataTypes::DataValue * , DataTypes::DataValue * , std::vector<void *>& );
int32_t h_setLimit( int , DataTypes::DataValue * , DataTypes::DataValue * , std::vector<void *>& );
int32_t h_getParent( int , DataTypes::DataValue * , DataTypes::DataValue * , std::vector<void *>& );
int32_t h_getClientLegalId( int , DataTypes::DataValue * , DataTypes::DataValue * , std::vector<void *>& );
}

#endif
