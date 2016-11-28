#ifndef UTILITIES_HPP_INCLUDED
#define UTILITIES_HPP_INCLUDED

#include <sstream>
#include <string>
#include <algorithm>
#include <cstdlib>

#include "glm.hpp"


///* Loads the file to container.
// * Throws an exception if a problem occured. */
//void loadFile(std::string const& filePath,
//              std::string& container);
//
///* Replaces every instance of toBeReplaced by replacement in container. */
//void searchAndReplace (std::string const& tobeReplaced,
//                       std::string const& replacement,
//                       std::string& container);

float randFloat (float minValue=0.f, float maxValue=1.f);

glm::vec4 randomColor ();

template<typename T>
inline T clamp (T const& lowest, T const& heighest, T const& value)
{
    return std::min(heighest, std::max(lowest, value));
}

template<typename T>
std::string toString (T const& object)
{
    std::stringstream s;
    s << object;
    return s.str();
}
#endif // UTILITIES_HPP_INCLUDED
