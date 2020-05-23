#include <cstring>
#include <fstream>
#include "sha256.h"
#include <sstream>

/*
 * Updated to C++, zedwood.com 2012
 * Based on Olivier Gay's version
 * See Modified BSD License below: 
 *
 * FIPS 180-2 SHA-224/256/384/512 implementation
 * Issue date:  04/30/2005
 * http://www.ouah.org/ogay/sha2/
 *
 * Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

const unsigned int SHA256::sha256_k[64] = //UL = uint32
    {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
     0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
     0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
     0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
     0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
     0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
     0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
     0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
     0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
     0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
     0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
     0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
     0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
     0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
     0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
     0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

void SHA256::init()
{
    m_h[0] = 0x6a09e667;
    m_h[1] = 0xbb67ae85;
    m_h[2] = 0x3c6ef372;
    m_h[3] = 0xa54ff53a;
    m_h[4] = 0x510e527f;
    m_h[5] = 0x9b05688c;
    m_h[6] = 0x1f83d9ab;
    m_h[7] = 0x5be0cd19;
    m_len = 0;
    m_tot_len = 0;
}

void SHA256::update(const unsigned char *message, unsigned int len)
{
    tmpUnsigned4 = SHA224_256_BLOCK_SIZE - m_len;
    tmpUnsigned3 = len < tmpUnsigned4 ? len : tmpUnsigned4;
    memcpy(&m_block[m_len], message, tmpUnsigned3);
    if (m_len + len < SHA224_256_BLOCK_SIZE)
    {
        m_len += len;
        return;
    }
    tmpUnsigned2 = len - tmpUnsigned3;
    tmpUnsigned1 = tmpUnsigned2 / SHA224_256_BLOCK_SIZE;
    tmpConstUnsignedCharPtr = message + tmpUnsigned3;
    SHA2_TRANSFORM(m_block, 1);
    SHA2_TRANSFORM(tmpConstUnsignedCharPtr, tmpUnsigned1);
    tmpUnsigned3 = tmpUnsigned2 % SHA224_256_BLOCK_SIZE;
    memcpy(m_block, &tmpConstUnsignedCharPtr[tmpUnsigned1 << 6], tmpUnsigned3);
    m_len = tmpUnsigned3;
    m_tot_len += (tmpUnsigned1 + 1) << 6;
}

void SHA256::final(unsigned char *digest)
{
    tmpUnsigned1 = (1 + ((SHA224_256_BLOCK_SIZE - 9) < (m_len % SHA224_256_BLOCK_SIZE)));
    tmpUnsigned3 = (m_tot_len + m_len) << 3;
    tmpUnsigned2 = tmpUnsigned1 << 6;
    memset(m_block + m_len, 0, tmpUnsigned2 - m_len);
    m_block[m_len] = 0x80;
    SHA2_UNPACK32(tmpUnsigned3, m_block + tmpUnsigned2 - 4);
    SHA2_TRANSFORM(m_block, tmpUnsigned1);
    for (tmpInt1 = 0; tmpInt1 < 8; ++tmpInt1)
    {
        SHA2_UNPACK32(m_h[tmpInt1], &digest[tmpInt1 << 2]);
    }
}

std::string sha256(const std::string &input)
{
    SHA256 ctx = SHA256();
    return sha256(ctx, input);
}

std::string sha256(SHA256 &ctx, const std::string &input)
{
    unsigned char digest[SHA256::DIGEST_SIZE];
    sha256(ctx, input, digest);
    return sha256ToHex(digest);
}

void sha256(SHA256 &ctx, const std::string &input, unsigned char digest[])
{
    ctx.init();
    ctx.update((unsigned char *)input.c_str(), input.length());
    ctx.final(digest);
}

std::string sha256ToHex(unsigned char digest[])
{
    char buf[2 * SHA256::DIGEST_SIZE + 1];
    buf[2 * SHA256::DIGEST_SIZE] = 0;
    for (unsigned i = 0; i < SHA256::DIGEST_SIZE; i++)
        sprintf(buf + i * 2, "%02x", digest[i]);
    return std::string(buf);
}

void sha256ToDec(const std::string &hash, unsigned char digest[])
{
    for (unsigned i = 0; i < (hash.size() / 2); i++)
        digest[i] = stoi(hash.substr(i * 2, 2), 0, 16);
}