/**
 *
 * OFDevCon Example Code Sprint
 * Quaternion Example for plotting latitude and longitude onto a sphere
 *
 * Created by James George on 2/23/2012
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetFrameRate(30);
	ofEnableAlphaBlending();
//    ofDisableAlphaBlending();
	ofNoFill();
//    ofEnableDepthTest();
    ofEnableNormalizedTexCoords();
    
    
    // load data to buffer
    ofFile file("meteoritessize.csv");
    ofBuffer buffer(file);
    
    // read buffer to vector of meteorites
    bool bHeader = true;
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        
        // skip header line
        if (bHeader) {
            bHeader = false;
            continue;
        }
        
        // get this line
        string line = *it;
        if (line.find_first_not_of("\t\n ") == string::npos) continue; // skip empty lines
        vector<string> words;
        
        // some commas are within quotes, so delimit only when words are not in quotes
        while (!line.empty()) {
            
            // find the end index of the entry
            size_t endIndex = line.find_first_of(",");
            if (endIndex != string::npos) {
                // split by comma
                
                // check if quotation mark exists
                size_t firstQuoteIndex = line.find_first_of("\"");
                if (firstQuoteIndex != string::npos && endIndex > firstQuoteIndex) {
                    // quote exists and encompasses a comma
                    
                    // find the next quotation mark
                    size_t secondQuoteIndex = line.find_first_of("\"", firstQuoteIndex + 1);
                    // find comma after this second quotation mark
                    endIndex = line.find_first_of(",", secondQuoteIndex + 1);
                    
                    // remove these quotes from the line
                    line.erase(secondQuoteIndex, 1);
                    line.erase(firstQuoteIndex, 1);
                    // update the index
                    endIndex -= 2;
                }
            } else {
                // split by line break
                endIndex = line.find_first_of("\n");
                // or split by length by storing npos
            }
            
            // split the line appropriately and add entry to words vector
            if (endIndex != string::npos) {
                // split by the endIndex
                words.push_back(line.substr(0, endIndex));
                line.erase(0, endIndex + 1); // erase comma too
            } else {
                // add all of line
                words.push_back(line);
                line.erase();
            }
        }
        
        // put info into temp met object
        meteorite thisMet;
        thisMet.name = words[0];
        thisMet.type = words[1];
        // remove commas from words[2]
        size_t found = words[2].find_first_of(",");
        while (found != string::npos) {
            words[2].erase(found, 1);
            found = words[2].find_first_of(",");
        }
        thisMet.mass = ofToDouble(words[2]);
        thisMet.bFound = words[3].compare("Found") == 0;
        thisMet.year = ofToInt(words[4]);
        thisMet.database = words[5];
        thisMet.lat = ofToFloat(words[6]);
        thisMet.lon = ofToFloat(words[7]);
        
        // add object to vector
        mets.push_back(thisMet);
    }
    
//    cout << mets.size() << endl;
//    cout << mets[0].name << " " << mets[0].type << " " << mets[0].mass << " " << mets[0].bFound << " " << mets[0].year << " " << mets[0].database << " " << mets[0].lat << " " << mets[0].lon << endl;

    // put the meteorite data into a mesh
    metMesh.setMode(OF_PRIMITIVE_POINTS);
    metMesh.enableColors();
    for (unsigned int i = 0, end = mets.size(); i < end; i++) {
        // point at meteroite landing
        ofVec3f point(0, 0, 300);
        // two rotations
        ofQuaternion latRot, lonRot;
        latRot.makeRotate(mets[i].lat, 1, 0, 0);
        lonRot.makeRotate(mets[i].lon, 0, 1, 0);
        
        ofVec3f worldPt = latRot * lonRot * point;
        metMesh.addVertex(worldPt);
        metMesh.addColor(ofFloatColor(1));
    }
    
//    sphere.setMode(OF_PRIMITIVE_TRIANGLES);
    sphere.setMode(OF_PRIMITIVE_PATCHES);
    sphere.set(300, 100);
    sphere.enableTextures();

    ofImage img("popdensity2.png");
    tex = img.getTexture();
    
//    sphere.mapTexCoordsFromTexture(img.getTexture());

    
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	
	ofPushMatrix();
	
	//translate so that the center of the screen is 0,0
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
//    ofPushStyle();
//    ofSetColor(0, 200);
//    ofDrawCircle(0, 0, 10);
//    ofPopStyle();
	
	//add an extra spin at the rate of 1 degree per frame
	ofRotate(ofGetFrameNum()/3., 0, 1, 0);

    tex.bind();
    sphere.drawFaces();
//    ofDrawSphere(0, 0, 200);
//    ofDrawBox(0, 0, 0, 100);
    tex.unbind();
	
    ofRotate(180, 0, 1, 0);
    metMesh.drawVertices();
    
	ofPopMatrix();
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
