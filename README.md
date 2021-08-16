# sha3-verification
SAW Verification of OpenSSL's SHA3 Implementation

### Description of files in repository
* [keccak.bc](keccak.bc): Bitcode created from clang -- Used by SAW proof.
* [keccak.c](keccak.c): C code pulled from openSSL's sha-3 implementation for proof.
* [keccak_openssl.cry](keccak_openssl.cry): Cryptol sha-3 implementation that visually matches openssl's implementation.
* [keccak_spec.cry](keccak_spec.cry): Cryptol sha-3 implementation that visually matches the spec.
* [keccak_openssl.saw](keccak_openssl.saw): C bytecode to Cryptol proof between keccak.bc and keccak_openssl.cry. 
* [keccak_spec.saw](keccak_spec.saw): C bytecode to Cryptol proof between keccak.bc and keccak_spec.cry.
* [test_vectors/*](test_vectors): Cryptol files that compare given NIST test_vector digests to generated digests.

### Helpful Links
* [Test Vectors](https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/secure-hashing)
* [NIST SHA3 Specification](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf)
* [Cryptol](https://cryptol.net/files/ProgrammingCryptol.pdf)