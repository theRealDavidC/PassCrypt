# PassCrypt

A simple educational tool demonstrating reversible password encryption using XOR stream cipher.

## ⚠️ WARNING

**This is for LEARNING ONLY. DO NOT use for real password storage!**

For production systems, use proper password hashing like:
- bcrypt
- Argon2
- scrypt

## What is this?

PassCrypt shows how symmetric encryption works. It can:
- Encrypt a password into random-looking text
- Decrypt it back to the original using the same key

This is different from password hashing (which should be one-way only).

## How it works

1. Takes your password and a secret key
2. Generates a pseudo-random keystream from the key
3. XORs password bytes with keystream bytes
4. Encodes result in Base64

Decryption does the same thing in reverse. Because XOR is reversible, you get the original password back.

## Build & Run

```bash
g++ -std=c++11 -o passcrypt main.cpp
./passcrypt
```

Or with the Makefile:

```bash
make
./passcrypt
```

## Example

```
Password: "1234"
Key: "my-secret-key"
Encrypted: "coR+zQ=="
Decrypted: "1234"
```

## Why encryption ≠ password hashing

| Encryption (this) | Password Hashing (correct way) |
|-------------------|--------------------------------|
| Reversible | One-way only |
| Fast | Intentionally slow |
| Gets original back | Compare hashes only |
| Same key needed | No key needed |
| For data protection | For password verification |

## Security Limitations

This implementation has serious weaknesses:
- Weak random number generator (LCG)
- No salt or IV
- No authentication
- Deterministic output
- Simple key derivation

**Never use this for anything real!**

## What you'll learn

- How XOR-based stream ciphers work
- Why XOR is self-inverse
- Base64 encoding basics
- Difference between encryption and hashing
- Why proper crypto libraries are important

## License

MIT License - Use for education only

---

**Remember:** Encryption hides data temporarily. Password hashing protects passwords permanently. Use the right tool for the job!
