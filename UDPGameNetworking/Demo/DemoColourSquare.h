#pragma once
#include <SDL3/SDL.h>
#include "../Wrapper/IEngineObject.h"
class DemoWrapper;
class DemoColourSquare : public IEngineObject
{
private:
	uint32_t colour;
	DemoWrapper* wrapper;
	int x;
protected:
public:
	//TODO remove xpos and just render other clients' squares seperately
	DemoColourSquare(DemoWrapper* wrapper, int xPos);
	~DemoColourSquare();
	void HandleInput(SDL_Event& e);
	void Update(float deltaTime);

	void Render(SDL_Renderer* renderer);
	virtual void UpdateLibraryValues(std::vector<INetworkedValue*>* values) override;
	virtual void UpdateEngineValues(std::vector<INetworkedValue*>* values, LibSettings* settings) override;
};