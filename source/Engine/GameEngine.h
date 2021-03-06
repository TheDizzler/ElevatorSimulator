#include "../pch.h"
#pragma once


#include "GraphicsEngine.h"
#include "Input.h"

#include "../Managers/GUIOverlay.h"
#include "../Managers/GFXAssetManager.h"
#include "../Managers/GameManager.h"

extern unique_ptr<GUIOverlay> guiOverlay;
extern unique_ptr<GUIFactory> guiFactory;
extern unique_ptr<GFXAssetManager> gfxAssets;

/** The engine to connect higher level game code to the low level
	graphic and OS software. This class should be reusable for any 2D game,
	thus should not contain any game logic. */
class GameEngine : public GraphicsEngine, public Input {
public:

	GameEngine();
	~GameEngine();

	bool initEngine(HWND hwnd, HINSTANCE hInstance);
	void constructErrorDialogs();
	void onAudioDeviceChange();


	void run(double time, int fps);
	virtual void render(double time);

	void suspend();
	void resume();
	void exit();

	static inline bool reportError(HRESULT hr,
		wstring failMessage = L"This is SRS Error",
		wstring failTitle = L"Fatal Error", bool dontUseGUI = false) {

		if (FAILED(hr)) {

			_com_error err(hr);
			wostringstream wss;
			wss << failMessage;
			wss << "\nHRESULT: " << err.ErrorMessage();
			if (GUIFactory::initialized && !dontUseGUI)
				GameEngine::showWarningDialog(wss.str(), failTitle);
			else if (!Globals::FULL_SCREEN)
				MessageBox(NULL, wss.str().c_str(), failTitle.c_str(), MB_OK | MB_ICONERROR);
			else
				OutputDebugString(wss.str().c_str());
			return true;
		}

		return false;
	}

	

	static void showErrorDialog(wstring message, wstring title) {
		errorDialog->open();
		errorDialog->setTitle(title);
		errorDialog->setText(message);
		showDialog = errorDialog.get();
	}

	static void showWarningDialog(wstring message, wstring title) {
		warningDialog->open();
		warningDialog->setTitle(title);
		warningDialog->setText(message);
		warningDialog->setTextTint(Color(1, 1, 1, 1));
		showDialog = warningDialog.get();
	}

	/* Critical error dialog. Exits game when dismissed. */
	static unique_ptr<Dialog> errorDialog;
	/* Minor error dialog. Choice between exit game and continue. */
	static unique_ptr<Dialog> warningDialog;
private:

	unique_ptr<AudioEngine> audioEngine;
	unique_ptr<GameManager> game;

	unique_ptr<xml_document> docAssMan;

	void update(double time);

	bool initStage();

	HWND hwnd;
	bool retryAudio;

	
	static Dialog* showDialog;
};

