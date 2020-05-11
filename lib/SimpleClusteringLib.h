// Copyright 2020 Artyom Muntyanu. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the
// License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific language governing permissions
// and limitations under the License.

#ifndef SimpleClusteringLibH
#define SimpleClusteringLibH

#include "SimpleClustering.h"

#include <map>

#include <cstdint>

namespace NS_Clustering
{
    // Cluster object identifiers
    extern "C" __declspec(dllexport) const uint16_t  ID_DOT_;
    extern "C" __declspec(dllexport) const uint16_t  ID_CLUSTER_1_;
    extern "C" __declspec(dllexport) const uint16_t  ID_CLUSTER_2_;
    extern "C" __declspec(dllexport) const uint16_t  ID_CLUSTER_3_;
    extern "C" __declspec(dllexport) const uint16_t  ID_CLUSTER_4_;

    extern "C" __declspec(dllexport) const double  Dot_Radius_;
    extern "C" __declspec(dllexport) const double  Cluster_1_Radius_;
    extern "C" __declspec(dllexport) const double  Cluster_2_Radius_;
    extern "C" __declspec(dllexport) const double  Cluster_3_Radius_;
    extern "C" __declspec(dllexport) const double  Cluster_4_Radius_;

    /* An additional part of the radius - is added to the radius of the clustering object when checking the possibility of merging.
    It may be necessary when graphically displaying clustering elements so that it does not seem that the center of any element
    located on the edge of another element. */
    extern "C" __declspec(dllexport) const double AdditionalRadiusValue_;

    // Whether to use the additional part of the radius when checking the possibility of merging clustering objects
    extern "C" __declspec(dllexport) const int16_t Use_AdditionalRadiusValue_;


    class TClusterizator
    {
    public:
        uint32_t Id;

        TMapDot        MapDot;        // dots
        TMapCluster_1  MapCluster_1;  // clusters_1
        TMapCluster_2  MapCluster_2;  // clusters_2
        TMapCluster_3  MapCluster_3;  // clusters_3
        TMapCluster_4  MapCluster_4;  // clusters_4


        TClusterizator(void) noexcept;
        explicit TClusterizator(const uint32_t Id_) noexcept;
        ~TClusterizator(void) noexcept;

        // Clear all data
        void Clear(void) noexcept;

        // Get the amount of data
        std::size_t Get_Count(const uint16_t ClusteringElemId, bool All) const noexcept;
    };

    using TMapClusterizator = std::map<uint32_t, TClusterizator>; // clusterizators

    extern TMapClusterizator MapClusterizator; // clusterizators


    // Reset radius of all clustering elements to default values
    extern "C" __declspec(dllexport) void ClusteringElems_Set_Default_Radius(void) noexcept;

    // Set the radius of all clustering elements
    extern "C" __declspec(dllexport) int32_t ClusteringElems_Set_Radius(const double Dot_R, const double Cluster_1_R, const double Cluster_2_R, const double Cluster_3_R, const double Cluster_4_R) noexcept;

    // Reset settings of the additional radius value to default
    extern "C" __declspec(dllexport) void ClusteringElems_Set_Default_AdditionalRadiusValue_Settings(void) noexcept;

    // Set settings of the additional radius value
    extern "C" __declspec(dllexport) void ClusteringElems_Set_AdditionalRadiusValue_Settings(const double Value, const int16_t Usage) noexcept;


    // Create a new clusterizator. Its code is returned
    extern "C" __declspec(dllexport) uint32_t Clusterizator_New(void) noexcept;

    // Delete clusterizator
    extern "C" __declspec(dllexport) int32_t Clusterizator_Delete(const uint32_t ClusterizatorId) noexcept;

    // Delete all clusterizators
    extern "C" __declspec(dllexport) void Clusterizator_Delete_All(void) noexcept;


    // Initialize a set of dots
    extern "C" __declspec(dllexport) int32_t Clusterizator_Init_Dots(const uint32_t ClusterizatorId, const uint32_t ArrSize, const double* X_Arr, const double* Y_Arr, const uint64_t** Address_Of_Data_Arr);

    // Add single dot
    extern "C" __declspec(dllexport) int32_t Clusterizator_Add_Dot(const uint32_t ClusterizatorId, const double X, const double Y, const uint64_t* Address_Of_Data, uint64_t*& Out_DotTagAddress);

    // Create set of clusters_1
    extern "C" __declspec(dllexport) int32_t Clusterizator_Create_Clusters_1(const uint32_t ClusterizatorId, const int16_t SingleDot_To_Cluster_1) noexcept;

    // Create set of clusters_2
    extern "C" __declspec(dllexport) int32_t Clusterizator_Create_Clusters_2(const uint32_t ClusterizatorId, const int16_t SingleClusteringElem_To_Cluster_2) noexcept;

    // Create set of clusters_3
    extern "C" __declspec(dllexport) int32_t Clusterizator_Create_Clusters_3(const uint32_t ClusterizatorId, const int16_t SingleClusteringElem_To_Cluster_3) noexcept;

    // Create set of clusters_4
    extern "C" __declspec(dllexport) int32_t Clusterizator_Create_Clusters_4(const uint32_t ClusterizatorId, const int16_t SingleClusteringElem_To_Cluster_4) noexcept;


    // Clear all sets of clusters
    extern "C" __declspec(dllexport) int32_t Clusterizator_Clear_Clusters(const uint32_t ClusterizatorId) noexcept;

    // Clear clusterizator
    extern "C" __declspec(dllexport) int32_t Clusterizator_Clear(const uint32_t ClusterizatorId) noexcept;


    // Get the number of clustering elements of the specified id
    extern "C" __declspec(dllexport) uint64_t Clusterizator_Get_ClusteringElem_Count(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId) noexcept;

    /*** Get the amount of data for clustering element of the specified id ***/
    template <typename Type> std::size_t Clusterizator_Get_ClusteringElem_Data_Count(const std::map<TCoordKey, Type*>& MapClusteringElem, const double X, const double Y) noexcept
    {
        const auto it = MapClusteringElem.find(TCoordKey(X, Y)); // find clustering element in the set

        if (it != MapClusteringElem.end()) // found
            return it->second->Get_Count();
        else // not found
            return 0;
    }

    extern "C" __declspec(dllexport) uint64_t Clusterizator_Get_ClusteringElem_Data_Count(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, const double X, const double Y) noexcept;
    /*************************************************************************/

    // Get the amount of data for all clustering elements of the specified id
    extern "C" __declspec(dllexport) uint64_t Clusterizator_Get_All_ClusteringElem_Data_Count(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId) noexcept;

    // Get the amount of data for all clustering elements of clusterizator
    extern "C" __declspec(dllexport) uint64_t Clusterizator_Get_Data_Count(const uint32_t ClusterizatorId) noexcept;


    /*** Get a set of specified clustering elements ***/
    template <typename Type> void Clusterizator_Get_ClusteringElems(
        const std::map<TCoordKey, Type*>&  MapClusteringElem,
        double* const                      X_Arr,
        double* const                      Y_Arr,
        int16_t* const * const             Address_Of_InCluster_Arr,
        uint64_t* const * const            Address_Of_DataCount_Arr,
        uint64_t* const * const            Address_Of_TagAddress_Arr) noexcept
    {
        uint64_t i = 0;

        for (const auto& ce : MapClusteringElem)
        {
            X_Arr[i]  = ce.first.X;
            Y_Arr[i]  = ce.first.Y;

            if (Address_Of_InCluster_Arr)
                (*Address_Of_InCluster_Arr)[i] = static_cast<int16_t>(ce.second->InCluster);

            if (Address_Of_DataCount_Arr)
                (*Address_Of_DataCount_Arr)[i] = ce.second->Get_Count();

            if (Address_Of_TagAddress_Arr)
                (*Address_Of_TagAddress_Arr)[i] = reinterpret_cast<uint64_t>(&ce.second->Tag);

            ++i;
        }
    }

    extern "C" __declspec(dllexport) int32_t Clusterizator_Get_ClusteringElems(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, double* X_Arr, double* Y_Arr, int16_t** Address_Of_InCluster_Arr, uint64_t** Address_Of_DataCount_Arr, uint64_t** Address_Of_TagAddress_Arr) noexcept;
    /**************************************************/

    /*** Get data for all dots for specified clustering element ***/
    template <typename Type> int32_t Clusterizator_Get_ClusteringElem_Data(const std::map<TCoordKey, Type*>& MapClusteringElem, const double X, const double Y, uint64_t* const Data_Arr) noexcept
    {
        const auto it = MapClusteringElem.find(TCoordKey(X, Y)); // find clustering element in the set

        if (it != MapClusteringElem.end()) // found
        {
            uint64_t i = 0;

            for (const auto data : it->second->Get_Data())
                Data_Arr[i++] = data;

            return 0;
        }
        else // not found
            return 1;
    }

    extern "C" __declspec(dllexport) int32_t Clusterizator_Get_ClusteringElem_Data(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, const double X, const double Y, uint64_t* Data_Arr) noexcept;
    /**************************************************************/

    /*** Does the specified clustering element contain the specified tag (1 - yes, 0 - no) ***/
    template <typename Type> int16_t Clusterizator_Is_ClusteringElem_Have_Tag(const std::map<TCoordKey, Type*>& MapClusteringElem, const double X, const double Y, const uint64_t Tag) noexcept
    {
        const auto it = MapClusteringElem.find(TCoordKey(X, Y)); // find clustering element in the set

        if (it != MapClusteringElem.end()) // found
            return it->second->Tag == Tag ? 1 : 0;
        else // not found
            return 0;
    }

    extern "C" __declspec(dllexport) int16_t Clusterizator_Is_ClusteringElem_Have_Tag(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, const double X, const double Y, const uint64_t Tag) noexcept;
    /*****************************************************************************************/

    // Does the specified clustering element contain the specified tag among all its clustering elements (1 - yes, 0 - no)
    extern "C" __declspec(dllexport) int16_t Clusterizator_Is_ClusteringElem_Have_Tag_Inside(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, const double X, const double Y, const uint64_t Tag) noexcept;
}

#endif
