#include <iostream>

using namespace std;

int debug = 0;
int IP[8] = {2, 6, 3, 1, 4, 8, 5, 7};
int IP1[8] = {4, 1, 3, 5, 7, 2, 8, 6};
int P10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
int P8[8] = {6, 3, 7, 4, 8, 5, 10, 9};
int EP[8] = {4,1,2,3,2,3,4,1};

// Function to convert a char to an integer array (0s and 1s)
void charToIntArray(char c, int intArray[8])
{
    // Loop through each bit of the character
    for (int i = 0; i < 8; i++)
    {
        // Extract the bit using bitwise AND and shifting
        intArray[i] = (c >> (7 - i)) & 1; // >> does not modify the original value of c
    }
}

// Function to print the integer array
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
    // Copy elements of the first array into the combined array
    for (int i = 0; i < size1; i++)
    {
        combined[i] = arr1[i];
    }

    // Copy elements of the second array into the combined array
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
    int permutedKeyArray[10];
    permute(keyArray, P10, permutedKeyArray, 10);

    int leftKeyArray[5];
    int rightKeyArray[5];

    for (int i = 0; i < 5; i++)
    {
        leftKeyArray[i] = permutedKeyArray[i];
    }

    // Copy the second half (last 5 elements) into the secondHalf array
    for (int i = 0; i < 5; i++)
    {
        rightKeyArray[i] = permutedKeyArray[i + 5];
    }

    if (debug == 1)
    {
        cout << "Left Key Array: ";
        printIntArray(leftKeyArray, 5);
        cout << endl;

        cout << "Right Key Array: ";
        printIntArray(rightKeyArray, 5);
        cout << endl;
    }

    leftShift(leftKeyArray, 5);
    leftShift(rightKeyArray, 5);
    int leftShifted10[10];
    combineArrays(leftKeyArray, 5, rightKeyArray, 5, leftShifted10);
    permute(leftShifted10, P8, keyArray1, 8);

    leftShift(leftKeyArray, 5);
    leftShift(rightKeyArray, 5);
    combineArrays(leftKeyArray, 5, rightKeyArray, 5, leftShifted10);
    permute(leftShifted10, P8, keyArray2, 8);
}

void expansion(int initialArray[4], int expandedArray[8])
{
    for(int i = 0; i < 8; i++)
    {
        expandedArray[i] = initialArray[EP[i]-1];
    }
}

void feistal (int charArray[8], int keyArray[8])
{
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
    int expandedRight[8];
    expansion(rightCharArray, expandedRight);
    
}

int main()
{
    char myChar = 'j'; // Example character ('A' = 65 in ASCII)

    string key = "0x2AA";

    int keyArray1[8];
    int keyArray2[8];
    keygen(key, keyArray1, keyArray2);

    cout << "Key 1: ";
    printIntArray(keyArray1, 8);
    cout << "\nKey2: ";
    printIntArray(keyArray2, 8);
    cout << "\n";

    // Create an integer array to hold the 8 bits
    int intArray[8];

    // Convert the character to an integer array
    charToIntArray(myChar, intArray);

    int permutedArray[8];

    permute(intArray, IP, permutedArray, 8);

    // Print the integer array
    cout << "Integer array for character '" << myChar << "': ";
    printIntArray(intArray, 8);
    cout << "Permuted integer array for character '" << myChar << "': ";
    printIntArray(permutedArray, 8);

    return 0;
}
