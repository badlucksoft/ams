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
	int32_t len = 0l;
	json_t *item = json_object_get(KEY,"key_id");
	if( item != NULL){
		uuid_parse(json_string_value(item),this->key_id);
		json_object_clear(item);
	}
	item = json_object_get(KEY,"name");
	if( item != NULL && (len = json_string_length(item)) > 0)
	{
		this->name = (char*) calloc(len+1,1);
		strncpy(this->name,json_string_value(item),len);
		json_object_clear(item);
	}
	item = json_object_get(KEY,"email");
	if( item != NULL && (len = json_string_length(item)) > 0)
	{
		len = json_string_length(item);
		this->email = (char*) calloc(len+1,1);
		strncpy(this->email,json_string_value(item),len);
		json_object_clear(item);
	}
	item = json_object_get(KEY,"company");
	if( item != NULL && (len = json_string_length(item)) > 0)
	{
		len = json_string_length(item);
		this->company = (char*) calloc(len+1,1);
		strncpy(this->company,json_string_value(item),len);
		json_object_clear(item);
	}
	item = json_object_get(KEY,"description");
	if( item != NULL && (len = json_string_length(item)) > 0)
	{
		len = json_string_length(item);
		this->description = (char*) calloc(len+1,1);
		strncpy(this->description,json_string_value(item),len);
		json_object_clear(item);
	}
	item = json_object_get(KEY,"public_key");
	if( item != NULL && (len = json_string_length(item)) > 0)
	{
		this->pubKeyLength = json_string_length(item);
		this->pubKey = (unsigned char*) calloc(this->pubKeyLength+1,1);
		strncpy((char*)this->pubKey,json_string_value(item),this->pubKeyLength);
		json_object_clear(item);
	}
	item = json_object_get(KEY,"private_key");
	if( item != NULL && (len = json_string_length(item)) > 0)
	{
		this->privKeyLength = json_string_length(item);
		this->privKey = (unsigned char*) calloc(this->privKeyLength+1,1);
		strncpy((char*)this->privKey,json_string_value(item),this->privKeyLength);
		json_object_clear(item);
	}
	item = json_object_get(KEY,"creation_timestamp");
	if( item != NULL)
	{
		this->creation_timestamp = json_integer_value(item);
	}
}
Key::Key(const char *KEY)
{
	this->init();
}
Key::~Key() {
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
	if( this->name != NULL)
	{
		memset(this->name,0,strlen(this->name));
		free(this->name);
		this->name = NULL;
	}
	if( this->email != NULL)
	{
		memset(this->email,0,strlen(this->email));
		free(this->email);
		this->email = NULL;
	}
	if( this->company != NULL )
	{
		memset(this->company,0,strlen(this->company));
		free(this->company);
		this->company = NULL;
	}
	if( this->description != NULL)
	{
		memset(this->description,0,strlen(this->description));
		free(this->description);
		this->description = NULL;
	}
	this->creation_timestamp = 0L;
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
	char keyID[37];
	this->getKeyID(keyID);
	json_object_set_new(keyRoot,"key_id",json_stringn(keyID,36));
	json_object_set_new(keyRoot,"creation_timestamp",json_integer(this->creation_timestamp));
	if( this->name == NULL) json_object_set_new(keyRoot,"name",json_null());
	else json_object_set_new(keyRoot,"name",json_stringn(this->name,strlen(this->name)));
	if( this->email == NULL) json_object_set_new(keyRoot,"email",json_null());
	else json_object_set_new(keyRoot,"email",json_stringn(this->email,strlen(this->email)));
	if( this->company == NULL) json_object_set_new(keyRoot,"company",json_null());
	else json_object_set_new(keyRoot,"company",json_stringn(this->company,strlen(this->company)));
	json_object_set_new(keyRoot,"public_key",json_stringn((char*)this->pubKey,this->pubKeyLength));
	if( this->privKey != NULL) json_object_set_new(keyRoot,"private_key",json_stringn((char*)this->privKey,this->privKeyLength));
	return keyRoot;
}
bool Key::publishPublicKey()
{
	bool success = false;
	json_t *keyPackage = json_object(), *package = json_object();
	char keyID[37];
	this->getKeyID(keyID);
	this->toJSON(keyPackage);
	json_object_del(keyPackage,"private_key");
	json_object_set_new(package,"command",json_stringn("publish_public_key",18));
	json_object_set(package,"public_key",keyPackage);
	char *packageString = json_dumps(package,JSON_COMPACT);
	json_object_clear(keyPackage);
	json_object_clear(package);
	printf("public key package:\n%s\n",packageString);
	memset(packageString,0,strlen(packageString)+1);
	free(packageString);
	return success;
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
