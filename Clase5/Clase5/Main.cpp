#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <optional>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <SFML/Network.hpp>

using uint16 = unsigned short;
using uint32 = unsigned int;

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::list;
using std::size_t;
using std::optional;
using std::quoted;
using std::stringstream;

using sf::UdpSocket;
using sf::Socket;
using sf::IpAddress;

const uint16 APP_PORT = 35001;
const uint32 MESSAGE_BUFFER_SIZE = 128;

UdpSocket g_serverSocket;

optional<IpAddress> g_senderIP;
uint16 g_senderPort;

class NetworkClient {
private:

public:
	NetworkClient() = default;
	~NetworkClient() = default;

	bool operator ==(const NetworkClient& other) {
		return (m_IP.value().toInteger() == other.m_IP.value().toInteger()
			&& m_port == other.m_port);
	}

	bool operator !=(const NetworkClient& other) {
		return !this->operator==(other);
	}

	optional <IpAddress> m_IP;
	uint16 m_port;
};

list<NetworkClient> g_clientList;

void runUDPServer(uint16 port) {
	UdpSocket socket;

	if (socket.bind(port) != Socket::Done) {
		return;
	}

	cout << "Servidor esta escuchando en el puerto: " << port;
}

void waitForMessage() {
	NetworkClient client;
	vector<char> inBufferData;
	inBufferData.resize(MESSAGE_BUFFER_SIZE);
	size_t receivedMessageSize;

	Socket::Status retValue =
	g_serverSocket.receive(&inBufferData[0],
							inBufferData.size(),
							receivedMessageSize,
							client.m_IP,
							client.m_port);

	if (retValue != Socket::Done) {
		return;
	}

	cout << "Mensaje recivido del cliente: "
		<< g_senderIP.value() << ": "
		<< quoted(inBufferData.data()) << endl;
}

void sendMessage(const string& message) {
	int lenght = message.length();
	int size = message.size();

	g_serverSocket.send(message.data(),
						message.size() + 1,
						g_senderIP.value(),
						g_senderPort);
}

void ping(uint16 port) {
	system("ping" + port);
}

int main() {
	uint32 numMessagesReceived = 0;
	stringstream myMessageStream;

	runUDPServer(APP_PORT);
	while (true) {
		waitForMessage();
		++numMessagesReceived;
		myMessageStream.str("");
		myMessageStream.clear();
		myMessageStream << "Mensaje al cliente num: " << numMessagesReceived << endl;

	}

	ping(APP_PORT);

	return 0;
}