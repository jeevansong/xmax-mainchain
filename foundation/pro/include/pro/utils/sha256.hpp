#pragma once

#include <picosha2.h>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>

namespace pro
{
	class CSHA256
{
public:

	CSHA256();

	~CSHA256();

	template<typename T> void Hash(const T& src);

	void Hash(const std::vector<unsigned char>& src);

	void Hash(std::vector<unsigned char>::iterator start, std::vector<unsigned char>::iterator end);
	void Hash(std::vector<unsigned char>::const_iterator cstart, std::vector<unsigned char>::const_iterator cend);

	const std::string& GetHex() const;

	const unsigned char* Data() const;

	const std::vector<unsigned char>& GetDataVec() const;

private:
	
	mutable std::string				hexHashStr_;
	std::vector<unsigned char>		hash_;
};

template<typename T>
void CSHA256::Hash(const T& src)
{
	std::stringbuf sbuf;
	std::ostream os(&sbuf);
	cereal::BinaryOutputArchive archive(os);
	archive(CEREAL_NVP(src));

	size_t nSize = sbuf.pubseekoff(0, std::ios_base::end);
	sbuf.pubseekoff(0, std::ios_base::beg);
	std::vector<unsigned char> vec(nSize);
	sbuf.sgetn(&vec[0], nSize);

	picosha2::hash256(vec.begin(), vec.end(), hash_.begin(), hash_.end());

}

inline void CSHA256::Hash(const std::vector<unsigned char>& src)
{
	picosha2::hash256(src.begin(), src.end(), hash_);
}

inline void CSHA256::Hash(std::vector<unsigned char>::iterator start, std::vector<unsigned char>::iterator end)
{
	picosha2::hash256(start, end, hash_);
}

inline void CSHA256::Hash(std::vector<unsigned char>::const_iterator cstart, std::vector<unsigned char>::const_iterator cend)
{
	picosha2::hash256(cstart, cend, hash_);
}

inline const std::string& CSHA256::GetHex() const
{
	if (hexHashStr_.empty())
	{
		hexHashStr_ = picosha2::bytes_to_hex_string(hash_.begin(), hash_.end());
	}

	return hexHashStr_;
}

inline const unsigned char* CSHA256::Data() const
{
	return &hash_[0];
}

inline const std::vector<unsigned char>& CSHA256::GetDataVec() const
{
	return hash_;
}

class DoubleSHA256
{
public:

	void Hash(const std::vector<unsigned char>& src);

	void Hash(std::vector<unsigned char>::iterator start, std::vector<unsigned char>::iterator end);

	const unsigned char* Data() const;

protected:
private:

	CSHA256		sha256_;
};

inline void DoubleSHA256::Hash(const std::vector<unsigned char>& src)
{
	CSHA256 firstHash;
	firstHash.Hash(src);
	const std::vector<unsigned char>& hash = firstHash.GetDataVec();
	sha256_.Hash(hash);
}

inline void DoubleSHA256::Hash(std::vector<unsigned char>::iterator start, std::vector<unsigned char>::iterator end)
{
	CSHA256 firstHash;
	firstHash.Hash(start, end);
	const std::vector<unsigned char>& hash = firstHash.GetDataVec();
	sha256_.Hash(hash.begin(), hash.end() );
}

inline const unsigned char* DoubleSHA256::Data() const
{
	return sha256_.Data();
}

}