#include "vector.h"
#include "utils.h"
#include <cstdlib>
#include <cstring>

namespace structures
{
	Vector::Vector(size_t size) :
		memory_(std::calloc(size, 1)),
		size_(size)
	{	
	}

	Vector::Vector(Vector& other) :
		Vector(other.size_)
	{
		memory_ = std::memcpy(memory_, other.memory_, size_);
	}

	Vector::~Vector()
	{
		std::free(memory_);
		memory_ = nullptr;
		size_ = 0;
	}

	size_t Vector::size()
	{
		return size_;
	}

	Structure& Vector::assign(Structure& other)
	{
		
		if (this != &other) //ak referencie neukazuju na rovnaké miesto
		{
			const Vector& otherVector = dynamic_cast<const Vector&>(other); //ak referencie neukazuju na rovnaké miesto
			size_ = otherVector.size_;
			memory_ = std::realloc(memory_, size_); //realokujem aby pamatove miesto bolo rovnake

			memory_ = std::memcpy(memory_, otherVector.memory_, size_);
		}
		return *this;
	}

	bool Vector::equals(Structure& other)
	{
		if (this == &other)
		{
			return true;
		}

		Vector* otherVector = dynamic_cast<Vector*>(&other);
		if (otherVector != nullptr)
		{
			return size_ == otherVector->size_ && std::memcmp(memory_,otherVector->memory_,size_) == 0;
		} 
		else 
		{
			return false;
		}
	}

	byte& Vector::operator[](int index)
	{
		return at(index);
	}

	byte& Vector::at(int index)
	{
		Utils::rangeCheckExcept(index, size_, "Invalid index in Vector!");
		return *(reinterpret_cast<byte*>(memory_) + index);
	}

	void Vector::copy(Vector& src, int srcStartIndex, Vector& dest, int destStartIndex, int length)
	{
		if(length != 0)
		Utils::rangeCheckExcept(srcStartIndex, src.size_, "Invalid index in src!");
		Utils::rangeCheckExcept(destStartIndex, dest.size_, "Invalid index in dest!");
		Utils::rangeCheckExcept(srcStartIndex + length, src.size_ + 1, "Invalid end index in src!");
		Utils::rangeCheckExcept(destStartIndex + length, dest.size_ + 1, "Invalid end index in dest!");

		if (&src == &dest && abs(srcStartIndex - destStartIndex) < length)
			memmove(dest.getBytePointer(destStartIndex), src.getBytePointer(srcStartIndex), length);
		else
			memcpy(dest.getBytePointer(destStartIndex), src.getBytePointer(srcStartIndex), length);

	}

	byte* Vector::getBytePointer(int index)
	{
		Utils::rangeCheckExcept(index, size_, "Invalid index in Vector!");
		return (reinterpret_cast<byte*>(memory_) + index);
	}
}