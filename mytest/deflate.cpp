#include <fstream>
#include <iostream>
#include <vector>

#include "libdeflate.h"
#include "zlib.h"

using namespace std;

#define LOG(x) printf("%s\n", x)
#define LOGVAL(valname, val) printf("Value `%s` = %d\n", valname, val)

int main() {
    struct libdeflate_compressor* compressor;
    struct libdeflate_decompressor* decompressor;
    ofstream fout;

    std::ifstream input("inputdata.dta", std::ios::binary);

    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

    const int originalDataSize = buffer.size();

    char str[500];
    for (int i = 0; i < 100; ++i) {
        str[i] = 'a';
        str[i + 100] = 'b';
        str[i + 200] = 'c';
        str[i + 300] = 'd';
        str[i + 400] = 'e';
    }

    // Compress
    compressor = libdeflate_alloc_compressor(6);  // compression level = 9
    std::vector<unsigned char> outdata;
    outdata.resize(1000);

    LOG("Compressing...");
    // int cmpsize = libdeflate_deflate_compress(compressor, buffer.data(), originalDataSize, outdata.data(), originalDataSize);
    int cmpsize = libdeflate_deflate_compress(compressor, str, 500, outdata.data(), 500);

    LOGVAL("cmpsize", cmpsize);

    fout.open("compressed.bin", ios::binary | ios::out);
    fout.write((char*)outdata.data(), cmpsize);
    fout.close();

    // libdeflate_free_compressor(compressor);

    // Decompress
    size_t dec_size;
    // std::vector<unsigned char> dec_outdata;
    // dec_outdata.resize(originalDataSize);

    char str_out[5001];

    decompressor = libdeflate_alloc_decompressor();
    // libdeflate_deflate_decompress(decompressor, outdata.data(), cmpsize, dec_outdata.data(), originalDataSize, &dec_size);
    libdeflate_deflate_decompress(decompressor, outdata.data(), cmpsize, str_out, 5000, &dec_size);

    fout.open("decompressed.bin", ios::binary | ios::out);
    fout.write((char*)str_out, dec_size);
    fout.close();

    libdeflate_free_decompressor(decompressor);
    LOG("App finished.");
}
