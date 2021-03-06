#include "ofApp.h"

using namespace ofxCvGui;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Server");
	ofSetWindowShape(512, 1024);
	ofSetWindowPosition(256, 256);

	gui.init();
	this->widgetsPanel = gui.addWidgets();
	
	{
		this->listener.port.set("Port", 4444);
		this->listener.iterationsPerFrame.set("Iterations per frame", 100);
	}

	this->rebuildGui();
}

//--------------------------------------------------------------
void ofApp::update(){
	if (this->server) {
		bool contiguous = true;
		auto iterations = this->listener.iterationsPerFrame.get();
		for (int i = 0; i < iterations; i++) {
			auto dataGram = this->server->receive(2e5);
			if (dataGram) {
				const auto & message = dataGram->getMessage();
				if (message.length() > 8) {
					auto messageIntPtr = (int*)& message[0];
					this->latestDataGram.valid = true;
					contiguous &= messageIntPtr[0] - this->latestDataGram.frameIndex == 1;
					this->latestDataGram.frameIndex = messageIntPtr[0];
					this->latestDataGram.size = message.length();
					this->history.frameIndex.insert(this->latestDataGram.frameIndex);
				}
			}
		}
		this->latestDataGram.contiguous = contiguous;

		if (this->history.frameIndex.size() > 2) {
			auto firstIndex = this->history.frameIndex.begin();

			auto lastIndex = this->history.frameIndex.end();
			lastIndex--;

			auto intendedRange = *lastIndex - *firstIndex;
			auto actualRange = this->history.frameIndex.size();

			this->history.successRate = 100.0f * (float)actualRange / (float)intendedRange;
		}
		else {
			this->history.successRate = 0.0f;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::rebuildGui() {
	this->widgetsPanel->clear();

	this->widgetsPanel->add(Widgets::makeFps());

	this->widgetsPanel->add(Widgets::LiveValueHistory::make("Megabytes per second", [this]() {
		return (float) this->listener.iterationsPerFrame * this->latestDataGram.size / ofGetLastFrameTime() / 1e6;
	}));

	this->widgetsPanel->add(Widgets::Title::make("Listener"));
	{
		this->widgetsPanel->add(Widgets::EditableValue<int>::make(this->listener.port));
		
		this->widgetsPanel->add(Widgets::EditableValue<int>::make(this->listener.iterationsPerFrame));

		auto connectToggle = Widgets::Toggle::make("Connect", [this]() {
			if (this->server) {
				return true;
			}
			else {
				return false;
			}
		}, [this](bool connect) {
			if (connect) {
				this->connect();
			}
			else {
				this->disconnect();
			}
		});
		this->widgetsPanel->add(connectToggle);
	}

	if (this->server) {
		this->widgetsPanel->add(Widgets::Title::make("Latest DataGram"));
		{
			this->widgetsPanel->add(Widgets::Indicator::make("Valid", [this]() {
				if (this->latestDataGram.valid) {
					return Widgets::Indicator::Good;
				}
				else {
					return Widgets::Indicator::Clear;
				}
			}));
			this->widgetsPanel->add(Widgets::Indicator::make("Contiguous", [this]() {
				if (this->latestDataGram.contiguous) {
					return Widgets::Indicator::Good;
				}
				else {
					return Widgets::Indicator::Clear;
				}
			}));
			this->widgetsPanel->add(Widgets::LiveValue<int>::make("Size", [this]() {
				return this->latestDataGram.size;
			}));
			this->widgetsPanel->add(Widgets::LiveValue<int>::make("Last frame index", [this]() {
				return this->latestDataGram.frameIndex;
			}));
			this->widgetsPanel->add(Widgets::LiveValueHistory::make("Success rate [%]", [this]() {
				return this->history.successRate;
			}));
			this->widgetsPanel->add(Widgets::Button::make("Clear history", [this]() {
				this->history.frameIndex.clear();
			}));
		}
	}
}

//--------------------------------------------------------------
void ofApp::connect() {
	this->disconnect();

	this->server = make_shared<ofxAsio::UDP::Server>(this->listener.port);
	this->history.frameIndex.clear();

	this->rebuildGui();
}

//--------------------------------------------------------------
void ofApp::disconnect() {
	this->server.reset();
	this->rebuildGui();
}