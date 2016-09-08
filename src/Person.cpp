/*
 * Person.cpp
 *
 *  Created on: Sep 1, 2016
 *      Author: rrodgers
 */

#include "Person.h"
using namespace BLS::AMS;
Person::Person() {
	// TODO Auto-generated constructor stub
	this->init();
}

Person::~Person() {
	// TODO Auto-generated destructor stub
	if( this->key != NULL) {
		delete this->key;
		this->key = NULL;
	}
}

void Person::init(void)
{
	this->key = NULL;
}

json_t *Person::toJSON(json_t *ROOT)
{
	json_t *rootObj = NULL;
	return rootObj;
}
