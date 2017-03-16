
#ifndef MESH_ACCURACY_CR_TUPLES_GENERATOR_H
#define MESH_ACCURACY_CR_TUPLES_GENERATOR_H

#include <manifoldReconstructor/SfMData.h>

#include <CrossRatioTuple.hpp>
#include <alias_definition.hpp>
#include <meshac_type_definition.hpp>
#include <utilities.hpp>

namespace meshac {

    class CRTuplesGenerator {
    public:
        CRTuplesGenerator(GLMListArrayVec2 camObservations, int obsWidth, int obsHeight);
        ~CRTuplesGenerator();


        /*
         * Getter and setter for Cameras' Observations.
         */
        GLMListArrayVec2 getCamObservations();
        void setCamObservations(GLMListArrayVec2 camObservations);
        void setCamObservations(GLMListVec2 list, int camIndex);
        void updateCamObservations(GLMListVec2 list, int camIndex);


        /*
         * Getter and setter for Size of Camera's Observations.
         */
        void setObsSize(int obsWidth, int obsHeight);
        std::pair<int,int> getObsSize();



        /*
         * Getter for already computed CrossRatioTupleSet.
         */
        CrossRatioTupleSet getComputedTuples();
        CrossRatioTupleSet getComputedTuplesForCam(int camIndex);
        ListCrossRatioTupleSet getCrossRatioTupleSetList();


        /*
         * Extracts quadruplets of collinear points for each image.
         */
        virtual CrossRatioTupleSet determineTupleOfFourPoints(GLMListArrayVec2 camObservations, int obsWidth, int obsHeight);
        virtual CrossRatioTupleSet determineTupleOfFourPoints();

        /*
         * Extracts quadruplets of collinear points for the image obtained by the given camera.
         */
        virtual CrossRatioTupleSet determineTupleOfFourPointsForCam(GLMListArrayVec2 camObservations, int camIndex, int obsWidth, int obsHeight);
        virtual CrossRatioTupleSet determineTupleOfFourPointsForCam(int camIndex);
        virtual ListCrossRatioTupleSet determineTupleOfFourPointsForAllCam();


        
    protected:
        void setTuples(CrossRatioTupleSet tupleSet);
        void setTuplesPerCam(ListCrossRatioTupleSet tupleSetPerCam);
        void setTuplesPerCam(CrossRatioTupleSet tupleSet, int camIndex);

    private:

        CrossRatioTupleSet createsTuples(IntArrayList &combos, IntList &pointSet, GLMListVec2 &points2D);
        CVListVec2 createLinesFromPoints(int imgHeight, int imgWidth, GLMListVec2 &points2D);
        IntArrayList generateCorrespondances(CVListVec2 &lines, GLMListVec2 &points2D);
        void createFeatureImage(CVMat &logicalImg, GLMListVec2 &points2D);

        CrossRatioTupleSet collapseListSet(ListCrossRatioTupleSet &tupleSetPerCam);


        GLMListArrayVec2 camObservations;
        int obsHeight, obsWidth;

        ListCrossRatioTupleSet tupleSetPerCam;
        CrossRatioTupleSet tupleSet;

    };

    typedef CRTuplesGenerator * CRTuplesGeneratorPtr;
    

} // namespace meshac

#endif // MESH_ACCURACY_CR_TUPLES_GENERATOR_H
