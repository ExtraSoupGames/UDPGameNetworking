#include "DemoColourSquare.h"
#include "ColourValue.h"
DemoColourSquare::DemoColourSquare(DemoWrapper* demoWrapper, int xPos)
{
	wrapper = demoWrapper;
	colour = 0xFFFFFFFF;
	int x;
}
DemoColourSquare::~DemoColourSquare()
{
}
void DemoColourSquare::Render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer,
		(colour >> 24) & 0xff,
		(colour >> 16) & 0xff,
		(colour >> 8) & 0xff,
		colour * 0xff);
	const SDL_FRect* rect = new SDL_FRect{ 100, 100, 10, 10 };
	SDL_RenderRect(renderer, rect);
	delete rect;
}
void DemoColourSquare::UpdateLibraryValues(std::vector<INetworkedValue*>* values)
{
	((ColourValue*)values->at(0))->SetColour(colour);
}
void DemoColourSquare::UpdateEngineValues(std::vector<INetworkedValue*>* values, LibSettings* settings)
{
	colour = ((ColourValue*)values->at(0))->GetCurrentValue(wrapper->GetClientTime(), settings);
}
void DemoColourSquare::Update(float deltaTime)
{
	//nothing
}
void DemoColourSquare::HandleInput(SDL_Event& e)
{
	if (e.key.key == SDLK_R) {
		colour = 0xFF0000FF; // sets colour to full red
	}
	if (e.key.key == SDLK_G) {
		colour = 0x00FF00FF; // sets colour to full green
	}
	if (e.key.key == SDLK_B) {
		colour = 0x0000FFFF; // sets colour to full blue
	}
}