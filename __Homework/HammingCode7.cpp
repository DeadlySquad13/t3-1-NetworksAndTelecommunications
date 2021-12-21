#include <bits/stdc++.h>
#include <math.h>
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
using std::setw;
using BitVector = std::vector<int>;
 
std::ostream& operator << (std::ostream& os, const BitVector& vec) 
{
    os << "[";
    for (int v: vec)
    {
        os << " " << v;
    }
    os << " ]";
    return os;
}

// Function to generate hamming code.
BitVector generateHammingCode(
    BitVector msgBits, int informationVectorSize, int numberOfCheckingBits)
{
    BitVector hammingCode(numberOfCheckingBits + informationVectorSize);
 
    // Find positions of checking bits.
    for (int i = 0; i < numberOfCheckingBits; i++)
    {
 
        // Placing -1 at checking bits
        // place to identify it later.
        hammingCode[pow(2, i) - 1] = -1;
    }

    //cout << hammingCode;
 
    int j = 0;

    int hammingCodeSize = numberOfCheckingBits + informationVectorSize;
 
    // Iterate to update the code.
    for (int i = 0; i < hammingCodeSize; i++)
    {
 
        // Placing msgBits where -1 is
        // absent i.e., except checking
        // bits all positions are msgBits.
        if (hammingCode[i] != -1)
        {
            hammingCode[i] = msgBits[j];
            j++;
        }
    }
 
    for (int i = 0; i < hammingCodeSize; i++)
    {
 
        // If current bit is not checking
        // bit then continue.
        if (hammingCode[i] != -1)
            continue;
 
        int x = log2(i + 1);
        int one_count = 0;
 
        // Find msg bits containing
        // set bit at x'th position.
        for (int j = i + 2; j <= hammingCodeSize; ++j)
        {
            if (j & (1 << x))
            {
                if (hammingCode[j - 1] == 1)
                {
                    one_count++;
                }
            }
        }
 
        // Generating hamming code for
        // even parity.
        if (one_count % 2 == 0)
        {
            hammingCode[i] = 0;
        } else
        {
            hammingCode[i] = 1;
        }
    }
 
    // Return the generated code.
    return hammingCode;
}

constexpr int calculateNumberOfCheckingBits(int informationVectorSize)
{
    int numberOfCheckingBits = 1;

    while (pow(2, numberOfCheckingBits) < (informationVectorSize + numberOfCheckingBits + 1)) {
        numberOfCheckingBits++;
    }
    
    return numberOfCheckingBits;
}

// Function to find the hamming code.
// of the given message bit informationVector[].
BitVector encode(BitVector& informationVector)
{
    int informationVectorSize = informationVector.size();
 
    // Find no. of checking bits.
    int numberOfCheckingBits;

    numberOfCheckingBits = calculateNumberOfCheckingBits(informationVectorSize);
 
    // Generating Code.
    BitVector hammingCode
        = generateHammingCode(informationVector, informationVectorSize, numberOfCheckingBits);

    return hammingCode;
}

// We need in our program only 3.
const int MAX_NUMBER_OF_SYNDROM_DIGITS = 3;

// Calculate index of error.
int calculateSyndrome(BitVector distortedVector)
{
    // Hardcoded values for testing.
    //int h1 = distortedVector[0] ^ distortedVector[2] ^ distortedVector[4] ^ distortedVector[6];
    //int h2 = distortedVector[1] ^ distortedVector[2] ^ distortedVector[5] ^ distortedVector[6];
    //int h3 = distortedVector[3] ^ distortedVector[4] ^ distortedVector[5] ^ distortedVector[6];

    //cout << "h1: " << h1 << ", ";
    //cout << "h2: " << h2 << ", ";
    //cout << "h3: " << h3 << "; ";


    // Syndrome have to store enough digits to index element in vector.
    //   Syndrome is binary so we have get log2.
    int syndromeMaxIndex = log2(distortedVector.size());

    if (syndromeMaxIndex > MAX_NUMBER_OF_SYNDROM_DIGITS - 1)
    {
        cout << "MAX_NUMBER_OF_SYNDROM_DIGITS is not big enough to calculate syndrome!";
    }

    // We are calculating syndrome by digits in binary representation. 
    std::bitset<MAX_NUMBER_OF_SYNDROM_DIGITS> syndrome{0};

    // Calculating syndrome by digits.
    for (int syndromeIndex = 0; syndromeIndex <= syndromeMaxIndex; syndromeIndex++)
    {
        // Filtering indexes of the vector.
        for (int i = 0; i < distortedVector.size(); i++)
        {
            // We need indexes that have digit = 1 at syndromeIndex in binary representation.
            // For example i = 1 => i = 0b001 => for syndromeIndex = 1 we need this index.
            std::bitset<8> elBitset{i + 1};
            if (elBitset[syndromeIndex])
            {
                // XORing indexes.
                syndrome[syndromeIndex] = syndrome[syndromeIndex] ^ distortedVector[i];
            }
        }
    }

    // It's counted as if elements go from 1, but we are indexing from 0.
    return static_cast<int>(syndrome.to_ulong()) - 1;
}

unsigned nChooseK(unsigned n, unsigned k)
{
    if (k > n) return 0;
    if (k * 2 > n) k = n-k;
    if (k == 0) return 1;

    int result = n;
    for( int i = 2; i <= k; ++i ) {
        result *= (n-i+1);
        result /= i;
    }

    return result;
}

BitVector decode(BitVector hammingCode)
{
    // Find positions of checking bits.
    unsigned numberOfCheckingBits = 0;
    unsigned checkingBitIndex;

    unsigned hammingCodeSize = hammingCode.size();

    do
    {
        checkingBitIndex = pow(2, numberOfCheckingBits) - 1;
        // Placing -1 at checking bits
        // place to identify it later.
        hammingCode[checkingBitIndex] = -1;

        numberOfCheckingBits++;
    } while(checkingBitIndex <= hammingCodeSize);

     
    //int j = 0;

    int informationVectorSize = hammingCodeSize + numberOfCheckingBits;
    BitVector informationVector;

    for (int el: hammingCode)
    {
        if (el != -1)
        {
            informationVector.push_back(el);
        }
    }

    // Iterate to update the code.
    //for (int i = 0; i < hammingCodeSize; i++)
    //{
        //// Placing msgBits where -1 is
        //// absent i.e., except checking
        //// bits all positions are msgBits.
        //if (hammingCode[i] != -1) {
            //hammingCode[i] = msgBits[j];
            //j++;
        //}
    //}

    return informationVector;
}

std::pair<unsigned, unsigned> testWithErrorVectors(
    BitVector hammingCode, int multiplicity, int length, BitVector informationVector
) {
    unsigned errorsFound = 0;
    unsigned successfulDecodings = 0;

    if (multiplicity > length) 
    {
        cout << "multiplicity of an error vector should be lower it's length!";

        std::pair<unsigned, unsigned> result{errorsFound, successfulDecodings};

        return result;
    }

    BitVector errorVector = BitVector(length, 0);

    // Should be sorted ascending.
    for (int i = length - 1; i >= length - multiplicity; i--)
    {
        errorVector[i] = 1;
    }
 
    do {
        //cout << "\n" << errorVector;

        // Hamming code distorted by noise (by errorVector).
        BitVector distortedVector = errorVector;

        std::transform(
            distortedVector.begin(), distortedVector.end(), // First operand.
            hammingCode.begin(), // Second operand.
            distortedVector.begin(), // Result.
            std::bit_xor<int>() // Operation.
        );

        //cout << distortedVector;

        // Calculating syndrome.
        int syndrome = calculateSyndrome(distortedVector);

        if (syndrome != -1)
        {
            errorsFound++;
            BitVector fixedDistortedVector = distortedVector;
            // Negating the error bit (it's located at index = syndrome).
            fixedDistortedVector[syndrome] ^= 1;

            BitVector decodedInformationVector = decode(fixedDistortedVector);
            if (decodedInformationVector == informationVector)
            {
                successfulDecodings++;
            }
        }
    } while (std::next_permutation(errorVector.begin(), errorVector.end()));

    std::pair<unsigned, unsigned> result{errorsFound, successfulDecodings};

    return result;
}
 

const unsigned COL_WIDTH = 10;
const std::string DELIMETER = " |";

template<typename T>
void printCell(T cellContents)
{
  cout << setw(COL_WIDTH) << cellContents << DELIMETER;
}

int main()
{
    // Init.
    BitVector informationVector = { 1, 1, 0, 0 };

    //cout << "Information code is: ";
    //cout << informationVector;
 
    // Encoding.
    BitVector hammingCode = encode(informationVector);
 
    // * Print encoding result.
    //cout << "\nHamming code is: ";
    //cout << hammingCode;

    int hammingCodeSize = hammingCode.size();

    printCell("i");
    printCell("Cin");
    printCell("N0");
    printCell("C0");
    printCell("Nk");
    printCell("Ck");
    cout << endl;
    cout << std::string(COL_WIDTH * (6 + 1) + 2, '=') << endl;

    for (int i = 0; i <= hammingCodeSize; i++)
    {
        unsigned errorsOfMultiplicity = nChooseK(hammingCodeSize, i);

        unsigned errorsFound;
        unsigned successfulDecodings;
        std::tie(errorsFound, successfulDecodings) =
          testWithErrorVectors(hammingCode, i, hammingCodeSize, informationVector);
        float detectPower = static_cast<float>(errorsFound) / errorsOfMultiplicity;

        float correctPower = static_cast<float>(successfulDecodings) / errorsOfMultiplicity;
        printCell(i); // i.
        printCell(errorsOfMultiplicity); // Cin.
        printCell(errorsFound); // N0.
        printCell(detectPower); // C0.
        printCell(successfulDecodings); // Nk.
        printCell(correctPower); // Ck.
        cout << endl;
    }
 
    return 0;
}
