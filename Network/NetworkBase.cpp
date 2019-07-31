#include "NetworkBase.h"

NetworkBase::NetworkBase()
{
	int result = 0;

	result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (result != 0)
	{
		throw std::runtime_error("�ʱ�ȭ ����");
	}

	baseSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (baseSocket == INVALID_SOCKET)
	{
		errorCode = WSAGetLastError();
		WSACleanup();
		throw std::runtime_error("���� �ʱ�ȭ ����");
	}
}

NetworkBase::~NetworkBase()
{

}

int NetworkBase::Recv(char* Data,int Size)
{
	return recv(baseSocket, Data, Size, 0);
}

int NetworkBase::Send(char* Data, int Size)
{
	return send(baseSocket, Data, Size, 0);
}

void NetworkBase::Error(int code ,char** msgbuf)
{
	LPWSTR buffer = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buffer, 0, NULL);
	int len = WideCharToMultiByte(CP_ACP, 0, buffer, -1, NULL, 0, NULL, NULL); // buffer ���� ���� ��ȯ���� �̿�
	*msgbuf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, buffer, -1, *msgbuf, len, NULL, NULL); // �����ڵ��� buffer���� ��ȯ
	LocalFree(buffer);// �����ߴ� �����޼��� ���� ����
}

void NetworkBase::Close()
{
	shutdown(baseSocket, SD_SEND);
	closesocket(baseSocket);
	WSACleanup();
}
