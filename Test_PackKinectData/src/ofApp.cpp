#include "ofApp.h"

#include "snappy.h"

void ofApp::setup() {
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodyIndexSource();
	kinect.initBodySource();

	kinect.getColorSource()->setYuvPixelsEnabled(true);

	ofBackground(40);

	this->buffer.allocate(50e6);

	ofSetFrameRate(30);
}

//--------------------------------------------------------------
void printSize(ostream & os, size_t size) {
	os << (size / 1e6) << "MB\t";
	auto fps = (1e9 / 8) / size;
	os << "[" << fps << "fps]" << endl;
}

void ofApp::update() {
	PROFILE_START_FRAME;

	PROFILE_BEGIN("update");

	{
		PROFILE_SCOPE("reset buffer");
		this->buffer.reset();
	}

	{
		PROFILE_SCOPE("update kinect");
		kinect.update();
	}

	if (true || kinect.isFrameNew()) {
		{
			PROFILE_SCOPE("copy sources");
			for (auto source : this->kinect.getSources()) {
				if(this->depthEnabled) {
					PROFILE_SCOPE("Depth");
					buffer.append<ofxKFW2::Source::Depth>(source);
				}
				if(this->colorEnabled) {
					PROFILE_SCOPE("Color");
					buffer.append<ofxKFW2::Source::Color>(source);
				}
				if (this->infraredEnabled) {
					PROFILE_SCOPE("Infrared");
					buffer.append<ofxKFW2::Source::Infrared>(source);
				}
				if (this->bodyIndexEnabled) {
					PROFILE_SCOPE("BodyIndex");
					buffer.append<ofxKFW2::Source::BodyIndex>(source);
				}
			}
		}

		cout << "Uncompressed : \t";
		printSize(cout, buffer.size());

		string compressed;
		if (this->compressEnabled) {
			PROFILE_SCOPE("compress");
			snappy::Compress(buffer.begin(), buffer.size(), &compressed);
		}

		cout << "Compressed : \t";
		printSize(cout, compressed.size());

		cout << endl;
	}

	PROFILE_END();
}

//--------------------------------------------------------------
void ofApp::draw() {
	kinect.getColorSource()->draw(0, 0);

	stringstream msg;
	msg << (this->kinect.isFrameNew() ? "new" : "old") << endl;
	msg << "fps : " << ofGetFrameRate() << endl;
	
	msg << ofxProfiler::getResults();

	ofDrawBitmapStringHighlight(msg.str(), 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case '1':
		depthEnabled ^= true;
		break;
	case '2':
		colorEnabled ^= true;
		break;
	case '3':
		infraredEnabled ^= true;
		break;
	case '4':
		bodyIndexEnabled ^= true;
		break;
	case 'c':
		compressEnabled ^= true;
		break;
	}
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
