
#include <manifoldReconstructor/OpenMvgParser.h>
#include <manifoldReconstructor/SfMData.h>
#include <manifoldReconstructor/types_reconstructor.hpp>

#include <opencv/cv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>


#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include <CrossRatioTuple.hpp>
#include <CRTuplesGenerator.hpp>



int main(int argc, char **argv) {
  
    OpenMvgParser op(argv[1]);
    op.parse();
    
    SfMData points = op.getSfmData();

    meshac::CRTuplesGenerator crGenerator = meshac::CRTuplesGenerator(&points);

    meshac::ListCrossRatioTupleSet listCRSet = crGenerator.determineTupleOfFourPoints();

    //for (auto tupleSet : listCRSet) {
    //    for (auto point : tupleSet) {
    //        point.crossRatio();
    //    }
    //}

    return 0;
}



