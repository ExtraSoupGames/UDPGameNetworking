#include "NetworkUtilities.h"
#include "Senders/MessageSender.h"
std::vector<Uint8>* NetworkUtilities::PackMessage(std::string inData)
{
	//ensure message is only 1s and 0s
	if (!IsBinaryOnly(inData)) {
		std::cout << "Error while sending message, message was not exclusively binary characters. problem message: " << inData << std::endl;
		return nullptr;
	}
	//pad message to a byte
	while (inData.size() % 8 != 0) {
		inData.append("0");
	}
	//process message into compressed form
	std::vector<Uint8>* compressedMessage = new std::vector<Uint8>();
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
	case UserImportant:
		return true;
	default:
		return false;
	}
}
void NetworkUtilities::SendMessageDirect(NetworkMessageTypes messageType, std::string message, SDLNet_DatagramSocket* socket, SDLNet_Address* address, int port)
{
	//the header of the message as 4 bits in a string
	std::string messageHeader = PackHeader(messageType);
	//compresses string of binary into just a string
	std::vector<Uint8>* compressedMessage = PackMessage(messageHeader + message);
	//sends the compressed message to the specified port and address
	SDLNet_SendDatagram(socket, address, port, compressedMessage->data(), compressedMessage->size());
}
bool NetworkUtilities::GetNextIncoming(SDLNet_DatagramSocket* socket, NetworkMessage*& message, MessageSender* sender)
{
	SDLNet_Datagram* incoming = nullptr;
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
				std::cout << "Important message received but no sender provided to respond with! important message will get stuck!" << std::endl;
				delete tempMessage;
				return false;
			}
		}
		message = tempMessage;
		return message != nullptr;
	}
	return false;
}

void NetworkUtilities::SendMessageTo(NetworkMessageTypes messageType, std::string message, SDLNet_DatagramSocket* socket, SDLNet_Address* address, int port, MessageSender* sender)
{
	if (NetworkUtilities::IsImportantType(messageType)) {
		sender->SendImportantMessageTo(message, messageType, address, port);
		return;
	}
	SendMessageDirect(messageType, message, socket, address, port);
}

NetworkMessageTypes NetworkUtilities::UnpackHeader(std::string inData)
{
	int length = inData.size();
	if (length < 4) {
		return Error;
	}
	int header = stoi(inData.substr(0, 4), nullptr, 2);
	switch (header) {
	case 1: return NetworkedObjectMsg;
	case 2: return UserImportant; // Important
	case 3: return UserUnImportant;
	case 4: return ImportantMessageConfirmation;
	default: return Error;
	}
}
std::string NetworkUtilities::PackHeader(NetworkMessageTypes type) {
	switch (type) {
	case NetworkedObjectMsg: return "0001";
	case UserImportant: return "0010";
	case UserUnImportant: return "0011";
	case ImportantMessageConfirmation: return "0100";
	default: return "0000";
	}
}

std::string NetworkUtilities::UnpackMessage(Uint8* inData, int messageLength)
{
	std::string unpackedMessage = "";
	//iterate through the message to unpack each byte
	for (int i = 0; i < messageLength; i++) {
		//convert each char into a bitset of 8 bits
		std::bitset<8> newBits = inData[i];
		//add the bits to the unpacked message
		unpackedMessage += newBits.to_string();
	}
	if (!IsBinaryOnly(unpackedMessage)) {
		std::cout << "Error while unpacking message, not all returned characters were 1s and 0s. Problem message: " << unpackedMessage << std::endl;
	}
	return unpackedMessage;

}
//returns the int in BCD(Binary coded decimal) form
std::string NetworkUtilities::AsBinaryString(int outNibbles, int value)
{
	std::string digits = std::to_string(value);
	while (digits.size() < outNibbles) {
		digits = "0" + digits;
	}
	std::string outString = "";
	for (int i = 0; i < digits.size(); i++) {
		std::bitset<4> newBits = stoi(digits.substr(i, 1));
		std::string newNibble = newBits.to_string();
		if (!IsBinaryOnly(newNibble) || newNibble.size() != 4) {
			std::cout << "Error processing integer into binary coded decimal. Problem bits: " << newNibble << std::endl;
			continue;
		}
		outString += (newNibble);
	}
	outString = outString.substr(0, outNibbles * 4);
	return outString;
}
//digits is the number of nibbles to process
int NetworkUtilities::IntFromBinaryString(std::string binaryString, int digits)
{
	int out = 0;
	for (int i = 0; i < digits; i++) {
		int num = stoi(binaryString.substr(i * 4, 4), nullptr, 2);
		out = (out * 10) + num;
	}
	return out;
}
std::string NetworkUtilities::AsBinaryString(int outBits, std::string value) {
	std::string outString = "";
	for (int i = 0; i < value.size(); i++) {
		outString.push_back(std::stoi(value.substr(i, 1), nullptr, 2));
	}
	return outString;
}
std::string NetworkUtilities::StringFromBinaryString(std::string binaryString, int length)
{
	throw new std::exception;
}
bool NetworkUtilities::VeryifyMessageState(NetworkMessage* msg, std::string stateCode)
{
	if (msg->GetExtraData().size() < 4)return false;
	std::string messageState = msg->RemoveStateVerification();
	if (messageState == stateCode) return true;
	return false;
}
bool NetworkUtilities::IsBinaryOnly(std::string message) {
	for (int i = 0; i < message.size(); i++) {
		if (!(message.at(i) == '0' || message.at(i) == '1')) {
			return false;
		}
	}
	return true;
}
