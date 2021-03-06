#include <utilities.hpp>

namespace cameval {

    std::chrono::milliseconds now()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    }

    void log(std::string msg)
    {
        std::cout << std::endl << msg << std::endl;
    }

    // trim from start
    std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    // trim from end
    std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(),
                std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    // trim from both ends
    std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
    }

    std::string readStringFromFile(std::string &filename)
    {
        std::ifstream cin(filename);
        std::string content((std::istreambuf_iterator<char>(cin)), std::istreambuf_iterator<char>());
        cin.close();
        return content;
    }

    std::string concatBlocks(StringList &blocks)
    {
        std::string out;
        for (std::string el : blocks) {
            out += el + "_";
        }
        return out.substr(0, out.find_last_of("_"));
    }

    std::string extractFilename(std::string &entry)
    {
        std::string file;
        size_t charPos = entry.find_last_of("/");
        if (charPos < entry.size()) {
            file = entry.substr(charPos + 1, entry.size());
        }

        charPos = file.find_last_of(".");
        if (charPos < file.size()) {
            file = file.substr(0, charPos);
        }

        return file;
    }

    std::string removePartsFromEntry(std::string &entry)
    {
        std::string file = extractFilename(entry);

        size_t charPos = file.find_first_of("_");
        if (charPos < file.size()) {
            file = file.substr(charPos + 1, file.size());
        }

        return entry;
    }

    StringList divideStringPose(std::string &stringPose)
    {
        StringList blocks;
        boost::split(blocks, stringPose, boost::is_any_of("_"));

        return StringList(blocks.end() - 6, blocks.end());
    }

    AnglePose parseEntry(std::string &entry)
    {
        AnglePose pose;
        std::string stringPose = removePartsFromEntry(entry);
        StringList blocks = divideStringPose(stringPose);

        for (int i = 0; i < 3; i++) {
            pose.first[i] = std::strtod(blocks[i].c_str(), NULL);
        }
        for (int i = 0; i < 3; i++) {
            pose.second[i] = std::strtod(blocks[i + 3].c_str(), NULL);
        }
        
        return pose;
    }

    std::string getPoseString(std::string &entry)
    {
        std::string stringPose = removePartsFromEntry(entry);
        StringList blocks = divideStringPose(stringPose);
        return concatBlocks(blocks);
    }

    int countBlocks(std::string &str)
    {
        StringList blocks;
        boost::split(blocks, str, boost::is_any_of("_"));
        return blocks.size();
    }

    void eraseFromVector(IntList &removeIndex, GLMVec3List &list)
    {
        for (auto it = removeIndex.rbegin(); it != removeIndex.rend(); it++) {
            list.erase(list.begin() + *it);
        }
    }


    GLMVec3 convert(EigVector3 &arr)
    {
        return GLMVec3(arr[0], arr[1], arr[2]);
    }

    GLMMat3 convert(EigMatrix3 &mat)
    {
        return GLMMat3(mat(0, 0), mat(0, 1), mat(0, 2), mat(1, 0), mat(1, 1), mat(1, 2), mat(2, 0), mat(2, 1), mat(2, 2));
    }

    Float6Array convert(StringList &list)
    {
        Float6Array arr;
        for (int i = 0; i < arr.size(); i++) {
            arr[i] = strtod(list[i].c_str(), NULL);
        }
        return arr;
    }

    float rad(float deg)
    {
        return deg * M_PI / 180.0f;
    }

    float deg(float rad)
    {
        return rad * 180.0f / M_PI;
    }

    GLMMat3 rotationMatrix(float yaw, float pitch, float roll)
    {
        return rotationYaw(yaw) * glm::transpose(rotationPitch(pitch)) * rotationRoll(roll);
    }

    GLMMat3 rotationMatrix(GLMVec3 &angles)
    {
        return rotationYaw(angles.z) * glm::transpose(rotationPitch(angles.y)) * rotationRoll(angles.x);
    }

    GLMMat3 rotationRoll(float roll)
    {
        return GLMMat3(1.0f, 0.0f, 0.0f,
                0.0f, std::cos(roll), -std::sin(roll),
                0.0f, std::sin(roll), std::cos(roll));
    }

    GLMMat3 rotationPitch(float pitch)
    {
        return GLMMat3(std::cos(pitch), 0.0f, std::sin(pitch),
                        0.0f, 1.0f, 0.0f,
                        -std::sin(pitch), 0.0f, std::cos(pitch));
    }

    GLMMat3 rotationYaw(float yaw)
    {
        return GLMMat3(std::cos(yaw), -std::sin(yaw), 0.0f,
                        std::sin(yaw), std::cos(yaw), 0.0f,
                        0.0f, 0.0f, 1.0f);
    }

    Camera getCamera(StringList blocks)
    {
        GLMMat4 R = getRotation(blocks);
        GLMMat4 K = getIntrinsic(blocks);
        GLMVec4 t = getTranslation(blocks);

        return Camera(R, K, t);
    }

    GLMMat4 getRotation(StringList blocks)
    {
        GLMMat4 R;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                R[i][j] = strtod(blocks[10 + i * 3 + j].c_str(), NULL);     // check indices
            }
        }

        return R;
    }

    GLMMat4 getIntrinsic(StringList blocks)
    {
        GLMMat4 K;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                K[i][j] = strtod(blocks[1 + i * 3 + j].c_str(), NULL);     // check indices
            }
        }

        return K;
    }

    GLMVec4 getTranslation(StringList blocks)
    {
        GLMVec4 t;
        t.x = strtod(blocks[19].c_str(), NULL);
        t.y = strtod(blocks[20].c_str(), NULL);
        t.z = strtod(blocks[21].c_str(), NULL);
        return t;
    }


} // namespace cameval
