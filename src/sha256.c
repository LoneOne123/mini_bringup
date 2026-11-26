#include "sha256.h"

static const uint32_t K[64] = { 
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
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
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static uint32_t rotr(uint32_t value, size_t n){
    return (value >> n) | (value << (32u - n));
}

static uint32_t sigma_zero(uint32_t word){
    return (rotr(word, 7) ^ rotr(word, 18) ^ (word >> 3));
}

static uint32_t sigma_one(uint32_t word){
    return (rotr(word, 17) ^ rotr(word, 19) ^ (word >> 10));
}

static uint32_t big_sigma_0(uint32_t x){
    return (rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22));
}

static uint32_t big_sigma_1(uint32_t x){
    return (rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25));
}

static uint32_t ch(uint32_t x, uint32_t y, uint32_t z){
    return ((x & y) ^ (~x & z));
}

static uint32_t maj(uint32_t x, uint32_t y, uint32_t z){
    return ((x & y) ^ (x & z) ^ (y & z));
}

static void generate_messageschedule_from_block(const uint8_t* block, uint32_t words[64]){
    for (size_t i = 0; i < 16; i++){
        words[i] = ((uint32_t) block[4*i] << 24) | ((uint32_t) block[4*i+1] << 16) | ((uint32_t) block[4*i+2] << 8) | (uint32_t) block[4*i+3];
    }
    for (size_t i = 16; i < 64; i++){
        words[i] = sigma_one(words[i-2]) + words[i-7] + sigma_zero(words[i-15]) + words[i-16];
    }
}

static size_t build_tail(const uint8_t *data, size_t len, uint8_t tail[128]){
    size_t rest = len % 64;
    size_t len_tail;

    for (size_t i = 0; i < 128; i++){
        tail[i] = 0x00;
    }
    if (rest < 56)
        len_tail = 64;
    else
        len_tail = 128;
    size_t i = 0;
    while (i < rest){
        tail[i] = data[(len / 64) * 64 + i];
        i++;
    }
    tail[i] = 0x80;
    uint64_t size = len * 8UL;
    tail[len_tail - 8] = (size >> 56) & 0xff;
    tail[len_tail - 7] = (size >> 48) & 0xff;
    tail[len_tail - 6] = (size >> 40) & 0xff;
    tail[len_tail - 5] = (size >> 32) & 0xff;
    tail[len_tail - 4] = (size >> 24) & 0xff;
    tail[len_tail - 3] = (size >> 16) & 0xff;
    tail[len_tail - 2] = (size >> 8) & 0xff;
    tail[len_tail - 1] = size & 0xff;
    return len_tail;
}

static void process_block(const uint8_t* block_start, uint32_t hashvalues[8]){
    uint32_t message_schedule[64];
    uint32_t a, b, c, d, e, f, g, h;

    generate_messageschedule_from_block((const uint8_t *) block_start, message_schedule);
    a = hashvalues[0];
    b = hashvalues[1];
    c = hashvalues[2];
    d = hashvalues[3];
    e = hashvalues[4];
    f = hashvalues[5];
    g = hashvalues[6];
    h = hashvalues[7];    
    for (size_t t = 0; t < 64; t++){
        uint32_t T1 = h + big_sigma_1(e) + ch(e, f, g) + K[t] + message_schedule[t];
        uint32_t T2 = big_sigma_0(a) + maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + T1;
        d = c;
        c = b;
        b = a;
        a = T1 + T2;
    }
    hashvalues[0] += a;
    hashvalues[1] += b;
    hashvalues[2] += c;
    hashvalues[3] += d;
    hashvalues[4] += e;
    hashvalues[5] += f;
    hashvalues[6] += g;
    hashvalues[7] += h;
}

void sha256(const void* data, size_t len, uint8_t out[32]){
    uint32_t hashvalues[8] = { 
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    uint8_t tail[128];

    for (size_t i = 0; i < len / 64; i++){
        process_block((const uint8_t *) data + 64 * i, hashvalues);
    }
    size_t tail_len = build_tail((const uint8_t*) data, len, tail);
    process_block((const uint8_t *) tail, hashvalues);

    if (tail_len == 128){
        process_block((const uint8_t *) tail + 64, hashvalues);
    }

    for (size_t i = 0; i < 8; i++){
        out[4*i] = (hashvalues[i] >> 24) & 0xff;
        out[4*i+1] = (hashvalues[i] >> 16) & 0xff;
        out[4*i+2] = (hashvalues[i] >> 8) & 0xff;
        out[4*i+3] = hashvalues[i] & 0xff;
    }
}