#ifndef MESH_ACCURACY_WORST_TRACE_VARIANCE_ESTIMATOR_H
#define MESH_ACCURACY_WORST_TRACE_VARIANCE_ESTIMATOR_H

#include <meshac/alias_definition.hpp>
#include <meshac/Point3DVarianceEstimator.hpp>


namespace meshac {


    class WorstTraceVarianceEstimator : public Point3DVarianceEstimator {
    public:
        WorstTraceVarianceEstimator(PointAccuracyModelPtr accuracyModel, GLMVec3List &points);
        virtual ~WorstTraceVarianceEstimator();

    protected:
        virtual EigMatrix selectVarianceMatrix(EigMatrixList &mat); 
        /*
         * Computes the variance as the maximum trace of the matrix.
         */
        double computeVarianceFromMatrix(EigMatrix &varianceMatrix);

    };

} // namespace meshac

#endif // MESH_ACCURACY_WORST_VARIANCE_ESTIMATOR_H
