//
// Created by Spud on 6/3/21.
//

#ifndef FOXWEBHOOK_ERRORCODES_HPP
#define FOXWEBHOOK_ERRORCODES_HPP
class ErrorCodes {

public:

	const static int CANNOT_READ = -1;

	const static int INVALID_JSON_FORMAT = -2;

	const static int MISSING_KEYS = -3;

	const static int MISSING_WEBHOOK = -4;

	const static int WEBHOOK_NOT_ARRAY = -5;

	const static int MISSING_RETRIEVE_FROM = -6;

	const static int MISSING_SEND_TO = -7;

	const static int MISSING_KEY = -8;

	const static int CANNOT_GET_INITIAL_POST = -9;

};


#endif //FOXWEBHOOK_ERRORCODES_HPP
