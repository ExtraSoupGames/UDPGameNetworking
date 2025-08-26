#include "DemoColourSquare.h"
#include "ColourValue.h"
#include "DemoWrapper.h"
DemoColourSquare::DemoColourSquare(DemoWrapper* demoWrapper, int xPos)
{
	wrapper = demoWrapper;
	colour = 0xFF000000;
	x = xPos;
}
DemoColourSquare::~DemoColourSquare()
{
}
void DemoColourSquare::Render(SDL_Renderer* renderer)
{
	uint8_t r = (colour >> 24) & 0xFF;
	uint8_t g = (colour >> 16) & 0xFF;
	uint8_t b = (colour >> 8) & 0xFF;
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_FRect rect{ (float)x, 100, 10, 10 };
	SDL_RenderFillRect(renderer, &rect);
}
void DemoColourSquare::UpdateLibraryValues(std::vector<INetworkedValue*>* values)
{
	((ColourValue*)values->at(0))->SetColour(colour);
	return;
}
void DemoColourSquare::UpdateEngineValues(std::vector<INetworkedValue*>* values, LibSettings* settings)
{
	colour = ((ColourValue*)values->at(0))->GetCurrentValue(wrapper->GetClientTime(), settings);
	return;
}
void DemoColourSquare::Update(float deltaTime)
{
	//nothing
}
void DemoColourSquare::HandleInput(SDL_Event& e)
{
	bool colourChanged = false;

	if (e.key.key == SDLK_R) {
		colour = 0xFF0000FF; // red
		colourChanged = true;
	}
	if (e.key.key == SDLK_G) {
		colour = 0x00FF00FF; // green
		colourChanged = true;
	}
	if (e.key.key == SDLK_B) {
		colour = 0x0000FFFF; // blue
		colourChanged = true;
	}

}