//
//  map.h
//  Spriter2Moai
//

#ifndef Spriter2Moai_Map_h
#define Spriter2Moai_Map_h

#include <string>
#include "tinyxml2.h"

using namespace std;

class CharacterMap;

class Map {
private:
    CharacterMap* m_owner;
    int m_folder;
    int m_file;
    int m_target_folder;
    int m_target_file;
    
public:
    Map(): m_folder(-1), m_file(-1), m_target_folder(-1), m_target_file(-1)  {}
    Map(CharacterMap* a_owner): m_owner(a_owner), m_folder(-1), m_file(-1), m_target_folder(-1), m_target_file(-1) {}
    
    ~Map() {}
    
    inline int getFolder() const {return m_folder;}
    inline void setFolder(int a_folder) {m_folder = a_folder;}
    inline int getFile() const {return m_file;}
    inline void setFile(int a_file) {m_file = a_file;}
    inline int getTargetFolder() const {return m_target_folder;}
    inline void setTargetFolder(int a_target_folder) {m_target_folder = a_target_folder;}
    inline int getTargetFile() const {return m_target_file;}
    inline void setTargetFile(int a_target_file) {m_target_file = a_target_file;}
    
    friend std::ostream& operator<< (std::ostream& out, Map& map);
    
    void loadXML(const tinyxml2::XMLElement* a_element);
    
    friend std::ostream& operator<< (std::ostream& out, Map& map);
};

std::ostream& operator<< (std::ostream& out, CharacterMap& tag);

#endif
