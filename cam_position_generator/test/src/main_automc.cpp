
#include <cstdlib>
#include <iostream>
#include <omp.h>

#include <OpenMvgParser.h>

#include <opview/type_definition.h>
#include <opview/MCMCCamGenerator.hpp>
#include <opview/AutonomousMCMCCamGenerator.hpp>
#include <opview/utilities.hpp>

#define OMP_THREADS 8


int main(int argc, char **argv) {
    
    omp_set_num_threads(OMP_THREADS);

    std::string jsonFile = argv[1];
    std::string meshFile = argv[2];
    
    OpenMvgParser op_openmvg(jsonFile);
    op_openmvg.parse();
    std::cout << "sfm: " << op_openmvg.getSfmData().numCameras_ << " cams; " << op_openmvg.getSfmData().numPoints_ << " points" << std::endl << std::endl;

    SfMData sfm_data_ = op_openmvg.getSfmData();

    std::vector<glm::vec3> cams;
    for (auto cam : sfm_data_.camerasList_) {
        cams.push_back(cam.center);
    }

    std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<double> uncertainty;

    double x, y, z, unc;
    std::ifstream cin("points.txt");
    while(!cin.eof()) {
        cin >> x >> y >> z >> unc;
        points.push_back(glm::vec3(x, y, z));
        uncertainty.push_back(unc);
    }
    cin.close();

    cin.open("normals.txt");
    while(!cin.eof()) {
        cin >> x >> y >> z;
        normals.push_back(glm::vec3(x, y, z));
    }

    opview::CameraGeneralConfiguration camConfig(1920, 1080, 959.9965);
    // centroid, normal

    opview::MeshConfiguration meshConfig(meshFile, cams, points, normals, uncertainty);

    opview::MCConfiguration mcConfig(10, 100000, 100, 30); // size_t resamplingNum, size_t particles, size_t particlesUniform
    // maybe is better if less point in uniform? and then increase in the case of mc?

    size_t maxPoints = 10;
    long double thresholdUncertainty = 100000;
    
    // opview::MCMCCamGenerator model(camConfig, meshFile, cams, mcConfig);
    opview::AutonomousMCMCCamGenerator model(camConfig, meshConfig, mcConfig, maxPoints, thresholdUncertainty);
    
    model.estimateBestCameraPosition();

    return 0;
}
