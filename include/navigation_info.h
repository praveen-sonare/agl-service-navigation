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

	/* GetDirState */
	char mCurrentState[256];		//current direction state info

	/* GetTotalDistance and GetCumulativeDistance */
	char mCurrentTotalDistance[256];		//current total distance info
	char mCurrentCumulativeDistance[256];		//current cumulative distance info

	/* GetGuidanceState */
	char mGuidanceState[256];		//current guidance state info

	/*	getAllroutes	*/
	uint32_t mRoutes;				//current route count

	/*	getdestination	*/
	char mDestLatitude[256];		//Destination latitude info
	char mDestLongitude[256];		//Destination longitude info

	/*	demodestination	*/
	char mDemoLatitude[256];		//current latitude info when in demo
	char mDemoLongitude[256];		//current longitude info when in demo
	char mDemoDistance[256];		//the distance from current position(along the route)
	char mDemoDirection[256];		//current car direction

	/*	guidance start position information	*/
	char mGuidanceStartLatitude[256] = {0};		//guidance start latitude info
	char mGuidanceStarLongitude[256] = {0};;	//guidance start longitude info

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

	void setNaviInfoCurrentTotalDistance( char* totaldistance );
	int getNaviInfoCurrentTotalDistance();

	void setNaviInfoCurrentCumulativeDistance( char* cumulativedistance );
	int getNaviInfoCurrentCumulativeDistance();

	void setNaviInfoCurrentGuidanceState( char* state );
	char* getNaviInfoCurrentGuidanceState();

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

	void setGuidanceStartLatitude(char* latitude);
	char* getGuidanceStartLatitude();
	void setGuidanceStartLongitude(char* longitude);
	char* getGuidanceStartLongitude();
};

