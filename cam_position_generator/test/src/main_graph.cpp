
#include <realtimeMR/CameraPointsCollection.h>
#include <realtimeMR/ConfigParser.h>
#include <realtimeMR/ReconstructFromSfMData.h>
#include <realtimeMR/ReconstructFromSLAMData.h>
#include <realtimeMR/types_config.hpp>
#include <realtimeMR/types_reconstructor.hpp>
#include <realtimeMR/utilities/Chronometer.h>
#include <realtimeMR/utilities/Logger.h>

#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <omp.h>

#include <OpenMvgParser.h>

#include <glm/gtx/string_cast.hpp>

#include <opview/SolverGenerator.hpp>
#include <opview/ICMSolverGenerator.hpp>
#include <opview/FlipperSolverGenerator.hpp>
#include <opview/LOCSolverGenerator.hpp>
#include <opview/BruteForceSolverGenerator.hpp>
#include <opview/BasicGraphicalModel.hpp>
#include <opview/HierarchicalDiscreteGraphicalModel.hpp>
#include <opview/OrientationHierarchicalGraphicalModel.hpp>
#include <opview/type_definition.h>
#include <opview/DimensionDiagreementLists.hpp>
#include <opview/MultipointHierarchicalGraphicalModel.hpp>


#define OMP_THREADS 8
#define DEPTH 10
#define DISCRETE_LABELS 10


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

    // opview::SolverGeneratorPtr solver = new opview::FlipperSolverGenerator();
    // opview::SolverGeneratorPtr solver = new opview::ICMSolverGenerator();
    // opview::SolverGeneratorPtr solver = new opview::LOCSolverGenerator();
    opview::SolverGeneratorPtr solver = new opview::BruteForceSolverGenerator();

    opview::OrientationHierarchicalConfiguration config(DEPTH, DISCRETE_LABELS, 10);
    opview::CameraGeneralConfiguration camConfig(1920, 1080);

    // opview::BasicGraphicalModel model(solver, cams);
    // opview::HierarchicalDiscreteGraphicalModel model(solver, DEPTH, DISCRETE_LABELS, cams);
    // opview::OrientationHierarchicalGraphicalModel model(solver, config, meshFile, cams);
    opview::MultipointHierarchicalGraphicalModel model(solver, config, camConfig, meshFile, cams);

    // v1 = -0.443026 -0.075465 2.31818
    // n1 = -0.0383628, 0.272127, -0.961496
    // v2 = -0.40168 -0.0697156 2.3197
    // n2 = -0.14341, 0.238821, -0.960416
    // v3 = -0.405273 -0.0428495 2.34096
    // n3 = -0.215338, 0.405714, -0.888271
    // auto centroid = glm::vec3(-0.402134, -0.0704882, 2.31928);
    // auto normVector = glm::vec3(-0.14341, 0.238821, -0.960416);

    std::vector<glm::vec3> centroids = {glm::vec3(-0.402134, -0.0704882, 2.31928), 
                    glm::vec3(-0.443026, -0.075465, 2.31818), 
                    glm::vec3(-0.40168, -0.0697156, 2.3197), 
                    glm::vec3(-0.405273, -0.0428495, 2.34096)
                    };

    std::vector<glm::vec3> normals = {glm::vec3(-0.14341, 0.238821, -0.960416), 
                    glm::vec3(-0.0383628, 0.272127, -0.961496), 
                    glm::vec3(-0.14341, 0.238821, -0.960416), 
                    glm::vec3(-0.215338, 0.405714, -0.888271)
                    };

    // centroid, normal
    model.estimateBestCameraPosition(centroids, normals);


    return 0;
}