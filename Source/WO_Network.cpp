////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Network.cpp
///
///			Description:
///			Winsock host/join. TCP packets are uint32-le length plus payload.
///			UDP is a separate unreliable datagram channel on the same port.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Network.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>

#pragma comment(lib, "ws2_32.lib")

namespace WOtech
{
	namespace
	{
		static const UINT_PTR kInvalidSocket = static_cast<UINT_PTR>(INVALID_SOCKET);

		struct WinsockLifetime
		{
			bool Ready;

			WinsockLifetime()
				: Ready(false)
			{
				WSADATA data;
				ZeroMemory(&data, sizeof(data));
				if (WSAStartup(MAKEWORD(2, 2), &data) == 0)
				{
					Ready = true;
				}
			}

			~WinsockLifetime()
			{
				if (Ready)
				{
					WSACleanup();
				}
			}
		};

		WinsockLifetime g_winsock;

		SOCKET AsSocket(_In_ UINT_PTR value)
		{
			return static_cast<SOCKET>(value);
		}

		void SetNonBlocking(_In_ SOCKET socket)
		{
			u_long mode = 1;
			ioctlsocket(socket, FIONBIO, &mode);
		}

		void WriteU32(_Inout_ std::vector<UINT8>& out, _In_ UINT32 value)
		{
			out.push_back(static_cast<UINT8>(value & 0xFFu));
			out.push_back(static_cast<UINT8>((value >> 8) & 0xFFu));
			out.push_back(static_cast<UINT8>((value >> 16) & 0xFFu));
			out.push_back(static_cast<UINT8>((value >> 24) & 0xFFu));
		}

		bool ReadU32(_In_reads_bytes_(4) UINT8 const* bytes, _Out_ UINT32* value)
		{
			if (bytes == nullptr || value == nullptr)
			{
				return false;
			}
			*value = static_cast<UINT32>(bytes[0])
				| (static_cast<UINT32>(bytes[1]) << 8)
				| (static_cast<UINT32>(bytes[2]) << 16)
				| (static_cast<UINT32>(bytes[3]) << 24);
			return true;
		}

		bool PopPacket(_Inout_ std::vector<UINT8>& incoming, _Out_ std::vector<UINT8>* packet)
		{
			if (packet == nullptr || incoming.size() < 4)
			{
				return false;
			}
			UINT32 size = 0;
			ReadU32(incoming.data(), &size);
			if (size > kMaxPacketBytes)
			{
				incoming.clear();
				return false;
			}
			if (incoming.size() < static_cast<size_t>(size) + 4u)
			{
				return false;
			}
			packet->assign(incoming.begin() + 4, incoming.begin() + 4 + static_cast<std::ptrdiff_t>(size));
			incoming.erase(incoming.begin(), incoming.begin() + 4 + static_cast<std::ptrdiff_t>(size));
			return true;
		}

		void QueuePacket(_Inout_ std::vector<UINT8>& outgoing, _In_reads_bytes_(size) void const* data, _In_ UINT32 size)
		{
			if (data == nullptr && size != 0)
			{
				return;
			}
			if (size > kMaxPacketBytes)
			{
				return;
			}
			WriteU32(outgoing, size);
			if (size > 0)
			{
				UINT8 const* bytes = static_cast<UINT8 const*>(data);
				outgoing.insert(outgoing.end(), bytes, bytes + size);
			}
		}

		void FlushTcp(_In_ SOCKET socket, _Inout_ std::vector<UINT8>& outgoing)
		{
			while (!outgoing.empty())
			{
				int const chunk = static_cast<int>(outgoing.size() > 64u * 1024u ? 64u * 1024u : outgoing.size());
				int const sent = send(socket, reinterpret_cast<char const*>(outgoing.data()), chunk, 0);
				if (sent > 0)
				{
					outgoing.erase(outgoing.begin(), outgoing.begin() + sent);
				}
				else
				{
					break;
				}
			}
		}

		void ReceiveTcp(_In_ SOCKET socket, _Inout_ std::vector<UINT8>& incoming, _Out_ bool* closed)
		{
			if (closed != nullptr)
			{
				*closed = false;
			}
			UINT8 temp[4096];
			for (;;)
			{
				int const received = recv(socket, reinterpret_cast<char*>(temp), static_cast<int>(sizeof(temp)), 0);
				if (received > 0)
				{
					incoming.insert(incoming.end(), temp, temp + received);
					continue;
				}
				if (received == 0)
				{
					if (closed != nullptr)
					{
						*closed = true;
					}
				}
				else
				{
					int const error = WSAGetLastError();
					if (error != WSAEWOULDBLOCK && closed != nullptr)
					{
						*closed = true;
					}
				}
				break;
			}
		}

		bool ResolveIpv4(_In_z_ const char* host, _Out_ IN_ADDR* address)
		{
			if (host == nullptr || address == nullptr)
			{
				return false;
			}
			ZeroMemory(address, sizeof(*address));
			if (inet_pton(AF_INET, host, address) == 1)
			{
				return true;
			}
			addrinfo hints;
			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			addrinfo* result = nullptr;
			if (getaddrinfo(host, nullptr, &hints, &result) != 0 || result == nullptr)
			{
				return false;
			}
			sockaddr_in const* resolved = reinterpret_cast<sockaddr_in const*>(result->ai_addr);
			*address = resolved->sin_addr;
			freeaddrinfo(result);
			return true;
		}

		void PushBounded(_Inout_ std::vector<std::vector<UINT8>>& queue, _In_ std::vector<UINT8> const& packet)
		{
			if (queue.size() >= 64)
			{
				queue.erase(queue.begin());
			}
			queue.push_back(packet);
		}
	}

	struct Server::Impl
	{
		struct Conn
		{
			UINT_PTR Socket;
			std::vector<UINT8> Incoming;
			std::vector<UINT8> Outgoing;

			Conn()
				: Socket(kInvalidSocket)
			{
			}
		};

		NET_STATE State;
		UINT16 Port;
		UINT_PTR ListenSocket;
		UINT_PTR UdpSocket;
		std::string LastError;
		std::vector<Conn> Clients;
		std::vector<std::vector<UINT8>> Inbox;
		std::vector<std::vector<UINT8>> UdpInbox;

		Impl()
			: State(NET_STATE_IDLE)
			, Port(0)
			, ListenSocket(kInvalidSocket)
			, UdpSocket(kInvalidSocket)
		{
		}
	};

	struct Client::Impl
	{
		NET_STATE State;
		UINT_PTR Socket;
		UINT_PTR UdpSocket;
		std::string LastError;
		std::vector<UINT8> Incoming;
		std::vector<UINT8> Outgoing;
		std::vector<std::vector<UINT8>> Inbox;
		std::vector<std::vector<UINT8>> UdpInbox;

		Impl()
			: State(NET_STATE_IDLE)
			, Socket(kInvalidSocket)
			, UdpSocket(kInvalidSocket)
		{
		}
	};

	static void CloseSocket(_Inout_ UINT_PTR* socket)
	{
		if (socket == nullptr || *socket == kInvalidSocket)
		{
			return;
		}
		closesocket(AsSocket(*socket));
		*socket = kInvalidSocket;
	}

	Server::Server()
		: m_impl(new Impl())
	{
	}

	Server::~Server()
	{
		Stop();
		delete m_impl;
		m_impl = nullptr;
	}

	void Server::Start()
	{
		Start(kDefaultGamePort);
	}

	void Server::Start(_In_ UINT16 port)
	{
		Stop();
		if (!g_winsock.Ready)
		{
			m_impl->LastError = "Winsock failed to start";
			return;
		}
		if (port != 0 && port < 1024)
		{
			m_impl->LastError = "Refusing privileged port";
			return;
		}

		SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (listenSocket == INVALID_SOCKET)
		{
			m_impl->LastError = "socket failed";
			return;
		}

		BOOL reuse = TRUE;
		setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char const*>(&reuse), sizeof(reuse));
		SetNonBlocking(listenSocket);

		sockaddr_in address;
		ZeroMemory(&address, sizeof(address));
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(port);
		if (bind(listenSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR)
		{
			m_impl->LastError = "bind failed";
			closesocket(listenSocket);
			return;
		}
		if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			m_impl->LastError = "listen failed";
			closesocket(listenSocket);
			return;
		}

		sockaddr_in bound;
		ZeroMemory(&bound, sizeof(bound));
		int boundLength = sizeof(bound);
		getsockname(listenSocket, reinterpret_cast<sockaddr*>(&bound), &boundLength);

		SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (udpSocket != INVALID_SOCKET)
		{
			SetNonBlocking(udpSocket);
			sockaddr_in udpAddress = address;
			udpAddress.sin_port = bound.sin_port;
			if (bind(udpSocket, reinterpret_cast<sockaddr*>(&udpAddress), sizeof(udpAddress)) == SOCKET_ERROR)
			{
				closesocket(udpSocket);
				udpSocket = INVALID_SOCKET;
			}
		}

		m_impl->ListenSocket = static_cast<UINT_PTR>(listenSocket);
		m_impl->UdpSocket = static_cast<UINT_PTR>(udpSocket);
		m_impl->Port = ntohs(bound.sin_port);
		m_impl->State = NET_STATE_LISTENING;
		m_impl->LastError.clear();
	}

	void Server::Stop()
	{
		if (m_impl == nullptr)
		{
			return;
		}
		for (size_t i = 0; i < m_impl->Clients.size(); ++i)
		{
			CloseSocket(&m_impl->Clients[i].Socket);
		}
		m_impl->Clients.clear();
		CloseSocket(&m_impl->ListenSocket);
		CloseSocket(&m_impl->UdpSocket);
		m_impl->Inbox.clear();
		m_impl->UdpInbox.clear();
		m_impl->Port = 0;
		m_impl->State = NET_STATE_IDLE;
	}

	void Server::Send()
	{
		UINT8 const ping[4] = { 'W', 'O', 0, 1 };
		Send(ping, 4);
	}

	void Server::Send(_In_reads_bytes_(size) void const* data, _In_ UINT32 size)
	{
		if (m_impl == nullptr)
		{
			return;
		}
		for (size_t i = 0; i < m_impl->Clients.size(); ++i)
		{
			Impl::Conn& conn = m_impl->Clients[i];
			QueuePacket(conn.Outgoing, data, size);
			FlushTcp(AsSocket(conn.Socket), conn.Outgoing);
		}
	}

	void Server::SendUnreliable(_In_z_ const char* host, _In_ UINT16 port, _In_reads_bytes_(size) void const* data, _In_ UINT32 size)
	{
		if (m_impl == nullptr || m_impl->UdpSocket == kInvalidSocket || data == nullptr || size == 0 || size > kMaxPacketBytes)
		{
			return;
		}
		IN_ADDR address;
		if (!ResolveIpv4(host, &address))
		{
			m_impl->LastError = "UDP resolve failed";
			return;
		}
		sockaddr_in remote;
		ZeroMemory(&remote, sizeof(remote));
		remote.sin_family = AF_INET;
		remote.sin_addr = address;
		remote.sin_port = htons(port);
		sendto(AsSocket(m_impl->UdpSocket), static_cast<char const*>(data), static_cast<int>(size), 0, reinterpret_cast<sockaddr*>(&remote), sizeof(remote));
	}

	void Server::Poll()
	{
		if (m_impl == nullptr || m_impl->State != NET_STATE_LISTENING)
		{
			return;
		}

		for (;;)
		{
			SOCKET accepted = accept(AsSocket(m_impl->ListenSocket), nullptr, nullptr);
			if (accepted == INVALID_SOCKET)
			{
				break;
			}
			SetNonBlocking(accepted);
			BOOL noDelay = TRUE;
			setsockopt(accepted, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char const*>(&noDelay), sizeof(noDelay));
			Impl::Conn conn;
			conn.Socket = static_cast<UINT_PTR>(accepted);
			m_impl->Clients.push_back(conn);
		}

		for (size_t i = 0; i < m_impl->Clients.size();)
		{
			Impl::Conn& conn = m_impl->Clients[i];
			bool closed = false;
			ReceiveTcp(AsSocket(conn.Socket), conn.Incoming, &closed);
			std::vector<UINT8> packet;
			while (PopPacket(conn.Incoming, &packet))
			{
				PushBounded(m_impl->Inbox, packet);
			}
			FlushTcp(AsSocket(conn.Socket), conn.Outgoing);
			if (closed)
			{
				CloseSocket(&conn.Socket);
				m_impl->Clients.erase(m_impl->Clients.begin() + static_cast<std::ptrdiff_t>(i));
				continue;
			}
			++i;
		}

		if (m_impl->UdpSocket != kInvalidSocket)
		{
			UINT8 temp[2048];
			for (;;)
			{
				sockaddr_in from;
				ZeroMemory(&from, sizeof(from));
				int fromLength = sizeof(from);
				int const received = recvfrom(AsSocket(m_impl->UdpSocket), reinterpret_cast<char*>(temp), static_cast<int>(sizeof(temp)), 0, reinterpret_cast<sockaddr*>(&from), &fromLength);
				if (received <= 0)
				{
					break;
				}
				std::vector<UINT8> packet(temp, temp + received);
				PushBounded(m_impl->UdpInbox, packet);
			}
		}
	}

	bool Server::isListening() const
	{
		return m_impl != nullptr && m_impl->State == NET_STATE_LISTENING;
	}

	NET_STATE Server::getState() const
	{
		return m_impl != nullptr ? m_impl->State : NET_STATE_CLOSED;
	}

	UINT16 Server::getPort() const
	{
		return m_impl != nullptr ? m_impl->Port : 0;
	}

	UINT32 Server::getClientCount() const
	{
		return m_impl != nullptr ? static_cast<UINT32>(m_impl->Clients.size()) : 0;
	}

	bool Server::TryReceive(_Out_ std::vector<UINT8>* packet)
	{
		if (packet == nullptr || m_impl == nullptr || m_impl->Inbox.empty())
		{
			return false;
		}
		*packet = m_impl->Inbox.front();
		m_impl->Inbox.erase(m_impl->Inbox.begin());
		return true;
	}

	bool Server::TryReceiveUnreliable(_Out_ std::vector<UINT8>* packet)
	{
		if (packet == nullptr || m_impl == nullptr || m_impl->UdpInbox.empty())
		{
			return false;
		}
		*packet = m_impl->UdpInbox.front();
		m_impl->UdpInbox.erase(m_impl->UdpInbox.begin());
		return true;
	}

	const char* Server::getLastError() const
	{
		return m_impl != nullptr ? m_impl->LastError.c_str() : "";
	}

	Client::Client()
		: m_impl(new Impl())
	{
	}

	Client::~Client()
	{
		Disconnect();
		delete m_impl;
		m_impl = nullptr;
	}

	void Client::Connect(_In_z_ const char* host, _In_ UINT16 port)
	{
		Disconnect();
		if (!g_winsock.Ready)
		{
			m_impl->LastError = "Winsock failed to start";
			return;
		}
		if (port < 1024)
		{
			m_impl->LastError = "Refusing privileged port";
			return;
		}
		IN_ADDR address;
		if (!ResolveIpv4(host, &address))
		{
			m_impl->LastError = "resolve failed";
			return;
		}

		SOCKET socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (socket == INVALID_SOCKET)
		{
			m_impl->LastError = "socket failed";
			return;
		}
		SetNonBlocking(socket);
		sockaddr_in remote;
		ZeroMemory(&remote, sizeof(remote));
		remote.sin_family = AF_INET;
		remote.sin_addr = address;
		remote.sin_port = htons(port);
		int const result = connect(socket, reinterpret_cast<sockaddr*>(&remote), sizeof(remote));
		if (result == SOCKET_ERROR)
		{
			int const error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK && error != WSAEINPROGRESS && error != WSAEALREADY)
			{
				m_impl->LastError = "connect failed";
				closesocket(socket);
				return;
			}
			m_impl->State = NET_STATE_CONNECTING;
		}
		else
		{
			m_impl->State = NET_STATE_CONNECTED;
		}
		m_impl->Socket = static_cast<UINT_PTR>(socket);
		m_impl->LastError.clear();

		SOCKET udpSocket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (udpSocket != INVALID_SOCKET)
		{
			SetNonBlocking(udpSocket);
			sockaddr_in local;
			ZeroMemory(&local, sizeof(local));
			local.sin_family = AF_INET;
			local.sin_addr.s_addr = htonl(INADDR_ANY);
			local.sin_port = 0;
			if (bind(udpSocket, reinterpret_cast<sockaddr*>(&local), sizeof(local)) == SOCKET_ERROR)
			{
				closesocket(udpSocket);
				udpSocket = INVALID_SOCKET;
			}
		}
		m_impl->UdpSocket = static_cast<UINT_PTR>(udpSocket);
	}

	void Client::Disconnect()
	{
		if (m_impl == nullptr)
		{
			return;
		}
		CloseSocket(&m_impl->Socket);
		CloseSocket(&m_impl->UdpSocket);
		m_impl->Incoming.clear();
		m_impl->Outgoing.clear();
		m_impl->Inbox.clear();
		m_impl->UdpInbox.clear();
		m_impl->State = NET_STATE_IDLE;
	}

	void Client::Send()
	{
		UINT8 const ping[4] = { 'W', 'O', 0, 1 };
		Send(ping, 4);
	}

	void Client::Send(_In_reads_bytes_(size) void const* data, _In_ UINT32 size)
	{
		if (m_impl == nullptr || m_impl->Socket == kInvalidSocket)
		{
			return;
		}
		QueuePacket(m_impl->Outgoing, data, size);
		if (m_impl->State == NET_STATE_CONNECTED)
		{
			FlushTcp(AsSocket(m_impl->Socket), m_impl->Outgoing);
		}
	}

	void Client::SendUnreliable(_In_z_ const char* host, _In_ UINT16 port, _In_reads_bytes_(size) void const* data, _In_ UINT32 size)
	{
		if (m_impl == nullptr || m_impl->UdpSocket == kInvalidSocket || data == nullptr || size == 0 || size > kMaxPacketBytes)
		{
			return;
		}
		IN_ADDR address;
		if (!ResolveIpv4(host, &address))
		{
			m_impl->LastError = "UDP resolve failed";
			return;
		}
		sockaddr_in remote;
		ZeroMemory(&remote, sizeof(remote));
		remote.sin_family = AF_INET;
		remote.sin_addr = address;
		remote.sin_port = htons(port);
		sendto(AsSocket(m_impl->UdpSocket), static_cast<char const*>(data), static_cast<int>(size), 0, reinterpret_cast<sockaddr*>(&remote), sizeof(remote));
	}

	void Client::Poll()
	{
		if (m_impl == nullptr || m_impl->Socket == kInvalidSocket)
		{
			return;
		}

		if (m_impl->State == NET_STATE_CONNECTING)
		{
			fd_set writeSet;
			FD_ZERO(&writeSet);
			FD_SET(AsSocket(m_impl->Socket), &writeSet);
			timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = 0;
			int const selected = select(0, nullptr, &writeSet, nullptr, &timeout);
			if (selected > 0)
			{
				int soError = 0;
				int length = sizeof(soError);
				getsockopt(AsSocket(m_impl->Socket), SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&soError), &length);
				if (soError == 0)
				{
					m_impl->State = NET_STATE_CONNECTED;
					FlushTcp(AsSocket(m_impl->Socket), m_impl->Outgoing);
				}
				else
				{
					m_impl->LastError = "connect failed";
					Disconnect();
					m_impl->State = NET_STATE_CLOSED;
					return;
				}
			}
		}

		if (m_impl->State != NET_STATE_CONNECTED)
		{
			return;
		}

		bool closed = false;
		ReceiveTcp(AsSocket(m_impl->Socket), m_impl->Incoming, &closed);
		std::vector<UINT8> packet;
		while (PopPacket(m_impl->Incoming, &packet))
		{
			PushBounded(m_impl->Inbox, packet);
		}
		FlushTcp(AsSocket(m_impl->Socket), m_impl->Outgoing);
		if (closed)
		{
			Disconnect();
			m_impl->State = NET_STATE_CLOSED;
			return;
		}

		if (m_impl->UdpSocket != kInvalidSocket)
		{
			UINT8 temp[2048];
			for (;;)
			{
				sockaddr_in from;
				int fromLength = sizeof(from);
				int const received = recvfrom(AsSocket(m_impl->UdpSocket), reinterpret_cast<char*>(temp), static_cast<int>(sizeof(temp)), 0, reinterpret_cast<sockaddr*>(&from), &fromLength);
				if (received <= 0)
				{
					break;
				}
				PushBounded(m_impl->UdpInbox, std::vector<UINT8>(temp, temp + received));
			}
		}
	}

	bool Client::WaitConnected(_In_ UINT32 timeoutMs)
	{
		ULONGLONG const start = GetTickCount64();
		for (;;)
		{
			Poll();
			if (isConnected())
			{
				return true;
			}
			if (getState() == NET_STATE_CLOSED || getState() == NET_STATE_IDLE)
			{
				return false;
			}
			if (GetTickCount64() - start > timeoutMs)
			{
				return false;
			}
			Sleep(1);
		}
	}

	bool Client::isConnected() const
	{
		return m_impl != nullptr && m_impl->State == NET_STATE_CONNECTED;
	}

	NET_STATE Client::getState() const
	{
		return m_impl != nullptr ? m_impl->State : NET_STATE_CLOSED;
	}

	bool Client::TryReceive(_Out_ std::vector<UINT8>* packet)
	{
		if (packet == nullptr || m_impl == nullptr || m_impl->Inbox.empty())
		{
			return false;
		}
		*packet = m_impl->Inbox.front();
		m_impl->Inbox.erase(m_impl->Inbox.begin());
		return true;
	}

	bool Client::TryReceiveUnreliable(_Out_ std::vector<UINT8>* packet)
	{
		if (packet == nullptr || m_impl == nullptr || m_impl->UdpInbox.empty())
		{
			return false;
		}
		*packet = m_impl->UdpInbox.front();
		m_impl->UdpInbox.erase(m_impl->UdpInbox.begin());
		return true;
	}

	const char* Client::getLastError() const
	{
		return m_impl != nullptr ? m_impl->LastError.c_str() : "";
	}
}
