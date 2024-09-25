#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

int debug = 0;
int IP[8] = {2, 6, 3, 1, 4, 8, 5, 7};
int IP1[8] = {4, 1, 3, 5, 7, 2, 8, 6};
int P10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
int P8[8] = {6, 3, 7, 4, 8, 5, 10, 9};
int EP[8] = {4, 1, 2, 3, 2, 3, 4, 1};
int S0[4][4] = {{1, 0, 3, 2},
                {3, 2, 1, 0},
                {0, 2, 1, 3},
                {3, 1, 3, 2}};
int S1[4][4] = {{0, 1, 2, 3},
                {2, 0, 1, 3},
                {3, 0, 1, 0},
                {2, 1, 0, 3}};
int P4[4] = {2, 4, 3, 1};

// function to convert a char to an integer array (0s and 1s)
void charToIntArray(char c, int intArray[8])
{
    // loop through each bit of the character
    for (int i = 0; i < 8; i++)
    {
        // extract the bit using bitwise AND and shifting
        intArray[i] = (c >> (7 - i)) & 1; // >> does not modify the original value of c
    }
}

// function to print the integer array
void printIntArray(const int intArray[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << intArray[i];
    }
    cout << endl;
}

void permute(int initialArray[], int permuteArray[], int newArray[], int size)
{
    for (int i = 0; i < size; i++)
    {
        newArray[i] = initialArray[permuteArray[i] - 1];
    }
}

void leftShift(int array[], int size)
{
    int temp = array[0];
    for (int i = 0; i < size - 1; i++)
    {
        array[i] = array[i + 1];
    }
    array[size - 1] = temp;
}

void combineArrays(int arr1[], int size1, int arr2[], int size2, int combined[])
{
    // copy elements of the first array into the combined array
    for (int i = 0; i < size1; i++)
    {
        combined[i] = arr1[i];
    }

    // copy elements of the second array into the combined array
    for (int i = 0; i < size2; i++)
    {
        combined[size1 + i] = arr2[i];
    }
}

void keygen(string hex, int keyArray1[], int keyArray2[])
{
    int keyArray[10];
    int key = stoi(hex, nullptr, 16);
    if (debug == 1)
    {
        cout << "Key String: " << key << endl;
    }
    // convert key string into int bit array
    for (int i = 0; i < 10; i++)
    {
        keyArray[i] = (key >> (9 - i)) & 1;
    }

    if (debug == 1)
    {
        cout << "Key Array: ";
        printIntArray(keyArray, 10);
        cout << endl;
    }

    // permute key array with P10
    int permutedKeyArray[10];
    permute(keyArray, P10, permutedKeyArray, 10);

    if (debug == 1)
    {
        cout << "P10 Key Array: ";
        printIntArray(permutedKeyArray, 10);
        cout << endl;
    }

    // break permuted key array into left and right halves
    int leftKeyArray[5];
    int rightKeyArray[5];
    for (int i = 0; i < 5; i++)
    {
        leftKeyArray[i] = permutedKeyArray[i];
    }
    for (int i = 0; i < 5; i++)
    {
        rightKeyArray[i] = permutedKeyArray[i + 5];
    }

    // left shift each half once
    leftShift(leftKeyArray, 5);
    leftShift(rightKeyArray, 5);
    // combine shifted halves back into one size 10 array
    int leftShifted10[10];
    combineArrays(leftKeyArray, 5, rightKeyArray, 5, leftShifted10);
    // convert combined array to 8bit array using P8 and assign it to keyArray1
    permute(leftShifted10, P8, keyArray1, 8);

    // left shift each half again twice
    leftShift(leftKeyArray, 5);
    leftShift(leftKeyArray, 5);
    leftShift(rightKeyArray, 5);
    leftShift(rightKeyArray, 5);
    // combine shifted halves back into one size 10 array
    combineArrays(leftKeyArray, 5, rightKeyArray, 5, leftShifted10);
    // convert combined array to 8bit array using P8 and assign it to keyArray1
    permute(leftShifted10, P8, keyArray2, 8);
}

void expansion(int initialArray[4], int expandedArray[8])
{
    for (int i = 0; i < 8; i++)
    {
        expandedArray[i] = initialArray[EP[i] - 1];
    }
}

void feistal(int charArray[8], int keyArray[8], int output[8])
{
    // split charArray into left and right halves
    int leftCharArray[4];
    int rightCharArray[4];
    for (int i = 0; i < 4; i++)
    {
        leftCharArray[i] = charArray[i];
    }
    for (int i = 0; i < 4; i++)
    {
        rightCharArray[i] = charArray[i + 4];
    }

    // expand right half of the array into 8 bits
    int expandedRight[8];
    expansion(rightCharArray, expandedRight);

    // xor the expanded right array and the secret key
    int xorResultArray[8];
    for (int i = 0; i < 8; i++)
    {
        xorResultArray[i] = (expandedRight[i] ^ keyArray[i]);
    }

    // split xor array into left and right halves
    int leftXorArray[4];
    int rightXorArray[4];
    for (int i = 0; i < 4; i++)
    {
        leftXorArray[i] = xorResultArray[i];
    }
    for (int i = 0; i < 4; i++)
    {
        rightXorArray[i] = xorResultArray[i + 4];
    }

    // look at bits convert to table coordinates
    // left coordinates
    string row1binary = to_string(leftXorArray[0]) + to_string(leftXorArray[3]);
    int row1 = stoi(row1binary, nullptr, 2);
    string col1binary = to_string(leftXorArray[1]) + to_string(leftXorArray[2]);
    int col1 = stoi(col1binary, nullptr, 2);

    // right coordinates
    string row2binary = to_string(rightXorArray[0]) + to_string(rightXorArray[3]);
    int row2 = stoi(row2binary, nullptr, 2);
    string col2binary = to_string(rightXorArray[1]) + to_string(rightXorArray[2]);
    int col2 = stoi(col2binary, nullptr, 2);

    // find table values from coordinates
    int leftTableVal = S0[row1][col1];
    int rightTableVal = S1[row2][col2];

    // convert back to binary array of 2 bits each
    int leftSubstitutionArray[2];
    for (int i = 0; i < 2; i++)
    {
        leftSubstitutionArray[i] = (leftTableVal >> (1 - i)) & 1;
    }
    int rightSubstitutionArray[2];
    for (int i = 0; i < 2; i++)
    {
        rightSubstitutionArray[i] = (rightTableVal >> (1 - i)) & 1;
    }

    // combine left and right binary arrays
    int substitutionArray[4];
    combineArrays(leftSubstitutionArray, 2, rightSubstitutionArray, 2, substitutionArray);

    // permute substitution array with P4
    int permutedSubstitutionArray[4];
    permute(substitutionArray, P4, permutedSubstitutionArray, 4);

    // xor left half of char array and permuted substitution array
    int leftXorPermutedSubstitutionResultArray[4];
    for (int i = 0; i < 4; i++)
    {
        leftXorPermutedSubstitutionResultArray[i] = (leftCharArray[i] ^ permutedSubstitutionArray[i]);
    }

    // combine left xor permuted substitution array with original right char array
    combineArrays(leftXorPermutedSubstitutionResultArray, 4, rightCharArray, 4, output);
}

void switchArray(int array[8], int switchedArray[8])
{
    // split array into left and right halves
    int leftArray[4];
    int rightArray[4];
    for (int i = 0; i < 4; i++)
    {
        leftArray[i] = array[i];
    }
    for (int i = 0; i < 4; i++)
    {
        rightArray[i] = array[i + 4];
    }
    combineArrays(rightArray, 4, leftArray, 4, switchedArray);
}

// char bitsToChar(int bits[8])
// {
//     char result = 0;
//     // iterate over the 8 bits
//     for (int i = 0; i < 8; i++)
//     {
//         // shift the bit and combine it using bitwise OR
//         result |= (bits[i] << (7 - i));
//     }

//     return result;
// }

char bitsToChar(int bits[8])
{
    int temp;
    for (int i = 0; i < 8; ++i)
    {
        temp += bits[i] * pow(2, 7 - i);
    }
    char finChar = temp;
    return finChar;
}

char process(char c, string key)
{
    // create an integer array to hold the 8 bits
    int intArrayOfC[8];

    // convert the character to an integer array
    charToIntArray(c, intArrayOfC);

    if (debug == 1)
    {
        cout << "Initial Char Array: ";
        printIntArray(intArrayOfC, 8);
        cout << endl;
    }

    // permute int array of c with IP
    int permutedArray[8];
    permute(intArrayOfC, IP, permutedArray, 8);

    if (debug == 1)
    {
        cout << "Permuted with IP Array: ";
        printIntArray(permutedArray, 8);
        cout << endl;
    }

    // generate both keys
    int keyArray1[8];
    int keyArray2[8];
    keygen(key, keyArray1, keyArray2);

    if (debug == 1)
    {
        cout << "Key Arrays: " << endl;
        printIntArray(keyArray1, 8);
        cout << endl;
        printIntArray(keyArray2, 8);
        cout << endl;
    }

    // run feistal function using first key
    int feistalOutput1[8];
    feistal(permutedArray, keyArray1, feistalOutput1);

    if (debug == 1)
    {
        cout << "Feistal Output 1: ";
        printIntArray(feistalOutput1, 8);
        cout << endl;
    }

    // switch feistal output data
    int switchedFeistalOutput1[8];
    switchArray(feistalOutput1, switchedFeistalOutput1);

    if (debug == 1)
    {
        cout << "Switched Feistal Output 1: ";
        printIntArray(switchedFeistalOutput1, 8);
        cout << endl;
    }

    // run feistal function using second key on switched feistal output
    int feistalOutput2[8];
    feistal(switchedFeistalOutput1, keyArray2, feistalOutput2);

    if (debug == 1)
    {
        cout << "Feistal Output 2: ";
        printIntArray(feistalOutput2, 8);
        cout << endl;
    }

    // permute second feistal output with IP-1
    int finalOutput[8];
    permute(feistalOutput2, IP1, finalOutput, 8);

    if (debug == 1)
    {
        cout << "Encrypted Char Array: ";
        printIntArray(finalOutput, 8);
        cout << endl;
    }

    // convert final output array back into a char
    return bitsToChar(finalOutput);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Invalid Number of Arguments" << endl;
    }
    string keyString = (argv[1]);
    char current = getchar();

    while (current != EOF)
    {
        cout << process(current, keyString);
        current = getchar();
    }

    return 0;
}
