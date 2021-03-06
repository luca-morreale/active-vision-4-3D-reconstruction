#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

#include <utilities.hpp>
#include <aliases.h>
#include <SystemEvaluator.hpp>

#define ARGS 5


int main(int argc, char **argv) {

    if (argc < ARGS + 1) {
        std::cout << "Usage: " << std::string(argv[0]) << " steps accuracyExe optimalExe basicPovfile.pov output_report.txt" << std::endl;
        return 1;
    }

    int steps = atoi(argv[1]);
    std::string accuracyExe = "./" + std::string(argv[2]);
    std::string optimalCamExe = "./" + std::string(argv[3]);
    std::string basicPovFile = argv[4];
    std::string outputEvaluation = argv[5]; // "output/output_evaluation_emulate.txt";

    std::string intrinsicParams;
    std::ifstream cin("K.txt");
    cin >> intrinsicParams;
    cin.close();

    std::string groundTruthFilename = "pcl_gt.asc";
    std::string basicPosesFilename = "poses_cam.txt";
    std::string baseImageFolder = "images";
    // std::string intrinsicParams = "959.9965;0;960;0;-959.9965;540;0;0;1";
    // std::string intrinsicParams = "1662.8;0;960;0;-1662.8;540;0;0;1"; // car

    std::ofstream out("fake_poses.txt");
    out << "0 0 0 0 0" << std::endl;
    out.close();

    std::string reconstructionExe = "./manifoldReconstructor ";
    
    cameval::SystemEvaluator eval(groundTruthFilename, basicPosesFilename, baseImageFolder, intrinsicParams, outputEvaluation, basicPovFile);
    eval.systemEvaluation(steps, reconstructionExe, accuracyExe, optimalCamExe);

    return 0;
}
