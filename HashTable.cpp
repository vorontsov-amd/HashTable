#include <iostream>
#include <string>
#include <climits>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include "List.h"

namespace iLab
{
	using std::string;
	using std::cout;
	using std::endl;

	class word
	{
	public:
		word();
		word(string str, unsigned count);
		word(const word& value);
		~word();

		string str()
		{
			return __str__;
		}
		unsigned count()
		{
			return __count__;
		}


		word& operator++ ()
		{
			++__count__;
			return *this;
		}
		friend std::ostream& operator<<(std::ostream& stream, word const& value)
		{
			stream << "Word: " << std::setw(20) << value.__str__ << std::setw(10) << " Count: " << std::setw(5) << value.__count__;
			return stream;
		}

	private:
		string __str__;
		unsigned __count__;
	};

	word::word()
	{
		__str__ = "";
		__count__ = 0;
	}

	word::word(string str, unsigned count)
	{
		__str__ = str;
		__count__ = count;
	}

	word::word(const word& value)
	{
		__str__ = value.__str__;
		__count__ = value.__count__;
	}

	word::~word()
	{
	}



	unsigned __StandartHash__(string str);

	class HashTable
	{
		friend std::ostream& operator<< (std::ostream& stream, HashTable const& table);

	private:
		size_t __size__;
		size_t __capacity__;

		unsigned (*__HashFunc__) (string str);

		iLab::List<word>* __container__;

	public:
		HashTable(unsigned (*func) (string str) = __StandartHash__, size_t capacity = 1000);
		~HashTable();
		HashTable(const HashTable& table);

		HashTable& operator= (HashTable& table);

		void InstallHashFunc(unsigned (*func) (string str));
		unsigned Incert(const string& value);
		unsigned Search(const string& str);
		size_t Size();
	};

	size_t HashTable::Size()
	{
		return __size__;
	}

	HashTable::HashTable(unsigned (*func) (string str), size_t capacity)
	{
		__size__ = 0;
		__capacity__ = capacity;
		__HashFunc__ = func;
		__container__ = new iLab::List<word>[__capacity__] {};
	}

	HashTable::~HashTable()
	{
		delete[] __container__;
	}


	HashTable::HashTable(const HashTable& table)
	{
		__container__ = new iLab::List<word>[table.__capacity__];
		__size__ = table.__size__;
		__capacity__ = table.__capacity__;
		__HashFunc__ = table.__HashFunc__;

		for (int i = 0; i < table.__capacity__; i++)
		{
			__container__[i] = table.__container__[i];
		}
	}


	void HashTable::InstallHashFunc(unsigned (*func) (string str))
	{
		__HashFunc__ = func;
	}


	HashTable& HashTable::operator=(HashTable& table)
	{
		if (__capacity__ < table.__capacity__)
		{
			delete[] __container__;
			__container__ = new iLab::List<word>[table.__capacity__];
		}

		__size__ = table.__size__;
		__capacity__ = table.__capacity__;
		__HashFunc__ = table.__HashFunc__;

		for (int i = 0; i < table.__capacity__; i++)
		{
			__container__[i] = table.__container__[i];
		}

		return *this;
	}


	unsigned HashTable::Incert(const string& value)
	{
		unsigned hash = Search(value);
		if (hash == -1)
		{
			word new_element = {
				value,
				1,
			};
			hash = __HashFunc__(value) % __capacity__;
			__container__[hash].PushBack(new_element);
			__size__++;
		}
		else
		{
			for (int i = 1; i <= __container__[hash].Size(); i++)
			{
				word element = __container__[hash].Show(i);
				if (element.str() == value)
				{
					int index = __container__[hash].Delete(i);
					__container__[hash].Insert(index, ++element);
				}
			}
		}
		return hash;
	}

	unsigned __StandartHash__(string str)
	{
		size_t length = str.length();
		const int p = 256;
		unsigned hash = 0, p_pow = 1;
		for (size_t i = 0; i < length; ++i)
		{
			hash += (unsigned)str[i] * p_pow;
			p_pow *= p;
		}
		return hash;
	}

	std::ostream& operator<< (std::ostream& stream, HashTable const& table)
	{
		for (int i = 0; i < table.__capacity__; i++)
		{
			for (int k = 1; k <= table.__container__[i].Size(); k++)
			{
				stream << table.__container__[i].Show(k) << endl;
			}
		}
		return stream;
	}

	unsigned HashTable::Search(const string& str)
	{
		unsigned hash = __HashFunc__(str) % __capacity__;

		for (int i = 1; i <= __container__[hash].Size(); i++)
		{
			if (__container__[hash].Show(i).str() == str)
			{
				return hash;
			}
		}
		return -1;
	}
}


using namespace iLab;
int main()
{
	HashTable words;
	std::ifstream in;
	std::ofstream out;

	in.open("text.txt");
	out.open("words.txt");
	string str;
	while (!in.eof())
	{
		in >> str;
		words.Incert(str);
	}

	out << "Number of words: " << words.Size() << endl;
	out << words;
	in.close();
	out.close();
}