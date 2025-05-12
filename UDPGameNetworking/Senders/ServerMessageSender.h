#pragma once
#include "MessageSender.h"
struct ImportantBroadcast : public UnsentMessage {
	std::vector<ClientInfo*> unconfirmedClients;
	void ConfirmationReceived(NetworkMessage* confirmationMessage);
	bool IsFullyConfirmed();
	ImportantBroadcast(NetworkMessageTypes type, std::string message, std::vector<ClientInfo*> clients, int ID);
};
struct ClientMessageChecker {
	ClientInfo* client;
	std::vector<int> receivedMessages;
	//ClientInfo is deleted automatically
	ClientMessageChecker(ClientInfo* c) {
		client = c;
	}
	~ClientMessageChecker() {
		delete client;
	}
};
class ServerMessageSender : public MessageSender {
private:
	std::vector<ClientInfo*> clients;
	std::vector<ImportantBroadcast*> broadcasts;
	std::vector<ClientMessageChecker*> messageCheckers;
	void SendUnsentBroadcasts();
public:
	ServerMessageSender(SDLNet_DatagramSocket* socket, std::vector<ClientInfo*>* clients);
	void SendImportantMessage(NetworkMessageTypes type, std::string message, ClientInfo* client);
	void BroadcastImportantMessage(NetworkMessageTypes type, std::string message);

	virtual void SendUnsentMessages(bool skipCheck) override;
	virtual void ConfirmationRecieved(NetworkMessage* message) override;

	void NewClientConnected(ClientInfo* client);
	void ClientDisconnected(ClientInfo* client);

	virtual ImportantMessage* ProcessImportantMessage(NetworkMessage* importantMessage) override;
};