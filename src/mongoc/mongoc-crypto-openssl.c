/*
 * Copyright 2016 MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mongoc-config.h"
#include <bson.h>

#ifdef MONGOC_ENABLE_CRYPTO_LIBCRYPTO
#include "mongoc-crypto-openssl-private.h"
#include "mongoc-crypto-private.h"

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>



void
mongoc_crypto_openssl_hmac_sha1 (mongoc_crypto_t     *crypto,
                                 const void          *key,
                                 int                  key_len,
                                 const unsigned char *d,
                                 int                  n,
                                 unsigned char       *md /* OUT */)
{
   /* U1 = HMAC(input, salt + 0001) */
   HMAC (EVP_sha1 (),
         key,
         key_len,
         d,
         n,
         md,
         NULL);
}

bool
mongoc_crypto_openssl_sha1 (mongoc_crypto_t     *crypto,
                            const unsigned char *input,
                            const size_t         input_len,
                            unsigned char       *output /* OUT */)
{
   EVP_MD_CTX digest_ctx;
   bool rval = false;

   EVP_MD_CTX_init (&digest_ctx);

   if (1 != EVP_DigestInit_ex (&digest_ctx, EVP_sha1 (), NULL)) {
      goto cleanup;
   }

   if (1 != EVP_DigestUpdate (&digest_ctx, input, input_len)) {
      goto cleanup;
   }

   rval = (1 == EVP_DigestFinal_ex (&digest_ctx, output, NULL));

cleanup:
   EVP_MD_CTX_cleanup (&digest_ctx);

   return rval;
}


#endif
