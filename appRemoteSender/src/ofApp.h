#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "ofxProfiler.h"

class Buffer {
public:
	Buffer() {
		this->buffer = nullptr;
		this->position = 0;
		this->_size = 0;
		this->allocation = 0;
	}

	~Buffer() {
		this->deallocate();
	}

	void allocate(size_t size) {
		this->deallocate();
		this->buffer = new char[size];
		this->allocation = size;
	}

	void deallocate() {
		if (this->buffer) {
			delete[] this->buffer;
			this->buffer = nullptr;
			this->position = 0;
			this->_size = 0;
			this->allocation = 0;
		}
	}

	size_t size() const {
		return this->_size;
	}

	char * begin() const {
		return this->buffer;
	}

	void append(const char * data, size_t size) {
		auto newPosition = this->position + size;
		if (newPosition > this->allocation) {
			throw(std::out_of_range("Cannot append outside of allocated region"));
		}
		memcpy(this->buffer + position, data, size);
		this->position = newPosition;
		this->_size += size;
	}

	template<typename T>
	void appendRaw(const T & data) {
		append((const char *)& data, sizeof(T));
	}

	void append(const string & data) {
		append(&data[0], data.length());
	}

	template OFXKFW2_BaseImageSimple_TEMPLATE_ARGS
	void append(const ofxKFW2::Source::BaseImage OFXKFW2_BaseImageSimple_TEMPLATE_ARGS_TRIM & source) {
		this->appendRaw(source.getWidth());
		this->appendRaw(source.getHeight());
		this->append((const char *)source.getPixels().getData(), source.getPixels().size() * sizeof(PixelType));
	}

	template<typename SourceType>
	void append(shared_ptr<ofxKFW2::Source::Base> untypedSource) {
		auto source = dynamic_pointer_cast<ofxKFW2::Source::Color>(untypedSource);
		if (source) {
			this->appendRaw(source->getTypeName().size());
			this->append(source->getTypeName());
			this->append(*source);
		}
	}
protected:
	char * buffer;
	size_t position;
	size_t _size;
	size_t allocation;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxKFW2::Device kinect;
		ICoordinateMapper* coordinateMapper;

		ofImage bodyIndexImg, foregroundImg;
		vector<ofVec2f> colorCoords;
		int numBodiesTracked;
		bool bHaveAllStreams;
};
