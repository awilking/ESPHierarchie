/**
	ESP Domain.
*/

#ifndef INC_DOMAIN_H
#define INC_DOMAIN_H

///DOXYS_OFF
///DOXYS_ON

/**
	public interface.
*/

extern "C" {
	int32_t h_addDomain( int , DataTypes::DataValue * , DataTypes::DataValue * , std::vector<void *>& );
}

#endif
