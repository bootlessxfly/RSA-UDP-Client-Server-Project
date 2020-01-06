Comp 6376 Project
The makefile for this project can be found in Debug/
cd Debug
make
* This code generates an RSA private and public key. It contains a client and server that communicate with each other over UDP sockets. Once a connection is made, the key exchange is activated. The keys are validated using validation ciphers, and once both the client and server have validated the cipher, the client sends the encrypted contract. The server decryptes the contract, signs it, encrypts it again, and sends it back. The encryption process uses the acotrs private key first, and the other actors public key to finish the encyption. The decryption is the inverse of this.

* Please refer to the word document for more details
