# Arx

[Arx](https://eprint.iacr.org/2016/591) is an encrypted database system that places two proxies between a standard client and the database server:

- The trusted client proxy encrypts the client queries and forwards it to the server proxy.
- The untrusted server proxy handles encrypted queries by searching in encrypted indexes, then queries the database server for encrypted documents.

## Disclaimer

Unfortunately, the whole Arx system is not yet functional. However, PoCs can be found in the [main.cpp](./src/server-proxy/main.cpp) of the server proxy.

## Dependencies

This implementation of Arx requires the following dependencies:

- [`crypto++`](https://www.cryptopp.com/) `>= 8.6.0`
- [`boost`](https://www.boost.org/) `>= 1.74.0`

These can be installed on a Debian-based Linux distribution by running:

```sh
sudo apt install libcrypto++-dev libboost-all-dev
```

## Build & Run

To build the PoCs:

```sh
make bin/server-proxy
```

To run the PoCs

```
./bin/server-proxy
```

To clean all the compiled files:

```sh
make clean
```
