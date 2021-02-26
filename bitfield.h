#ifndef BITFIELD_H
#define BITFIELD_H

/* Safe Mode 
   Performs bounds checking, enable when writing code
   that uses the bitfield class, disable for release build 
   for more efficient execution times */
#define SAFE_MODE 1

#include <iostream>

typedef unsigned int uint;

class BitField
{
	public:
		BitField(uint numBits)
		{
			this->_bitsPerInt = (sizeof(int)*8);
			this->_numBits = numBits;

			// get number of ints it takes to create bitfield
			this->_numInts = this->_numBits / this->_bitsPerInt;
			if(this->_numBits%this->_bitsPerInt > 0)
				this->_numInts++;

			// initialize bitfield and set to zero
			this->_bits = new uint[this->_numInts];
			this->clear();
		}

		~BitField()
		{
			delete this->_bits;
		}

		void clear()
		{
			memset(this->_bits, 0, this->_numInts*sizeof(uint));
		}

		void setBit(uint bitNum)
		{
			#if SAFE_MODE
			if(bitNum <= this->_numBits)
			{
			#endif
				uint idx = bitNum / this->_bitsPerInt;
				uint pos = bitNum % this->_bitsPerInt;
				this->_bits[idx] |= (1 << pos);
			#if SAFE_MODE
			}
			else
			{
				std::cout << "Error: Bit out of range." << std::endl;
			}
			#endif
		}

		// range set
		void setBitRange(uint startBit, uint endBit)
		{
			#if SAFE_MODE
			if(startBit <= this->_numBits && endBit <= this->_numBits)
			{
				if(startBit < endBit)
				{
			#endif
					uint idx = startBit / this->_bitsPerInt;
					uint pos = startBit % this->_bitsPerInt;

					for(uint i = 0; i <= endBit - startBit; i++)
					{
						this->_bits[idx] |= (1 << pos);
						if(++pos == this->_bitsPerInt)
						{
							pos = 0;
							idx++;
						}
					}
			#if SAFE_MODE
				}
				else
				{
					std::cout << "Error: startBit must be less than endBit." << std::endl;
				}
			}
			else
			{
				std::cout << "Error: Bit out of range." << std::endl;
			}
			#endif
		}

		void unsetBit(uint bitNum)
		{
			#if SAFE_MODE
			if(bitNum <= this->_numBits)
			{
			#endif
				uint idx = bitNum / this->_bitsPerInt;
				uint pos = bitNum % this->_bitsPerInt;
				this->_bits[idx] |= (1 << pos);
				this->_bits[idx] ^= (1 << pos);
			#if SAFE_MODE
			}
			else
			{
				std::cout << "Error: Bit out of range." << std::endl;
			}
			#endif
		}

		// range unset
		void unsetBitRange(uint startBit, uint endBit)
		{
			#if SAFE_MODE
			if(startBit <= this->_numBits && endBit <= this->_numBits)
			{
				if(startBit < endBit)
				{
			#endif
					uint idx = startBit / this->_bitsPerInt;
					uint pos = startBit % this->_bitsPerInt;
					for(uint i = 0; i <= endBit - startBit; i++)
					{
						this->_bits[idx] |= (1 << pos);
						this->_bits[idx] ^= (1 << pos);
						if(++pos == this->_bitsPerInt)
						{
							pos = 0;
							idx++;
						}
					}
			#if SAFE_MODE
				}
				else
				{
					std::cout << "Error: startBit must be less than endBit." << std::endl;
				}
			}
			else
			{
				std::cout << "Error: Bit out of range." << std::endl;
			}
			#endif
		}

		bool isBitSet(uint bitNum)
		{
			#if SAFE_MODE
			if(bitNum <= this->_numBits)
			{
			#endif
				uint idx = bitNum / this->_bitsPerInt;
				uint pos = bitNum % this->_bitsPerInt;
				if(this->_bits[idx] & (1 << pos))
					return true;
			#if SAFE_MODE
			}
			else
			{
				std::cout << "Error: Bit out of range." << std::endl;
			}
			#endif
			return false;
		}

		// is range set
		bool isBitRangeSet(uint startBit, uint endBit)
		{
			#if SAFE_MODE
			if(startBit <= _numBits && endBit <= _numBits)
			{
				if(startBit < endBit)
				{
			#endif
					for(unsigned int i = startBit; i <= endBit; i++)
						if(!this->isBitSet(i))
							return false;
			#if SAFE_MODE
				}
				else
				{
					std::cout << "Error: startBit must be less than endBit." << std::endl;
					return false;
				}
			}
			else
			{
				std::cout << "Error: Bit out of range." << std::endl;
				return false;
			}
			#endif
			return true;
		}

		// bit copy
		void bitCopy(uint from, uint to, uint numBits)
		{
			#if SAFE_MODE
			if(from <= _numBits && (to + numBits) <= _numBits)
			{
			#endif
				uint *_tmpBits = new uint[this->_numInts];
				memcpy(_tmpBits, this->_bits, this->_numInts*sizeof(uint));
				uint idx = to / this->_bitsPerInt;
				uint pos = to % this->_bitsPerInt;
				for(uint i = from; i < from + numBits; i++)
				{
					if(this->isBitSet(i))
					{
						_tmpBits[idx] |= (1 << pos);
					}
					else
					{
						_tmpBits[idx] |= (1 << pos);
						_tmpBits[idx] ^= (1 << pos);
					}
					if(++pos == this->_bitsPerInt)
					{
						pos = 0;
						idx++;
					}
				}
				memcpy(this->_bits, _tmpBits, this->_numInts*sizeof(uint));
				delete _tmpBits;
			#if SAFE_MODE
			}
			else
			{
				std::cout << "Error: Bit out of range." << std::endl;
			}
			#endif
		}

		void print()
		{
			uint numInts = this->_numBits / this->_bitsPerInt;
			if(this->_numBits%this->_bitsPerInt > 0)
				numInts++;

			for(int i = numInts - 1; i >= 0; i--)
				std::cout << std::hex << _bits[i] << " ";
			std::cout << std::endl;
		}

	private:
		uint		_numInts;
		uint		_bitsPerInt;
		uint		_numBits;
		uint*		_bits;
};

#endif

