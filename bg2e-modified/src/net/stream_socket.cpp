
#include <bg/net/stream_socket.hpp>
#include <sstream>

#if BG2E_WINDOWS
#else

#include <arpa/inet.h>

#endif

namespace bg {
namespace net {

template <class T>
void send(Socket * s , T data) {
	int bytes = 0;
	switch (sizeof(T)) {
		case 1: {
			bytes = s->send(reinterpret_cast<unsigned char*>(&data), 1);
			break;
		}
		case 2: { 
			uint16_t * p_data = reinterpret_cast<uint16_t*>(&data);
			uint16_t n_data = htons(*p_data);
			bytes = s->send(reinterpret_cast<const unsigned char*>(&n_data), 2);
			break;
		}
		case 4: {
			uint32_t * p_data = reinterpret_cast<uint32_t*>(&data);
			uint32_t n_data = htonl(*p_data);
			bytes = s->send(reinterpret_cast<const unsigned char*>(&n_data), 4);
			break;
		}
		default:
			bytes = s->send(reinterpret_cast<unsigned char*>(&data), sizeof(T));
	}
	bg::base::ThrowWhen<bg::base::ConnectionClosedException>(bytes<0);
	bg::base::Assert<bg::base::WriteNetException>(bytes>0);
}
	
template <class T>
void receive(Socket * s, T & data) {
	int bytes = 0;
	
	switch (sizeof(T)) {
		case 1:
			bytes = s->receive(reinterpret_cast<unsigned char*>(&data), 1);
			break;
		case 2: {
			uint16_t n_data;
			bytes = s->receive(reinterpret_cast<unsigned char *>(&n_data),2);
			data = static_cast<T>(ntohs(n_data));
			break;
		}
		case 4: {
			uint32_t n_data;
			bytes = s->receive(reinterpret_cast<unsigned char*>(&n_data),4);
			data = static_cast<T>(ntohl(n_data));
			break;
		}
	}
    bg::base::ThrowWhen<bg::base::ConnectionClosedException>(bytes<0);
    bg::base::Assert<bg::base::ReadNetException>(bytes>0);
}

StreamSocket::StreamSocket(Socket * s)
	:_socket(s)
{
	
}

StreamSocket::~StreamSocket() {
	
}

StreamSocket & StreamSocket::operator>> (char& val) {
	int bytes = _socket->receive(reinterpret_cast<unsigned char*>(&val), 1);
	bg::base::ThrowWhen<bg::base::ConnectionClosedException>(bytes<0);
	bg::base::Assert<bg::base::ReadNetException>(bytes == 1);
	return *this;
}

StreamSocket & StreamSocket::operator>> (bool& val) {
	int bytes = _socket->receive(reinterpret_cast<unsigned char*>(&val), 1);
	bg::base::ThrowWhen<bg::base::ConnectionClosedException>(bytes<0);
	bg::base::Assert<bg::base::ReadNetException>(bytes == 1);
	return *this;
}

StreamSocket & StreamSocket::operator>> (short& val) {
	std::string str_val;
	(*this) >> str_val;
	val = static_cast<short>(std::stoi(str_val));
	return *this;
}

StreamSocket & StreamSocket::operator>> (unsigned short& val) {
	std::string str_val;
	(*this) >> str_val;
	val = static_cast<unsigned short>(std::stoi(str_val));
	return *this;
}

StreamSocket & StreamSocket::operator>> (int& val) {
	std::string str_val;
	(*this) >> str_val;
	val = std::stoi(str_val);
	return *this;
}

StreamSocket & StreamSocket::operator>> (unsigned int& val) {
	std::string str_val;
	(*this) >> str_val;
	val = static_cast<unsigned int>(std::stoi(str_val));
	return *this;
}

StreamSocket & StreamSocket::operator>> (long& val) {
	std::string str_val;
	(*this) >> str_val;
	val = std::stol(str_val);
	return *this;
}

StreamSocket & StreamSocket::operator>> (unsigned long& val) {
	std::string str_val;
	(*this) >> str_val;
	val = static_cast<unsigned long>(std::stol(str_val));
	return *this;
}

StreamSocket & StreamSocket::operator>> (long long& val) {
	std::string str_val;
	(*this) >> str_val;
	val = std::stoll(str_val);
	return *this;
}

StreamSocket & StreamSocket::operator>> (unsigned long long& val) {
	std::string str_val;
	(*this) >> str_val;
	val = static_cast<unsigned long long>(std::stoll(str_val));
	return *this;
}

StreamSocket & StreamSocket::operator>> (float& val) {
	std::string str_val;
	(*this) >> str_val;
	val = std::stof(str_val);
	return *this;
}

StreamSocket & StreamSocket::operator>> (double& val) {
	std::string str_val;
	(*this) >> str_val;
	val = std::stod(str_val);
	return *this;
}

StreamSocket & StreamSocket::operator>> (std::string & val) {
	int size;
	int bytes = 0;
	receive(_socket, size);
	if (size>0) {
		char * buffer = new char[size + 1];
		bytes = _socket->receive(reinterpret_cast<unsigned char*>(buffer), size);
		if (bytes==size) {
			buffer[size] = '\0';
			val = buffer;
		}
		delete [] buffer;
	}
	bg::base::ThrowWhen<bg::base::ConnectionClosedException>(bytes<0);
	bg::base::Assert<bg::base::ReadNetException>(bytes>0);
	return *this;
}


StreamSocket & StreamSocket::operator<< (char val) {
	int bytes = _socket->send(reinterpret_cast<unsigned char*>(&val), 1);
	bg::base::ThrowWhen<bg::base::ConnectionClosedException>(bytes<0);
	bg::base::Assert<bg::base::ReadNetException>(bytes == 1);
	return *this;
}

StreamSocket & StreamSocket::operator<< (bool val) {
	int bytes = _socket->send(reinterpret_cast<unsigned char*>(&val), 1);
	bg::base::ThrowWhen<bg::base::ConnectionClosedException>(bytes<0);
	bg::base::Assert<bg::base::ReadNetException>(bytes == 1);
	return *this;
}

StreamSocket & StreamSocket::operator<< (short val) {
	std::string str_val = std::to_string(val);
	(*this) << str_val;
	return *this;
}

StreamSocket & StreamSocket::operator<< (unsigned short val) {
	std::string str_val = std::to_string(val);
	(*this) << str_val;
	return *this;
}

StreamSocket & StreamSocket::operator<< (int val) {
	std::string str_val = std::to_string(val);
	(*this) << str_val;
	return *this;
}

StreamSocket & StreamSocket::operator<< (unsigned int val) {
	std::string str_val = std::to_string(val);
	(*this) << str_val;
	return *this;
}

StreamSocket & StreamSocket::operator<< (long val) {
	std::string str_val = std::to_string(val);
	(*this) << str_val;
	return *this;
}

StreamSocket & StreamSocket::operator<< (unsigned long val) {
	std::string str_val = std::to_string(val);
	(*this) << str_val;
	return *this;
}

StreamSocket & StreamSocket::operator<< (long long val) {
	std::string str_val = std::to_string(val);
	(*this) << str_val;
	return *this;
}

StreamSocket & StreamSocket::operator<< (unsigned long long val) {
	std::string str_val = std::to_string(val);
	(*this) << str_val;
	return *this;
}

StreamSocket & StreamSocket::operator<< (float val) {
	std::string str_val = std::to_string(val);
	(*this) << str_val;
	return *this;
}

StreamSocket & StreamSocket::operator<< (double val) {
	std::string str_val = std::to_string(val);
	(*this) << str_val;
	return *this;
}

StreamSocket & StreamSocket::operator<< (const std::string & val) {
	int size = static_cast<int>(val.size());
	send(_socket, size);
	if (size>0) {
		int bytes = _socket->send(reinterpret_cast<const unsigned char*>(&val[0]), size);
		bg::base::ThrowWhen<bg::base::ConnectionClosedException>(bytes<0);
		bg::base::Assert<bg::base::ReadNetException>(bytes>0);
	}
	return *this;
}


	
}
}
