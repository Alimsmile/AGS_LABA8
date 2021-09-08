#include "CNetProtocol.h"

// ��������� ���������� (��������� - IP-����� � ����� �����)
bool CNetProtocol::init(std::string ip, short port) {
	//-----------------------------------------------------------------
	// ������������� ���������� winsock
	WSADATA wsa;
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code : %d\n", WSAGetLastError());
	}
	else {
		printf("OK\n");
	}
	// �������� ������
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
		printf_s("Could not create socket : %d\n", WSAGetLastError());
	else
		printf_s("Socket created.\n");
	sockaddr_in server;
	// ������� ������ ������� � ������ �����
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip.c_str());
	server.sin_port = htons(port);

	// ���������� � �������
	int connect_error = connect(s, (struct sockaddr *)&server, sizeof(server));
	if (connect_error == SOCKET_ERROR) {
		printf_s("Connection failed : %d\n\n", WSAGetLastError());
		return false;
	}
	else {
		printf_s("Connection is established.\n\n");
		return true;
	}
}

// ��������� ��������������� ��������� (������� 00)
std::string CNetProtocol::getMessage(void) {
	std::string str;
	SNetworkHeader header;
	header.transactionID = 0;		// ����� ����������
	header.frameNumber = 1;			// ������ ����� ...
	header.frameCount = 1;			// ... �� ������
	header.dataLen = 1;				// ������������ ���� ���� ������ (��� �������)
	header.funcID = 0;				// ��� ������� = 0 (������ ��������������� ���������)
	int send_bytes_count = send(s, (const char*)&header, sizeof(header), 0);
	if (send_bytes_count < sizeof(header)) {
		return std::string("Data was not send: %d.\n" + WSAGetLastError());
	}
	waitingToRead(sizeof(SNetworkHeader));
	int receive_data_count = recv(s, (char*)&header, sizeof(header), 0);

	char *st = new char[header.dataLen - 1];
	waitingToRead(header.dataLen - 1);
	recv(s, st, header.dataLen - 1, 0);
	str = std::string(st);
	//delete[] st;

	return str;
}

// �������� ����-����� (������� 01)
std::vector<SGameObjectDescription> CNetProtocol::getDemoScene(void) {
	SNetworkHeader header;
	header.transactionID = 0;
	header.frameNumber = 1;
	header.frameCount = 1;
	header.dataLen = 1;
	header.funcID = 1;
	std::vector<SGameObjectDescription> out;

	int send_bytes_count = send(s, (const char*)&header, sizeof(header), 0);
	if (send_bytes_count < sizeof(header)) {
		printf_s("Data was not send: %d.\n", WSAGetLastError());
	}
	waitingToRead(sizeof(header));
	// ���������� ��������������� ��������� � ������� (��� ������� - 0)
	int receive_data_count = recv(s, (char*)&header, sizeof(header), 0);
	if (receive_data_count <= 0) {
		printf_s("Data was not received: %d.\n", WSAGetLastError());
	}
	if (header.dataLen > 1) {
		int count;
		waitingToRead(sizeof(int));
		receive_data_count = recv(s, (char *)&count, sizeof(int), 0);
		SGameObjectDescription description;
		for (int i = 0; i < count; i++) {
			waitingToRead(sizeof(SGameObjectDescription));
			receive_data_count = recv(s, (char*)&description, sizeof(description), 0);
			out.push_back(description);
		}
	}
	else {
		std::cout << "Information receiving error\n";
	}
	return out;
}

// �������� ����� ������� ������� (������� 02)
std::vector<SGameObjectDescription> CNetProtocol::getNearbyObjects(glm::vec3 pos, float r) {
	parameters_synchronizer.lock();
	last_position = pos;
	last_radius = r;
	parameters_synchronizer.unlock();

	if (!polling.joinable())
	{
		stop_thread = false;
		//�������� ������ 
		//������ �������� - �������
		//������ �������� - ��������� �� ������ ������5
		polling = std::thread(&CNetProtocol::NearbyObjects, this);
	}

	std::lock_guard<std::mutex> lock(result_synchronizer);
	return nearby_objects;
}

// �������� ������� ���������� ���� � ������ ������
void CNetProtocol::waitingToRead(unsigned long count) {
	unsigned long bytes_to_read = 0;
	while (bytes_to_read < count)
		ioctlsocket(s, FIONREAD, &bytes_to_read);
}

void CNetProtocol::NearbyObjects()
{
	//���������� ������
	while (!stop_thread)
	{
		//������� - �����, ��� �������������
		parameters_synchronizer.lock();
		//������ ������ � �������
		SNetworkHeader2 request = {
			transactionId++,
			1,
			1,
			17,
			2,
			last_position.x,
			last_position.y,
			last_position.z,
		last_radius};
		parameters_synchronizer.unlock();

		std::vector<SGameObjectDescription> out;
		//������ ������ � ����� - �����, �����, �����, ����
		send(s, reinterpret_cast<char*>(&request), sizeof(SNetworkHeader2), 0);
		waitingToRead(sizeof(SNetworkHeader));
		//������ ������ �� ������
		recv(s, reinterpret_cast<char*>(&request), sizeof(SNetworkHeader), 0);
		//����� ���������� ����� ������ > 1
		if (request.dataLen > 1)
		{
			unsigned int count;
			waitingToRead(sizeof(int));
			//������ ������ �� ������
			recv(s, reinterpret_cast<char*>(&count), sizeof(int), 0);
			SGameObjectDescription descr;
			for (unsigned int i = 0; i < count; i++)
			{
				waitingToRead(sizeof(SGameObjectDescription));
				//������ ������ �� ������
				recv(s, reinterpret_cast<char*>(&descr), sizeof(SGameObjectDescription), 0);
				out.push_back(descr);
			}
		}
		else
		{
			std::cout << " Can't get near obj!\n";
		}

		result_synchronizer.lock();
		//����� �� ���� ������ ���������
		nearby_objects = std::move(out);
		result_synchronizer.unlock();
	}
}