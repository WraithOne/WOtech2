////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: Network.h
///
///			Description:
///			TCP game-command channel and a small unreliable UDP channel.
///			Packets are little-endian length-prefixed. Call Poll from the
///			update loop. Does not bind privileged ports.
///
///			Created:	13.05.2014
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_NETWORK_H
#define WO_NETWORK_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"
#include "WO_Utilities.hpp"

#include <vector>

namespace WOtech
{
	enum NET_STATE
	{
		NET_STATE_IDLE = 0,
		NET_STATE_LISTENING = 1,
		NET_STATE_CONNECTING = 2,
		NET_STATE_CONNECTED = 3,
		NET_STATE_CLOSED = 4
	};

	static const UINT16 kDefaultGamePort = 27015;
	static const UINT32 kMaxPacketBytes = 1024u * 1024u;

	class Server
	{
	public:
		Server();
		~Server();

		Server(_In_ Server const&) = delete;
		Server& operator=(_In_ Server const&) = delete;

		void Start();
		void Start(_In_ UINT16 port);
		void Stop();
		void Send();
		void Send(_In_reads_bytes_(size) void const* data, _In_ UINT32 size);
		void SendUnreliable(_In_z_ const char* host, _In_ UINT16 port, _In_reads_bytes_(size) void const* data, _In_ UINT32 size);
		void Poll();

		bool isListening() const;
		NET_STATE getState() const;
		UINT16 getPort() const;
		UINT32 getClientCount() const;
		bool TryReceive(_Out_ std::vector<UINT8>* packet);
		bool TryReceiveUnreliable(_Out_ std::vector<UINT8>* packet);
		const char* getLastError() const;

	private:
		struct Impl;
		Impl*		m_impl;
	};

	class Client
	{
	public:
		Client();
		~Client();

		Client(_In_ Client const&) = delete;
		Client& operator=(_In_ Client const&) = delete;

		void Connect(_In_z_ const char* host, _In_ UINT16 port);
		void Disconnect();
		void Send();
		void Send(_In_reads_bytes_(size) void const* data, _In_ UINT32 size);
		void SendUnreliable(_In_z_ const char* host, _In_ UINT16 port, _In_reads_bytes_(size) void const* data, _In_ UINT32 size);
		void Poll();
		bool WaitConnected(_In_ UINT32 timeoutMs);

		bool isConnected() const;
		NET_STATE getState() const;
		bool TryReceive(_Out_ std::vector<UINT8>* packet);
		bool TryReceiveUnreliable(_Out_ std::vector<UINT8>* packet);
		const char* getLastError() const;

	private:
		struct Impl;
		Impl*		m_impl;
	};
}

#endif
