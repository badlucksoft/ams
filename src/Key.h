/*
 * Key.h
 *
 *  Created on: Sep 1, 2016
 *      Author: rrodgers
 */

#ifndef SRC_KEY_H_
#define SRC_KEY_H_
#include <stdint.h>
#include <uuid/uuid.h>
#include <time.h>
#include <jansson.h>
//! Bad Luck Software namespace.
namespace BLS {
//! Advanced Mail System namespace.
namespace AMS {
/**
 * \brief Public key creation and management class.
 */
class Key {
private:
	unsigned char *privKey; //!< Private key
	unsigned char *pubKey; //!< Public key
	uint32_t keyBits; //!< Size of the key in bits.
	uint32_t privKeyLength; //!< Size of the private key in bytes.
	uint32_t pubKeyLength; //!< Size of the public key in bytes.
	void init(void); //!< Initialize the object.
	uuid_t key_id; //!< UUID key identification.
	char *name; //!< Optional user name of the key owner.
	char *email; //!< Optional email address of the key owner.
	char *company; //!< Optional company name of the key owner.
	char *description; //!< Optional description/purpose of the key.
	time_t creation_timestamp; //!< Unix timestamp from when the key was created.
public:
	Key(); //!< Default constructor
	Key(json_t *KEY); //!< JSON Constructor
	Key(const char *KEY); //!< UUID [String] Constructor
	virtual ~Key();
	char* getKeyID(char *KEYID);
	char *getName();
	char *getEmail();
	char *getCompany();
	char *getDescription();
	uint32_t getKeyBits();
	void setName(const char *NAME, uint32_t LENGTH);
	void setEmail(const char *EMAIL, uint32_t LENGTH);
	void setCompany(const char *COMPANY, uint32_t LENGTH);
	void setDescription(const char *DESCRIPTION, uint32_t LENGTH);
	static Key* generateKey(uint32_t KEY_BITS = 2048L);
	json_t *toJSON(json_t *ROOT = NULL);
};
}
}
#endif /* SRC_KEY_H_ */
