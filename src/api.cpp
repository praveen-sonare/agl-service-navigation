// Copyright 2017 AW SOFTWARE CO.,LTD
// Copyright 2017 AISIN AW CO.,LTD

#include <string.h>

#include "binder_reply.h"
#include "genivi_request.h"
#include "analyze_request.h"
#include "genivi/genivi-navicore-constants.h"
#include "navigation_info.h"

#define AFB_BINDING_VERSION 3

extern "C" {
	#include <afb/afb-binding.h>
}
using namespace std;

static void StartDemoCarlaclient(afb_api_t api);
static void StopDemoCarlaclient(afb_api_t api);

static const char GpsEventName[] = "gps/setlocation";
static const char CarlaEventName[] = "carlaclient/positionUpdated";
static bool demo = false;

/**
 *  Variable declaration
 */
GeniviRequest* geniviRequest;	// Send request to Genivi
BinderReply* binderReply;	// Convert Genivi response result to json format
AnalyzeRequest* analyzeRequest;	// Analyze BinderClient's request and create arguments to pass to GeniviAPI
NavigationInfo* navigationInfo;	//get/set navigation infomation
static afb_event_t setwaypoints_event;
static afb_event_t pausesimulation_event;
static afb_event_t setdemorouteinfo_event;
static afb_event_t stopdemo_event;
static afb_event_t arrivedest_event;
static afb_event_t adddest_event;
static afb_event_t gps_event;
static afb_event_t getdestdir_event;
static afb_event_t heading_event;
static afb_event_t cancelguidance_event;

/**
 *  @brief navicore_getposition request callback
 *  @param[in] req Request from client
 */
void OnRequestNavicoreGetPosition(afb_req_t req)
{
	AFB_API_NOTICE(req->api, "--> Start %s()", __func__);
	AFB_REQ_DEBUG(req, "request navicore_getposition");

	// Request of Json format request
	json_object* req_json = afb_req_json(req);
	const char* req_json_str = json_object_to_json_string(req_json);
	AFB_API_NOTICE(req->api, "req_json_str = %s", req_json_str);

	// Request analysis and create arguments to pass to Genivi
	std::vector< int32_t > Params;
	if( !analyzeRequest->CreateParamsGetPosition( req_json_str, Params ))
	{
		afb_req_fail(req, "failed", "navicore_getposition Bad Request");
		return;
	}

#if 0
	// GENIVI API call
	std::map< int32_t, double > posList = geniviRequest->NavicoreGetPosition( Params );
#else
	std::map< int32_t, double > posList;
	posList[NAVICORE_LATITUDE] = strtod(navigationInfo->getNaviInfoCurrentLatitude(),NULL);
	posList[NAVICORE_LONGITUDE] = strtod(navigationInfo->getNaviInfoCurrentLongitude(),NULL);
#endif
	// Convert to json style response
	APIResponse response = binderReply->ReplyNavicoreGetPosition( posList );

	// On success
	if(response.isSuccess)
	{
		AFB_API_NOTICE(req->api, "res_json_str = %s", json_object_to_json_string(response.json_data));
		// Return success to BinderClient
		afb_req_success(req, response.json_data, "navicore_getposition");
	}
	else
	{
		AFB_REQ_ERROR(req, "%s - %s:%d", response.errMessage.c_str(), __FILE__, __LINE__);
		afb_req_fail(req, "failed", "navicore_getposition Bad Request");
	}

	// Json object release
	json_object_put(response.json_data);

	AFB_API_NOTICE(req->api, "<-- End %s()", __func__);
}


/**
 *  @brief navicore_getallroutes request callback
 *  @param[in] req Request from client
 */
void OnRequestNavicoreGetAllRoutes(afb_req_t req)
{
	AFB_API_NOTICE(req->api, "--> Start %s()", __func__);
	AFB_REQ_DEBUG(req, "request navicore_getallroutes");

	// No request information in json format
	AFB_API_NOTICE(req->api, "req_json_str = none");

#if 0
	// GENEVI API call
	std::vector< uint32_t > allRoutes = geniviRequest->NavicoreGetAllRoutes();
#else
	std::vector< uint32_t > allRoutes;
	allRoutes.push_back(navigationInfo->getNaviInfoAllRoutes());
#endif

	// Convert to json style response
	APIResponse response = binderReply->ReplyNavicoreGetAllRoutes( allRoutes );

	// On success
	if(response.isSuccess)
	{
		AFB_API_NOTICE(req->api, "res_json_str = %s", json_object_to_json_string(response.json_data));
		// Return success to BinderClient
		afb_req_success(req, response.json_data, "navicore_getallroutes");
	}
	else
	{
		AFB_REQ_ERROR(req, "%s - %s:%d", response.errMessage.c_str(), __FILE__, __LINE__);
		afb_req_fail(req, "failed", "navicore_getallroutes Bad Request");
	}

	// json object release
	json_object_put(response.json_data);

	AFB_API_NOTICE(req->api, "<-- End %s()", __func__);
}


/**
 *  @brief navicore_createroute request callback
 *  @param[in] req Request from client
 */
void OnRequestNavicoreCreateRoute(afb_req_t req)
{
	AFB_API_NOTICE(req->api, "--> Start %s ", __func__);
	AFB_REQ_DEBUG(req, "request navicore_createroute");

	// Request of json format request
	json_object* req_json = afb_req_json(req);
	const char* req_json_str = json_object_to_json_string(req_json);
	AFB_API_NOTICE(req->api, "req_json_str = %s", req_json_str);

	// Request analysis and create arguments to pass to Genivi
	uint32_t sessionHdl = 0;
	if( !analyzeRequest->CreateParamsCreateRoute( req_json_str, sessionHdl ))
	{
		afb_req_fail(req, "failed", "navicore_createroute Bad Request");
		return;
	}

#if 0
	// GENEVI API call
	uint32_t routeHdl = geniviRequest->NavicoreCreateRoute( sessionHdl );
#else
	uint32_t routeHdl = navigationInfo->getNaviInfoAllRoutes();
#endif

	// Convert to json style response
	APIResponse response = binderReply->ReplyNavicoreCreateRoute( routeHdl );

	// On success
	if(response.isSuccess)
	{
		AFB_API_NOTICE(req->api, "res_json_str = %s", json_object_to_json_string(response.json_data));
		// Return success to BinderClient
		afb_req_success(req, response.json_data, "navicore_createroute");
	}
	else
	{
		AFB_REQ_ERROR(req, "%s - %s:%d", response.errMessage.c_str(), __FILE__, __LINE__);
		afb_req_fail(req, "failed", "navicore_createroute Bad Request");
	}

	// json object release
	json_object_put(response.json_data);

	AFB_API_NOTICE(req->api, "<-- End %s()", __func__);
}


/**
 *  @brief navicore_pausesimulation request callback
 *  @param[in] req Request from client
 */
void OnRequestNavicorePauseSimulation(afb_req_t req)
{
	AFB_API_NOTICE(req->api, "--> Start %s()", __func__);
	AFB_REQ_DEBUG(req, "request navicore_pausesimulation");

	// Request of json format request
	json_object* req_json = afb_req_json(req);
	const char* req_json_str = json_object_to_json_string(req_json);
	AFB_API_NOTICE(req->api, "req_json_str = %s", req_json_str);

	// Request analysis and create arguments to pass to Genivi
	uint32_t sessionHdl = 0;
	if( !analyzeRequest->CreateParamsPauseSimulation( req_json_str, sessionHdl ))
	{
		afb_req_fail(req, "failed", "navicore_pausesimulation Bad Request");
		return;
	}

#if 0
	// GENEVI API call
	geniviRequest->NavicorePauseSimulation( sessionHdl );
#else
	afb_event_push(pausesimulation_event, req_json);
#endif

	// No reply unnecessary API for conversion to json format response is unnecessary
	AFB_API_NOTICE(req->api, "res_json_str = none");

	// Return success to BinderClient
	afb_req_success(req, NULL, "navicore_pausesimulation");

	AFB_API_NOTICE(req->api, "<-- End %s()", __func__);
}


/**
 *  @brief navicore_setsimulationmode request callback
 *  @param[in] req Request from client
 */
void OnRequestNavicoreSetSimulationMode(afb_req_t req)
{
	AFB_API_NOTICE(req->api, "--> Start %s()", __func__);
	AFB_REQ_DEBUG(req, "request navicore_setsimulationmode");

	// Request of json format request
	json_object* req_json = afb_req_json(req);
	const char* req_json_str = json_object_to_json_string(req_json);
	AFB_API_NOTICE(req->api, "req_json_str = %s", req_json_str);

	// Request analysis and create arguments to pass to Genivi
	uint32_t sessionHdl = 0;
	bool simuMode = false;
	if( !analyzeRequest->CreateParamsSetSimulationMode( req_json_str, sessionHdl, simuMode ))
	{
		afb_req_fail(req, "failed", "navicore_setsimulationmode Bad Request");
		return;
	}

#if 0
	// GENEVI API call
	geniviRequest->NavicoreSetSimulationMode( sessionHdl, simuMode );
#endif

	// No reply unnecessary API for conversion to json format response is unnecessary
	AFB_API_NOTICE(req->api, "res_json_str = none");

	// Return success to BinderClient
	afb_req_success(req, NULL, "navicore_setsimulationmode");

	AFB_API_NOTICE(req->api, "<-- End %s()", __func__);
}


/**
 *  @brief navicore_cancelroutecalculation request callback
 *  @param[in] req Request from client
 */
void OnRequestNavicoreCancelRouteCalculation(afb_req_t req)
{
	AFB_API_NOTICE(req->api, "--> Start %s()", __func__);
	AFB_REQ_DEBUG(req, "request navicore_cancelroutecalculation");

	// Request of Json format request
	json_object* req_json = afb_req_json(req);
	const char* req_json_str = json_object_to_json_string(req_json);
	AFB_API_NOTICE(req->api, "req_json_str = %s", req_json_str);

	// Request analysis and create arguments to pass to Genivi
	uint32_t sessionHdl = 0;
	uint32_t routeHdl = 0;
	if( !analyzeRequest->CreateParamsCancelRouteCalculation( req_json_str, sessionHdl, routeHdl ))
	{
		afb_req_fail(req, "failed", "navicore_cancelroutecalculation Bad Request");
		return;
	}

#if 0
	// GENEVI API call
	geniviRequest->NavicoreCancelRouteCalculation( sessionHdl, routeHdl );
#endif

	// No reply unnecessary API for conversion to json format response is unnecessary
	AFB_API_NOTICE(req->api, "res_json_str = none");

	// Return success to BinderClient
	afb_req_success(req, NULL, "navicore_cancelroutecalculation");

	AFB_API_NOTICE(req->api, "<-- End %s()", __func__);
}


/**
 *  @brief navicore_setwaypoints request callback
 *  @param[in] req Request from client
 */
void OnRequestNavicoreWaypoints(afb_req_t req)
{
	AFB_API_NOTICE(req->api, "--> Start %s()", __func__);
	AFB_REQ_DEBUG(req, "request navicore_setwaypoints");

	// Request of Json format request
	json_object* req_json = afb_req_json(req);
	const char* req_json_str = json_object_to_json_string(req_json);
	AFB_API_NOTICE(req->api, "req_json_str = %s", req_json_str);

	// Request analysis and create arguments to pass to Genivi
	uint32_t sessionHdl = 0;
	uint32_t routeHdl = 0;
	bool currentPos = false;
	std::vector<Waypoint> waypointsList;
	if( !analyzeRequest->CreateParamsSetWaypoints( req_json_str, sessionHdl, routeHdl, currentPos, waypointsList ))
	{
		afb_req_fail(req, "failed", "navicore_setwaypoints Bad Request");
		return;
	}

#if 0
	// GENEVI API call
	geniviRequest->NavicoreSetWaypoints( sessionHdl, routeHdl, currentPos, waypointsList );
#else
	struct json_object *current  = NULL;
	struct json_object *wpl  = NULL;

	struct json_object* response_json = json_object_new_array();
	struct json_object* obj = json_object_new_object();

	if ((json_object_object_get_ex(req_json, "startFromCurrentPosition", &current)) &&
		(json_object_object_get_ex(req_json, "", &wpl)))
	{
		if (json_object_is_type(current, json_type_boolean) &&
			json_object_is_type(wpl, json_type_array))
		{
			bool  currentPos = json_object_get_boolean(current);
			json_object_object_add(obj, "startFromCurrentPosition", json_object_new_boolean(currentPos));

			// Get latitude, longitude
			for (int i = 0; i < json_object_array_length(wpl); ++i)
			{
				struct json_object *array = json_object_array_get_idx(wpl, i);
				struct json_object *lati  = NULL;
				struct json_object *longi = NULL;

				if (json_object_object_get_ex(array, "latitude", &lati) &&
					json_object_object_get_ex(array, "longitude", &longi)) {
					double latitude  = json_object_get_double(lati);
					double longitude = json_object_get_double(longi);
					json_object_object_add(obj, "latitude", json_object_new_double(latitude));
					json_object_object_add(obj, "longitude", json_object_new_double(longitude));
				}
		   }
		}
	}
	json_object_array_add(response_json, obj);

	const char* response_json_str = json_object_to_json_string(response_json);
	AFB_API_NOTICE(req->api, "response_json_str = %s", response_json_str);
	afb_event_push(setwaypoints_event, response_json);
#endif
	// No reply unnecessary API for conversion to json format response is unnecessary
	AFB_API_NOTICE(req->api, "res_json_str = none");

	// Return success to BinderClient
	afb_req_success(req, NULL, "navicore_setwaypoints");

	AFB_API_NOTICE(req->api, "<-- End %s()", __func__);
}


/**
 *  @brief navicore_calculateroute request callback
 *  @param[in] req Request from client
 */
void OnRequestNavicoreCalculateRoute(afb_req_t req)
{
	AFB_API_NOTICE(req->api, "--> Start %s()", __func__);
	AFB_REQ_DEBUG(req, "request navicore_calculateroute");

	// Request of Json format request
	json_object* req_json = afb_req_json(req);
	const char* req_json_str = json_object_to_json_string(req_json);
	AFB_API_NOTICE(req->api, "req_json_str = %s", req_json_str);

	// Request analysis and create arguments to pass to Genivi
	uint32_t sessionHdl = 0;
	uint32_t routeHdl = 0;
	if( !analyzeRequest->CreateParamsCalculateRoute( req_json_str, sessionHdl, routeHdl ))
	{
		afb_req_fail(req, "failed", "navicore_calculateroute Bad Request");
		return;
	}

#if 0
	// GENEVI API call
	geniviRequest->NavicoreCalculateRoute( sessionHdl, routeHdl );
#endif

	// No reply unnecessary API for conversion to json format response is unnecessary
	AFB_API_NOTICE(req->api, "res_json_str = none");

	// Return success to BinderClient
	afb_req_success(req, NULL, "navicore_calculateroute");

	AFB_API_NOTICE(req->api, "<-- End %s()", __func__);
}


/**
 *  @brief navicore_getallsessions request callback
 *  @param[in] req Request from client
 */
void OnRequestNavicoreGetAllSessions(afb_req_t req)
{
	AFB_API_NOTICE(req->api, "--> Start %s()", __func__);
	AFB_REQ_DEBUG(req, "request navicore_getallsessions");

	// No request information in Json format
	AFB_API_NOTICE(req->api, "req_json_str = none");

#if 0
	// GENEVI API call
	std::map<uint32_t, std::string> allSessions = geniviRequest->NavicoreGetAllSessions();
#else
	std::map<uint32_t, std::string> allSessions;
	allSessions[navigationInfo->getNaviInfoSessionHandle()] = navigationInfo->getNaviInfoClient();
#endif
	// Convert to json style response
	APIResponse response = binderReply->ReplyNavicoreGetAllSessions( allSessions );

	// On success
	if(response.isSuccess)
	{
		AFB_API_NOTICE(req->api, "res_json_str = %s", json_object_to_json_string(response.json_data));
		// Return success to BinderClient
		afb_req_success(req, response.json_data, "navicore_getallsessions");
	}
	else
	{
		AFB_REQ_ERROR(req, "%s - %s:%d", response.errMessage.c_str(), __FILE__, __LINE__);
		afb_req_fail(req, "failed", "navicore_getallsessions Bad Request");
	}

	// json object release
	json_object_put(response.json_data);

	AFB_API_NOTICE(req->api, "<-- End %s()", __func__);
}

/**
 *  @brief navicore_setcurrentpos request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreSetCurrentPos(afb_req_t req)
{
	const char* latitude = afb_req_value(req, "latitude");
	const char* longitude = afb_req_value(req, "longitude");

	navigationInfo->setNaviInfoCurrentLatitude(const_cast<char*>(latitude));
	navigationInfo->setNaviInfoCurrentLongitude(const_cast<char*>(longitude));

	afb_req_success(req, NULL, NULL);
}

/**
 *  @brief navicore_getcurrentpos request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreGetCurrentPos(afb_req_t req)
{
	struct json_object* response_json = json_object_new_array();
	struct json_object* obj = json_object_new_object();

	char* current_latitude  = navigationInfo->getNaviInfoCurrentLatitude();
	char* current_longitude = navigationInfo->getNaviInfoCurrentLongitude();
	char* current_heading = navigationInfo->getNaviInfoCurrentHeading();

	json_object_object_add(obj, "CurrentLatitude", json_object_new_string(current_latitude));
	json_object_object_add(obj, "CurrentLongitude", json_object_new_string(current_longitude));
	json_object_object_add(obj, "CurrentHeading", json_object_new_string(current_heading));

	json_object_array_add(response_json, obj);
	const char* obj_str = json_object_to_json_string(obj);
	AFB_API_NOTICE(req->api, "obj_str = %s", obj_str);

	// Return success to BinderClient
	afb_req_success(req, response_json, "navicore_getcurrentpos");

	AFB_API_NOTICE(req->api, "<-- End %s()", __func__);
}

/**
 *  @brief navicore_setallsessions request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreSetAllSessions(afb_req_t req)
{
	const char* sessionhandle = afb_req_value(req, "sessionHandle");
	const char* client = afb_req_value(req, "client");

	navigationInfo->setNaviInfoSessionHandle(const_cast<char*>(sessionhandle));
	navigationInfo->setNaviInfoClient(const_cast<char*>(client));

	afb_req_success(req, NULL, NULL);
}

/**
 *  @brief navicore_setallsessions request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreSetAllRoutes(afb_req_t req)
{
	const char* route = afb_req_value(req, "route");

	navigationInfo->setNaviInfoAllRoutes(const_cast<char*>(route));

	afb_req_success(req, NULL, NULL);
}

/**
 *  @brief navicore_setdestpos request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreSetDestPos(afb_req_t req)
{
	const char* destination_latitude = afb_req_value(req, "DestLatitude");
	const char* destination_longitude = afb_req_value(req, "DestLongitude");

	navigationInfo->setNaviInfoDestLatitude(const_cast<char*>(destination_latitude));
	navigationInfo->setNaviInfoDestLongitude(const_cast<char*>(destination_longitude));

	afb_req_success(req, NULL, NULL);

	struct json_object* response_json = json_object_new_array();
	struct json_object* obj = json_object_new_object();

	AFB_API_NOTICE(req->api, "OnRequestNavicoreSetDestPos");

	uint32_t allroutes = navigationInfo->getNaviInfoAllRoutes();
	json_object_object_add(obj, "AllRoutes", json_object_new_int(allroutes));

	json_object_object_add(obj, "DestinationLatitude", json_object_new_string(destination_latitude));
	json_object_object_add(obj, "DestinationLongitude", json_object_new_string(destination_longitude));

	json_object_array_add(response_json, obj);

	afb_event_push(adddest_event, response_json);

}

/**
 *  @brief navicore_setdemorouteinfo request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreSetDemoRouteInfo(afb_req_t req)
{
	const char* demo_latitude = afb_req_value(req, "DemoLatitude");
	const char* demo_longitude = afb_req_value(req, "DemoLongitude");
	const char* demo_distance = afb_req_value(req, "DemoDistance");
	const char* demo_direction = afb_req_value(req, "DemoDirection");

	navigationInfo->setNaviInfoDemoLatitude(const_cast<char*>(demo_latitude));
	navigationInfo->setNaviInfoDemoLongitude(const_cast<char*>(demo_longitude));

	navigationInfo->setNaviInfoDemoDistance(const_cast<char*>(demo_distance));
	navigationInfo->setNaviInfoDemoDirection(const_cast<char*>(demo_direction));

	struct json_object* response_json = json_object_new_array();
	struct json_object* obj = json_object_new_object();

	json_object_object_add(obj, "DemoLatitude", json_object_new_string(navigationInfo->getNaviInfoDemoLatitude()));
	json_object_object_add(obj, "DemoLongitude", json_object_new_string(navigationInfo->getNaviInfoDemoLongitude()));
	json_object_object_add(obj, "DemoDistance", json_object_new_string(navigationInfo->getNaviInfoDemoDistance()));
	json_object_object_add(obj, "DemoDirection", json_object_new_string(navigationInfo->getNaviInfoDemoDirection()));
	json_object_array_add(response_json, obj);

	afb_event_push(setdemorouteinfo_event, response_json);

	afb_req_success(req, NULL, NULL);
}

/**
 *  @brief navicore_stopdemo request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreStopDemo(afb_req_t req)
{
	struct json_object* response_json = json_object_new_array();
	struct json_object* obj = json_object_new_object();

	json_object_object_add(obj, "StopDemo", json_object_new_boolean(true));
	json_object_array_add(response_json, obj);

	afb_event_push(stopdemo_event, response_json);

	afb_req_success(req, NULL, NULL);
}

/**
 *  @brief navicore_arrivedest request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreArriveDest(afb_req_t req)
{
	struct json_object* response_json = json_object_new_array();
	struct json_object* obj = json_object_new_object();

	json_object_object_add(obj, "ArriveDest", json_object_new_boolean(true));
	json_object_array_add(response_json, obj);

	afb_event_push(arrivedest_event, response_json);

	afb_req_success(req, NULL, NULL);
}
/**
 *  @brief navicore_setdestdir request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreSetDestDir(afb_req_t req)
{
	const char* state = afb_req_value(req, "state");

	AFB_REQ_NOTICE(req, "state = %s", state);

	afb_req_success(req, NULL, NULL);

	navigationInfo->setNaviInfoCurrentDirState(const_cast<char*>(state));

	struct json_object* response_json = json_object_new_object();

	AFB_REQ_NOTICE(req, "OnRequestNavicoreSetDestDir");

	json_object_object_add(response_json, "state", json_object_new_string(state));

	const char* response_json_str = json_object_to_json_string(response_json);
	AFB_REQ_NOTICE(req, "response_json_str = %s", response_json_str);

	afb_event_push(getdestdir_event, response_json);
}
/**
 *  @brief navicore_getcurrentdestdir request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreGetCurrentDestDir(afb_req_t req)
{
	struct json_object* response_json = json_object_new_array();
    struct json_object* obj = json_object_new_object();

	char* current_dirstate  = navigationInfo->getNaviInfoCurrentDirState();

	json_object_object_add(obj, "CurrentDirState", json_object_new_string(current_dirstate));

	json_object_array_add(response_json, obj);
	const char* obj_str = json_object_to_json_string(obj);
	AFB_API_NOTICE(req->api, "obj_str = %s", obj_str);

	// Return success to BinderClient
	afb_req_success(req, response_json, "navicore_getcurrentdestdir");

	AFB_API_NOTICE(req->api, "<-- End %s()", __func__);
}
/**
 *  @brief navicore_startguidance request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreStartGuidance(afb_req_t req)
{
	AFB_REQ_NOTICE(req, "OnRequestNavicoreStartGuidance");
	StartDemoCarlaclient(req->api);

	afb_req_success(req, NULL, NULL);
}

/**
 *  @brief navicore_cancelguidance request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreCancelGuidance(afb_req_t req)
{
	AFB_REQ_NOTICE(req, "OnRequestNavicoreCancelGuidance");
	StopDemoCarlaclient(req->api);
	
	afb_event_push(cancelguidance_event, NULL);

	afb_req_success(req, NULL, NULL);
}

/**
 *  @brief navicore_demostate request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreDemoState(afb_req_t req)
{
	AFB_REQ_NOTICE(req, "OnRequestNavicoreDemoState");
	const char* DemoState = afb_req_value(req, "DemoState");

	if (strcmp(DemoState,"true") == 0){
		demo = true;
	}
	else{
		demo = false;
	}
	AFB_REQ_NOTICE(req, "Demo state = %d", demo);

	afb_req_success(req, NULL, NULL);
}

/**
 *  @brief subscribe request callback
 *  @param[in] req Request from server
 */
void subscribe(afb_req_t req)
{
	const char *value = afb_req_value(req, "value");

	if (value && !strcasecmp(value, "setwaypoints")) {
		AFB_API_NOTICE(req->api, "afb_req_subscribe");
		afb_req_subscribe(req, setwaypoints_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "pausesimulation")){
		afb_req_subscribe(req, pausesimulation_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "setdemopos")){
		afb_req_subscribe(req, setdemorouteinfo_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "stopdemo")){
		afb_req_subscribe(req, stopdemo_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "arrivedest")){
		afb_req_subscribe(req, arrivedest_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "adddest")){
		afb_req_subscribe(req, adddest_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "gps")){
		afb_req_subscribe(req, gps_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "heading")){
		afb_req_subscribe(req, heading_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "getdestdir")){
		afb_req_subscribe(req, getdestdir_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "cancelguidance")){
		afb_req_subscribe(req, cancelguidance_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	afb_req_fail(req, "failed", "Invalid event");
}

/**
 *  @brief unsubscribe request callback
 *  @param[in] req Request from server
 */
void unsubscribe(afb_req_t req)
{
	const char *value = afb_req_value(req, "value");

	if (value && !strcasecmp(value, "setwaypoints")) {
		afb_req_unsubscribe(req, setwaypoints_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "pausesimulation"))
	{
		afb_req_unsubscribe(req, pausesimulation_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "setdemopos")){
		afb_req_unsubscribe(req, setdemorouteinfo_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "stopdemo")){
		afb_req_unsubscribe(req, stopdemo_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "arrivedest")){
		afb_req_unsubscribe(req, arrivedest_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "adddest")){
		afb_req_unsubscribe(req, adddest_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "gps")){
		afb_req_unsubscribe(req, gps_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "heading")){
		afb_req_unsubscribe(req, heading_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "getdestdir")){
		afb_req_unsubscribe(req, getdestdir_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	else if(value && !strcasecmp(value, "cancelguidance")){
		afb_req_unsubscribe(req, cancelguidance_event);
		afb_req_success(req, NULL, NULL);
		return;
	}
	afb_req_fail(req, "failed", "Invalid event");
}

/**
 *  @brief GetRouteInfo request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreGetRouteInfo(afb_req_t req)
{
	struct json_object* response_json = json_object_new_array();
	struct json_object* obj = json_object_new_object();

	AFB_API_NOTICE(req->api, "OnRequestNavicoreGetRouteInfo");

	uint32_t allroutes = navigationInfo->getNaviInfoAllRoutes();
	json_object_object_add(obj, "AllRoutes", json_object_new_int(allroutes));

	char* current_latitude  = navigationInfo->getNaviInfoCurrentLatitude();
	char* current_longitude = navigationInfo->getNaviInfoCurrentLongitude();
	json_object_object_add(obj, "CurrentLatitude", json_object_new_string(current_latitude));
	json_object_object_add(obj, "CurrentLongitude", json_object_new_string(current_longitude));

	char* destination_latitude  = navigationInfo->getNaviInfoDestLatitude();
	char* destination_longitude = navigationInfo->getNaviInfoDestLongitude();

	json_object_object_add(obj, "DestinationLatitude", json_object_new_string(destination_latitude));
	json_object_object_add(obj, "DestinationLongitude", json_object_new_string(destination_longitude));

	json_object_array_add(response_json, obj);

	// No reply unnecessary API for conversion to json format response is unnecessary
	const char* response_json_str = json_object_to_json_string(response_json);
	AFB_API_NOTICE(req->api, "response_json_str = %s", response_json_str);

	// No reply unnecessary API for conversion to json format response is unnecessary
	AFB_API_NOTICE(req->api, "res_json_str = none");

	// Return success to BinderClient
	afb_req_success(req, response_json, "navicore_getrouteinfo");

	AFB_API_NOTICE(req->api, "<-- End %s()", __func__);
}

/**
 *  @brief GPS binding event function
 *  @param[in] api the api serving the request
 *  @param[in] event event name
 *  @param[in] object event json object
 */
static void onevent(afb_api_t api, const char *event, struct json_object *object)
{
	AFB_API_NOTICE(api, "on_event %s , object %s", event, json_object_get_string(object));

	if (demo == false){
		if (strcmp(event,GpsEventName) == 0)
		{
			struct json_object *lati  = NULL;
			struct json_object *longi  = NULL;
			if ((json_object_object_get_ex(object, "latitude", &lati)) &&
			(json_object_object_get_ex(object, "longitude", &longi)))
			{
				const char* latitude = json_object_get_string(lati);
				const char* longitude = json_object_get_string(longi);

				navigationInfo->setNaviInfoCurrentLatitude(const_cast<char*>(latitude));
				navigationInfo->setNaviInfoCurrentLongitude(const_cast<char*>(longitude));

				struct json_object* gps_obj = json_object_new_object();

				json_object_object_add(gps_obj, "latitude", json_object_new_string(latitude));
				json_object_object_add(gps_obj, "longitude", json_object_new_string(longitude));

				AFB_API_NOTICE(api, "event name %s , gps_obj %s", event, json_object_get_string(gps_obj));

				afb_event_push(gps_event, gps_obj);
			}
		}
		else if(strcmp(event,CarlaEventName) == 0)
		{
			struct json_object *yaw  = NULL;
			if ((json_object_object_get_ex(object, "yaw", &yaw)))
			{
				const char* heading = json_object_get_string(yaw);

				navigationInfo->setNaviInfoCurrentHeading(const_cast<char*>(heading));

				struct json_object* heading_obj = json_object_new_object();

				json_object_object_add(heading_obj, "heading", json_object_new_string(heading));

				AFB_API_NOTICE(api, "event name %s , heading_obj %s", event, json_object_get_string(heading_obj));

				afb_event_push(heading_event, heading_obj);
			}
		}
	}
	else{
		/* when is demo on, do nothing */
	}
}

/**
 *  @brief the callback function
 *  @param[in] closure the user defined closure pointer 'closure'
 *  @param[in] a JSON object returned (can be NULL)
 *  @param[in] a string not NULL in case of error but NULL on success
 *  @param[in] a string handling some info (can be NULL)
 *  @param[in] api the api
 */
static void api_callback(void *closure, struct json_object *object, const char *error, const char *info, afb_api_t api)
{
	AFB_API_NOTICE(api, "asynchronous call, error=%s, info=%s, object=%s.", error, info, json_object_get_string(object));	
}

/**
 *  @brief call api
 *  @param[in] api the api serving the request
 *  @param[in] service the api name of service
 *  @param[in] verb the verb of service
 *  @param[in] args parameter
 */
static void api_call(afb_api_t api, const char *service, const char *verb, struct json_object *args)
{    
	AFB_API_NOTICE(api, "service=%s verb=%s, args=%s.", service, verb, json_object_get_string(args));
	afb_api_call(api, service, verb, args, api_callback, nullptr);
}

/**
 *  @brief call carlaclient start demo function
 *  @param[in] api the api serving the request
 */
static void StartDemoCarlaclient(afb_api_t api)
{
	struct json_object* obj = json_object_new_object();
	json_object_object_add(obj, "demo", json_object_new_string("true"));
	api_call(api, "carlaclient", "demo" ,obj);
	AFB_API_NOTICE(api, "StartDemoCarlaclient.");
}

/**
 *  @brief call carlaclient stop demo function
 *  @param[in] api the api serving the request
 */
static void StopDemoCarlaclient(afb_api_t api)
{
	struct json_object* obj = json_object_new_object();
	json_object_object_add(obj, "demo", json_object_new_string("false"));
	api_call(api, "carlaclient", "demo" ,obj);
	AFB_API_NOTICE(api, "StopDemoCarlaclient.");
}

/**
 *  @brief call gps subscribe function
 *  @param[in] api the api serving the request
 */
static void SubscribeGps(afb_api_t api)
{
	struct json_object* obj = json_object_new_object();
	json_object_object_add(obj, "value", json_object_new_string("location"));
	api_call(api, "gps", "subscribe" ,obj);
}

/**
 *  @brief call carlaclient subscribe function
 *  @param[in] api the api serving the request
 */
static void SubscribeCarlaclient(afb_api_t api)
{
	struct json_object* obj = json_object_new_object();
	json_object_object_add(obj, "event", json_object_new_string("positionUpdated"));
	api_call(api, "carlaclient", "subscribe" ,obj);
}
/**
 *  @brief Callback called at service startup
 */
int init(afb_api_t api)
{

	// Create instance
	geniviRequest   = new GeniviRequest();
	binderReply	 = new BinderReply();
	analyzeRequest  = new AnalyzeRequest();
	navigationInfo = new NavigationInfo();

	setwaypoints_event = afb_api_make_event(afbBindingV3root, "navicore_setwaypoints");
	pausesimulation_event = afb_api_make_event(afbBindingV3root, "navicore_pausesimulation");
	setdemorouteinfo_event= afb_api_make_event(afbBindingV3root, "navicore_setdemorouteinfo");
	stopdemo_event = afb_api_make_event(afbBindingV3root, "navicore_stopdemo");
	arrivedest_event = afb_api_make_event(afbBindingV3root, "navicore_arrivedest");
	adddest_event = afb_api_make_event(afbBindingV3root, "navicore_setdestpos");
	gps_event = afb_api_make_event(afbBindingV3root,"navicore_gps");
	heading_event = afb_api_make_event(afbBindingV3root,"navicore_heading");
	getdestdir_event = afb_api_make_event(afbBindingV3root,"navicore_getdestdir");
    cancelguidance_event = afb_api_make_event(afbBindingV3root,"navicore_cancelguidance");

	SubscribeGps(api);
	SubscribeCarlaclient(api);
	
	return 0;
}	

/**
 *  @brief API definition
 */
const afb_verb_t verbs[] = 
{
	 { verb : "navicore_getposition",			callback : OnRequestNavicoreGetPosition },
	 { verb : "navicore_getallroutes",		   callback : OnRequestNavicoreGetAllRoutes },
	 { verb : "navicore_createroute",			callback : OnRequestNavicoreCreateRoute },
	 { verb : "navicore_pausesimulation",		callback : OnRequestNavicorePauseSimulation },
	 { verb : "navicore_setsimulationmode",	  callback : OnRequestNavicoreSetSimulationMode },
	 { verb : "navicore_cancelroutecalculation", callback : OnRequestNavicoreCancelRouteCalculation },
	 { verb : "navicore_setwaypoints",		   callback : OnRequestNavicoreWaypoints },
	 { verb : "navicore_calculateroute",		 callback : OnRequestNavicoreCalculateRoute },
	 { verb : "navicore_getallsessions",		 callback : OnRequestNavicoreGetAllSessions },
	 { verb : "navicore_setcurrentpos",			callback : OnRequestNavicoreSetCurrentPos },
	 { verb : "navicore_getcurrentpos",			callback : OnRequestNavicoreGetCurrentPos },
	 { verb : "navicore_setallsessions",		callback : OnRequestNavicoreSetAllSessions },
	 { verb : "navicore_setallroutes",		   	callback : OnRequestNavicoreSetAllRoutes },
	 { verb : "navicore_setdestpos",			callback : OnRequestNavicoreSetDestPos },
	 { verb : "navicore_setdemorouteinfo",		callback : OnRequestNavicoreSetDemoRouteInfo },
	 { verb : "navicore_getrouteinfo",		   	callback : OnRequestNavicoreGetRouteInfo },
	 { verb : "navicore_stopdemo",		   		callback : OnRequestNavicoreStopDemo	 },
	 { verb : "navicore_arrivedest",		   	callback : OnRequestNavicoreArriveDest	 },
	 { verb : "navicore_setdestdir",            callback : OnRequestNavicoreSetDestDir	 },
	 { verb : "navicore_getcurrentdestdir",	    callback : OnRequestNavicoreGetCurrentDestDir },
	 { verb : "navicore_startguidance",         callback : OnRequestNavicoreStartGuidance	},
	 { verb : "navicore_cancelguidance",        callback : OnRequestNavicoreCancelGuidance	},
	 { verb : "navicore_demostate",             callback : OnRequestNavicoreDemoState	},
	 { verb : "subscribe",		   				callback : subscribe 						},
	 { verb : "unsubscribe",		   			callback : unsubscribe						},
	 { verb : NULL }
};

/**
 *  @brief Service definition
 */
const afb_binding_t afbBindingExport = 
{
	 "naviapi",
	 "",
	 "",
	 verbs,
	 NULL,
	 init,
	 onevent
};

