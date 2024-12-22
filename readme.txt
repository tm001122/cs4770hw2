--==<< Usage >>==--

./encrypt <-d> <key> 
  - '-d' is optional for debugging
  - key should be entered as 0xYYY hexidecimal number

'make'
  - compiles source code into executable called 'encrypt'

'make clean'
  - removes encrypt and encrypt.o files made during compilation

Example Usage
  - ./encrypt 0x36C < "encrypt_in 1.txt" > out.txt 2> outerr.txt
  - ./encrypt -d 0x36C < "encrypt_in 1.txt" 2> outerr.txt

Notes
  - Debug <-d> output is sent to standard out rather than standard error so that it doesn't interfere with the logs but this will obviously mess up the output
  - Intermediate step logs are generated with and without debug flag and sent to standard error