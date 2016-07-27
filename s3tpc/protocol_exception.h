#ifndef S3TPC_PROTOCOL_EXCEPTION_H_
#define S3TPC_PROTOCOL_EXCEPTION_H_


#include <stdexcept>
#include <string>


namespace s3tpc {


class ProtocolException : public std::runtime_error {
public:
	ProtocolException(const std::string &what);
};


}


#endif
