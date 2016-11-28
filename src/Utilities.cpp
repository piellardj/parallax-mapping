#include "Utilities.hpp"

#include <stdexcept>
#include <fstream>
#include <sstream>

#include <ctime>


static int randomInt ()
{
    static bool firstTime = true;

    if (firstTime) {
        firstTime = false;
        srand(time(NULL));
    }

    return rand();
}

float randFloat (float minValue, float maxValue)
{
    float range = maxValue - minValue;
    float randomValue = static_cast<float>(randomInt()) / RAND_MAX; //normalized

    return minValue + randomValue * range;
}

glm::vec4 randomColor ()
{
    int dominant = randomInt() % 3;
    int missing = randomInt() % 2;
    float varying = randFloat(0.f, 1.f);

    if (dominant == 0) {
        if (missing == 0)
            return glm::vec4(1.f, 0.f, varying, 1.f);
        else
            return glm::vec4(1.f, varying, 0.f, 1.f);
    } else if (dominant == 1) {
        if (missing == 0)
            return glm::vec4(0.f, 1.f, varying, 1.f);
        else
            return glm::vec4(varying, 1.f, 0.f, 1.f);
    } else {
        if (missing == 0)
            return glm::vec4(0.f, varying, 1.f, 1.f);
        else
            return glm::vec4(varying, 0.f, 1.f, 1.f);
    }
}

//void loadFile(std::string const& filePath,
//              std::string& container)
//{
//    std::ifstream t(filePath);
//    if (!t.is_open())
//        throw std::runtime_error("unable to open file " + filePath + ".");
//
//    std::stringstream buffer;
//    buffer << t.rdbuf();
//    container = buffer.str();
//}
//
//void searchAndReplace (std::string const& tobeReplaced,
//                       std::string const& replacement,
//                       std::string& container)
//{
//    std::string::size_type pos = 0u;
//    while((pos = container.find(tobeReplaced, pos)) != std::string::npos){
//        container.replace(pos, tobeReplaced.length(), replacement);
//        pos += replacement.length();
//    }
//}
