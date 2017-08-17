
#include <cstdlib>
#include <iostream>
#include <omp.h>

#include <OpenMvgParser.h>

#include <opview/type_definition.h>
#include <opview/MCMCCamGenerator.hpp>
#include <opview/utilities.hpp>

#include <aliases.hpp>
#define TIMING

#define OMP_THREADS 8
#define ARGS 2


int main(int argc, char **argv) {
    
    omp_set_num_threads(OMP_THREADS);
    if (argc < ARGS + 1) {
        std::cout << "Usage: " << argv[0] << " mgvjson.json meshfile.off" << std::endl;
        return 1;
    }

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

    opview::CameraGeneralConfiguration camConfig(1920, 1080, 959.9965);

    glm::vec3 centroid(-0.402134, -0.0704882, 2.31928);

    glm::vec3 normal(-0.14341, 0.238821, -0.960416);
    // // centroid, normal

    opview::MCConfiguration mcConfig(10, 100000, 100, 45); // size_t resamplingNum, size_t particles, size_t particlesUniform
    // maybe is better if less point in uniform? and then increase in the case of mc?
    
    opview::MCMCCamGenerator model(camConfig, meshFile, cams, mcConfig);

#ifdef TIMING
    millis start = now();
#endif

    model.estimateBestCameraPosition(centroid, normal);

#ifdef TIMING
        std::cout << std::endl << std::endl << "Total time to compute optimal pose: " << (now()-start).count() << "ms" << std::endl;
#endif

    return 0;
}
