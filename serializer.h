#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <set>

/**
 * define serializer
 */

template <typename SerializableType>
std::string serialize(SerializableType &a) {
	std::stringstream ss;
	ss << a;
	return ss.str();
}

class Serializer {
	public:
		// 默认以二进制写入文件
		Serializer(): os(std::ios::binary) {}

		template <typename SerializableType>
		Serializer& operator<<(SerializableType &a) {
			std::string x = ::serialize(a);
			os.write(x.data(), x.size());
			return *this;
		}

		template <typename BasicType>
		Serializer& operator<<(std::vector<BasicType> &a) {
			// 将vector的长度和元素写入
			std::string x = ::serialize(a.size());
			os.write(x.data(), x.size());

			for (const auto &ele : a) {
				std::string item = serialize(ele);
				os.write(item.data(), item.size());
			}

			return *this;
		}

		template <typename BasicType>
		Serializer& operator<<(std::set<BasicType> &a) {
			std::vector<BasicType> tmp;
			std::copy(a.cbegin(), a.cend(), std::back_inserter(tmp));
			return this->operator<<(tmp);
		}

		std::string str() {
			return os.str();
		}

		std::ostringstream os;
};

template <typename SerializableType>
int deserialize(std::string str, SerializableType &a) {
	std::stringstream ss(str);
	ss >> a;
	return str.size() - ss.str().size();	
}

class DeSerializer {
	public:
		DeSerializer(const std::string &s) : str(s), total(s.size()) {}

		template <typename SerializableType>
		DeSerializer& operator>>(SerializableType &a) {
			// 返回第一个开始的位置
			int ret = ::deserialize(str, a);
			// 移除已经反序列化的部分
			str = str.substr(ret); 
			return *this; 
		}
	 
		template <typename BasicType> 
		DeSerializer& operator>>(std::vector<BasicType> &a) {
			int len = 0;
			int ret = deserialize(str, len);
			str = str.substr(ret);

			while (len--) {
				BasicType item;
				int size = ::deserialize(str, item);
				str = str.substr(size);
				a.push_back(item);
			}
			return *this;
		}

		template <typename BasicType>
		DeSerializer& operator>>(std::set<BasicType> &a) {
			std::vector<BasicType> tmp;
			std::copy(a.cbegin(), a.cend(), tmp);
			return this->operator>>(tmp);
		}
	protected:
		std::string str;
		int total;
};

class Serialiable {
	public:
		// 纯虚函数	
		virtual std::string serialize() = 0;
		virtual int deserialize(std::string&) = 0;
};

#endif
