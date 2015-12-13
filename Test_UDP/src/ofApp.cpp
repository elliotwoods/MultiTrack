#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	this->serverThread = thread([this]() {
		ofxAsio::UDP::Server server(4444);
		
		while (true) {
			auto dataGram = server.receive();
			if (dataGram) {
				this->lockOutput.lock();
				cout << "[" << ofGetFrameNum() << "] - Server received - ";
				cout << "\"" << dataGram->getMessage() << "\"";
				cout << endl;
				this->lockOutput.unlock();

				auto & message = dataGram->getMessage();
				message = "Returning {" + message + "}";
				server.send(dataGram);
			}
		}
	});

	this->clientThread= thread([this]() {
		ofxAsio::UDP::Server client;

		while (true) {
			{
				auto sendDataGram = make_shared<ofxAsio::UDP::DataGram>();
				sendDataGram->setEndPoint(ofxAsio::UDP::EndPoint("127.0.0.1", 4444));
				sendDataGram->setMessage("Client to server");
				client.send(sendDataGram);
			}

			{
				auto dataGram = client.receive();
				if (dataGram) {
					this->lockOutput.lock();
					cout << "[" << ofGetFrameNum() << "] - Client received - ";
					cout << "\"" << dataGram->getMessage() << "\"";
					cout << endl;
					this->lockOutput.unlock();
				}
			}
		}
	});
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	stringstream msg;
	msg << "fps : " << ofGetFrameRate() << endl;
	
	msg << ofxProfiler::getResults();

	ofDrawBitmapStringHighlight(msg.str(), 10, 20);
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
