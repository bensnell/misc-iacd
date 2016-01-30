#pragma once

#include "ofMain.h"

struct meteorite {
public:
    string name;
    string type;
    double mass; // grams
    bool bFound;
    int year;
    string database;
    float lat;
    float lon;
};

class ofApp : public ofBaseApp{

  public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
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
	
    // holds meteorite data
    vector<meteorite> mets;
    
    // map mesh
    ofMesh metMesh;
    ofTexture tex;
    ofSpherePrimitive sphere;
    ofSpherePrimitive innersphere;
    
    
};
