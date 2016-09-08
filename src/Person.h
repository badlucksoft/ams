/*
 * Person.h
 *
 *  Created on: Sep 1, 2016
 *      Author: rrodgers
 */

#ifndef SRC_PERSON_H_
#define SRC_PERSON_H_
#include "Key.h"
namespace BLS {
namespace AMS {
/**
 * \brief Contact and identity management class.
 */
class Person {
private:
	Key *key; //!< Person's Key.
	void init(void);
public:
	Person();
	virtual ~Person();
	json_t *toJSON(json_t *ROOT = NULL);
};
}
}
#endif /* SRC_PERSON_H_ */
