#include "ofApp.h"

void ofApp::setup() {

	string text = " \n\
		....XXXXXXXX \n\
		..XXX..ooooXXX \n\
		..X....oooo..X \n\
		.X....oooooo..X \n\
		XX...oo....oo.XX \n\
		Xooooo......oooX \n\
		Xo..oo......oooX \n\
		X....o......oo.X \n\
		X....oo....oo..X \n\
		Xo..ooooooooo..X \n\
		XoooXXXXXXXXoo.X \n\
		.XXX..X..X..XXX \n\
		..X...X..X...X \n\
		..X..........X \n\
		...X........X \n\
		....XXXXXXXX \n\
		";


	auto codec = ofxSquash::getCodecList()["density"];

	cout << "start" << endl;
	stringstream ss;

	ofxSquash::Stream compressStream(codec, ofxSquash::Direction::Compress);
	ofxSquash::Stream decompressStream(codec, ofxSquash::Direction::Decompress);

	compressStream.setWriteFunction([&decompressStream](const uint8_t* data, size_t size) {
		cout << "Compressed" << endl;
		cout << "==========" << endl;
		cout << string((const char*)data, size) << endl;

		decompressStream.read(data, size);

		cout << size << endl;
	}); 
	
	decompressStream.setWriteFunction([](const uint8_t* data, size_t size) {
		cout << "Decompressed" << endl;
		cout << "============" << endl;
		cout << string((const char*)data, size) << endl;
	});

	
	for (int i = 0; i < 10; i++)
	{
		compressStream << text << std::to_string(i);
	}
	compressStream << ofxSquash::Stream::Finish();
	decompressStream << ofxSquash::Stream::Finish();

	cout << "Press ENTER to close program...";
	cin.get();
	
	ofExit();
}