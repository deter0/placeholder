#include <stdint.h>

uint32_t hash_u32( uint32_t key ) {
    key = ~key + ( key << 15 );
    key = key ^ ( key >> 12 );
    key = key + ( key << 2 );
    key = key ^ ( key >> 4 );
    key = (key + ( key << 3 ) ) + ( key << 11 );
    key = key ^ ( key >> 16);
    return key;
}


uint32_t hash_u64( uint64_t key ) {
    key = ( ~key ) + ( key << 18 );
    key = key ^ ( key >> 31 );
    key = ( key + ( key << 2 ) ) + ( key << 4 );
    key = key ^ ( key >> 11 );
    key = key + ( key << 6 );
    key = key ^ ( key >> 22 );  
    return (uint32_t) key;
}


uint32_t hash_str( char const* key ) {
    uint32_t hash = 5381u; 
    for( char const* s = key; *s != '\0'; ++s ) {
        hash = ( ( hash << 5u ) + hash ) ^ (*s);
    }
    return hash;
}


uint32_t murmur_hash( const void * key, int len, uint32_t seed ) {
	uint32_t const m = 0x5bd1e995;
	int const r = 24;
	uint32_t h = seed ^ len;
	
    uint8_t const* data = (uint8_t const*) key;
	while( len >= 4 ) {
		#ifdef PLATFORM_BIG_ENDIAN
			uint32_t k = ( data[0] ) + ( data[1] << 8 ) + ( data[2] << 16 ) + ( data[3] << 24 );
		#else
			uint32_t k = *(uint32_t *)data;
		#endif

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

    switch(len) {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
                h *= m;
	};

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}
