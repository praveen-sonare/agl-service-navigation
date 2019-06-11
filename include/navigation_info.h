// Copyright 2019 NEDL CO.,LTD
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

/**
 *  @brief get or set navigation infomation
 */
class NavigationInfo
{
private:
	/*	AllSessions	*/
	uint32_t mSessionHandle;		//sessionhandle
	std::string	mClient;			//client name

	/* GetPosition */
	char mCurrentLatitude[256];		//current latitude info
	char mCurrentLongitude[256];	//current longitude info
	char mCurrentHeading[256];	     //current heading info

	/*	getAllroutes	*/
	uint32_t mRoutes;				//current route count

	/* GetDirState */
	char mCurrentState[256];		//current direction state info

	/*	getdestination	*/
	char mDestLatitude[256];		//Destination latitude info
	char mDestLongitude[256];		//Destination longitude info

	/*	demodestination	*/
	char mDemoLatitude[256];		//current latitude info when in demo
	char mDemoLongitude[256];		//current longitude info when in demo
	char mDemoDistance[256];		//the distance from current position(along the route)
	char mDemoDirection[256];		//current car direction

public:
	//get&set function for per members
	void setNaviInfoSessionHandle( char* sessionHandle );
	uint32_t getNaviInfoSessionHandle();

	void setNaviInfoClient( std::string client );
	std::string getNaviInfoClient();

	void setNaviInfoCurrentLatitude( char* latitude );
	char* getNaviInfoCurrentLatitude();

	void setNaviInfoCurrentLongitude( char* longitude );
	char* getNaviInfoCurrentLongitude();

	void setNaviInfoCurrentHeading( char* heading );
	char* getNaviInfoCurrentHeading();

	void setNaviInfoCurrentDirState( char* state );
	char* getNaviInfoCurrentDirState();

	void setNaviInfoAllRoutes( char* route );
	uint32_t getNaviInfoAllRoutes();

	void setNaviInfoDestLatitude( char* latitude );
	char* getNaviInfoDestLatitude();

	void setNaviInfoDestLongitude( char* longitude );
	char* getNaviInfoDestLongitude();

	void setNaviInfoDemoLatitude( char* latitude );
	char* getNaviInfoDemoLatitude();

	void setNaviInfoDemoLongitude( char* longitude );
	char* getNaviInfoDemoLongitude();

	void setNaviInfoDemoDistance( char* distance );
	char* getNaviInfoDemoDistance();

	void setNaviInfoDemoDirection( char* direction );
	char* getNaviInfoDemoDirection();
};

