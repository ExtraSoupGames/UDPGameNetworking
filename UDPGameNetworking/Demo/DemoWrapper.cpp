#include "DemoWrapper.h"
#include "Demo.h"
#include "DemoCallback.h"
DemoWrapper::DemoWrapper(int port, int lerpDelay, bool lerpEnabled)
{
	plannedPort = port;
	server = nullptr;
	otherPlayers = new std::vector<DemoPlayer*>();
	otherSquares = new std::vector<DemoColourSquare*>();
	registeredCallbacks = new std::vector<Callback*>();
	settings = new LibSettings();
	settings->lerpDelay = lerpDelay;
	settings->lerpEnabled = lerpEnabled;
}

void DemoWrapper::Update(float deltaTime)
{
	client->Update(deltaTime);
	if (server != nullptr) {
		server->Update(deltaTime);
	}
}

void DemoWrapper::Initialize()
{
	client = new Client(plannedPort, this, settings);
}

void DemoWrapper::RegisterObject(IEngineObject* object)
{
	client->RegisterObject(object);
}

void DemoWrapper::UnregisterObject(int ID)
{
	//TODO implement
}

void DemoWrapper::RegisterCallback(int ID)
{
	registeredCallbacks->push_back(new DemoCallback(ID));
}

void DemoWrapper::StartClient()
{
	client->ConnectToServer("127.0.0.1");
}

void DemoWrapper::StartServer()
{
	server = new Server("127.0.0.1", 55533, this, settings);
}

void DemoWrapper::ApplySettings()
{
}

void DemoWrapper::InvokeRegisteredCallback(int callbackID, std::string optionalExtraData)
{
	std::cout << "callback being called with ID: " << callbackID << std::endl;
	for (Callback* cb : *registeredCallbacks) {
		if (cb->matchesID(callbackID)) {
			cb->Invoke(optionalExtraData);
		}
	}
}

IEngineObject* DemoWrapper::NewNetworkedObject(int objectType, bool belongsToClient)
{
	//TODO fix default type being player
	DemoPlayer* dp = new DemoPlayer(this);
	if (belongsToClient) {
		otherPlayers->push_back(dp);
	}
	return dp;
}

INetworkedValue* DemoWrapper::NewNetworkedValue(int valueID, int valueType)
{
	switch (valueType) {
	case 0:
		return new PositionLerp2D(valueID, 0 ,0);
	case 1:
		return new ColourValue(valueID, 0);
	default:
		return nullptr;
	}
}

void DemoWrapper::DrawOtherPlayers(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	for (DemoPlayer* dp : *otherPlayers) {
		const SDL_FRect rect = dp->GetRect();
		SDL_RenderRect(renderer, &rect);
	}
	for (DemoColourSquare* dcs : *otherSquares) {
		dcs->Render(renderer);
	}
}

void DemoWrapper::CallbackTest()
{
	client->SendServerMessage(UserImportant, NetworkUtilities::AsBinaryString(500, 3) + "1010");
}

int DemoWrapper::GetClientTime()
{
	return client->GetTime();;
}
