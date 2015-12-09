#include "ofApp.h"

#include "snappy.h"

void ofApp::setup() {
	kinect.open();
	kinect.initDepthSource();
	kinect.initColorSource();
	kinect.initInfraredSource();
	kinect.initBodyIndexSource();
	kinect.initBodySource();

	ofBackground(40);
}

//--------------------------------------------------------------
void printSize(ostream & os, size_t size) {
	os << (size / 1e6) << "MB\t";
	auto fps = (1e9 / 8) / size;
	os << "[" << fps << "fps]" << endl;
}

void ofApp::update() {
	PROFILE_BEGIN("update");

	PROFILE_BEGIN("update kinect");
	kinect.update();
	PROFILE_END();

	if (true || kinect.isFrameNew()) {
		Buffer buffer;

		{
			PROFILE_SCOPE("allocate");
			buffer.allocate(50000000);
		}

		{
			PROFILE_SCOPE("copy sources");
			for (auto source : this->kinect.getSources()) {
				buffer.append<ofxKFW2::Source::Depth>(source);
				buffer.append<ofxKFW2::Source::Color>(source);
				buffer.append<ofxKFW2::Source::Infrared>(source);
				buffer.append<ofxKFW2::Source::BodyIndex>(source);
			}
		}

		cout << "Uncompressed : \t";
		printSize(cout, buffer.size());

		string compressed;
		PROFILE_BEGIN("compress");
		snappy::Compress(buffer.begin(), buffer.size(), & compressed);
		PROFILE_END();
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
