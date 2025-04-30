#include "NetworkUtilities.h"
#include "Senders/MessageSender.h"
vector<Uint8>* NetworkUtilities::PackMessage(string inData)
{
	//ensure message is only 1s and 0s
	if (!IsBinaryOnly(inData)) {
		cout << "Error while sending message, message was not exclusively binary characters. problem message: " << inData << endl;
		return nullptr;
	}
	//pad message to a byte
	while (inData.size() % 8 != 0) {
		inData.append("0");
	}
	//process message into compressed form
	vector<Uint8>* compressedMessage = new vector<Uint8>();
	//iterates through the message, 1 byte at a time
	for (int i = 0; i < inData.size() / 8; i++) {
		//processes the byte into its compressed form
		char newBits = stoi(inData.substr(i * 8, 8), nullptr, 2);
		//append the new byte to the compressed message
		compressedMessage->push_back(newBits);
	}
	return compressedMessage;
}
bool NetworkUtilities::IsImportantType(NetworkMessage* message)
{
	return IsImportantType(message->GetMessageType());
}
bool NetworkUtilities::IsImportantType(NetworkMessageTypes type)
{
	switch (type) {
	case Connect:
	case ConnectConfirm:
	case GameStateSync:
	case Test:
	case ImportantStateMessage1:
	case ImportantStateMessage2:
	case ImportantStateMessage3:
		return true;
	default:
		return false;
	}
}
void NetworkUtilities::SendMessageDirect(NetworkMessageTypes messageType, string message, SDLNet_DatagramSocket* socket, SDLNet_Address* address, int port)
{
	//the header of the message as 4 bits in a string
	string messageHeader = PackHeader(messageType);
	//compresses string of binary into just a string
	vector<Uint8>* compressedMessage = PackMessage(messageHeader + message);
	//sends the compressed message to the specified port and address
	SDLNet_SendDatagram(socket, address, port, compressedMessage->data(), compressedMessage->size());
}
bool NetworkUtilities::GetNextIncoming(SDLNet_DatagramSocket* socket, NetworkMessage*& message, MessageSender* sender)
{
	SDLNet_Datagram* incoming;
	if (SDLNet_ReceiveDatagram(socket, &incoming)) {
		if (!incoming) {
			return false;
		}
		NetworkMessage* tempMessage = (new NetworkMessage(incoming));
		SDLNet_DestroyDatagram(incoming);
		if (IsImportantType(tempMessage)) {
			if (sender != nullptr) {
				ImportantMessage* returnMsg = sender->ProcessImportantMessage(tempMessage); // this deletes tempMessage
				message = returnMsg;
				return message != nullptr;
			}
			else {
				cout << "Important message received but no sender provided to respond with! important message will get stuck!" << endl;
				delete tempMessage;
				return false;
			}
		}
		message = tempMessage;
		return message != nullptr;
	}
	return false;
}

void NetworkUtilities::SendMessageTo(NetworkMessageTypes messageType, string message, SDLNet_DatagramSocket* socket, SDLNet_Address* address, int port, MessageSender* sender)
{
	if (NetworkUtilities::IsImportantType(messageType)) {
		sender->SendImportantMessageTo(message, messageType, address, port);
		return;
	}
	SendMessageDirect(messageType, message, socket, address, port);
}

NetworkMessageTypes NetworkUtilities::UnpackHeader(string inData)
{
	int length = inData.size();
	if (length < 4) {
		return Error;
	}
	int header = stoi(inData.substr(0, 4), nullptr, 2);
	switch (header) {
	case 1: return Connect; // Important
	case 2: return ConnectConfirm; // Important
	case 3: return Heartbeat;
	case 4: return GameStateSync; // Important
	case 5: return Test;  // Important (sometimes)
	case 6: return ImportantMessageConfirmation;
	case 7: return StateMessage1;
	case 8: return StateMessage2;
	case 9: return StateMessage3;
	case 10: return ImportantStateMessage1; //Important
	case 11: return ImportantStateMessage2; //Important
	case 12: return ImportantStateMessage3; //Important
	default: return Error;
	}
}
string NetworkUtilities::PackHeader(NetworkMessageTypes type) {
	switch (type) {
	case Connect: return "0001";
	case ConnectConfirm: return "0010";
	case Heartbeat: return "0011";
	case GameStateSync: return "0100";
	case Test: return "0101";
	case ImportantMessageConfirmation: return "0110";
	case StateMessage1: return "0111";
	case StateMessage2: return "1000";
	case StateMessage3: return "1001";
	case ImportantStateMessage1: return "1010";
	case ImportantStateMessage2: return "1011";
	case ImportantStateMessage3: return "1100";
	default: return "0000";
	}
}

string NetworkUtilities::UnpackMessage(Uint8* inData, int messageLength)
{
	string unpackedMessage = "";
	//iterate through the message to unpack each byte
	for (int i = 0; i < messageLength; i++) {
		//convert each char into a bitset of 8 bits
		bitset<8> newBits = inData[i];
		//add the bits to the unpacked message
		unpackedMessage += newBits.to_string();
	}
	if (!IsBinaryOnly(unpackedMessage)) {
		cout << "Error while unpacking message, not all returned characters were 1s and 0s. Problem message: " << unpackedMessage << endl;
	}
	return unpackedMessage;

}
//returns the int in BCD(Binary coded decimal) form
string NetworkUtilities::AsBinaryString(int outNibbles, int value)
{
	string digits = to_string(value);
	while (digits.size() < outNibbles) {
		digits = "0" + digits;
	}
	string outString = "";
	for (int i = 0; i < digits.size(); i++) {
		bitset<4> newBits = stoi(digits.substr(i, 1));
		string newNibble = newBits.to_string();
		if (!IsBinaryOnly(newNibble) || newNibble.size() != 4) {
			cout << "Error processing integer into binary coded decimal. Problem bits: " << newNibble << endl;
			continue;
		}
		outString += (newNibble);
	}
	outString = outString.substr(0, outNibbles * 4);
	return outString;
}
//digits is the number of nibbles to process
int NetworkUtilities::IntFromBinaryString(string binaryString, int digits)
{
	int out = 0;
	for (int i = 0; i < digits; i++) {
		int num = stoi(binaryString.substr(i * 4, 4), nullptr, 2);
		out = (out * 10) + num;
	}
	return out;
}
string NetworkUtilities::AsBinaryString(int outBits, string value) {
	string outString = "";
	for (int i = 0; i < value.size(); i++) {
		outString.push_back(stoi(value.substr(i, 1), nullptr, 2));
	}
	return outString;
}
bool NetworkUtilities::VeryifyMessageState(NetworkMessage* msg, string stateCode)
{
	if (msg->GetExtraData().size() < 4)return false;
	string messageState = msg->RemoveStateVerification();
	if (messageState == stateCode) return true;
	return false;
}
bool NetworkUtilities::IsBinaryOnly(string message) {
	for (int i = 0; i < message.size(); i++) {
		if (!(message.at(i) == '0' || message.at(i) == '1')) {
			return false;
		}
	}
	return true;
}
