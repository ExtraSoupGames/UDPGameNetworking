#include "DemoWrapper.h"
#include "Demo.h"
#include "DemoCallback.h"
DemoWrapper::DemoWrapper(int port)
{
	plannedPort = port;
	server = nullptr;
	otherPlayers = new std::vector<DemoPlayer*>();
	registeredCallbacks = new std::vector<Callback*>();
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
	client = new Client(plannedPort, this);
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
	//TODO fix this
}

void DemoWrapper::StartClient()
{
	client->ConnectToServer("127.0.0.1");
}

void DemoWrapper::StartServer()
{
	server = new Server("127.0.0.1", 55533, this);
}

void DemoWrapper::ApplySettings()
{
}

void DemoWrapper::InvokeRegisteredCallback(int callbackID)
{
	std::cout << "callback being called with ID: " << callbackID << std::endl;
	for (Callback* cb : *registeredCallbacks) {
		if (cb->matchesID(callbackID)) {
			cb->Invoke();
		}
	}
}

IEngineObject* DemoWrapper::NewNetworkedObject(int objectType, bool belongsToClient)
{
	DemoPlayer* dp = new DemoPlayer(this);
	if (belongsToClient) {
		otherPlayers->push_back(dp);
	}
	return dp;
}

void DemoWrapper::DrawOtherPlayers(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	for (DemoPlayer* dp : *otherPlayers) {
		const SDL_FRect rect = dp->GetRect();
		SDL_RenderRect(renderer, &rect);
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
