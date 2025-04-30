#pragma once
#include "MessageSender.h"
using namespace std;
struct ImportantBroadcast : public UnsentMessage {
	vector<ConnectedClient*> unconfirmedClients;
	void ConfirmationReceived(NetworkMessage* confirmationMessage);
	bool IsFullyConfirmed();
	ImportantBroadcast(NetworkMessageTypes type, string message, vector<ConnectedClient*> clients, int ID);
};
struct ClientMessageChecker {
	ConnectedClient* client;
	vector<int> receivedMessages;
	//ConnectedClient is deleted automatically
	ClientMessageChecker(ConnectedClient* c) {
		client = c;
	}
	~ClientMessageChecker() {
		delete client;
	}
};
class ServerMessageSender : public MessageSender {
private:
	vector<ConnectedClient*> clients;
	vector<ImportantBroadcast*> broadcasts;
	vector<ClientMessageChecker*> messageCheckers;
	void SendUnsentBroadcasts();
public:
	ServerMessageSender(SDLNet_DatagramSocket* socket, vector<ConnectedClient*> clients);
	void SendImportantMessage(NetworkMessageTypes type, string message, ConnectedClient* client);
	void BroadcastImportantMessage(NetworkMessageTypes type, string message);

	virtual void SendUnsentMessages(bool skipCheck) override;
	virtual void ConfirmationRecieved(NetworkMessage* message) override;

	void NewClientConnected(ConnectedClient* client);
	void ClientDisconnected(ConnectedClient* client);

	virtual ImportantMessage* ProcessImportantMessage(NetworkMessage* importantMessage) override;
};