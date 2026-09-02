# Image Steganography Using LSB Mechanism

## 📌 Project Overview

Image Steganography is a technique used to hide secret information inside an image without visibly changing the image.

This project implements **Image Steganography using the Least Significant Bit (LSB) technique** in the C programming language.

The project supports:
- Encoding secret data into a BMP image.
- Hiding a secret file inside the image.
- Decoding and extracting the hidden data from the stego image.

## 🎯 Objectives

- To understand the concept of image steganography.
- To implement the LSB technique using C.
- To securely hide data inside an image.
- To extract the hidden data from the encoded image.
- To understand file handling and bit-level operations in C.

## 🛠️ Technologies Used

- **Programming Language:** C
- **Concepts:** 
  - File Handling
  - Pointers
  - Structures
  - Bit Manipulation
  - Command Line Arguments
  - LSB Steganography
- **Operating System:** Linux / Ubuntu
- **Compiler:** GCC

## ⚙️ How It Works

### Encoding

During encoding, the secret information is hidden inside the image by modifying the **Least Significant Bits (LSB)** of the image's pixel data.

The basic process is:

1. Read the source BMP image.
2. Read the secret file.
3. Check whether the image has enough capacity.
4. Copy the BMP header.
5. Encode a magic string to identify the stego file.
6. Encode the secret file extension.
7. Encode the secret file size.
8. Encode the secret file data into the image.
9. Generate the final stego image.

### Decoding

During decoding:

1. Read the stego image.
2. Verify the magic string.
3. Extract the secret file extension.
4. Extract the secret file size.
5. Decode the hidden data from the image.
6. Recreate the original secret file.

## 📂 Project Structure

```text
Image-Steganography-using-LSB-mechanism/
│
├── encode.c
├── encode.h
├── decode.c
├── decode.h
├── types.h
├── test_encode.c
├── test_decode.c
├── common.h
├── Makefile
└── README.md
File names may vary depending on the project implementation.
▶️ Compilation
Compile the project using GCC:
gcc *.c
Or, if a Makefile is provided:
make
🚀 Usage
Encoding
./a.out -e source.bmp secret.txt output.bmp
Example:
./a.out -e beautiful.bmp secret.txt stego.bmp
Decoding
./a.out -d stego.bmp
Example:
./a.out -d stego.bmp
📸 Example
Original Image
      +
 Secret File
      ↓
   Encoding
      ↓
Stego Image
      ↓
   Decoding
      ↓
Secret File
🔑 Key Concept – LSB
LSB stands for Least Significant Bit.
For example, if a pixel contains:
10110110
The last bit is the LSB:
1011011[0]
To hide one bit of secret data, the LSB can be changed:
10110110 → 10110111
This small change is generally not noticeable to the human eye.
💡 Key Features
Hides secret data inside BMP images.
Uses the LSB steganography technique.
Supports encoding and decoding.
Uses bit-level data manipulation.
Uses command-line arguments.
Performs image capacity checking.
Preserves the visual appearance of the image.
🧠 Key Challenges
Understanding how image pixel data is stored.
Implementing bit-level encoding and decoding.
Managing file pointers and binary files.
Checking whether the source image has enough capacity.
Extracting the hidden data correctly during decoding.
📚 Learning Outcomes
Improved understanding of C programming.
Learned binary file handling.
Learned bitwise operations.
Understood LSB-based steganography.
Gained experience with structures and pointers.
Learned how to work with command-line arguments.
Improved debugging and problem-solving skills.
👨‍💻 Author
Raj Chilshetty
Project
Image Steganography Using LSB Mechanism
