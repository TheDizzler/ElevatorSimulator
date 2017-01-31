#include "../pch.h"
#pragma once


#include "../Engine/Camera.h"
#include "../GameObjects/Building.h"


//extern unique_ptr<Building> building;

class GameEngine;

/** The lowest level of class where game code should be included.
	Everything below this (GameEngine downward) should generally go unmodified. */
class GameManager {
public:
	GameManager(GameEngine* gameEngine);
	~GameManager();


	bool initializeGame(HWND hwnd, ComPtr<ID3D11Device> device, shared_ptr<MouseController> mouse);

	void generateBuilding();

	void update(double deltaTime, KeyboardController* keys, MouseController* mouse);
	void draw(SpriteBatch* batch);


	void loadLevel(string levelName);
	void loadMainMenu();

	void pause();
	void exit();

	vector<ComPtr<IDXGIAdapter> > getAdapterList();
	vector<ComPtr<IDXGIOutput> > getDisplayList();
	vector<ComPtr<IDXGIOutput> > getDisplayListFor(size_t displayIndex);
	vector<ComPtr<IDXGIOutput> > getDisplayListFor(ComPtr<IDXGIAdapter> adapter);
	vector<DXGI_MODE_DESC> getDisplayModeList(size_t displayIndex);
	vector<DXGI_MODE_DESC> getDisplayModeList(ComPtr<IDXGIOutput> display);

	bool setAdapter(size_t adapterIndex);
	bool setDisplayMode(size_t displayModeIndex);
	bool setFullScreen(bool isFullScreen);

	size_t getSelectedAdapterIndex();
	size_t getSelectedDisplayIndex();
	size_t getSelectedDisplayModeIndex();

private:

	Screen* currentScreen = 0;
	Screen* switchTo = NULL;
	Screen* lastScreen = 0;



	GameEngine* gameEngine;
	shared_ptr<MouseController> mouse;
	ComPtr<ID3D11Device> device;

	size_t spinnerID;
	unique_ptr<Building> building;

	//shared_ptr<Camera> camera;


};


class GenerateBuildingListener : public Button::OnClickListener {
public:
	GenerateBuildingListener(GameManager* gm) : game(gm) {
	}
	virtual void onClick(Button* button) override {
		game->generateBuilding();
	}

private:
	GameManager* game;

};