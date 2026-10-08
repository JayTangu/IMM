#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <filesystem>

#pragma pack(push, 1) // align to every 1-byte 
struct WavHeader {
    //RIFF block (12 bytes)
    char riff[4] = {'R','I','F','F'};
    uint32_t fileSize_Minus8;
    char wave[4] = {'W','A','V','E'};

    //fmt block (24 bytes)
    char fmt[4] = {'f','m','t',' '};
    uint32_t fmtSize = 16;
    uint16_t audioFormat = 1;
    uint16_t numChannels = 1;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign = 2;
    uint16_t bitsPerSample = 16; 

    //data block(8 bytes)
    char data[4] = {'d','a','t','a'};
    uint32_t dataSize;
};
#pragma pack(pop)

const double PI = 3.14159265358979323846;

void generateWav(const std::string& filename, uint32_t headerSampleRate, const std::vector<int16_t>& samples) {
    WavHeader header;
    header.sampleRate = headerSampleRate;
    header.dataSize = static_cast<uint32_t>(samples.size() * sizeof(int16_t)); //samples.size -> 64-bit, static_cast告訴編譯器直接轉型太沒差
    header.byteRate = header.sampleRate* header.blockAlign;
    header.fileSize_Minus8 = 36 + header.dataSize;

    std::filesystem::path fullPath = std::filesystem::path(__FILE__).parent_path() / filename;

    std::ofstream outputfile(fullPath, std::ios::binary);
    if(!outputfile) {
        std::cerr<<"檔案建立失敗: "<<filename<<std::endl;
        return;
    }
    outputfile.write(reinterpret_cast<const char*>(&header),sizeof(header));
    outputfile.write(reinterpret_cast<const char*>(samples.data()), header.dataSize);
    outputfile.close();
    std::cout<<"成功輸出: "<<filename<<std::endl;

}

int main() {
    double k; //Hz
    uint32_t s; //smaple rate
    double r; //sec

    std::cout<<"請輸入聲音頻率k(Hz)、取樣頻率s、秒數r : ";
    std::cin>>k>>s>>r;

    uint32_t totalSamples1 = static_cast<uint32_t>(s*r);
    std::vector<int16_t> samples1(totalSamples1);
    for(uint32_t i = 0; i < totalSamples1; i++) {
        double wave_val = std::cos(2.0 * PI * k * i / s);
        samples1[i] = static_cast<int16_t>(wave_val * 32767); //因為我給16 bits 把cos值拉長存入
    }
    generateWav("Mywav1.wav",s,samples1);

    return 0;
}