#pragma once

#include "pro/utils/singleton.hpp"

namespace google
{
	namespace protobuf
	{
		class Message;
	}
}

namespace xmax
{
	enum MsgId
	{
		MSG_TEST = 0,
		MSG_VER,
		MSG_VERACK,

	};

	struct MsgHeader
	{
		unsigned char	id;
		unsigned int	version;
		unsigned int	msgLength;

		MsgHeader()
			: id(0),
			  msgLength(0),
			  version(0)
		{

		}
	};

	class NetMessage
	{
public:

		char*	ToBinary() const;
		size_t	TotalLength() const;
	
		MsgHeader header;
		google::protobuf::Message *pMsg;
	};


}