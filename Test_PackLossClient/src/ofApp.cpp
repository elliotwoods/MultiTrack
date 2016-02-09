#include "ofApp.h"

using namespace ofxCvGui;

const float frameRate = 60;

//--------------------------------------------------------------
void ofApp::setup(){
	gui.init();
	this->widgetsPanel = gui.addWidgets();

	{
		this->target.ipAddress.set("IP Address", "127.0.0.1");
		this->target.port.set("Port", 4444);
	}

	{
		this->payload.size.set("Size", 1024 * 9);
		this->payload.speed.set("Target speed [MB/s]", 500);
	}

	this->rebuildGui();

	ofSetFrameRate(frameRate);
}

//--------------------------------------------------------------
void ofApp::update(){
	auto frameIndex = ofGetFrameNum();
	if (this->client) {
		int iterations = this->payload.speed * 1e6 / (frameRate * this->payload.size);
		auto dataGram = make_shared<ofxAsio::UDP::DataGram>();
		dataGram->setEndPoint(ofxAsio::UDP::EndPoint(this->target.ipAddress, this->target.port));

		for (int i = 0; i < iterations; i++) {
			auto & message = dataGram->getMessage();

			message.resize(this->payload.size);
			auto messagePtr = &message[0];
			auto messagePtrAsInteger = (int*)messagePtr;

			for (int i = 0; i < this->payload.size / 16; i++) {
				*messagePtrAsInteger++ = frameIndex;
				*messagePtrAsInteger++ = packetIndex;
				*messagePtrAsInteger++ = i;
				*messagePtrAsInteger++ = 0;
			}

			this->client->send(dataGram);

			this->packetIndex++;
			//std::this_thread::sleep_for(1us);
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
	ofSetWindowTitle("Client");
	ofSetWindowShape(512, 1024);
	ofSetWindowPosition(256 + 512, 256);

	this->widgetsPanel->clear();

	this->widgetsPanel->add(Widgets::makeFps());

	this->widgetsPanel->add(Widgets::Title::make("Target"));
	{
		auto ipAddressWidget = Widgets::EditableValue<string>::make(this->target.ipAddress);
		auto portWidget = Widgets::EditableValue<int>::make(this->target.port);

		this->widgetsPanel->add(ipAddressWidget);
		this->widgetsPanel->add(portWidget);

		auto connectToggle = Widgets::Toggle::make("Connect", [this]() {
			if (this->client) {
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

	if (this->client) {
		this->widgetsPanel->add(Widgets::Title::make("Payload"));
		{
			auto sizeWidget = Widgets::EditableValue<int>::make(this->payload.size);
			auto speedWidget = Widgets::EditableValue<int>::make(this->payload.speed);
			this->widgetsPanel->add(sizeWidget);
			this->widgetsPanel->add(speedWidget);
		}

		this->widgetsPanel->add(Widgets::LiveValue<int>::make("Frame index", [this]() {
			return ofGetFrameNum();
		}));

		this->widgetsPanel->add(Widgets::LiveValue<int>::make("Packet index", [this]() {
			return this->packetIndex;
		}));
	}
}

//--------------------------------------------------------------
void ofApp::connect() {
	this->disconnect();
	this->client = make_shared<ofxAsio::UDP::Client>();
	this->rebuildGui();
	this->packetIndex = 0;
}

//--------------------------------------------------------------
void ofApp::disconnect() {
	this->client.reset();
	this->rebuildGui();
}