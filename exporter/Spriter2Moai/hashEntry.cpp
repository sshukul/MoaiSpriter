//
//  hashEntry.cpp
//  Spriter2Moai
//
//  Created by Saurabh Shukul on 09/05/16.
//

#include "hashEntry.h"

HashEntry::HashEntry(int key, int value) {
    this->key = key;
    this->value = value;
}

int HashEntry::getKey() {
    return key;
}

int HashEntry::getValue() {
    return value;
}
