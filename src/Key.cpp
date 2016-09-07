/*
 * Key.cpp
 *
 *  Created on: Sep 1, 2016
 *      Author: rrodgers
 */

#include "Key.h"
#include <stdlib.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/err.h>

using namespace BLS::AMS;

Key::Key() {
	// TODO Auto-generated constructor stub
	this->init();
}
Key::Key(json_t *KEY)
{
	this->init();
}
Key::Key(const char *KEY)
{
	this->init();
}
Key::~Key() {
	// TODO Auto-generated destructor stub
	if (this->privKey != NULL && this->privKeyLength != 0) {
		memset(this->privKey, 0, this->privKeyLength + 1);
		free(this->privKey);
		this->privKey = NULL;
		this->privKeyLength = 0;
	}
	if (this->pubKey != NULL && this->pubKeyLength != 0) {
		memset(this->pubKey, 0, this->pubKeyLength + 1);
		free(this->pubKey);
		this->pubKey = NULL;
		this->pubKeyLength = 0;
	}
	uuid_clear(this->key_id);
}
void Key::init(void) {
	this->privKey = NULL;
	this->pubKey = NULL;
	this->privKeyLength = 0L;
	this->pubKeyLength = 0L;
	uuid_clear(this->key_id);
	this->name = NULL;
	this->email = NULL;
	this->company = NULL;
	this->description = NULL;
	this->creation_timestamp = 0;
}
char *Key::getKeyID(char *KEYID) {
	if (KEYID == NULL)
		return NULL;
	memset(KEYID, 0, 37);
	uuid_unparse(this->key_id, KEYID);
	return KEYID;
}
char* Key::getName() {
	return this->name;
}

char* Key::getEmail() {
	return this->email;
}

char* Key::getCompany() {
	return this->company;
}

char* Key::getDescription() {
	return this->description;
}
uint32_t Key::getKeyBits() {
	return this->keyBits;
}
void Key::setName(const char* NAME, uint32_t LENGTH) {
	if (this->name == NULL) {
		this->name = (char*) calloc(LENGTH + 1, 1);
	} else if (this->name != NULL) {
		memset(this->name, 0, strlen(name) + 1);
		this->name = (char*) realloc(this->name, LENGTH + 1);
	}
	strncpy(this->name, NAME, LENGTH);
}

void Key::setEmail(const char* EMAIL, uint32_t LENGTH) {
	if (this->email == NULL) {
		this->email = (char*) calloc(LENGTH + 1, 1);
	} else if (this->email != NULL) {
		memset(this->email, 0, strlen(email) + 1);
		this->email = (char*) realloc(this->email, LENGTH + 1);
	}
	strncpy(this->email, EMAIL, LENGTH);
}

void Key::setCompany(const char* COMPANY, uint32_t LENGTH) {
	if (this->company == NULL) {
		this->company = (char*) calloc(LENGTH + 1, 1);
	} else if (this->company != NULL) {
		memset(this->company, 0, strlen(company) + 1);
		this->company = (char*) realloc(this->company, LENGTH + 1);
	}
	strncpy(this->company, COMPANY, LENGTH);
}

void Key::setDescription(const char* DESCRIPTION, uint32_t LENGTH) {
	if (this->description == NULL) {
		this->description = (char*) calloc(LENGTH + 1, 1);
	} else if (this->description != NULL) {
		memset(this->description, 0, strlen(description) + 1);
		this->description = (char*) realloc(this->description, LENGTH + 1);
	}
	strncpy(this->description, DESCRIPTION, LENGTH);
}
json_t *Key::toJSON(json_t *ROOT)
{
	json_t *keyRoot = NULL;
	if( ROOT != NULL) keyRoot = ROOT;
	else keyRoot = json_object();
	json_t *item = NULL;
	json_t *keyObject = json_object();
	json_object_set(keyRoot,"key",keyObject);
	char keyID[37];
	this->getKeyID(keyID);
	json_object_set_new(keyObject,"key_id",json_stringn(keyID,36));
	if( this->getName() != NULL)
	{
		json_object_set_new(keyObject,"name",json_stringn(this->getName(),strlen(this->getName())));
	}
	else json_object_set_new(keyObject,"name",json_null());
	if( this->getEmail() != NULL)
		json_object_set_new(keyObject,"email",json_stringn(this->getEmail(),strlen(this->getEmail())));
	else
		json_object_set_new(keyObject,"email",json_null());
	return keyRoot;
}
Key *Key::generateKey(uint32_t KEY_BITS) {
	Key *k = new Key();
	uuid_generate(k->key_id);
	k->keyBits = KEY_BITS;
	RSA * rsa = NULL;
	BIGNUM *e = BN_new();
	BN_set_word(e, RSA_F4);
	rsa = RSA_new();
	int ret = RSA_generate_key_ex(rsa, k->keyBits, e, NULL);
	BIO *bio = BIO_new(BIO_s_mem());
	PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL);
	k->privKeyLength = BIO_pending(bio);
	k->privKey = (unsigned char*) calloc(k->privKeyLength + 1, 1);
	BIO_read(bio, k->privKey, k->privKeyLength);
	BIO_free_all(bio);

	bio = BIO_new(BIO_s_mem());
	PEM_write_bio_RSAPublicKey(bio, rsa);
	k->pubKeyLength = BIO_pending(bio);
	k->pubKey = (unsigned char*) calloc(k->pubKeyLength + 1, 1);
	BIO_read(bio, k->pubKey, k->pubKeyLength);
	BIO_free_all(bio);
	BN_free(e);
	RSA_free(rsa);
	time(&k->creation_timestamp);
	return k;
}
