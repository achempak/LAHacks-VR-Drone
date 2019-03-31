#include "Client.h"

using namespace std;

int main()
{
	string ipAddress = "192.168.4.1";
	int port = 8080;
	//Initialize Winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock";

		return -1;
	}

	//Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket";
		WSACleanup();
		return -1;
	}
	//Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server";
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	//Do-while loop to send and receive data
	char buf[60000];
	string userInput;
	
	while (true) {
		//Sleep(30);
		int bytesReceived = recv(sock, buf, 60000, 0);
		if (bytesReceived > 0) {
			FILE* lol;
			errno_t err = fopen_s(&lol, "../newPicture.jpg", "wb+");
			//fseek(lol, 0, SEEK_SET);
			fwrite(buf, 1, bytesReceived, lol);
			cout <<"outer "<< bytesReceived << endl;
			//memset(buf, 0, 60000);
			//fclose(lol);
			//while((bytesReceived = recv(sock, buf, 60000, 0)) > 0) {
			//	FILE* lol2;
			//	errno_t err2 = fopen_s(&lol2, "C:/Users/HNguy/Downloads/newServer.jpg", "ab+");
			//	fwrite(buf, 1, bytesReceived, lol);
			//	cout <<"inner " << bytesReceived << endl;
//				fclose(lol2);
		//	}
//			fflush()
			//break;
			//memset(buf, 0, 60000);
			fclose(lol);
			
		}
	}

//	fclose(lol);
	closesocket(sock);
	WSACleanup();
}