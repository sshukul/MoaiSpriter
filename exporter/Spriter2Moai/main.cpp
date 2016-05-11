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
#include "Spriter2MoaiConfig.h"

using namespace std;
using namespace tinyxml2;
using namespace boost::filesystem;

int main(int argc, const char * argv[])
{
    if ( argc < 2 ) {
        // print usage
        /*fprintf(stdout,"%s Version %d.%d\n",
            argv[0],
            Spriter2Moai_VERSION_MAJOR,
            Spriter2Moai_VERSION_MINOR); */
        fprintf(stdout,"Usage: Spriter2Moai <path_to_scml_file>\n");
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

