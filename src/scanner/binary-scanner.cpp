#include <vector>
#include <iostream>
#include <fstream>

void scanBinary(const std::string &filename, const std::vector<unsigned char> &pattern){
    std::ifstream file(filename, std::ios::binary);
    if(!file){
        std::cerr<<"Error: Couldn't open the file."<<std::endl;
    }

    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    

    size_t pattern_size = pattern.size();
    size_t count = 0;
    size_t totalSets= 0;

    for(size_t i=0; i<= buffer.size() - pattern.size();){
        size_t start_addr;
        bool found = true;
        

        for (size_t j =0; j < pattern.size(); ++j){
            if(buffer[i+j] != pattern[j]){
                found= false;
                break;
            }
        }
        if(found){
            i += pattern_size;
            //std::cout<<"Pattern found at address: " << std::hex << (i) <<std::dec << std::endl;
            ++count;

        }else{
            if(count > 10){
                std::cout  << "Found " << count << " consecutive sets of the pattern at offset: 0x" << std::hex << ((i - count * pattern_size)) << std::dec << std::endl;
                totalSets += count;
            }
            count = 0;
            ++i;
        }
        

    }


}

int main(){
    std::string filename = "hello.exe";
    std::vector<unsigned char> pattern = {0x00};
    scanBinary(filename, pattern);
    system("pause");
    return 0;
}