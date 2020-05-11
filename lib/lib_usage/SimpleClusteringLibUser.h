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

#ifndef SimpleClusteringLibUserH
#define SimpleClusteringLibUserH

#include <string>

#include <cstdint>

#include <windows.h>

namespace NS_Clustering
{
    class TClustering
    {
    private:
        // Reset radius of all clustering elements to default values
        using _ClusteringElems_Set_Default_Radius = void (*)(void);

        // Set the radius of all clustering elements
        using _ClusteringElems_Set_Radius = int32_t (*)(const double Dot_R, const double Cluster_1_R, const double Cluster_2_R, const double Cluster_3_R, const double Cluster_4_R);

        // Reset settings of the additional radius value to default
        using _ClusteringElems_Set_Default_AdditionalRadiusValue_Settings = void (*)(void);

        // Set settings of the additional radius value
        using _ClusteringElems_Set_AdditionalRadiusValue_Settings = void (*)(const double Value, const int16_t Usage);


        // Create a new clusterizator. Its code is returned
        using _Clusterizator_New = uint32_t (*)(void);

        // Delete clusterizator
        using _Clusterizator_Delete = int32_t (*)(const uint32_t ClusterizatorId);

        // Delete all clusterizators
        using _Clusterizator_Delete_All = void (*)(void);


        // Initialize a set of dots
        using _Clusterizator_Init_Dots = int32_t (*)(const uint32_t ClusterizatorId, const uint32_t ArrSize, const double* X_Arr, const double* Y_Arr, const uint64_t** Address_Of_Data_Arr);

        // Add single dot
        using _Clusterizator_Add_Dot = int32_t (*)(const uint32_t ClusterizatorId, const double X, const double Y, const uint64_t* Address_Of_Data, uint64_t*& Out_DotTagAddress);

        // Create set of clusters_1
        using _Clusterizator_Create_Clusters_1 = int32_t (*)(const uint32_t ClusterizatorId, const int16_t SingleDot_To_Cluster_1);

        // Create set of clusters_2
        using _Clusterizator_Create_Clusters_2 = int32_t (*)(const uint32_t ClusterizatorId, const int16_t SingleClusteringElem_To_Cluster_2);

        // Create set of clusters_3
        using _Clusterizator_Create_Clusters_3 = int32_t (*)(const uint32_t ClusterizatorId, const int16_t SingleClusteringElem_To_Cluster_3);

        // Create set of clusters_4
        using _Clusterizator_Create_Clusters_4 = int32_t (*)(const uint32_t ClusterizatorId, const int16_t SingleClusteringElem_To_Cluster_4);


        // Clear all sets of clusters
        using _Clusterizator_Clear_Clusters = int32_t (*)(const uint32_t ClusterizatorId);

        // Clear clusterizator
        using _Clusterizator_Clear = int32_t (*)(const uint32_t ClusterizatorId);


        // Get the number of clustering elements of the specified id
        using _Clusterizator_Get_ClusteringElem_Count = uint64_t (*)(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId);

        // Get the amount of data for clustering element of the specified id
        using _Clusterizator_Get_ClusteringElem_Data_Count = uint64_t (*)(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, const double X, const double Y);

        // Get the amount of data for all clustering elements of the specified id
        using _Clusterizator_Get_All_ClusteringElem_Data_Count = uint64_t (*)(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId);

        // Get the amount of data for all clustering elements of clusterizator
        using _Clusterizator_Get_Data_Count = uint64_t (*)(const uint32_t ClusterizatorId);


        // Get a set of specified clustering elements
        using _Clusterizator_Get_ClusteringElems = int32_t (*)(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, double* X_Arr, double* Y_Arr, int16_t** Address_Of_InCluster_Arr, uint64_t** Address_Of_DataCount_Arr, uint64_t** Address_Of_TagAddress_Arr);

        // Get data for all dots for specified clustering element
        using _Clusterizator_Get_ClusteringElem_Data = int32_t (*)(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, const double X, const double Y, uint64_t* Data_Arr);


        // Does the specified clustering element contain the specified tag (1 - yes, 0 - no)
        using _Clusterizator_Is_ClusteringElem_Have_Tag = int16_t (*)(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, const double X, const double Y, const uint64_t Tag);

        // Does the specified clustering element contain the specified tag among all its clustering elements (1 - yes, 0 - no)
        using _Clusterizator_Is_ClusteringElem_Have_Tag_Inside = int16_t (*)(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, const double X, const double Y, const uint64_t Tag);

    public:
        HINSTANCE HandleDll; // id for DLL

        // Cluster object identifiers
        const uint16_t*  ID_DOT;
        const uint16_t*  ID_CLUSTER_1;
        const uint16_t*  ID_CLUSTER_2;
        const uint16_t*  ID_CLUSTER_3;
        const uint16_t*  ID_CLUSTER_4;

        const double*  Dot_Radius;
        const double*  Cluster_1_Radius;
        const double*  Cluster_2_Radius;
        const double*  Cluster_3_Radius;
        const double*  Cluster_4_Radius;

        /* An additional part of the radius - is added to the radius of the clustering object when checking the possibility of merging.
        It may be necessary when graphically displaying clustering elements so that it does not seem that the center of any element
        located on the edge of another element. */
        const double* AdditionalRadiusValue;

        // Whether to use the additional part of the radius when checking the possibility of merging clustering objects
        const int16_t* Use_AdditionalRadiusValue;

        _ClusteringElems_Set_Default_Radius                          ClusteringElems_Set_Default_Radius;
        _ClusteringElems_Set_Radius                                  ClusteringElems_Set_Radius;
        _ClusteringElems_Set_Default_AdditionalRadiusValue_Settings  ClusteringElems_Set_Default_AdditionalRadiusValue_Settings;
        _ClusteringElems_Set_AdditionalRadiusValue_Settings          ClusteringElems_Set_AdditionalRadiusValue_Settings;

        _Clusterizator_New         Clusterizator_New;
        _Clusterizator_Delete      Clusterizator_Delete;
        _Clusterizator_Delete_All  Clusterizator_Delete_All;

        _Clusterizator_Init_Dots          Clusterizator_Init_Dots;
        _Clusterizator_Add_Dot            Clusterizator_Add_Dot;
        _Clusterizator_Create_Clusters_1  Clusterizator_Create_Clusters_1;
        _Clusterizator_Create_Clusters_2  Clusterizator_Create_Clusters_2;
        _Clusterizator_Create_Clusters_3  Clusterizator_Create_Clusters_3;
        _Clusterizator_Create_Clusters_4  Clusterizator_Create_Clusters_4;

        _Clusterizator_Clear_Clusters  Clusterizator_Clear_Clusters;
        _Clusterizator_Clear           Clusterizator_Clear;

        _Clusterizator_Get_ClusteringElem_Count           Clusterizator_Get_ClusteringElem_Count;
        _Clusterizator_Get_ClusteringElem_Data_Count      Clusterizator_Get_ClusteringElem_Data_Count;
        _Clusterizator_Get_All_ClusteringElem_Data_Count  Clusterizator_Get_All_ClusteringElem_Data_Count;
        _Clusterizator_Get_Data_Count                     Clusterizator_Get_Data_Count;

        _Clusterizator_Get_ClusteringElems      Clusterizator_Get_ClusteringElems;
        _Clusterizator_Get_ClusteringElem_Data  Clusterizator_Get_ClusteringElem_Data;

        _Clusterizator_Is_ClusteringElem_Have_Tag         Clusterizator_Is_ClusteringElem_Have_Tag;
        _Clusterizator_Is_ClusteringElem_Have_Tag_Inside  Clusterizator_Is_ClusteringElem_Have_Tag_Inside;


        ~TClustering(void) noexcept;

        // Load DLL
        bool LoadDll(const std::wstring& DllDir) noexcept;
    };
}

#endif
