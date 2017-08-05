#ifndef MESH_ACCURACY_COMPUTER_VISION_ACCURACY_MODEL_H
#define MESH_ACCURACY_COMPUTER_VISION_ACCURACY_MODEL_H

#include <realtimeMR/SfMData.h>

#include <meshac/alias_definition.hpp>
#include <meshac/PhotogrammetristAccuracyModel.hpp>

namespace meshac {
    
    class ComputerVisionAccuracyModel : public PhotogrammetristAccuracyModel {
    public:
        ComputerVisionAccuracyModel(SfMData &data, DoublePair &pixelSize);
        ComputerVisionAccuracyModel(SfMData &data, std::string &pathPrefix, DoublePair &pixelSize);
        
        virtual ~ComputerVisionAccuracyModel() { };

    protected:
        /*
         * Evaluates the function using homogeneous coordinates.
         */
        virtual EigVector evaluateFunctionIn(CameraMatrix &cam, GLMVec2 &point);

        virtual EigMatrix computeJacobian(CrossRatioTuple &tuple, CameraMatrix &cam);

        virtual void iterativeEstimationOfCovariance(EigMatrixList &destList, EigMatrixList &pointMatrixList, EigMatrix &jacobian);
        
        virtual void updateVariancesList(DoubleList &varianesList, EigMatrix &varianceMat, EigMatrixList &jacobianList, EigMatrix &jacobianMat);

        virtual EigMatrix getAccuracyForPointInImage(CamPointPair &cameraObsPair) override;
        virtual EigMatrix replicateVarianceForTuple(EigMatrix &singleVariance) override;

    private:
        typedef PhotogrammetristAccuracyModel super;
    };


} // namespace meshac


#endif // MESH_ACCURACY_COMPUTER_VISION_ACCURACY_MODEL_H
