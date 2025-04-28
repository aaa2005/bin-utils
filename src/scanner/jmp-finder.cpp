#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>

uint32_t calcJmpTarget(uint32_t current_Addr, int32_t offset){
    return current_Addr + 5 + offset;
}



void findJMP(const std::string &filename){
    std::ifstream file(filename, std::ios::binary);
    if(!file){
        std::cerr << "Error: Could not open the file." << std::endl;
    }
    
    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    for(size_t i = 0; i < buffer.size() - 5; ++i){
        if(buffer[i] == 0xE9){
            int32_t offset = *reinterpret_cast<int32_t*>(&buffer[i + 1]);
            uint32_t currentAddress = static_cast<uint32_t>(i);
            uint32_t targetAddress = calcJmpTarget(currentAddress, offset);

            std::cout << "JMP found at address: 0x" << std::hex << currentAddress << " -> Target address: 0x" << targetAddress << std::dec << std::endl;

        }
    }
}


void modifyJMP(const std::string& filePath, const std::vector<std::pair<std::streampos, uint8_t>>& modifications){
    std::fstream file(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if(!file.is_open()){
        std::cerr << "Error: Unable to open file" << filePath <<std::endl;  
    return;
    }

    for(const auto& modification : modifications){
        std::streampos address = modification.first;
        std::cout << "Address: " << address <<std::endl;
        uint8_t newValue = modification.second;

        file.seekp(address);
        file.write(reinterpret_cast<char*>(&newValue), sizeof(newValue));

        if(!file){
            std::cerr << "Error: Failed to write to the address " << address << std::endl;
        }
    }

    file.close();
    std::cout << "Binary file modifiesd successfully!" << std::endl;
}


int main(){
    std::string filename;
    std::cout << "Enter the path to the birary file: ";
    std::cin >> filename;

    findJMP(filename);

    std::vector<std::pair<std::streampos, uint8_t>> modifications = {
        {0x90a, 0xE9},   // Modify the byte at address 10 to 0xFF
        {20, 0xAB},   // Modify the byte at address 20 to 0xAB
        {50, 0x00}    // Modify the byte at address 50 to 0x00
    };

    modifyJMP(filename, modifications);

    int a;
    std::cin >> a;
    return 0;
}