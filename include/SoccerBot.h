#ifndef SOCCERBOT_H
#define SOCCERBOT_H

#include "Vision.h"
#include "Controller.h"
#include "Server.h"
#include "Command.h"
#include <string>

class BaseCamera;
class XimeaCamera;
class VirtualCamera;
class Blobber;
class ProcessThread;
class Gui;
class FpsCounter;
class Robot;
class Communication;

class SoccerBot {

public:
	SoccerBot();
	~SoccerBot();

	void setup();
	void run();

	void setupVision();
	void setupProcessors();
	void setupFpsCounter();
	void setupCameras();
	void setupRobot();
	void setupControllers();
	void setupSignalHandler();
	void setupGui();
	void setupServer();
	void setupCommunication();

	void addController(std::string name, Controller* controller);
    Controller* getController(std::string name);
    bool setController(std::string name);
    std::string getActiveControllerName();

	void handleServerMessages();
	void handleServerMessage(Server::Message* message);
	void handleGetControllerCommand(Server::Message* message);
	void handleSetControllerCommand(Command::Parameters parameters, Server::Message* message);
	void handleGetFrameCommand();
	void handleStreamChoiceCommand(Command::Parameters parameters);
	void handleCameraChoiceCommand(Command::Parameters parameters);
	void handleBlobberThresholdCommand(Command::Parameters parameters);
	void handleBlobberClearCommand(Command::Parameters parameters);
	void handleScreenshotCommand(Command::Parameters parameters);
	void handleListScreenshotsCommand(Server::Message* message);

	void handleCommunicationMessages();
	void handleCommunicationMessage(std::string message);

	std::string getStateJSON();

	bool debugVision;
	bool showGui;

private:
	void setupXimeaCamera(std::string name, XimeaCamera* camera);
	bool fetchFrame(BaseCamera* camera, ProcessThread* processor);
	void broadcastFrame(unsigned char* rgb, unsigned char* classification);
	void broadcastScreenshots();

	BaseCamera* frontCamera;
	BaseCamera* rearCamera;
	XimeaCamera* ximeaFrontCamera;
	XimeaCamera* ximeaRearCamera;
	VirtualCamera* virtualFrontCamera;
	VirtualCamera* virtualRearCamera;
	Blobber* frontBlobber;
	Blobber* rearBlobber;
	Vision* frontVision;
	Vision* rearVision;
	ProcessThread* frontProcessor;
	ProcessThread* rearProcessor;
	Gui* gui;
	FpsCounter* fpsCounter;
	Vision::Results* visionResults;
	Server* server;
	Robot* robot;
	Controller* activeController;
	Communication* com;
	ControllerMap controllers;
	std::string activeControllerName;
	std::string activeStreamName;

	bool controllerRequested;
	bool running;
	bool frameRequested;
	bool useScreenshot;
	float dt;
	double lastStepTime;
	float totalTime;
	Dir debugCameraDir;

	unsigned char* jpegBuffer;
	unsigned char* screenshotBufferFront;
	unsigned char* screenshotBufferRear;
};

#endif // SOCCERBOT_H