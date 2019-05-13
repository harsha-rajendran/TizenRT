/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>

#include <security/security_auth.h>
#include "security_internal.h"

#include "mbedtls/pk.h"
#include "mbedtls/pem.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/pk_internal.h"

/**
 * Authentication
 */
int auth_generate_random(security_handle hnd, unsigned int size, security_data *random)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

	hal_data hrand = {ctx->data1, ctx->dlen1, NULL, 0};
	hal_result_e hres = HAL_SUCCESS;

	SECAPI_CALL(sl_generate_random(ctx->sl_hnd, size, &hrand, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	random->data = (unsigned char *)malloc(hrand.data_len);
	if (!random->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	SECAPI_DATA_DCOPY(hrand, random);

	SECAPI_RETURN(SECURITY_OK);
}

int auth_generate_certificate(security_handle hnd, const char *cert_name, security_csr *csr, security_data *cert)
{
	SECAPI_ENTER;
	//TODO

	SECAPI_RETURN(SECURITY_NOT_SUPPORT);
}

int auth_set_certificate(security_handle hnd, const char *cert_name, security_data *cert)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

    /* convert path */
	uint32_t cert_idx = 0;
	SECAPI_CONVERT_PATH(cert_name, &cert_idx);

	hal_data cert_in = {cert->data, cert->length, NULL, 0};
	hal_result_e hres = HAL_SUCCESS;

	SECAPI_CALL(sl_set_certificate(ctx->sl_hnd, cert_idx, &cert_in, &hres));

	SECAPI_HAL_RETURN(hres);
}

int auth_get_certificate(security_handle hnd, const char *cert_name, security_data *cert)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

    /* convert path */
	uint32_t cert_idx = 0;
	SECAPI_CONVERT_PATH(cert_name, &cert_idx);

	hal_data cert_out = {ctx->data1, ctx->dlen1, NULL, 0};
	hal_result_e hres = HAL_SUCCESS;

	SECAPI_CALL(sl_get_certificate(ctx->sl_hnd, cert_idx, &cert_out, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	cert->data = (unsigned char *)malloc(cert_out.data_len);
	if (!cert->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	SECAPI_DATA_DCOPY(cert_out, cert);

	SECAPI_RETURN(SECURITY_OK);
}

int auth_remove_certificate(security_handle hnd, const char *cert_name)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

    /* convert path */
	uint32_t cert_idx = 0;
	SECAPI_CONVERT_PATH(cert_name, &cert_idx);

	hal_result_e hres = HAL_SUCCESS;
	SECAPI_CALL(sl_remove_certificate(ctx->sl_hnd, cert_idx, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}
	SECAPI_RETURN(SECURITY_OK);
}

int auth_get_rsa_signature(security_handle hnd, security_rsa_param mode, const char *key_name, security_data *hash, security_data *sign)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

    /* convert path */
	uint32_t key_idx = 0;
	SECAPI_CONVERT_PATH(key_name, &key_idx);

	hal_rsa_mode hmode;
	SECAPI_CONVERT_RSAPARAM(&mode, &hmode);

	hal_data h_hash = {hash->data, hash->length, NULL, 0};
	hal_data h_sign = {ctx->data1, ctx->dlen1, NULL, 0};
	hal_result_e hres = HAL_SUCCESS;

	SECAPI_CALL(sl_rsa_sign_md(ctx->sl_hnd, hmode, &h_hash, key_idx, &h_sign, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	sign->data = (unsigned char *)malloc(h_sign.data_len);
	if (!sign->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	SECAPI_DATA_DCOPY(h_sign, sign);

	SECAPI_RETURN(SECURITY_OK);
}

int auth_verify_rsa_signature(security_handle hnd, security_rsa_param mode, const char *key_name, security_data *hash, security_data *sign)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

    /* convert path */
	uint32_t key_idx = 0;
	SECAPI_CONVERT_PATH(key_name, &key_idx);

	hal_rsa_mode hmode;
	SECAPI_CONVERT_RSAPARAM(&mode, &hmode);

	hal_data h_hash = {hash->data, hash->length, NULL, 0};
	hal_data h_sign = {sign->data, sign->length, NULL, 0};
	hal_result_e hres = HAL_SUCCESS;

	SECAPI_CALL(sl_rsa_verify_md(ctx->sl_hnd, hmode, &h_hash, &h_sign, key_idx, &hres));

	SECAPI_HAL_RETURN(hres);
}

int auth_get_ecdsa_signature(security_handle hnd, security_ecdsa_param mode, const char *key_name, security_data *hash, security_data *sign)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

    /* convert path */
	uint32_t key_idx = 0;
	SECAPI_CONVERT_PATH(key_name, &key_idx);

	hal_ecdsa_mode hmode;
	SECAPI_CONVERT_ECDSAPARAM(&mode, &hmode);

	hal_data h_hash = {hash->data, hash->length, NULL, 0};
	hal_data h_sign = {ctx->data1, ctx->dlen1, NULL, 0};
	hal_result_e hres = HAL_SUCCESS;

	SECAPI_CALL(sl_ecdsa_sign_md(ctx->sl_hnd, hmode, &h_hash, key_idx, &h_sign, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	sign->data = (unsigned char *)malloc(h_sign.data_len);
	if (!sign->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	SECAPI_DATA_DCOPY(h_sign, sign);

	SECAPI_RETURN(SECURITY_OK);
}

int auth_verify_ecdsa_signature(security_handle hnd, security_ecdsa_param mode, const char *key_name, security_data *hash, security_data *sign)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

    /* convert path */
	uint32_t key_idx = 0;
	SECAPI_CONVERT_PATH(key_name, &key_idx);

	hal_ecdsa_mode hmode;
	SECAPI_CONVERT_ECDSAPARAM(&mode, &hmode);

	hal_data h_hash = {hash->data, hash->length, NULL, 0};
	hal_data h_sign = {sign->data, sign->length, NULL, 0};
	hal_result_e hres = HAL_SUCCESS;

	SECAPI_CALL(sl_ecdsa_verify_md(ctx->sl_hnd, hmode, &h_hash, &h_sign, key_idx, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}


	SECAPI_HAL_RETURN(hres);
}

int auth_get_hash(security_handle hnd, security_hash_mode mode, security_data *data, security_data *hash)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

	hal_hash_type h_type;
	SECAPI_CONVERT_HASHMODE(mode, h_type);

	hal_data input = {data->data, data->length, NULL, 0};
	hal_data output = {ctx->data1, ctx->dlen1, NULL, 0};
	hal_result_e hres = HAL_SUCCESS;

	SECAPI_CALL(sl_get_hash(ctx->sl_hnd, h_type, &input, &output, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	hash->data = (unsigned char *)malloc(output.data_len);
	if (!hash->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}

	SECAPI_DATA_DCOPY(output, hash);

	SECAPI_RETURN(SECURITY_OK);
}

int auth_get_hmac(security_handle hnd, security_hmac_mode mode, const char *key_name, security_data *data, security_data *hmac)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

    /* convert path */
	uint32_t key_idx = 0;
	SECAPI_CONVERT_PATH(key_name, &key_idx);

	hal_hmac_type h_type;
	SECAPI_CONVERT_HMACMODE(mode, h_type);

	hal_data input  = {data->data, data->length, NULL, 0};
	hal_data output = {ctx->data1, ctx->dlen1, NULL, 0};
	hal_result_e hres = HAL_SUCCESS;

	SECAPI_CALL(sl_get_hmac(ctx->sl_hnd, h_type, &input, key_idx, &output, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	hmac->data = (unsigned char *)malloc(output.data_len);
	if (!hmac->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}

	SECAPI_DATA_DCOPY(output, hmac);

	SECAPI_RETURN(SECURITY_OK);
}

int auth_generate_dhparams(security_handle hnd, const char *dh_name, security_dh_param *params)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

    /* convert path */
	uint32_t dh_idx = 0;
	SECAPI_CONVERT_PATH(dh_name, &dh_idx);

	hal_data G_tmp = {NULL, 0, NULL, 0};
	hal_data P_tmp = {NULL, 0, NULL, 0};
	/* pubkey is output */
	unsigned char pubkey_buf[SECURITY_MAX_KEY_BUF];
	hal_data pubkey_tmp = {pubkey_buf, SECURITY_MAX_KEY_BUF, NULL, 0};
	hal_dh_data h_data = {HAL_DH_UNKNOWN, &G_tmp, &P_tmp, &pubkey_tmp};
	/* copy mode, G, and P */
	SECAPI_CONVERT_DHPARAM(params, &h_data);
	hal_result_e hres = HAL_SUCCESS;

	SECAPI_CALL(sl_dh_generate_param(ctx->sl_hnd, dh_idx, &h_data, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	SECAPI_RETURN(SECURITY_OK);
}

int auth_compute_dhparams(security_handle hnd, const char *dh_name, security_dh_param *params, security_data *secret)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

	/* convert path */
	uint32_t dh_idx = 0;
	SECAPI_CONVERT_PATH(dh_name, &dh_idx);

	hal_data G_tmp = {NULL, 0, NULL, 0};
	hal_data P_tmp = {NULL, 0, NULL, 0};
	hal_data pubkey_tmp = {NULL, 0, NULL, 0};
	hal_dh_data h_data = {HAL_DH_UNKNOWN, &G_tmp, &P_tmp, &pubkey_tmp};
	SECAPI_CONVERT_DHPARAM(params, &h_data);

	hal_data shared_secret = {ctx->data1, ctx->dlen1, NULL, 0};

	hal_result_e hres = HAL_SUCCESS;
	SECAPI_CALL(sl_dh_compute_shared_secret(ctx->sl_hnd, &h_data, dh_idx, &shared_secret, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	secret->data = (unsigned char *)malloc(shared_secret.data_len);
	if (!secret->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}

	SECAPI_DATA_DCOPY(shared_secret, secret);

	SECAPI_RETURN(SECURITY_OK);
}

int auth_generate_ecdhkey(security_handle hnd, const char *ecdh_name, security_ecdh_param *params)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

	/* convert path */
	uint32_t ecdh_idx = 0;
	SECAPI_CONVERT_PATH(ecdh_name, &ecdh_idx);

	/* params->curve is input */
	hal_key_type key_type = secutils_convert_ecdsamode_to_key_s2h(params->curve);
	hal_result_e hres = HAL_SUCCESS;

	SECAPI_CALL(sl_generate_key(ctx->sl_hnd, key_type, ecdh_idx, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	hal_data key = {ctx->data1, ctx->dlen1, ctx->data2, ctx->dlen2};

	SECAPI_CALL(sl_get_key(ctx->sl_hnd, key_type, ecdh_idx, &key, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	params->pubkey_x = (security_data *)malloc(sizeof(security_data));
	if (!params->pubkey_x) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	params->pubkey_y = (security_data *)malloc(sizeof(security_data));
	if (!params->pubkey_y) {
		free(params->pubkey_x);
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	params->pubkey_x->data = (unsigned char *)malloc(key.data_len);
	if (!params->pubkey_x->data) {
		free(params->pubkey_x);
		free(params->pubkey_y);
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}
	params->pubkey_y->data = (unsigned char *)malloc(key.priv_len);
	if (!params->pubkey_y->data) {
		free(params->pubkey_x);
		free(params->pubkey_y);
		free(params->pubkey_x->data);
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}

	SECAPI_DATA_DCOPY(key, params->pubkey_x);
	SECAPI_PRIV_DCOPY(key, params->pubkey_y);

	SECAPI_RETURN(SECURITY_OK);
}

int auth_compute_ecdhkey(security_handle hnd, const char *ecdh_name, security_ecdh_param *params, security_data *secret)
{
	SECAPI_ENTER;
	SECAPI_ISHANDLE_VALID(hnd);
	struct security_ctx *ctx = (struct security_ctx *)hnd;

	/* convert path */
	uint32_t ecdh_idx = 0;
	SECAPI_CONVERT_PATH(ecdh_name, &ecdh_idx);

	hal_data pubkey_x = {NULL, 0, NULL, 0};
	hal_data pubkey_y = {NULL, 0, NULL, 0};
	hal_ecdh_data h_data = {HAL_ECDSA_UNKNOWN, &pubkey_x, &pubkey_y};
	SECAPI_CONVERT_ECDHPARAM(params, &h_data);

	hal_data shared_secret = {ctx->data1, ctx->dlen1, NULL, 0};
	hal_result_e hres = HAL_SUCCESS;

	SECAPI_CALL(sl_ecdh_compute_shared_secret(ctx->sl_hnd, &h_data, ecdh_idx, &shared_secret, &hres));
	if (hres != HAL_SUCCESS) {
		SECAPI_HAL_RETURN(hres);
	}

	secret->data = (unsigned char *)malloc(shared_secret.data_len);
	if (!secret->data) {
		SECAPI_RETURN(SECURITY_ALLOC_ERROR);
	}

	SECAPI_DATA_DCOPY(shared_secret, secret);

	SECAPI_RETURN(SECURITY_OK);
}
