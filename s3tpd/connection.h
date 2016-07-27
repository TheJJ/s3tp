// Copyright 2016-2016 the s3tp authors, LGPLv3+. See copying.md for legal info.


#pragma once

#include <cstdint>


/**
 * All s3tpd things are in here
 */
namespace s3tpd {


using connid_t = uint64_t;

class Connection {
public:
	Connection() = default;
	~Connection() = default;
};

} // s3tp
