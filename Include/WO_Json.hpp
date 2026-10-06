////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Json.hpp
///
///			Description:
///			Small JSON DOM owned by the engine. Used for scenes, prefabs,
///			and the editor command API. Not a general-purpose library.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////
#ifndef WO_JSON_H
#define WO_JSON_H

//////////////
// INCLUDES //
//////////////
#include "..\WO_pch.hpp"

#include <string>
#include <vector>

namespace WOtech
{
	enum JSON_TYPE
	{
		JSON_TYPE_NULL = 0,
		JSON_TYPE_BOOL = 1,
		JSON_TYPE_NUMBER = 2,
		JSON_TYPE_STRING = 3,
		JSON_TYPE_ARRAY = 4,
		JSON_TYPE_OBJECT = 5
	};

	class JsonValue
	{
	public:
		JsonValue();
		JsonValue(_In_ JsonValue const& other);
		JsonValue& operator=(_In_ JsonValue const& other);
		~JsonValue();

		JSON_TYPE getType() const;
		bool isNull() const;
		bool isBool() const;
		bool isNumber() const;
		bool isString() const;
		bool isArray() const;
		bool isObject() const;

		void setNull();
		void setBool(_In_ bool value);
		void setNumber(_In_ double value);
		void setString(_In_z_ const char* value);

		bool getBool(_In_ bool fallback) const;
		double getNumber(_In_ double fallback) const;
		const char* getString() const;

		UINT32 getCount() const;
		JsonValue* getAt(_In_ UINT32 index);
		JsonValue const* getAt(_In_ UINT32 index) const;
		JsonValue& Add();

		JsonValue* Find(_In_z_ const char* key);
		JsonValue const* Find(_In_z_ const char* key) const;
		JsonValue& Set(_In_z_ const char* key);

		std::string Stringify() const;

		static bool Parse(_In_z_ const char* text, _Out_ JsonValue* value, _Out_opt_ std::string* error);

	private:
		struct Member
		{
			std::string Key;
			JsonValue* Value;

			Member();
			Member(_In_ Member const& other);
			Member& operator=(_In_ Member const& other);
			~Member();
		};

		void Clear();
		void CopyFrom(_In_ JsonValue const& other);
		void StringifyInto(_Inout_ std::string* out) const;

		JSON_TYPE					m_type;
		bool						m_bool;
		double						m_number;
		std::string					m_string;
		std::vector<JsonValue*>		m_array;
		std::vector<Member>			m_object;
	};
}

#endif
