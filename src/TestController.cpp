#include "TestController.h"

#include "Robot.h"
#include "Command.h"
#include "Util.h"

TestController::TestController(Robot* robot, Communication* com) : BaseAI(robot, com), running(false) {
	setupStates();
};

void TestController::setupStates() {
	states["idle"] = new IdleState(this);
	states["watch-ball"] = new WatchBallState(this);
}

void TestController::step(float dt, Vision::Results* visionResults) {
	if (currentState == NULL) {
		setState("idle");
	}

	if (running) {
		currentStateDuration += dt;
		totalDuration += dt;

		if (currentState != NULL) {
			currentState->step(dt, totalDuration, currentStateDuration);
		}
	} else {
		robot->stop();
	}
}

bool TestController::handleCommand(const Command& cmd) {
    if (cmd.name == "toggle-go") {
        handleToggleGoCommand();
    } else if (cmd.name == "stop" ||cmd.name == "toggle-side") {
        handleResetCommand();
    } else if (cmd.name.substr(0, 4) == "run-") {
        setState(cmd.name.substr(4));

		running = true;
    } else {
		return false;
	}

    return true;
}

void TestController::handleToggleGoCommand() {
	if (!toggleGoBtn.toggle()) {
		return;
	}

	running = !running;

	std::cout << "! " << (running ? "Starting test" : "Pausing test") << std::endl;
}

void TestController::handleResetCommand() {
	if (!resetBtn.toggle()) {
		return;
	}

	std::cout << "! Resetting test controller" << std::endl;

	running = false;
	totalDuration = 0.0f;
	currentStateDuration = 0.0f;

	setState(currentStateName);
}

std::string TestController::getJSON() {
	std::stringstream stream;

	stream << "{";
	stream << "\"Running\": \"" << (running ? "yes" : "no") << "\",";
	stream << "\"Current state\": \"" << currentStateName << "\",";
	stream << "\"State duration\": \"" << currentStateDuration << "\",";
	stream << "\"Total duration\": \"" << totalDuration << "\"";
	stream << "}";

	return stream.str();
}

// watch ball
void TestController::WatchBallState::onEnter() {
	std::cout << "! Enter test watch ball" << std::endl;
}

void TestController::WatchBallState::onExit() {
	std::cout << "! Exit test watch ball" << std::endl;
}

void TestController::WatchBallState::step(float dt, float totalDuration, float stateDuration) {
	std::cout << "! Step test watch ball state: " << dt << ", " << totalDuration << ", " << stateDuration << std::endl;
}