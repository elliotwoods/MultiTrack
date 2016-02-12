#include "pch.h"
#include "ofApp.h"

using namespace ofxCvGui;

!!
//Pretty sure this doesn't work

#include "StreamBufferSequence.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Client");
	ofSetWindowShape(512, 1024);
	ofSetWindowPosition(512, 256);

	gui.init();
	this->widgetsPanel = gui.addWidgets();
	
	{
		this->sender.ip.set("IP address", "127.0.0.1");
		this->sender.port.set("Port", 4444);
	}

	this->rebuildGui();
}

//--------------------------------------------------------------
void ofApp::update(){
	if (this->client) {
		asio::io_service ioService;
		asio::ip::udp::socket socket(ioService);

		StreamBuffer streamBuffer;
		socket.send(streamBuffer);

		string str;
		asio::buffer(str);
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

	this->widgetsPanel->add("Client");
	{
		this->widgetsPanel->add(this->sender.ip);
		this->widgetsPanel->add(this->sender.port);

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

	}
}

//--------------------------------------------------------------
void ofApp::connect() {
	this->disconnect();

	this->clientMutex.lock();
	{
		this->client = make_shared<ofxAsio::UDP::Socket>();
	}
	this->clientMutex.unlock();

	this->rebuildGui();
}

//--------------------------------------------------------------
void ofApp::disconnect() {
	this->clientMutex.lock();
	{
		this->client.reset();
	}
	this->clientMutex.unlock();

	this->rebuildGui();
}