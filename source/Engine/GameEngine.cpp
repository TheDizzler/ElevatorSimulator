#include "GameEngine.h"

unique_ptr<GUIOverlay> guiOverlay;
unique_ptr<GUIFactory> guiFactory;
unique_ptr<GFXAssetManager> gfxAssets;

unique_ptr<Dialog> GameEngine::errorDialog;
unique_ptr<Dialog> GameEngine::warningDialog;
Dialog* GameEngine::showDialog = NULL;


GameEngine::GameEngine() {

}


GameEngine::~GameEngine() {
	if (audioEngine != NULL)
		audioEngine->Suspend();
}

#include "../DXTKGui/GuiAssets.h"
bool GameEngine::initEngine(HWND hw, HINSTANCE hInstance) {

	hwnd = hw;

	if (!initD3D(hwnd)) {
		MessageBox(0, L"Direct3D Initialization Failed", L"Error", MB_OK);
		return false;
	}

	if (!initRawInput(hwnd)) {
		MessageBox(0, L"Raw Input Init failed", L"Error", MB_OK);
		return false;
	}

	// Initialize Audio Engine
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	AUDIO_ENGINE_FLAGS audioFlags = AudioEngine_Default;
	//#ifdef _DEBUG
	//	audioFlags = audioFlags | AudioEngine_Debug;
	//#endif
	audioEngine.reset(new AudioEngine(audioFlags));
	retryAudio = false;

	if (!audioEngine->IsAudioDevicePresent()) {
		// no audio device found. Operating in silent mode.
		MessageBox(0, L"No audio device found!", L"Error initializing Audio Device", MB_OK);
		return false;
	}

	// get graphical assets from xml file
	docAssMan.reset(new pugi::xml_document());
	if (!docAssMan->load_file(GUIAssets::assetManifestFile)) {
		MessageBox(0, L"Could not read AssetManifest file!",
			L"Fatal Read Error!", MB_OK);
		return false;
	}

	xml_node guiAssetsNode = docAssMan->child("root").child("gui");
	guiFactory = make_unique<GUIFactory>(hwnd, guiAssetsNode);
	if (!guiFactory->initialize(device, deviceContext,
		swapChain, batch.get(), mouse)) {

		MessageBox(0, L"Failed to load GUIFactory", L"Fatal Error", MB_OK);
		return false;
	}

	xml_node gfxAssetNode = docAssMan->child("root").child("gfx");
	gfxAssets.reset(new GFXAssetManager(gfxAssetNode));
	if (!gfxAssets->initialize(device)) {
		MessageBox(0, L"Failed to load GFX Assets.", L"Fatal Error", MB_OK);
		return false;
	}


	guiOverlay = make_unique<GUIOverlay>();

	if (!initStage()) {
		MessageBox(0, L"Stage Initialization Failed", L"Error", MB_OK);
		return false;
	}

	
	Vector2 viewarea = guiOverlay->getPlayArea(); // for some reason this step is necessary
	Vector2 viewposition = guiOverlay->getPlayPosition();
	camera->updateViewport(viewarea, viewposition, true);

	ShowCursor(false);

	return true;
}

void GameEngine::onAudioDeviceChange() {
	retryAudio = true;
}

class QuitButtonListener : public Button::OnClickListener {
public:
	QuitButtonListener(GameEngine* eng) : engine(eng) {
	}
	virtual void onClick(Button * button) override {
		engine->exit();
	}

	GameEngine* engine;
};

bool GameEngine::initStage() {

	game.reset(new GameManager(this));
	if (!game->initializeGame(hwnd, device, mouse))
		return false;

	return true;
}

void GameEngine::constructErrorDialogs() {

	errorDialog.reset(guiFactory->createDialog(true, true));
	Vector2 dialogPos, dialogSize;
	dialogSize = Vector2(Globals::WINDOW_WIDTH / 2, Globals::WINDOW_HEIGHT / 2);
	dialogPos = dialogSize;
	dialogPos.x -= dialogSize.x / 2;
	dialogPos.y -= dialogSize.y / 2;
	errorDialog->setDimensions(dialogPos, dialogSize);
	errorDialog->setTint(Color(0, 120, 207));
	unique_ptr<Button> quitButton;
	quitButton.reset(guiFactory->createButton());
	quitButton->setText(L"Exit Program");
	quitButton->setOnClickListener(new QuitButtonListener(this));
	errorDialog->setCancelButton(move(quitButton));

	/*ScrollBarDesc scrollBarDesc;
	scrollBarDesc.upButtonImage = "ScrollBar Up Custom";
	scrollBarDesc.upPressedButtonImage = "ScrollBar Up Pressed Custom";
	scrollBarDesc.trackImage = "ScrollBar Track Custom";
	scrollBarDesc.scrubberImage = "Scrubber Custom";*/
	warningDialog.reset(guiFactory->createDialog(true, true));
	warningDialog->setDimensions(dialogPos, dialogSize);
	//warningDialog->setScrollBar(scrollBarDesc);
	warningDialog->setTint(Color(0, 120, 207));
	warningDialog->setCancelButton(L"Continue");
	unique_ptr<Button> quitButton2;
	quitButton2.reset(guiFactory->createButton());
	quitButton2->setText(L"Exit Program");
	quitButton2->setOnClickListener(new QuitButtonListener(this));
	warningDialog->setConfirmButton(move(quitButton2));

	showDialog = warningDialog.get();
}

bool warningCanceled = false;
void GameEngine::run(double deltaTime, int fps) {


	update(deltaTime);
	render(deltaTime);

	if (!audioEngine->IsAudioDevicePresent() && !warningCanceled) {
		// no audio device found. Operating in silent mode.
		showWarningDialog(L"No audio device found. Operating in Silent Mode.\nEnd Message...",
			L"Audio Engine failure");
		warningCanceled = true;
	}

	if (retryAudio) {
		retryAudio = false;
		if (audioEngine->Reset()) {
			// restart looped sounds
		}
	} else if (!audioEngine->Update()) {
		if (audioEngine->IsCriticalError()) {
			//ErrorDialog(L"Audio device lost!", L"Audio Engine failure");
			retryAudio = true;
		}
	}
}


void GameEngine::update(double deltaTime) {

	mouse->saveMouseState();
	keys->saveKeyboardState();
	//guiFactory->updateMouse();
	/*if (showDialog->isOpen) {
		showDialog->update(deltaTime);
	} else*/
		game->update(deltaTime, keys.get(), mouse.get());

	guiOverlay->update(deltaTime);
}



void GameEngine::render(double deltaTime) {

	deviceContext->ClearRenderTargetView(renderTargetView.Get(), Colors::PeachPuff);
	/*CommonStates blendState(device.Get());*/
	batch->Begin(SpriteSortMode_Deferred, NULL, NULL, NULL, NULL, NULL, camera->translationMatrix());
	{
		game->draw(batch.get());
		
	}
	batch->End();

	batch->Begin(SpriteSortMode_Deferred);
	{
		guiOverlay->draw(batch.get());
		showDialog->draw(batch.get());
		mouse->draw(batch.get());
	}
	batch->End();

	swapChain->Present(0, 0);
}

void GameEngine::suspend() {

	stopFullScreen();
	if (game != NULL)
		game->pause();
	if (audioEngine != NULL)
		audioEngine->Suspend();
}

void GameEngine::resume() {

	setFullScreen(Globals::FULL_SCREEN);
	if (audioEngine != NULL)
		audioEngine->Resume();
}

void GameEngine::exit() {

	//debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	if (swapChain.Get() != NULL)
		swapChain->SetFullscreenState(false, NULL);
	DestroyWindow(hwnd);
}
