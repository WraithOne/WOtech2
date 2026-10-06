# Network

`Server` and `Client` use Winsock. TCP packets are a little-endian uint32 length plus payload. A UDP socket on the same port is the unreliable channel. The default port is 27015. Privileged ports are refused. Call `Poll` from the update loop. Loopback is covered by `WOtech2Tests`.
