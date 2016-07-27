// Copyright 2016-2016 the s3tp authors, LGPLv3+. See copying.md for legal info.


#pragma once

#include <unordered_map>

#include "connection.h"


/**
 * All s3tpd things are in here
 */
namespace s3tpd {

class S3TPD {
public:
	S3TPD() = default;
	~S3TPD() = default;

	void loop();

protected:
	std::unordered_map<connid_t, Connection> connections;
};

} // s3tp
