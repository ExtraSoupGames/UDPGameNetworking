#include "TestWrapper.h"
//TODO populate
void TestWrapper::Update(float deltaTime)
{
}

void TestWrapper::Initialize()
{
}

void TestWrapper::RegisterObject(IEngineObject*)
{
	
}

void TestWrapper::UnregisterObject(int ID)
{
}

void TestWrapper::RegisterCallback()
{
}

void TestWrapper::StartClient()
{
}

void TestWrapper::StartServer()
{
}

void TestWrapper::ApplySettings()
{
}

void TestWrapper::InvokeRegisteredCallback(int callbackID)
{
}

IEngineObject* TestWrapper::NewNetworkedObject(int objectType)
{
	return new TestEngineObject(5, 5);
}

TestCallback::TestCallback(int callbackID) : Callback(callbackID)
{
}

void TestCallback::Invoke()
{
}