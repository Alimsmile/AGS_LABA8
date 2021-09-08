#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once

#include <winsock2.h>
#include <Windows.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>
#include <glm/glm.hpp>

#pragma comment(lib, "ws2_32.lib")

#pragma pack(push,1)
struct SNetworkHeader {
	// номер транзакции
	unsigned short transactionID;
	// номер пакета среди группы пакетов одного запроса/ответа
	unsigned short frameNumber;
	// всего пакетов в группе пакетов одного запроса/ответа
	unsigned short frameCount;
	// длина оставшейс¤ части данных текущего пакета (включа¤ funcID)
	unsigned int dataLen;
	// код запроса/ответа в соответствии с протоколом
	unsigned char funcID;
};

#pragma pack(push,1)
struct SNetworkHeader2 {
	unsigned short transactionID;
	unsigned short frameNumber;
	unsigned short frameCount;
	unsigned int dataLen;
	unsigned char funcID;
	float px, py, pz;
	float radius;
};

#pragma pack(push, 1) 
struct SGameObjectDescription {
	unsigned int ObjectID;
	unsigned char ObjectType;
	float px, py, pz;
	float Yaw;
	char Model[16];
	float aabbx, aabby, aabbz;
	char Reserved[47];
};

class CNetProtocol {
private:
	unsigned short transactionId;
	// сокет, используемый дл¤ обмена сообщени¤ми
	SOCKET s;
	// признак установленного соединени¤
	bool connected;
public:
	// установка соединени¤ (параметры - IP-адрес и номер порта)
	bool init(std::string ip, short port);
	// получение приветственного сообщени¤ (команда 00)
	std::string getMessage(void);
	// получить демо-сцену (команда 01)
	std::vector<SGameObjectDescription> getDemoScene(void);
	// получить р¤дом лежащие объекты (команда 02)
	std::vector<SGameObjectDescription> getNearbyObjects(glm::vec3 pos, float r);
private:
	std::vector<SGameObjectDescription> nearby_objects;
	// ожидание нужного количества байт в буфере приема
	void waitingToRead(unsigned long count);
	glm::vec3 last_position;
	float last_radius;
	//мьютексы
	std::mutex parameters_synchronizer, result_synchronizer;
	//поток
	std::thread polling;
	//поток открыт\закрыт
	bool stop_thread;
	//функция считывания данных с сервера
	void NearbyObjects();
};