#include "Token.h"

static CK_MECHANISM_TYPE mechanismsAvailable[] = { CKM_RSA_PKCS_KEY_PAIR_GEN, CKM_DES3_KEY_GEN, CKM_AES_KEY_GEN };

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

Token::Token(CK_ULONG ID)
{
	/* Objects */
	objectCounter = 0;
	objectList = NULL;
	/* Pin */
	wrongPinCounter = 0;
	pin = new unsigned char[10];
	memcpy(pin, (void*)"1234567890", 10);
	pinLen = 10;
	/* Token Info */
	tokenID = ID;
	tokenInfo.firmwareVersion.major = 1;
	tokenInfo.firmwareVersion.minor = 0;
	tokenInfo.flags = CKF_USER_PIN_INITIALIZED;
	tokenInfo.hardwareVersion.major = 1;
	tokenInfo.hardwareVersion.minor = 0;
	char label[] = "Virtual Token v1.0";
	memcpy(tokenInfo.label, label, strnlen_s(label, 32));
	/* Session and Pin*/
	tokenInfo.ulMaxPinLen = CKU_MAX_PIN_LEN;
	tokenInfo.ulMinPinLen = CKU_MIN_PIN_LEN;
	tokenInfo.ulMaxRwSessionCount = CKU_MAX_RW_SESSION_COUNT;
	tokenInfo.ulMaxSessionCount = CKU_MAX_SESSION_COUNT;
	tokenInfo.ulSessionCount = 0;
	tokenInfo.ulRwSessionCount = 0;
	/* Memory */
	tokenInfo.ulTotalPrivateMemory = CKU_TOTAL_PRIVATE_MEMORY;
	tokenInfo.ulTotalPublicMemory = CKU_TOTAL_PUBLIC_MEMORY;
	tokenInfo.ulFreePrivateMemory = tokenInfo.ulTotalPrivateMemory;
	tokenInfo.ulFreePublicMemory = tokenInfo.ulTotalPublicMemory;
	/* Time */
	memcpy(tokenInfo.utcTime, __TIME__, strnlen_s(__TIME__, 16));
	mechanismList = NULL_PTR;
	/* Database */
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char* data = "Callback function called";
	/* Open database */
	rc = sqlite3_open("db.sqlite", &pDatabase);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pDatabase));
		exit(0);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}
	/* Create Keys Table*/
	sql = "CREATE TABLE IF NOT EXISTS [Keys] (" \
		"[ID] DOUBLE NOT NULL PRIMARY KEY," \
		"[Token ID] DOUBLE NOT NULL," \
		"[Key Object] BLOB NOT NULL," \
		"[Key Length] DOUBLE NOT NULL)";
	/* Execute SQL statement */
	rc = sqlite3_exec(pDatabase, sql, callback, (void*)data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Operation done successfully\n");
	}
	/* Create Attribute Table*/
	sql = "CREATE TABLE IF NOT EXISTS [Attributes] (" \
		"[KeyID] DOUBLE NOT NULL," \
		"[ID] DOUBLE," \
		"[LABEL] MVARCHAR(50)," \
		"[TOKEN] BOOLEAN," \
		"[PRIVATE] BOOLEAN," \
		"[SENSITIVE] BOOLEAN," \
		"[ENCRYPT] BOOLEAN," \
		"[DECRYPT] BOOLEAN," \
		"[SIGN] BOOLEAN," \
		"[VERIFY] BOOLEAN," \
		"[WRAP] BOOLEAN," \
		"[UNWRAP] BOOLEAN," \
		"[MODULUS_BITS] DOUBLE," \
		"[PUBLIC_EXPONENT] DOUBLE," \
		"[PRIVATE_EXPONENT] DOUBLE," \
		"[KEY_TYPE] DOUBLE NOT NULL," \
		"[PRIME_1] DOUBLE," \
		"[PRIME_2] DOUBLE," \
		"[EXPONENT_1] DOUBLE," \
		"[EXPONENT_2] DOUBLE," \
		"[MODULUS] DOUBLE," \
		"[COEFFICIENT] DOUBLE," \
		"FOREIGN KEY(KeyID) REFERENCES Keys(ID))";
	/* Execute SQL statement */
	rc = sqlite3_exec(pDatabase, sql, callback, (void*)data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Operation done successfully\n");
	}
}

Token::~Token()
{
	if (pin)
		delete[] pin;
	pin = NULL_PTR;
	if (mechanismList)
		delete[] mechanismList;
	mechanismList = NULL_PTR;
	sqlite3_close(pDatabase);
}

CK_RV Token::C_InsertAttributes(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_KEY_TYPE keyType, CK_ULONG keyHandler)
{
	/* Set Attributes */
	CK_BYTE id = 0;
	char* label = "";
	CK_BBOOL token, privat, sensitive, encrypt, decrypt, sign, verify, wrap, unwrap;
	CK_BYTE prime1 = 0, modulus = 0, exponent1 = 0, prime2 = 0, exponent2 = 0, coefficient = 0, publicExponent = 0, privateExponent = 0, modulusBits = 0;
	sqlite3_stmt *stmt = 0;
	token = CK_FALSE;
	privat = CK_FALSE;
	sensitive = CK_FALSE;
	encrypt = CK_FALSE;
	decrypt = CK_FALSE;
	sign = CK_FALSE;
	verify = CK_FALSE;
	wrap = CK_FALSE;
	unwrap = CK_FALSE;

	for (int i = 0; i < ulCount; i++)
	{
		switch (pTemplate[i].type)
		{
		case CKA_ID:
			id = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_LABEL:
			label = (char*)pTemplate[i].pValue;
			break;
		case CKA_KEY_TYPE:
			keyType = *(CK_KEY_TYPE*)pTemplate[i].pValue;
			break;
		case CKA_TOKEN:
			token = CK_TRUE;
			break;
		case CKA_PRIVATE:
			privat = CK_TRUE;
			break;
		case CKA_SENSITIVE:
			sensitive = CK_TRUE;
			break;
		case CKA_ENCRYPT:
			encrypt = CK_TRUE;
			break;
		case CKA_DECRYPT:
			decrypt = CK_TRUE;
			break;
		case CKA_SIGN:
			sign = CK_TRUE;
			break;
		case CKA_VERIFY:
			verify = CK_TRUE;
			break;
		case CKA_WRAP:
			wrap = CK_TRUE;
			break;
		case CKA_UNWRAP:
			unwrap = CK_TRUE;
			break;
		case CKA_MODULUS_BITS:
			modulusBits = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_PUBLIC_EXPONENT:
			publicExponent = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_PRIVATE_EXPONENT:
			privateExponent = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_MODULUS:
			modulus = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_PRIME_1:
			prime1 = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_PRIME_2:
			prime2 = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_EXPONENT_1:
			exponent1 = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_EXPONENT_2:
			exponent2 = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_COEFFICIENT:
			coefficient = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
		default:
			break;
		}
	}
	/* Insert Key Attributes */
	char *sqlAtt = "INSERT INTO Attributes(KeyID, ID, LABEL, TOKEN, PRIVATE," \
		"SENSITIVE, ENCRYPT, DECRYPT, SIGN, VERIFY, WRAP, UNWRAP," \
		"MODULUS_BITS, PUBLIC_EXPONENT, PRIVATE_EXPONENT, KEY_TYPE, PRIME_1, PRIME_2," \
		"EXPONENT_1, EXPONENT_2, MODULUS, COEFFICIENT) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";

	sqlite3_prepare_v2(pDatabase, sqlAtt, strlen(sqlAtt) + 1, &stmt, NULL);
	if (stmt != NULL) {
		sqlite3_bind_double(stmt, 1, keyHandler);
		sqlite3_bind_double(stmt, 2, id);
		sqlite3_bind_text(stmt, 3, label, -1, NULL);
		sqlite3_bind_int(stmt, 4, token);
		sqlite3_bind_int(stmt, 5, privat);
		sqlite3_bind_int(stmt, 6, sensitive);
		sqlite3_bind_int(stmt, 7, encrypt);
		sqlite3_bind_int(stmt, 8, decrypt);
		sqlite3_bind_int(stmt, 9, sign);
		sqlite3_bind_int(stmt, 10, verify);
		sqlite3_bind_int(stmt, 11, wrap);
		sqlite3_bind_int(stmt, 12, unwrap);
		sqlite3_bind_double(stmt, 13, modulusBits);
		sqlite3_bind_double(stmt, 14, publicExponent);
		sqlite3_bind_double(stmt, 15, privateExponent);
		sqlite3_bind_double(stmt, 16, keyType);
		sqlite3_bind_double(stmt, 17, prime1);
		sqlite3_bind_double(stmt, 18, prime2);
		sqlite3_bind_double(stmt, 19, exponent1);
		sqlite3_bind_double(stmt, 20, exponent2);
		sqlite3_bind_double(stmt, 21, modulus);
		sqlite3_bind_double(stmt, 22, coefficient);
	}

	if (sqlite3_step(stmt) != SQLITE_DONE)
		printf("Error message: %s\n", sqlite3_errmsg(pDatabase));
	sqlite3_finalize(stmt);

	return CK_RV();
}

CK_RV Token::C_InsertKey(CK_MECHANISM_PTR pMechanism, CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount, CK_OBJECT_HANDLE_PTR phKey)
{	
	CK_KEY_TYPE keyType = -1;
	/* Generate Key */
	char *zErrMsg = 0;
	sqlite3_stmt *stmt = 0;
	char *sql = "INSERT INTO Keys(ID, [Token ID], [Key Object], [Key Length])"\
		"VALUES(? , ? , ? , ?)";
	const char* data = "Callback function called";
	srand(time(NULL));
	int rc;
	unsigned long err;
	CK_ULONG keyHandler = rand() % RAND_MAX;
	switch (pMechanism->mechanism)
	{
	case CKM_DES3_KEY_GEN:
		keyType = CKK_DES3;
	case CKM_AES_KEY_GEN:
	{
		keyType = CKK_AES;
		CK_BYTE CK_PTR key;
		int keylen = 128;
		key = new CK_BYTE[keylen / sizeof(CK_BYTE)];
		rc = RAND_bytes(key, sizeof(key));
		err = ERR_get_error();
		/* Database Insert Key */

		sqlite3_prepare_v2(pDatabase, sql, strlen(sql) + 1, &stmt, NULL);
		if (stmt != NULL) {
			sqlite3_bind_double(stmt, 1, keyHandler);
			sqlite3_bind_double(stmt, 2, tokenID);
			sqlite3_bind_blob(stmt, 3, key, keylen, SQLITE_TRANSIENT);
			sqlite3_bind_double(stmt, 4, keylen);
		}

		if (sqlite3_step(stmt) != SQLITE_DONE)
			printf("Error message: %s\n", sqlite3_errmsg(pDatabase));
		sqlite3_finalize(stmt);
		delete key;
		key = NULL;
		break;
	}
	default:
		return CKR_KEY_TYPE_INCONSISTENT;
		break;
	}
	C_InsertAttributes(pTemplate, ulCount, keyType, keyHandler);
	return CKR_OK;
}

CK_RV Token::C_InsertKeyPair(CK_MECHANISM_PTR pMechanism, CK_ATTRIBUTE_PTR pPublicKeyTemplate, CK_ULONG ulPublicKeyAttributeCount, CK_ATTRIBUTE_PTR pPrivateKeyTemplate, CK_ULONG ulPrivateKeyAttributeCount, CK_OBJECT_HANDLE_PTR phPublicKey, CK_OBJECT_HANDLE_PTR phPrivateKey)
{
	CK_KEY_TYPE keyType = -1;
	/* Generate Key */
	char *zErrMsg = 0;
	sqlite3_stmt *stmt = 0;
	char *sql = "INSERT INTO Keys(ID, [Token ID], [Key Object], [Key Length])"\
		"VALUES(? , ? , ? , ?)";
	const char* data = "Callback function called";
	srand(time(NULL));
	int rc;
	unsigned long err;
	CK_ULONG privateKeyHandler = rand() % RAND_MAX;
	CK_ULONG publicKeyHandler = rand() % RAND_MAX;
	switch (pMechanism->mechanism)
	{
	case CKM_RSA_PKCS_KEY_PAIR_GEN:
	{
		CK_KEY_TYPE keyType = CKK_RSA;
		const int kBits = 1024;
		unsigned char* pemKey, *pubKey;
		const int kExp = 3;
		RSA *rsa = RSA_generate_key(kBits, kExp, 0, 0);

		/* To get the C-string PEM form: */
		BIO *bio = BIO_new(BIO_s_mem());
		PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL);
		int keylen = BIO_pending(bio);
		pemKey = (unsigned char*)calloc(keylen + 1, sizeof(unsigned char)); /* Null-terminate */
		BIO_read(bio, pemKey, keylen);
		printf("%s", pemKey);
		/* Database Insert Key */

		sqlite3_prepare_v2(pDatabase, sql, strlen(sql) + 1, &stmt, NULL);
		if (stmt != NULL) {
			sqlite3_bind_double(stmt, 1, privateKeyHandler);
			sqlite3_bind_double(stmt, 2, tokenID);
			sqlite3_bind_blob(stmt, 3, pemKey, keylen, SQLITE_TRANSIENT);
			sqlite3_bind_double(stmt, 4, keylen);
		}

		if (sqlite3_step(stmt) != SQLITE_DONE)
			printf("Error message: %s\n", sqlite3_errmsg(pDatabase));
		sqlite3_finalize(stmt);
		C_InsertAttributes(pPrivateKeyTemplate, ulPrivateKeyAttributeCount, keyType, privateKeyHandler);

		BIO *bio2 = BIO_new(BIO_s_mem());
		PEM_write_bio_RSAPublicKey(bio2, rsa);
		keylen = BIO_pending(bio2);

		pubKey = (unsigned char*)calloc(keylen + 1, sizeof(unsigned char));
		BIO_read(bio2, pubKey, keylen);
		printf("%s", pubKey);

		/* Database Insert Key */

		sqlite3_prepare_v2(pDatabase, sql, strlen(sql) + 1, &stmt, NULL);
		if (stmt != NULL) {
			sqlite3_bind_double(stmt, 1, publicKeyHandler);
			sqlite3_bind_double(stmt, 2, tokenID);
			sqlite3_bind_blob(stmt, 3, pubKey, keylen, SQLITE_TRANSIENT);
			sqlite3_bind_double(stmt, 4, keylen);
		}

		if (sqlite3_step(stmt) != SQLITE_DONE)
			printf("Error message: %s\n", sqlite3_errmsg(pDatabase));
		sqlite3_finalize(stmt);
		C_InsertAttributes(pPublicKeyTemplate, ulPublicKeyAttributeCount, keyType, publicKeyHandler);
		BIO_free_all(bio2);
		BIO_free_all(bio);
		RSA_free(rsa);
		free(pemKey);
	}
	default:
		return CKR_KEY_TYPE_INCONSISTENT;
		break;
	}

	return CKR_OK;
}

CK_RV Token::C_FindObjects(CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulCount)
{
	objectList = new CK_OBJECT_HANDLE[CKU_MAX_TOKEN_OBJECTS];

	CK_BYTE id = 0;
	char* label = "";
	CK_BBOOL token, privat, sensitive, encrypt, decrypt, sign, verify, wrap, unwrap;
	CK_BYTE prime1 = 0, modulus = 0, exponent1 = 0, prime2 = 0, exponent2 = 0, coefficient = 0, publicExponent = 0, privateExponent = 0, modulusBits = 0;
	CK_KEY_TYPE keyType = -1;

	token = CK_FALSE;
	privat = CK_FALSE;
	sensitive = CK_FALSE;
	encrypt = CK_FALSE;
	decrypt = CK_FALSE;
	sign = CK_FALSE;
	verify = CK_FALSE;
	wrap = CK_FALSE;
	unwrap = CK_FALSE;
	for (int i = 0; i < ulCount; i++)
	{
		switch (pTemplate[i].type)
		{
		case CKA_ID:
			id = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_LABEL:
			label = (char*)pTemplate[i].pValue;
			break;
		case CKA_KEY_TYPE:
			keyType = *(CK_KEY_TYPE*)pTemplate[i].pValue;
			break;
		case CKA_TOKEN:
			token = CK_TRUE;
			break;
		case CKA_PRIVATE:
			privat = CK_TRUE;
			break;
		case CKA_SENSITIVE:
			sensitive = CK_TRUE;
			break;
		case CKA_ENCRYPT:
			encrypt = CK_TRUE;
			break;
		case CKA_DECRYPT:
			decrypt = CK_TRUE;
			break;
		case CKA_SIGN:
			sign = CK_TRUE;
			break;
		case CKA_VERIFY:
			verify = CK_TRUE;
			break;
		case CKA_WRAP:
			wrap = CK_TRUE;
			break;
		case CKA_UNWRAP:
			unwrap = CK_TRUE;
			break;
		case CKA_MODULUS_BITS:
			modulusBits = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_PUBLIC_EXPONENT:
			publicExponent = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_PRIVATE_EXPONENT:
			privateExponent = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_MODULUS:
			modulus = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_PRIME_1:
			prime1 = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_PRIME_2:
			prime2 = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_EXPONENT_1:
			exponent1 = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_EXPONENT_2:
			exponent2 = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
			break;
		case CKA_COEFFICIENT:
			coefficient = *static_cast<CK_BYTE_PTR>(pTemplate[i].pValue);
		default:
			break;
		}
	}
	
	sqlite3_stmt *stmt = 0;
	char *zErrMsg = 0;
	int rc;
	unsigned long err;
	const char* data = "Callback function called";
	char *sqlAtt = "SELECT * FROM Keys INNER JOIN " \
		"Attributes ON Attributes.KeyID = Keys.ID " \
		"WHERE Attributes.ID = ? AND Attributes.TOKEN = ? " \
		"AND Attributes.PRIVATE = ? AND Attributes.SENSITIVE = ? " \
		"AND Attributes.ENCRYPT = ? AND Attributes.DECRYPT = ? " \
		"AND Attributes.SIGN = ? AND Attributes.VERIFY = ? " \
		"AND Attributes.WRAP = ? AND Attributes.UNWRAP = ? " \
		"AND MODULUS_BITS = ? AND PUBLIC_EXPONENT = ? " \
		"AND PRIVATE_EXPONENT = ? AND KEY_TYPE = ? " \
		"AND PRIME_1 = ? AND PRIME_2 = ? " \
		"AND EXPONENT_1 = ? AND EXPONENT_2 = ? " \
		"AND MODULUS = ? AND COEFFICIENT = ? ";
	//AND Keys.[Token ID] = ?";
	sqlite3_prepare_v2(pDatabase, sqlAtt, strlen(sqlAtt) + 1, &stmt, NULL);
	if (stmt != NULL) {
		sqlite3_bind_double(stmt, 1, id);
		sqlite3_bind_int(stmt, 2, token);
		sqlite3_bind_int(stmt, 3, privat);
		sqlite3_bind_int(stmt, 4, sensitive);
		sqlite3_bind_int(stmt, 5, encrypt);
		sqlite3_bind_int(stmt, 6, decrypt);
		sqlite3_bind_int(stmt, 7, sign);
		sqlite3_bind_int(stmt, 8, verify);
		sqlite3_bind_int(stmt, 9, wrap);
		sqlite3_bind_int(stmt, 10, unwrap);
		sqlite3_bind_double(stmt, 11, modulusBits);
		sqlite3_bind_double(stmt, 12, publicExponent);
		sqlite3_bind_double(stmt, 13, privateExponent);
		sqlite3_bind_double(stmt, 14, keyType);
		sqlite3_bind_double(stmt, 15, prime1);
		sqlite3_bind_double(stmt, 16, prime2);
		sqlite3_bind_double(stmt, 17, exponent1);
		sqlite3_bind_double(stmt, 18, exponent2);
		sqlite3_bind_double(stmt, 19, modulus);
		sqlite3_bind_double(stmt, 20, coefficient);
		//sqlite3_bind_double(stmt, 14, tokenID);
	}

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) 
	{
		if (objectCounter < CKU_MAX_TOKEN_OBJECTS)
		{
			objectList[objectCounter++] = sqlite3_column_int(stmt, 0);
		}
		printf("%s %d\n", sqlite3_column_text(stmt, 0), sqlite3_column_int(stmt, 1));  /* 3 */
	}

	sqlite3_finalize(stmt);
	
	return CKR_OK;
}

CK_BBOOL Token::C_IsMechanismAvailable(CK_MECHANISM_TYPE mechanismType)
{
	for (int i = 0; i < sizeof(mechanismsAvailable) / sizeof(CK_MECHANISM_TYPE); i++)
	{
		if (mechanismsAvailable[i] == mechanismType)
			return CK_TRUE;
	}
	return CK_FALSE;
}
