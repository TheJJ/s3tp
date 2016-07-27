#include "protocol_exception.h"


namespace s3tpc {


ProtocolException::ProtocolException(const std::string &what)
	:
	std::runtime_error{what} {
}


}
