/**
 *
 * Created by Ben Snell on Jan 24, 2016
 *
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetFrameRate(30);
	ofEnableAlphaBlending();
    
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
    
    // print out size of vector and first entry
//    cout << mets.size() << endl;
//    cout << mets[0].name << " " << mets[0].type << " " << mets[0].mass << " " << mets[0].bFound << " " << mets[0].year << " " << mets[0].database << " " << mets[0].lat << " " << mets[0].lon << endl;
    
    // add all names to vector
    for (unsigned int i = 0, end = mets.size(); i < end; i++) {
        names.push_back(mets[i].name);
    }
    // sort vector
    sort(names.begin(), names.end());
    // add lowercase entries to their own searchable vector
    for (unsigned int i = 0, end = names.size(); i < end; i++) {
        namesLC.push_back(ofToLower(names[i]));
    }
    
    // print all names in alphabetical order
//    for (std::map<string, int>::iterator it = names.begin(), end = names.end(); it != end; ++it) {
//        cout << it->first << endl;
//    }
    
    font.load("OpenSans-Regular.ttf", 60);
    smallFont.load("OpenSans-Regular.ttf", 25);
    
    // initialized matched names vector
    for (int i = 0; i < nChars; i++) {
        vector<string> tempVec;
        matchedNames.push_back(tempVec);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
	
    // if the query has changed, search again
    if (query.compare(prevQuery)) {
        
        string queryLC = ofToLower(query);
    
        // clear the matched names vector
        for (int i = 0; i < nChars; i++) {
            matchedNames[i].clear();
        }
        // clear top matches
        topMatches.clear();
        
        // find the name matches
        if (!query.empty()) {
            
            // search through all names
            for (unsigned int i = 0, end = names.size(); i < end; i++) {
                
                // check if this query matches any other
                size_t found = namesLC[i].find(queryLC);
                if (found != string::npos) {
                    
                    // match found, so add it to the appropriate vector of matched names
                    matchedNames[(int)found].push_back(names[i]);
                }
            }
        }
        
        // find the top nMatches matches
        for (int i = 0; i < nChars; i++) {
            for (int j = 0; j < matchedNames[i].size(); j++) {
                topMatches.push_back(matchedNames[i][j]);
                if (topMatches.size() >= nMatches) break;
            }
            if (topMatches.size() >= nMatches) break; //  needed?
        }
        
        // print top nmatches matches
//        for (int i = 0; i < topMatches.size(); i++) {
//            cout << topMatches[i] << " ";
//        }
//        cout << endl;
    }
    
    prevQuery = query;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(15);
    
    // draw typed word
    ofSetColor(255);
    font.drawString(query, 60, 120);
    
    // draw flashing line
    int rectWidth = 0;
    if (query.size()) rectWidth = (font.getStringBoundingBox(query, 60, 120)).width;
    if (ofGetElapsedTimeMillis()/500 % 2) ofDrawRectangle(rectWidth + 80, 50, 8, 80);
    
    // draw top matches to screen
    for (int i = 0; i < topMatches.size(); i++) {
        smallFont.drawString(topMatches[i], 60, 200 + i * 55);
    }
    
//    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // add typed keys and delete last one with delete key
    if (key == 127) {
        if (query.size() != 0) {
            query.resize(query.size() - 1);
        }
    } else {
        if (key >= 32 && key <= 126) { //  must be actual symbol
            query = query + ofToString((char)key);
        }
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
