#ifndef CAM_POSITION_GENERATOR_ANGLE_DISTANCE_VIEW_ESTIMATOR_H
#define CAM_POSITION_GENERATOR_ANGLE_DISTANCE_VIEW_ESTIMATOR_H

#include <cmath>

#include <opview/alias_definition.h>
#include <opview/DistanceCameraViewEstimator.hpp>
#include <opview/CplexVectorOperations.hpp>
#include <opview/utilities.hpp>

namespace opview {

    #define MIN_DISTANCE 100.0

    class AngleDistanceViewEstimator : public DistanceCameraViewEstimator {
    public:
        AngleDistanceViewEstimator(GLMVec3List &cams);
        ~AngleDistanceViewEstimator();

    protected:
        virtual void addGoal(IloModel model, CGALVec3 &normVector, CGALVec3 &centroid);
        virtual IloExpr createGoalExpression(CGALVec3 &targetDirection, CGALVec3 &centroid, IloEnv env);
    };

    typedef AngleDistanceViewEstimator* AngleDistanceViewEstimatorPtr;

} // namespace opview

#endif // CAM_POSITION_GENERATOR_ANGLE_DISTANCE_VIEW_ESTIMATOR_H