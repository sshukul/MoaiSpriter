//
//  main.cpp
//  Spriter2Moai
//
//  Created by Cartwright Samuel on 6/17/13.
//  Copyright (c) 2013 Samuel Cartwright. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>
#include "tinyxml2.h"
#include "SpriterData.h"

using namespace std;
using namespace tinyxml2;
using namespace boost::filesystem;

int main(int argc, const char * argv[])
{
    if ( argc < 1 ) {
        // print usage
        return 1;
    }
    
    path inputFile(argv[1]);
    
    XMLDocument* doc;
    doc = new XMLDocument();
    doc->LoadFile( inputFile.c_str() );
    
    int errorID = doc->ErrorID();
    if (errorID) {
    
        delete doc;
        doc = NULL;
        
        cout <<  "Error loading file '"<< inputFile.c_str() << "'. ErrorID="<< errorID << endl;
        switch(errorID) {
            case 3:
                cout << "No such file or directory" << endl;
        }
        return 1;
    }
    
    SpriterData spriter;
    spriter.loadXML(doc->FirstChildElement());
    
    path outPath = inputFile.replace_extension(".lua");
    
    // create lua file.
    ofstream out(outPath.c_str(), ofstream::out);
    
    out << spriter;
    
    out.flush();
    out.close();
    
    return 0;
}

