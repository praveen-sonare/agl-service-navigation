// Copyright 2017 AW SOFTWARE CO.,LTD
// Copyright 2017 AISIN AW CO.,LTD

#include <string.h>

#include "binder_reply.h"
#include "genivi_request.h"
#include "analyze_request.h"
#include "genivi/genivi-navicore-constants.h"
#include "navigation_info.h"
#include <unistd.h>
#define AFB_BINDING_VERSION 3

extern "C" {
	#include <afb/afb-binding.h>
}
using namespace std;

static void StartDemoCarlaclient(afb_api_t api);

static const char GpsEventName[] = "gps/setlocation";
static const char CarlaEventName[] = "carlaclient/positionUpdated";

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
void sendevent();
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

	// char* current_latitude  = navigationInfo->getNaviInfoCurrentLatitude();
	// char* current_longitude = navigationInfo->getNaviInfoCurrentLongitude();
	// char* current_heading = navigationInfo->getNaviInfoCurrentHeading();
	const char* current_latitude  = "35.6673965582";
  	const char* current_longitude = "139.7491882778";
	json_object_object_add(obj, "CurrentLatitude", json_object_new_string(current_latitude));
	json_object_object_add(obj, "CurrentLongitude", json_object_new_string(current_longitude));
	// json_object_object_add(obj, "CurrentHeading", json_object_new_string(current_heading));

	json_object_array_add(response_json, obj);
	const char* obj_str = json_object_to_json_string(obj);
	AFB_API_NOTICE(req->api, "obj_str = %s", obj_str);

	// Return success to BinderClient
	afb_req_success(req, response_json, "navicore_getcurrentpos");

	AFB_API_NOTICE(req->api, "<-- End %s()", __func__);
	// sendevent();
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
void sendevent(){
	const char* latitude[] = {"35.6673965582","35.66740408677707","35.66741161535195","35.66741914392463","35.6674266724951","35.66743420106332","35.66744172962928","35.667449258192924","35.66745678675417","35.667464315312834","35.667469","35.66746318432962","35.66745736865655","35.66745155298065","35.667444","35.667438328315455","35.66743265662846","35.66742698493902","35.66741564155277","35.66740996985597","35.6674042981567","35.66739862645497","35.667392954750774","35.66738728304411","35.66738161133497","35.66737593962335","35.66737026790924","35.66736459619264","35.66735892447355","35.667353252751944","35.66734758102782","35.66734190930116","35.66733623757194","35.66733056584014","35.667324894105725","35.66731922236864","35.66731355062879","35.667307878885985","35.667303","35.66729731682851","35.66729163365453","35.667285950478046","35.66728026729905","35.66727458411753","35.667268900933465","35.66726321774683","35.6672575345576","35.66725185136573","35.66724616817115","35.66724048497372","35.667233","35.66722737741701","35.66722175483148","35.667216132243404","35.66721050965275","35.66720488705949","35.667199264463584","35.66719364186496","35.667188019263506","35.66718","35.66717448371771","35.66716896743293","35.66716345114566","35.6671579348559","35.667152418563646","35.66714690226889","35.667141385971625","35.667135869671846","35.667130353369544","35.667124837064705","35.66711932075732","35.667113804447375","35.66710828813485","35.66710277181972","35.66709725550195","35.66709173918148","35.66708622285821","35.667080706531955","35.667076","35.667070449064866","35.667064898126995","35.667059347186246","35.667052","35.66704715620221","35.66704231240168","35.66703746859838","35.66703262479221","35.66702778098301","35.667022","35.667007837819966","35.66699367563963","35.666979513459","35.66696535127807","35.66695118909685","35.66693702691533","35.66692286473351","35.666908702551396","35.666894540368986","35.66688037818628","35.66686621600327","35.666852053819966","35.66683789163637","35.666823729452474","35.66680956726828","35.66679540508379","35.666781242899","35.66676708071391","35.666752918528516","35.666733","35.666718641556024","35.66670428311182","35.666689924667395","35.66667556622274","35.666661207777864","35.66664684933276","35.66663249088742","35.66661813244187","35.66660377399609","35.66658941555008","35.66657505710384","35.66656069865738","35.66654634021069","35.66653198176377","35.66651762331662","35.66650326486925","35.66648890642165","35.66647454797382","35.666458","35.66644358464129","35.66642916928238","35.66641475392326","35.66640033856394","35.666385923204416","35.66637150784469","35.66635709248475","35.66634267712461","35.666328261764264","35.66631384640371","35.66629943104295","35.666285015681986","35.666270600320814","35.66625618495944","35.66624176959785","35.666227354236064","35.66621293887407","35.66619852351186","35.66618","35.66616555635867","35.66615111271715","35.666136669075435","35.66612222543352","35.66610778179141","35.6660933381491","35.666078894506605","35.6660644508639","35.666052","35.66603753328984","35.66602306657949","35.66600859986896","35.665994133158236","35.66597966644733","35.66596519973623","35.66595073302494","35.66593626631347","35.665924","35.665909746193734","35.66589549238721","35.66588123858042","35.665866984773366","35.66585273096605","35.665838477158466","35.66582422335062","35.66580996954251","35.665795715734134","35.6657814619255","35.66576720811659","35.66575295430742","35.66573870049799","35.665724446688294","35.665710192878336","35.66569593906811","35.66568168525762","35.665667431446856","35.665651","35.665636732961616","35.66562246592298","35.66560819888408","35.665593931844924","35.665579664805506","35.66556539776583","35.665551130725895","35.6655368636857","35.665522596645246","35.665508329604535","35.66549406256356","35.66547979552232","35.66546552848083","35.665451261439074","35.665436994397055","35.665422727354766","35.665411","35.6653965337602","35.66538206752022","35.66536760128005","35.665353135039695","35.66533866879915","35.66532420255842","35.665309736317504","35.6652952700764","35.66528080383511","35.66526633759363","35.66525187135196","35.665237405110105","35.66522293886806","35.665213","35.665198358962364","35.665183717924606","35.66516907688672","35.665156","35.66514116224622","35.66512632449239","35.66511148673851","35.66509664898458","35.6650818112306","35.66506697347657","35.66505213572249","35.66503729796836","35.66502246021418","35.66500762245995","35.664992784705674","35.66497794695134","35.664963109196954","35.66494827144252","35.664939","35.66492413951694","35.66490927903384","35.6648944185507","35.66487955806752","35.66486469758429","35.66484983710102","35.66483497661771","35.664820116134365","35.664805255650975","35.66479039516754","35.66477553468407","35.66476067420055","35.66474581371699","35.66473095323339","35.66471609274975","35.664694","35.66467911458472","35.66466422916942","35.66464934375408","35.66463445833871","35.66461957292331","35.664604687507875","35.66458980209241","35.66457491667691","35.66456003126138","35.66454514584582","35.664530260430226","35.664515375014595","35.664500489598936","35.66448560418324","35.66447071876752","35.66445583335176","35.66444094793597","35.66442606252015","35.66441117710429","35.664393","35.664378250700686","35.66436350140129","35.66434875210181","35.66433400280224","35.6643192535026","35.66430450420287","35.66428975490305","35.66427500560316","35.66426025630318","35.664245507003116","35.664230757702974","35.66421600840275","35.664202","35.664187110494495","35.66417222098896","35.66415733148339","35.66414244197779","35.66412755247216","35.664112662966495","35.6640977734608","35.66408288395508","35.664067994449326","35.66405310494354","35.66403821543773","35.66402332593188","35.664008436426","35.663993546920096","35.663978657414155","35.663963767908186","35.66394887840218","35.66393398889615","35.663919099390085","35.66390420988399","35.66388932037786","35.66387443087171","35.663855","35.66384002990568","35.66382505981135","35.663810089717025","35.6637951196227","35.66378014952837","35.663765179434044","35.663751","35.663736028078596","35.66372105615719","35.66370608423579","35.66369111231439","35.66367614039299","35.663664","35.66364903584228","35.66363407168457","35.663619107526856","35.663604143369135","35.663589179211414","35.663574215053686","35.66355925089596","35.66354428673823","35.663529322580494","35.66351435842276","35.66349939426502","35.663484430107275","35.66346946594953","35.66345450179178","35.663439537634034","35.66342457347628","35.66340960931852","35.66339464516076","35.66337968100299","35.66336471684523","35.66334975268746","35.66333478852969","35.66331982437191","35.66330486021413","35.663289896056355","35.66327493189857","35.663259967740785","35.66324500358299","35.6632300394252","35.66321507526741","35.66320011110961","35.66318514695181","35.663170182794005","35.6631552186362","35.66314025447839","35.66312529032058","35.663110326162766","35.663095362004945","35.663080397847125","35.6630654336893","35.66305046953147","35.66303550537364","35.66302054121581","35.66300557705797","35.66299061290013","35.66297564874229","35.66296068458445","35.6629457204266","35.66293075626875","35.662915792110894","35.66290082795304","35.66288586379518","35.66287089963732","35.662857","35.66284203558074","35.66282707116147","35.6628121067422","35.66279714232293","35.66278217790365","35.66276721348437","35.66275224906509","35.66273728464581","35.66272232022652","35.66270735580723","35.662692391387935","35.66267742696864","35.66266246254934","35.66264749813004","35.662632533710735","35.66261756929143","35.66260260487212","35.662587640452806","35.66257267603349","35.66255771161417","35.66254274719485","35.66252778277553","35.6625128183562","35.66249785393687","35.662482889517534","35.6624679250982","35.66245296067886","35.66243799625952","35.66242303184018","35.66240806742083","35.66239310300148","35.66237813858213","35.66236317416277","35.66234820974341","35.66233324532405","35.662318280904685","35.66230331648532","35.66228835206595","35.66227338764658","35.6622584232272","35.66224345880782","35.662228494388444","35.66221352996906","35.66219856554967","35.66218360113029","35.662168636710895","35.6621536722915","35.6621387078721","35.6621237434527","35.662113","35.66209803261422","35.66208","35.66206512837674","35.66205025675344","35.662035385130096","35.66202051350672","35.662005641883304","35.66199077025985","35.661975898636356","35.66196102701283","35.661946155389266","35.66193128376566","35.66191641214202","35.66190154051834","35.66188666889463","35.66187179727088","35.661856925647086","35.66184205402326","35.66182718239939","35.66181231077549","35.66179743915155","35.661782567527574","35.661767695903556","35.6617528242795","35.661737952655415","35.66172308103128","35.66170820940712","35.661693337782914","35.661678466158676","35.661663594534396","35.661647","35.66163230683813","35.66161761367615","35.66160292051406","35.66158822735188","35.661573534189586","35.661558841027194","35.661544147864696","35.66152945470209","35.66151476153939","35.661500068376576","35.661485375213665","35.66147068205065","35.66145598888753","35.66144129572431","35.66142660256098","35.66141190939755","35.661397216234015","35.66138252307038","35.66136782990664","35.6613531367428","35.66133844357885","35.661323750414795","35.66130905725064","35.66129436408638","35.66127967092202","35.661264977757554","35.66125028459298","35.66123559142831","35.66122089826353","35.66120620509865","35.661191511933666","35.66117681876858","35.66116212560339","35.66114743243809","35.661132739272695","35.66111804610719","35.66110335294159","35.66108865977588","35.66107396661006","35.661059273444145","35.66104458027812","35.661029887112","35.66101519394577","35.66100050077944","35.660985807613","35.66097111444646","35.66095642127982","35.66094172811307","35.660927034946226","35.66091234177927","35.66089764861221","35.660882955445054","35.66086826227779","35.66085356911042","35.66083887594295","35.66082418277537","35.66080948960769","35.66079479643991","35.66078010327202","35.66076541010403","35.66075071693592","35.660742","35.66072737159572","35.66071274319131","35.660698114786776","35.66068348638211","35.66066885797732","35.6606542295724","35.660639601167354","35.66062497276218","35.660610344356876","35.660595715951445","35.660581087545886","35.6605664591402","35.660551830734384","35.66053720232845","35.660522573922385","35.660507945516194","35.660493317109875","35.66047868870343","35.66046406029685","35.66044943189015","35.66043480348332","35.66042017507636","35.660405546669274","35.66039091826206","35.66037628985472","35.66036166144725","35.66034703303965","35.660332","35.66031729631057","35.66030259262104","35.660287888931414","35.660273185241685","35.66025848155186","35.66024377786193","35.6602290741719","35.660214370481775","35.66019966679155","35.66018496310122","35.6601702594108","35.66015555572027","35.66014085202965","35.660126148338925","35.6601114446481","35.66009674095718","35.66008203726615","35.660061","35.66004723120386","35.660033462407284","35.66001969361027","35.66000592481282","35.65999215601494","35.65997838721661","35.659964618417845","35.65995084961864","35.65993708081899","35.6599233120189","35.659905","35.659895532978794","35.65988606595581","35.659876598931035","35.659867131904434","35.65985766487596","35.6598481978455","35.659842","35.659834001091596","35.659826002181","35.659818003268136","35.659810004352856","35.659804","35.659803960912555","35.659803921822245","35.65980388272907","35.65980384363303","35.659803804534114","35.65980376543232","35.65980372632765","35.6598036872201","35.65980364810966","35.65980360899633","35.6598035698801","35.65980353076097","35.659803491638925","35.65980345251396","35.65980341338607","35.65980337425524","35.65980333512146","35.65980329598471","35.65980325684497","35.6598032177022","35.65980317855637","35.65980313940743","35.65980310025529","35.65980306109979","35.65980302194058","35.659803","35.659802053462165","35.659801106921435","35.6598001603778","35.65979921383125","35.65979826728177","35.65979732072936","35.65979637417399","35.659795427615656","35.65979448105431","35.659793534489935","35.65979258792247","35.65979164135184","35.65979069477792","35.65978974820042","35.659789","35.65978747980887","35.65978595961482","35.65978443941785","35.65978291921793","35.659781399015046","35.659779878809154","35.65977835860022","35.65977683838818","35.65977531817293","35.65977379795427","35.659772","35.65976619001108","35.65976038001965","35.65975457002569","35.659748760029174","35.65974295003007","35.65973714002833","35.65973133002385","35.65972552001648","35.65972","35.659708499750586","35.65969699949997","35.659685499248155","35.659673998995125","35.65966249874086","35.659650998485304","35.659643","35.65962985227587","35.659616704551084","35.659603556825644","35.659590409099536","35.65957726137277","35.65956411364531","35.659555","35.65954108701615","35.659527174031915","35.659513261047294","35.65949934806229","35.6594854350769","35.659471522091124","35.65945760910496","35.65944369611841","35.65942978313147","35.659415870144144","35.659401957156426","35.65938804416832","35.659369","35.65935506107838","35.65934112215639","35.659327183234026","35.659313244311285","35.65929930538817","35.65928536646468","35.659271427540816","35.659257488616575","35.65924354969196","35.65922961076697","35.65921567184161","35.65920173291587","35.65918779398975","35.65917385506326","35.659159916136396","35.65914597720916","35.65913203828154","35.65911809935355","35.65910416042518","35.659090221496434","35.65907628256731","35.659062343637814","35.659048404707946","35.6590344657777","35.65902052684707","35.65900658791607","35.658992648984686","35.65897871005293","35.658964771120786","35.65895083218827","35.658936893255365","35.65892295432208","35.658911","35.658896750431424","35.65888250086258","35.65886825129347","35.658848","35.658833596618436","35.65881919323665","35.65880478985466","35.658785","35.65877021547826","35.65875543095646","35.65874064643459","35.65872586191264","35.65871107739063","35.65869629286854","35.65868150834638","35.658668","35.6586531658089","35.658638331617745","35.65862349742654","35.658608663235285","35.65859382904398","35.658578994852625","35.658564160661214","35.65854932646975","35.65853449227824","35.65851965808668","35.658504823895065","35.6584899897034","35.65847515551168","35.65846032131991","35.658445487128084","35.658436","35.658440966687756","35.65844593337277","35.65845090005498","35.65845586673427","35.65846083341038","35.658464","35.65846749258909","35.65847098517529","35.658474477758574","35.65847797033885","35.65848146291598","35.658484955489676","35.658487","35.65849333800478","35.658499676007224","35.65850601400733","35.6585123520051","35.65851869000053","35.65852502799362","35.658531365984366","35.65853770397277","35.65854404195883","35.65855037994253","35.658556717923894","35.6585630559029","35.65856939387955","35.658575731853844","35.658582069825776","35.65858840779534","35.65859474576254","35.65860108372737","35.658607421689815","35.658613759649874","35.658620097607546","35.658626435562816","35.65863277351567","35.658639111466094","35.65864544941407","35.658651787359574","35.658658125302566","35.65866446324299","35.658670801180754","35.658677139115625","35.658681","35.65868791979114","35.658694839579994","35.65870175936655","35.658708679150806","35.658715598932744","35.65872251871235","35.65872943848961","35.65873635826448","35.65874327803693","35.65875019780688","35.65875711757419","35.658764","35.6587718690267","35.65877973805132","35.65878760707386","35.65879547609431","35.65880334511267","35.65881121412893","35.65881908314309","35.65882695215513","35.65883482116506","35.658842690172854","35.6588505591785","35.65885842818197","35.658866297183245","35.65887416618226","35.658892"
};

	const char* longitude[] = {"139.7491882778","139.74917234830562","139.74915641880818","139.74914048930768","139.74912455980407","139.74910863029737","139.74909270078754","139.74907677127456","139.74906084175834","139.74904491223876","139.749035","139.74905198177655","139.74906896355049","139.74908594532172","139.749108","139.74912505539717","139.7491421107919","139.74915916618417","139.74919327696134","139.74921033234625","139.74922738772872","139.74924444310875","139.7492614984863","139.74927855386142","139.74929560923405","139.74931266460425","139.74932971997197","139.74934677533722","139.7493638307","139.7493808860603","139.74939794141812","139.74941499677345","139.74943205212628","139.7494491074766","139.7494661628244","139.74948321816964","139.74950027351227","139.7495173288522","139.749532","139.74954904957258","139.7495660991427","139.74958314871034","139.74960019827552","139.7496172478382","139.74963429739836","139.74965134695603","139.74966839651117","139.74968544606375","139.74970249561375","139.7497195451611","139.749742","139.7497590799666","139.74977615993072","139.74979323989237","139.74981031985155","139.74982739980823","139.74984447976237","139.74986155971396","139.74987863966294","139.749903","139.749920132392","139.74993726478158","139.74995439716878","139.74997152955356","139.74998866193596","139.75000579431594","139.75002292669353","139.7500400590687","139.75005719144147","139.75007432381182","139.75009145617972","139.7501085885452","139.75012572090824","139.7501428532688","139.7501599856269","139.75017711798253","139.7501942503356","139.75021138268602","139.750226","139.75024311540344","139.75026023080434","139.75027734620264","139.7503","139.75031743770558","139.75033487540892","139.75035231311006","139.75036975080891","139.75038718850544","139.750408","139.75040202148","139.7503960429621","139.75039006444635","139.75038408593272","139.75037810742123","139.75037212891183","139.75036615040457","139.75036017189944","139.75035419339645","139.75034821489558","139.75034223639682","139.7503362579002","139.7503302794057","139.75032430091332","139.75031832242308","139.75031234393498","139.750306365449","139.75030038696516","139.75029440848348","139.750286","139.7502807787298","139.7502755574615","139.75027033619506","139.75026511493053","139.75025989366787","139.7502546724071","139.7502494511482","139.75024422989117","139.75023900863602","139.75023378738277","139.7502285661314","139.7502233448819","139.75021812363428","139.75021290238857","139.75020768114473","139.75020245990277","139.7501972386627","139.75019201742455","139.750186","139.7501810220173","139.75017604403638","139.75017106605728","139.75016608807996","139.75016111010444","139.75015613213074","139.75015115415883","139.7501461761887","139.7501411982204","139.7501362202539","139.75013124228917","139.75012626432627","139.75012128636516","139.75011630840584","139.75011133044833","139.75010635249262","139.75010137453873","139.75009639658666","139.75009","139.75008514783204","139.75008029566584","139.7500754435014","139.75007059133873","139.7500657391778","139.7500608870187","139.7500560348613","139.7500511827057","139.750047","139.75004225310371","139.75003750620914","139.7500327593163","139.7500280124252","139.7500232655358","139.75001851864812","139.75001377176218","139.75000902487798","139.750005","139.74999936111232","139.74999372222666","139.749988083343","139.74998244446135","139.74997680558172","139.7499711667041","139.74996552782852","139.74995988895495","139.7499542500834","139.74994861121388","139.7499429723464","139.7499373334809","139.74993169461743","139.749926055756","139.74992041689657","139.74991477803917","139.74990913918379","139.74990350033045","139.749897","139.74989141205998","139.74988582412198","139.74988023618596","139.74987464825193","139.74986906031992","139.7498634723899","139.7498578844619","139.7498522965359","139.7498467086119","139.7498411206899","139.74983553276988","139.74982994485188","139.7498243569359","139.7498187690219","139.74981318110994","139.74980759320002","139.749803","139.74979825097057","139.74979350194283","139.74978875291683","139.74978400389256","139.74977925487","139.74977450584916","139.74976975683006","139.74976500781267","139.749760258797","139.74975550978309","139.7497507607709","139.7497460117604","139.74974126275166","139.749738","139.74973414709325","139.7497302941879","139.749726441284","139.749723","139.74972053843086","139.74971807686265","139.74971561529534","139.74971315372895","139.7497106921635","139.74970823059894","139.7497057690353","139.74970330747257","139.74970084591075","139.74969838434987","139.7496959227899","139.74969346123083","139.74969099967268","139.74968853811546","139.749687","139.74968475575724","139.74968251151535","139.74968026727427","139.74967802303405","139.74967577879465","139.7496735345561","139.7496712903184","139.7496690460815","139.74966680184545","139.74966455761023","139.74966231337586","139.74966006914232","139.7496578249096","139.74965558067774","139.7496533364467","139.74965","139.7496480218646","139.74964604372994","139.74964406559602","139.74964208746283","139.7496401093304","139.74963813119865","139.74963615306766","139.7496341749374","139.74963219680788","139.7496302186791","139.74962824055106","139.74962626242376","139.7496242842972","139.74962230617137","139.7496203280463","139.74961834992197","139.74961637179837","139.74961439367553","139.7496124155534","139.74961","139.74960683391294","139.74960366782705","139.74960050174232","139.74959733565876","139.7495941695764","139.7495910034952","139.74958783741516","139.7495846713363","139.7495815052586","139.74957833918208","139.74957517310673","139.74957200703255","139.749569","139.74956706907622","139.74956513815317","139.74956320723084","139.7495612763092","139.74955934538832","139.74955741446814","139.74955548354868","139.74955355262995","139.74955162171193","139.74954969079462","139.74954775987806","139.7495458289622","139.74954389804708","139.74954196713267","139.74954003621897","139.749538105306","139.74953617439374","139.7495342434822","139.7495323125714","139.74953038166132","139.74952845075194","139.74952651984327","139.749524","139.74952371211324","139.7495234242266","139.74952313634006","139.74952284845364","139.74952256056733","139.74952227268113","139.749522","139.749522","139.749522","139.749522","139.749522","139.749522","139.749522","139.74952140661983","139.74952081323988","139.74952021986016","139.74951962648066","139.7495190331014","139.74951843972232","139.74951784634348","139.74951725296486","139.74951665958648","139.74951606620832","139.74951547283035","139.74951487945262","139.7495142860751","139.74951369269783","139.74951309932078","139.74951250594395","139.74951191256733","139.74951131919093","139.74951072581476","139.74951013243881","139.7495095390631","139.7495089456876","139.74950835231232","139.74950775893726","139.74950716556242","139.7495065721878","139.74950597881343","139.74950538543925","139.7495047920653","139.74950419869157","139.74950360531807","139.7495030119448","139.74950241857175","139.7495018251989","139.7495012318263","139.7495006384539","139.74950004508173","139.7494994517098","139.7494988583381","139.74949826496658","139.7494976715953","139.74949707822424","139.7494964848534","139.7494958914828","139.74949529811244","139.74949470474226","139.74949411137231","139.7494935180026","139.7494929246331","139.74949233126384","139.74949173789477","139.74949114452593","139.74949055115732","139.74949","139.74948941670414","139.7494888334085","139.74948825011307","139.74948766681786","139.74948708352287","139.7494865002281","139.74948591693354","139.7494853336392","139.7494847503451","139.7494841670512","139.74948358375752","139.74948300046407","139.74948241717084","139.7494818338778","139.749481250585","139.74948066729243","139.74948008400006","139.7494795007079","139.74947891741598","139.74947833412426","139.74947775083277","139.7494771675415","139.74947658425046","139.74947600095962","139.749475417669","139.74947483437862","139.74947425108843","139.74947366779847","139.74947308450874","139.7494725012192","139.7494719179299","139.74947133464082","139.74947075135194","139.74947016806328","139.74946958477486","139.74946900148666","139.74946841819866","139.7494678349109","139.74946725162334","139.749466668336","139.74946608504888","139.749465501762","139.7494649184753","139.74946433518883","139.7494637519026","139.74946316861656","139.74946258533075","139.74946200204516","139.74946141875978","139.749461","139.74946054644275","139.74946","139.74945787056467","139.7494557411301","139.74945361169634","139.74945148226337","139.7494493528312","139.7494472233998","139.74944509396923","139.74944296453944","139.74944083511045","139.74943870568225","139.74943657625485","139.74943444682825","139.7494323174024","139.74943018797737","139.74942805855312","139.74942592912967","139.74942379970702","139.74942167028516","139.7494195408641","139.74941741144383","139.74941528202436","139.74941315260568","139.7494110231878","139.74940889377072","139.7494067643544","139.74940463493888","139.74940250552416","139.74940037611023","139.749398","139.7493944606125","139.7493909212263","139.7493873818414","139.74938384245777","139.74938030307547","139.74937676369447","139.74937322431478","139.7493696849364","139.7493661455593","139.7493626061835","139.749359066809","139.74935552743582","139.74935198806395","139.74934844869338","139.74934490932412","139.74934136995614","139.74933783058947","139.7493342912241","139.74933075186004","139.7493272124973","139.74932367313585","139.74932013377568","139.74931659441683","139.74931305505928","139.74930951570303","139.7493059763481","139.74930243699447","139.74929889764212","139.74929535829108","139.74929181894134","139.74928827959292","139.7492847402458","139.7492812009","139.74927766155548","139.74927412221226","139.74927058287034","139.74926704352973","139.74926350419042","139.74925996485243","139.74925642551574","139.74925288618033","139.74924934684623","139.74924580751343","139.74924226818194","139.74923872885176","139.7492351895229","139.74923165019533","139.74922811086904","139.74922457154406","139.7492210322204","139.74921749289803","139.74921395357697","139.74921041425722","139.74920687493878","139.74920333562162","139.74919979630576","139.7491962569912","139.74919271767797","139.74918917836604","139.7491856390554","139.74918209974612","139.74918","139.74917607528624","139.74917215057394","139.7491682258631","139.74916430115366","139.74916037644567","139.7491564517391","139.749152527034","139.74914860233034","139.7491446776281","139.7491407529273","139.74913682822796","139.74913290353004","139.74912897883357","139.74912505413852","139.74912112944492","139.74911720475276","139.74911328006203","139.74910935537275","139.74910543068492","139.7491015059985","139.74909758131355","139.74909365663004","139.74908973194795","139.7490858072673","139.74908188258811","139.74907795791034","139.74907403323402","139.74907","139.7490665275306","139.7490630550625","139.74905958259566","139.7490561101301","139.74905263766584","139.74904916520285","139.74904569274113","139.74904222028073","139.7490387478216","139.74903527536372","139.74903180290715","139.74902833045186","139.74902485799785","139.74902138554512","139.74901791309367","139.74901444064346","139.74901096819457","139.749006","139.7489987625417","139.7489915250859","139.7489842876326","139.74897705018182","139.74896981273355","139.74896257528778","139.74895533784454","139.7489481004038","139.74894086296558","139.7489336255299","139.748924","139.74890972431507","139.74889544863362","139.74888117295566","139.74886689728123","139.74885262161038","139.7488383459432","139.748829","139.74881342316326","139.74879784632975","139.7487822694996","139.74876669267286","139.748755","139.74873657291732","139.74871814583466","139.74869971875202","139.7486812916694","139.7486628645868","139.74864443750423","139.74862601042167","139.74860758333912","139.7485891562566","139.7485707291741","139.7485523020916","139.74853387500914","139.74851544792668","139.74849702084424","139.74847859376183","139.74846016667942","139.74844173959704","139.7484233125147","139.74840488543234","139.74838645835","139.74836803126772","139.74834960418542","139.74833117710315","139.74831275002092","139.74829432293868","139.748284","139.74826560971738","139.7482472194352","139.74822882915345","139.74821043887215","139.7481920485913","139.7481736583109","139.74815526803093","139.74813687775142","139.74811848747237","139.74810009719377","139.74808170691563","139.74806331663794","139.74804492636073","139.748026536084","139.748012","139.74799366809253","139.7479753361858","139.74795700427975","139.74793867237446","139.74792034046988","139.74790200856603","139.7478836766629","139.7478653447605","139.74784701285887","139.747828680958","139.747807","139.74779001691445","139.74777303383144","139.74775605075098","139.74773906767308","139.74772208459777","139.74770510152504","139.747688118455","139.74767113538766","139.747655","139.74764320102918","139.7476314020618","139.74761960309792","139.74760780413752","139.74759600518064","139.74758420622737","139.747576","139.7475671850389","139.74755837008072","139.7475495551255","139.7475407401732","139.74753192522385","139.7475231102775","139.747517","139.74751019309412","139.74750338619063","139.7474965792895","139.74748977239076","139.7474829654944","139.74747615860045","139.74746935170887","139.74746254481968","139.74745573793285","139.74744893104844","139.7474421241664","139.7474353172868","139.747426","139.74741927397343","139.7474125479492","139.74740582192734","139.74739909590784","139.7473923698907","139.74738564387587","139.74737891786341","139.74737219185332","139.74736546584558","139.74735873984017","139.74735201383712","139.74734528783642","139.74733856183806","139.74733183584206","139.74732510984842","139.74731838385713","139.7473116578682","139.74730493188162","139.74729820589738","139.7472914799155","139.747284753936","139.74727802795883","139.747271301984","139.74726457601153","139.74725785004142","139.74725112407367","139.74724439810828","139.74723767214525","139.7472309461846","139.74722422022631","139.74721749427042","139.7472107683169","139.747205","139.74719934540562","139.74719369081325","139.74718803622292","139.74718","139.74717497024446","139.74716994049078","139.74716491073892","139.747158","139.74715509363728","139.7471521872756","139.74714928091498","139.74714637455546","139.747143468197","139.74714056183964","139.74713765548336","139.747135","139.74713250631447","139.74713001262987","139.7471275189462","139.74712502526344","139.74712253158162","139.74712003790074","139.74711754422077","139.74711505054174","139.74711255686364","139.74711006318645","139.7471075695102","139.74710507583487","139.74710258216047","139.747100088487","139.74709759481445","139.747096","139.74707861662253","139.7470612332428","139.74704384986075","139.74702646647637","139.74700908308952","139.746998","139.74698008154868","139.74696216309573","139.74694424464113","139.74692632618488","139.7469084077269","139.74689048926712","139.74688","139.74686330569685","139.74684661139102","139.74682991708252","139.74681322277135","139.7467965284575","139.746779834141","139.7467631398218","139.74674644549995","139.74672975117542","139.74671305684822","139.74669636251835","139.7466796681858","139.74666297385056","139.74664627951265","139.74662958517206","139.74661289082877","139.7465961964828","139.74657950213415","139.7465628077828","139.74654611342874","139.74652941907198","139.74651272471252","139.74649603035036","139.74647933598547","139.74646264161785","139.74644594724748","139.74642925287435","139.7464125584984","139.7463958641196","139.7463791697378","139.746369","139.7463526593352","139.74633631866755","139.746319977997","139.74630363732356","139.7462872966472","139.74627095596796","139.74625461528578","139.74623827460067","139.74622193391258","139.74620559322148","139.74618925252724","139.746173","139.74615732347988","139.74614164695663","139.74612597043026","139.74611029390073","139.74609461736807","139.7460789408323","139.74606326429335","139.74604758775126","139.74603191120602","139.74601623465762","139.74600055810603","139.74598488155124","139.74596920499323","139.74595352843195","139.745918"};
	const char* carspeed = "60";

	int len = sizeof(latitude)/sizeof(char*);
		printf("Naviservice: len====== %d \n",len);
	for (int i=0;i<len;i++){
		struct json_object* obj = json_object_new_object();
		const char * lat = latitude[i];
		const char * lon = longitude[i];
		const char * speed = carspeed;
		json_object_object_add(obj, "latitude", json_object_new_string(lat));
		json_object_object_add(obj, "longitude", json_object_new_string(lon));
		json_object_object_add(obj, "carSpeed", json_object_new_string(speed));
		printf("lat %s lon %s speed %s \n",lat,lon,speed);
		// AFB_API_NOTICE(api, "Get gps date.Latitude %s Longitude %s", lat, lon);
		afb_event_push(gps_event, obj);
		// sleep(1);
		usleep(300*1000);
	}
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

	struct json_object* response_json = json_object_new_object();

	AFB_REQ_NOTICE(req, "OnRequestNavicoreSetDestDir");

	json_object_object_add(response_json, "state", json_object_new_string(state));

	const char* response_json_str = json_object_to_json_string(response_json);
	AFB_REQ_NOTICE(req, "response_json_str = %s", response_json_str);

	afb_event_push(getdestdir_event, response_json);
}

/**
 *  @brief navicore_startguidance request callback
 *  @param[in] req Request from server
 */
void OnRequestNavicoreStartGuidance(afb_req_t req)
{
	AFB_REQ_NOTICE(req, "OnRequestNavicoreStartGuidance");
	// StartDemoCarlaclient(req->api);
	sendevent();
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
 *  @brief call carlaclient startdemo function
 *  @param[in] api the api serving the request
 */
static void StartDemoCarlaclient(afb_api_t api)
{
	struct json_object* obj = json_object_new_object();
	json_object_object_add(obj, "startdemo", json_object_new_string("true"));
	api_call(api, "carlaclient", "startdemo" ,obj);
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

	// SubscribeGps(api);
	// SubscribeCarlaclient(api);
	
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
	 { verb : "navicore_startguidance",         callback : OnRequestNavicoreStartGuidance	},
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

