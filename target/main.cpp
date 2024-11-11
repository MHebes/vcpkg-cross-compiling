#include <iostream>
#include <fstream>

int main()
{
    std::ifstream ifs("cube.glb"); // installed alongside exe
    std::string contents((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    std::cout << contents.length() << "\n";
    return 0;
}