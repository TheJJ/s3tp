#ifndef S3TPC_CONNECTION_H_
#define S3TPC_CONNECTION_H_


#include <cstdint>


namespace s3tpc {


class S3TPClient;


class Connection {
public:
	static constexpr const uint16_t NEW_CONNECTION_ID = 0;

private:
	S3TPClient *parent;
	uint16_t id;
	bool initialized;

public:
	Connection(S3TPClient *parent);
	virtual ~Connection();

	void initialize(uint16_t id);

	int get_id() const;
	bool is_initialized() const;
};


}


#endif
