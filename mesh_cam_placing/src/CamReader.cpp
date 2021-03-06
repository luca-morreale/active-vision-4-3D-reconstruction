#include <CamReader.hpp>

namespace camplacing {

    CamReader::CamReader(std::string &camfile)
    {
        this->camfile = camfile;
        parse();
    }

    GLMVec3List CamReader::parse()
    {
        rapidjson::Document document;
        std::ifstream cin(this->camfile);
        std::string str((std::istreambuf_iterator<char>(cin)), std::istreambuf_iterator<char>());
        document.Parse(str.c_str());       

        const rapidjson::Value& camerasJson = document["cams"];
        
        this->cams.clear();
        this->rots.clear();
        this->scores.clear();

        for (rapidjson::SizeType curCam = 0; curCam < camerasJson.Size(); curCam++) {
            double x = camerasJson[curCam]["vals"][0].GetDouble();
            double y = camerasJson[curCam]["vals"][1].GetDouble();
            double z = camerasJson[curCam]["vals"][2].GetDouble();

            double pitch = camerasJson[curCam]["rots"][0].GetDouble();
            double yaw = camerasJson[curCam]["rots"][1].GetDouble();

            double score = camerasJson[curCam]["score"].GetDouble();

            this->cams.push_back(GLMVec3(x, y, z));
            this->rots.push_back(createRotationMatrix(pitch, yaw));
            this->scores.push_back(score);
        }

        cin.close();
        
        return this->cams;
    }

    GLMVec3List CamReader::getCameras()
    {
        return this->cams;
    }

    GLMVec3 CamReader::getBestCamera()
    {
        int index = bestScoreIndex();
        return this->cams[index];
    }

    GLMVec3 CamReader::getFinalCamera()
    {
        return this->cams[this->cams.size() - 1];
    }

    GLMMat3List CamReader::getRotations()
    {
        return this->rots;
    }

    GLMMat3 CamReader::getRotationOfBest()
    {
        int index = bestScoreIndex();
        return this->rots[index];
    }

    GLMMat3 CamReader::getRotationOfFinal()
    {
        return this->rots[this->rots.size() - 1];
    }

    int CamReader::bestScoreIndex()
    {
        int index = 0;
        for (int i = 0; i < this->cams.size(); i++) {
            if (this->scores[i] > this->scores[index]) {
                index = i;
            }
        }
        return index;
    }
    
    GLMMat3 CamReader::createRotationMatrix(double pitch, double yaw)
    {
        auto Rz = GLMMat3(std::cos(radians(yaw)), -std::sin(radians(yaw)), 0,
                        std::sin(radians(yaw)), std::cos(radians(yaw)), 0,
                        0, 0, 1);
        auto Ry = GLMMat3(std::cos(radians(pitch)), 0, std::sin(radians(pitch)),
                        0, 1, 0,
                        -std::sin(radians(pitch)), 0, std::cos(radians(pitch)));
        auto Rx = GLMMat3(1, 0, 0,
                        0, std::cos(radians(0)), -std::sin(radians(0)),
                        0, std::sin(radians(0)), std::cos(radians(0)));
        return (Rz * Ry) * Rx;
    }

    double CamReader::radians(double deg)
    {
        return deg * M_PI / 180.0;
    }




} // namespace camplacing
