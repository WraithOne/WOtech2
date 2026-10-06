////////////////////////////////////////////////////////////////////////////
///
///			WraithOne tech 2 Engine
///
///			https://github.com/WraithOne/WOtech2
///			by https://twitter.com/WraithOne
///
///			File: WO_Json.cpp
///
///			Description:
///			JSON parser and writer for the engine scene and command formats.
///
///			Created:	06.10.2026
///			Edited:		06.10.2026
///
////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include "WO_pch.hpp"
#include "WO_Json.hpp"

namespace WOtech
{
	namespace
	{
		void AppendEscaped(_Inout_ std::string* out, _In_z_ const char* text)
		{
			out->push_back('"');
			if (text != nullptr)
			{
				for (const char* cursor = text; *cursor != '\0'; ++cursor)
				{
					unsigned char const ch = static_cast<unsigned char>(*cursor);
					switch (ch)
					{
					case '"': *out += "\\\""; break;
					case '\\': *out += "\\\\"; break;
					case '\n': *out += "\\n"; break;
					case '\r': *out += "\\r"; break;
					case '\t': *out += "\\t"; break;
					default:
						if (ch < 0x20)
						{
							char hex[8];
							sprintf_s(hex, "\\u%04X", ch);
							*out += hex;
						}
						else
						{
							out->push_back(static_cast<char>(ch));
						}
						break;
					}
				}
			}
			out->push_back('"');
		}

		class Parser
		{
		public:
			const char* Cursor;
			std::string* Error;

			explicit Parser(_In_z_ const char* text, _In_opt_ std::string* error)
				: Cursor(text)
				, Error(error)
			{
			}

			void Fail(_In_z_ const char* message)
			{
				if (Error != nullptr && Error->empty())
				{
					*Error = message;
				}
			}

			void Skip()
			{
				while (*Cursor == ' ' || *Cursor == '\t' || *Cursor == '\r' || *Cursor == '\n')
				{
					++Cursor;
				}
			}

			bool ParseValue(_Out_ JsonValue* value)
			{
				Skip();
				if (*Cursor == '{')
				{
					return ParseObject(value);
				}
				if (*Cursor == '[')
				{
					return ParseArray(value);
				}
				if (*Cursor == '"')
				{
					std::string text;
					if (!ParseString(&text))
					{
						return false;
					}
					value->setString(text.c_str());
					return true;
				}
				if (*Cursor == 't' && strncmp(Cursor, "true", 4) == 0)
				{
					Cursor += 4;
					value->setBool(true);
					return true;
				}
				if (*Cursor == 'f' && strncmp(Cursor, "false", 5) == 0)
				{
					Cursor += 5;
					value->setBool(false);
					return true;
				}
				if (*Cursor == 'n' && strncmp(Cursor, "null", 4) == 0)
				{
					Cursor += 4;
					value->setNull();
					return true;
				}
				return ParseNumber(value);
			}

			bool ParseObject(_Out_ JsonValue* value)
			{
				value->setNull();
				if (*Cursor != '{')
				{
					Fail("expected object");
					return false;
				}
				++Cursor;
				Skip();
				if (*Cursor == '}')
				{
					++Cursor;
					return true;
				}
				for (;;)
				{
					Skip();
					std::string key;
					if (!ParseString(&key))
					{
						return false;
					}
					Skip();
					if (*Cursor != ':')
					{
						Fail("expected colon");
						return false;
					}
					++Cursor;
					JsonValue& child = value->Set(key.c_str());
					if (!ParseValue(&child))
					{
						return false;
					}
					Skip();
					if (*Cursor == ',')
					{
						++Cursor;
						continue;
					}
					if (*Cursor == '}')
					{
						++Cursor;
						return true;
					}
					Fail("expected comma or end of object");
					return false;
				}
			}

			bool ParseArray(_Out_ JsonValue* value)
			{
				value->setNull();
				if (*Cursor != '[')
				{
					Fail("expected array");
					return false;
				}
				++Cursor;
				Skip();
				if (*Cursor == ']')
				{
					++Cursor;
					return true;
				}
				for (;;)
				{
					JsonValue& child = value->Add();
					if (!ParseValue(&child))
					{
						return false;
					}
					Skip();
					if (*Cursor == ',')
					{
						++Cursor;
						continue;
					}
					if (*Cursor == ']')
					{
						++Cursor;
						return true;
					}
					Fail("expected comma or end of array");
					return false;
				}
			}

			bool ParseString(_Out_ std::string* text)
			{
				if (*Cursor != '"')
				{
					Fail("expected string");
					return false;
				}
				++Cursor;
				text->clear();
				while (*Cursor != '\0' && *Cursor != '"')
				{
					if (*Cursor == '\\')
					{
						++Cursor;
						switch (*Cursor)
						{
						case '"': text->push_back('"'); break;
						case '\\': text->push_back('\\'); break;
						case '/': text->push_back('/'); break;
						case 'n': text->push_back('\n'); break;
						case 'r': text->push_back('\r'); break;
						case 't': text->push_back('\t'); break;
						case 'u':
							Cursor += 4;
							text->push_back('?');
							break;
						default:
							Fail("bad escape");
							return false;
						}
						if (*Cursor != '\0')
						{
							++Cursor;
						}
						continue;
					}
					text->push_back(*Cursor);
					++Cursor;
				}
				if (*Cursor != '"')
				{
					Fail("unterminated string");
					return false;
				}
				++Cursor;
				return true;
			}

			bool ParseNumber(_Out_ JsonValue* value)
			{
				const char* start = Cursor;
				if (*Cursor == '-')
				{
					++Cursor;
				}
				if (*Cursor < '0' || *Cursor > '9')
				{
					Fail("expected number");
					return false;
				}
				while (*Cursor >= '0' && *Cursor <= '9')
				{
					++Cursor;
				}
				if (*Cursor == '.')
				{
					++Cursor;
					while (*Cursor >= '0' && *Cursor <= '9')
					{
						++Cursor;
					}
				}
				if (*Cursor == 'e' || *Cursor == 'E')
				{
					++Cursor;
					if (*Cursor == '+' || *Cursor == '-')
					{
						++Cursor;
					}
					while (*Cursor >= '0' && *Cursor <= '9')
					{
						++Cursor;
					}
				}
				std::string token(start, Cursor);
				value->setNumber(atof(token.c_str()));
				return true;
			}
		};
	}

	JsonValue::Member::Member()
		: Value(nullptr)
	{
	}

	JsonValue::Member::Member(_In_ Member const& other)
		: Key(other.Key)
		, Value(other.Value != nullptr ? new JsonValue(*other.Value) : nullptr)
	{
	}

	JsonValue::Member& JsonValue::Member::operator=(_In_ Member const& other)
	{
		if (this != &other)
		{
			delete Value;
			Key = other.Key;
			Value = other.Value != nullptr ? new JsonValue(*other.Value) : nullptr;
		}
		return *this;
	}

	JsonValue::Member::~Member()
	{
		delete Value;
		Value = nullptr;
	}

	JsonValue::JsonValue()
		: m_type(JSON_TYPE_NULL)
		, m_bool(false)
		, m_number(0.0)
	{
	}

	JsonValue::JsonValue(_In_ JsonValue const& other)
		: m_type(JSON_TYPE_NULL)
		, m_bool(false)
		, m_number(0.0)
	{
		CopyFrom(other);
	}

	JsonValue& JsonValue::operator=(_In_ JsonValue const& other)
	{
		if (this != &other)
		{
			Clear();
			CopyFrom(other);
		}
		return *this;
	}

	JsonValue::~JsonValue()
	{
		Clear();
	}

	void JsonValue::CopyFrom(_In_ JsonValue const& other)
	{
		m_type = other.m_type;
		m_bool = other.m_bool;
		m_number = other.m_number;
		m_string = other.m_string;
		for (size_t i = 0; i < other.m_array.size(); ++i)
		{
			m_array.push_back(new JsonValue(*other.m_array[i]));
		}
		m_object = other.m_object;
	}

	void JsonValue::Clear()
	{
		for (size_t i = 0; i < m_array.size(); ++i)
		{
			delete m_array[i];
		}
		m_array.clear();
		m_object.clear();
		m_string.clear();
		m_bool = false;
		m_number = 0.0;
		m_type = JSON_TYPE_NULL;
	}

	JSON_TYPE JsonValue::getType() const { return m_type; }
	bool JsonValue::isNull() const { return m_type == JSON_TYPE_NULL; }
	bool JsonValue::isBool() const { return m_type == JSON_TYPE_BOOL; }
	bool JsonValue::isNumber() const { return m_type == JSON_TYPE_NUMBER; }
	bool JsonValue::isString() const { return m_type == JSON_TYPE_STRING; }
	bool JsonValue::isArray() const { return m_type == JSON_TYPE_ARRAY; }
	bool JsonValue::isObject() const { return m_type == JSON_TYPE_OBJECT; }

	void JsonValue::setNull()
	{
		Clear();
	}

	void JsonValue::setBool(_In_ bool value)
	{
		Clear();
		m_type = JSON_TYPE_BOOL;
		m_bool = value;
	}

	void JsonValue::setNumber(_In_ double value)
	{
		Clear();
		m_type = JSON_TYPE_NUMBER;
		m_number = value;
	}

	void JsonValue::setString(_In_z_ const char* value)
	{
		Clear();
		m_type = JSON_TYPE_STRING;
		m_string = (value != nullptr) ? value : "";
	}

	bool JsonValue::getBool(_In_ bool fallback) const
	{
		return isBool() ? m_bool : fallback;
	}

	double JsonValue::getNumber(_In_ double fallback) const
	{
		return isNumber() ? m_number : fallback;
	}

	const char* JsonValue::getString() const
	{
		return isString() ? m_string.c_str() : "";
	}

	UINT32 JsonValue::getCount() const
	{
		if (isArray())
		{
			return static_cast<UINT32>(m_array.size());
		}
		if (isObject())
		{
			return static_cast<UINT32>(m_object.size());
		}
		return 0;
	}

	JsonValue* JsonValue::getAt(_In_ UINT32 index)
	{
		if (!isArray() || index >= m_array.size())
		{
			return nullptr;
		}
		return m_array[index];
	}

	JsonValue const* JsonValue::getAt(_In_ UINT32 index) const
	{
		if (!isArray() || index >= m_array.size())
		{
			return nullptr;
		}
		return m_array[index];
	}

	JsonValue& JsonValue::Add()
	{
		if (!isArray())
		{
			Clear();
			m_type = JSON_TYPE_ARRAY;
		}
		JsonValue* child = new JsonValue();
		m_array.push_back(child);
		return *child;
	}

	JsonValue* JsonValue::Find(_In_z_ const char* key)
	{
		return const_cast<JsonValue*>(static_cast<JsonValue const*>(this)->Find(key));
	}

	JsonValue const* JsonValue::Find(_In_z_ const char* key) const
	{
		if (!isObject() || key == nullptr)
		{
			return nullptr;
		}
		for (size_t i = 0; i < m_object.size(); ++i)
		{
			if (m_object[i].Key == key)
			{
				return m_object[i].Value;
			}
		}
		return nullptr;
	}

	JsonValue& JsonValue::Set(_In_z_ const char* key)
	{
		if (!isObject())
		{
			Clear();
			m_type = JSON_TYPE_OBJECT;
		}
		JsonValue* existing = Find(key);
		if (existing != nullptr)
		{
			return *existing;
		}
		Member member;
		member.Key = (key != nullptr) ? key : "";
		member.Value = new JsonValue();
		m_object.push_back(member);
		return *m_object.back().Value;
	}

	void JsonValue::StringifyInto(_Inout_ std::string* out) const
	{
		switch (m_type)
		{
		case JSON_TYPE_NULL:
			*out += "null";
			break;
		case JSON_TYPE_BOOL:
			*out += m_bool ? "true" : "false";
			break;
		case JSON_TYPE_NUMBER:
			{
				char buffer[64];
				sprintf_s(buffer, "%.9g", m_number);
				*out += buffer;
			}
			break;
		case JSON_TYPE_STRING:
			AppendEscaped(out, m_string.c_str());
			break;
		case JSON_TYPE_ARRAY:
			out->push_back('[');
			for (size_t i = 0; i < m_array.size(); ++i)
			{
				if (i != 0)
				{
					out->push_back(',');
				}
				m_array[i]->StringifyInto(out);
			}
			out->push_back(']');
			break;
		case JSON_TYPE_OBJECT:
			out->push_back('{');
			for (size_t i = 0; i < m_object.size(); ++i)
			{
				if (i != 0)
				{
					out->push_back(',');
				}
				AppendEscaped(out, m_object[i].Key.c_str());
				out->push_back(':');
				m_object[i].Value->StringifyInto(out);
			}
			out->push_back('}');
			break;
		default:
			*out += "null";
			break;
		}
	}

	std::string JsonValue::Stringify() const
	{
		std::string out;
		StringifyInto(&out);
		return out;
	}

	bool JsonValue::Parse(_In_z_ const char* text, _Out_ JsonValue* value, _Out_opt_ std::string* error)
	{
		if (value == nullptr || text == nullptr)
		{
			if (error != nullptr)
			{
				*error = "null json";
			}
			return false;
		}
		if (error != nullptr)
		{
			error->clear();
		}
		Parser parser(text, error);
		if (!parser.ParseValue(value))
		{
			return false;
		}
		parser.Skip();
		if (*parser.Cursor != '\0')
		{
			parser.Fail("trailing data");
			return false;
		}
		return true;
	}
}
