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
	//TODO fix heap allocation of FRect
	int r = (colour >> 24) & 0xff;
	int g = (colour >> 16) & 0xff;
	int b = (colour >> 8) & 0xff;
	std::cout << "r is: " << r << " g: " << g << "b: " << b << std::endl;
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	const SDL_FRect* rect = new SDL_FRect{ (float)x, 100, 10, 10};
	SDL_RenderRect(renderer, rect);
	delete rect;
}
void DemoColourSquare::UpdateLibraryValues(std::vector<INetworkedValue*>* values)
{
	((ColourValue*)values->at(0))->SetColour(colour);
	return;
	std::cout << "Lib values being updated from demo square (client owned square)" << std::endl;
	std::cout << "colour is: " << ((ColourValue*)values->at(0))->Debug() << std::endl;
}
void DemoColourSquare::UpdateEngineValues(std::vector<INetworkedValue*>* values, LibSettings* settings)
{
	colour = ((ColourValue*)values->at(0))->GetCurrentValue(wrapper->GetClientTime(), settings);
	return;
	std::cout << "colour square value being set from lib (foreign owned square)" << std::endl;
	std::cout << "colour is: " << ((ColourValue*)values->at(0))->Debug() << std::endl;

}
void DemoColourSquare::Update(float deltaTime)
{
	//nothing
}
void DemoColourSquare::HandleInput(SDL_Event& e)
{
	if (e.key.key == SDLK_R) {
		std::cout << "setting colour to red!" << std::endl;
		colour = 0xFF0000FF; // sets colour to full red
	}
	if (e.key.key == SDLK_G) {
		colour = 0x00FF00FF; // sets colour to full green
	}
	if (e.key.key == SDLK_B) {
		colour = 0x0000FFFF; // sets colour to full blue
	}
}