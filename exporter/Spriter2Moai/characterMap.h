//
//  characterMap.h
//  Spriter2Moai
//

#ifndef Spriter2Moai_characterMap_h
#define Spriter2Moai_characterMap_h

#include <string>
#include <vector>
#include "tinyxml2.h"

using namespace std;

class Entity;
class Map;

class CharacterMap {
private:
    Entity* m_owner;
    int m_id;
    string m_name;
    vector<Map*> m_maps;
    
public:
    CharacterMap(): m_id(-1), m_name("")  {}
    CharacterMap(Entity* a_owner): m_owner(a_owner), m_id(-1), m_name("") {}
    
    ~CharacterMap();
    
    inline int getId() const {return m_id;}
    inline void setId(int a_id) {m_id = a_id;}
    inline const string& getName() const {return m_name;}
    inline void setName(string a_name) {m_name = a_name;}
    string getFileName(unsigned int a_folderIndex, unsigned int a_fileIndex);
    
    friend std::ostream& operator<< (std::ostream& out, CharacterMap& characterMap);
    
    void addMap(Map* a_map);
    
    void loadXML(const tinyxml2::XMLElement* a_element);
    
    friend std::ostream& operator<< (std::ostream& out, CharacterMap& characterMap);
};

std::ostream& operator<< (std::ostream& out, CharacterMap& tag);

#endif
