# sha3-verification
SAW Verification of OpenSSL's SHA3 Implementation.

### Our previous work
This verification is similar to [work](https://bitbucket.org/byu-vv/saw-verified-crypto/src/master/) we previously completed on parts of OpenSSL's SHA2-256 implementation.  In that proof, we wrote an implementation of the algorithm using the domain-specific language [Cryptol](https://cryptol.net/files/ProgrammingCryptol.pdf) which was simpler than the C code to visually inspect for correctness against the NIST standard. We then used the [Software Analysis Workbench](https://saw.galois.com/) to prove out functional equivalence between our Cryptol code and the OpenSSL implementation.  This proof only covered the Hash Block and SHA Final functions, as these contained the bulk of the computation.  Both Cryptol and the SAW tool were written by [Galois, Inc](https://galois.com/).

### This work
After finishing the proofs on SHA2, we wanted to see if the methodology used there could be extended to a newer cryptographic algorithm in SHA3.  As with our previous work, we have written a Cryptol specification that is easier to compare visually to the NIST standard for the algorithm compared to OpenSSL's C code.  Our proof shows functional equivalence between the Cryptol code and the OpenSSL code for the Keccak function (this function hashes a single block of data and contains the most significant computations performed by SHA3).  

A significant part of our methodology is to modularize the code and simplify the input/output relationships for functions in order to take advantage of SAW's override system (A more in-depth review of SAW and the override system can be found in the main README of our [previous work](https://bitbucket.org/byu-vv/saw-verified-crypto/src/master/)).  Unlike the SHA2 code, we found that SHA3 was well suited to this methodology, because the Keccak function is simply a for loop running 5 inner functions.  Keccak and all of these inner functions simply take in a single 3d array and modify it, which means we already had something fairly modular, and with a simple input/output relationship.

While the code required much less restructuring because of the way the algorithm was written, the computations that take place in SHA3 are more complex than that of SHA2, which means that the workload on the SMT solver was much heavier.  This increase in complexity is reflected in the times required to run the function proofs as shown in the table below.  These times are much longer than the times required to complete the SHA2 proof (the full hash block with overrides took only 0.46 seconds).

Function | Average Time to complete 
-----------|-------------
Pi | 0.55846s
Rho | 1.84304s
Theta | 13.87495s
Chi | 3.37179s
Iota | 4.03463s
Keccak (with all 5 overrides) | 225.19360s


### Trusted Code Base (TCB)
Our TCB includes the following:
* LLVM bitcode compiler.
* The Software Analysis Workbench (SAW) including the conversion from LLVM bitcode to SAWCore terms, the Cryptol compiler to SAWCore terms, SAW's interface with SMT solvers.
* The Yices SMT solver.

### Downloading SAW and Solvers
We recommend that you use the binaries provided by Galois to download SAW.  You can find these binaries on the [SAW tool website](https://saw.galois.com/downloads.html).  Links to each of the solvers we used can also be found on this site.  The Cryptol compiler and interpreter is included in the SAW binary, or can be downloaded separately [here](https://cryptol.net/downloads.html). 

### Commands to run proof
##### C code compile command
clang -g -emit-llvm -o keccak.bc -c keccak.c
##### Run SAW script
saw keccak.saw
##### Run Test Vectors:
PARKER WILL YOU COVER THIS SINCE YOU WROTE THAT PART OF THE CODE?

### Description of files in repository
* [keccak.bc](keccak.bc): Bitcode of OpenSSL's C implementation compiled through Clang.
* [keccak.c](keccak.c): Relevant C code pulled from OpenSSL's SHA-3 implementation.
* [keccak_openssl.cry](keccak_openssl.cry): Cryptol sha-3 implementation that visually matches openssl's implementation.  We created this intermediate representation as a reference to help us build a Cryptol implementation that matches the spec but is provable with the C code.  This code is not used in the final proof.
* [keccak_spec.cry](keccak_spec.cry): Cryptol sha-3 implementation that visually matches the NIST specification.
* [keccak.saw](keccak.saw): C bytecode to Cryptol proof between keccak.bc and keccak_spec.cry.
* [proof_output.txt](proof_output.txt): Text output from running the full keccak.saw proof.
* [test_vectors/*](test_vectors): Cryptol files that compare given NIST test_vector digests to generated digests.  Instructions for running these test vectors can be found above.

### References:
* Decker B., Winters B., Mercer E. (2021) Towards Verifying SHA256 in OpenSSL with the Software Analysis Workbench. In: Dutle A., Moscato M.M., Titolo L., Muñoz C.A., Perez I. (eds) NASA Formal Methods. NFM 2021. Lecture Notes in Computer Science, vol 12673. Springer, Cham. https://doi.org/10.1007/978-3-030-76384-8_5

* [Test Vectors](https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/secure-hashing)
* [NIST SHA3 Specification](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf)
* [Cryptol](https://cryptol.net/files/ProgrammingCryptol.pdf)