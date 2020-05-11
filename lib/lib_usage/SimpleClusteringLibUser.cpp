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

#include "SimpleClusteringLibUser.h"

/*** TClustering ************************************************************************************/
NS_Clustering::TClustering::~TClustering(void) noexcept
{
    if (HandleDll)
        try { FreeLibrary(HandleDll); } catch (...) {}
}

// Load DLL
bool NS_Clustering::TClustering::LoadDll(const std::wstring& DllDir) noexcept
{
    try
    {
        // Load DLL
        #if defined(_WIN64)
          HandleDll = LoadLibraryW((DllDir + L"SimpleClustering_x64.dll").c_str());
        #else
          HandleDll = LoadLibraryW((DllDir + L"SimpleClustering_x32.dll").c_str());
        #endif

        if (HandleDll)
        {
            ID_DOT        = reinterpret_cast<const uint16_t*>(GetProcAddress(HandleDll, "ID_DOT_"));
            ID_CLUSTER_1  = reinterpret_cast<const uint16_t*>(GetProcAddress(HandleDll, "ID_CLUSTER_1_"));
            ID_CLUSTER_2  = reinterpret_cast<const uint16_t*>(GetProcAddress(HandleDll, "ID_CLUSTER_2_"));
            ID_CLUSTER_3  = reinterpret_cast<const uint16_t*>(GetProcAddress(HandleDll, "ID_CLUSTER_3_"));
            ID_CLUSTER_4  = reinterpret_cast<const uint16_t*>(GetProcAddress(HandleDll, "ID_CLUSTER_4_"));

            Dot_Radius        = reinterpret_cast<const double*>(GetProcAddress(HandleDll, "Dot_Radius_"));
            Cluster_1_Radius  = reinterpret_cast<const double*>(GetProcAddress(HandleDll, "Cluster_1_Radius_"));
            Cluster_2_Radius  = reinterpret_cast<const double*>(GetProcAddress(HandleDll, "Cluster_2_Radius_"));
            Cluster_3_Radius  = reinterpret_cast<const double*>(GetProcAddress(HandleDll, "Cluster_3_Radius_"));
            Cluster_4_Radius  = reinterpret_cast<const double*>(GetProcAddress(HandleDll, "Cluster_4_Radius_"));

            AdditionalRadiusValue = reinterpret_cast<const double*>(GetProcAddress(HandleDll, "AdditionalRadiusValue_"));

            Use_AdditionalRadiusValue = reinterpret_cast<const int16_t*>(GetProcAddress(HandleDll, "Use_AdditionalRadiusValue_"));

            ClusteringElems_Set_Default_Radius                          = reinterpret_cast<_ClusteringElems_Set_Default_Radius>(GetProcAddress(HandleDll, "ClusteringElems_Set_Default_Radius"));
            ClusteringElems_Set_Radius                                  = reinterpret_cast<_ClusteringElems_Set_Radius>(GetProcAddress(HandleDll, "ClusteringElems_Set_Radius"));
            ClusteringElems_Set_Default_AdditionalRadiusValue_Settings  = reinterpret_cast<_ClusteringElems_Set_Default_AdditionalRadiusValue_Settings>(GetProcAddress(HandleDll, "ClusteringElems_Set_Default_AdditionalRadiusValue_Settings"));
            ClusteringElems_Set_AdditionalRadiusValue_Settings          = reinterpret_cast<_ClusteringElems_Set_AdditionalRadiusValue_Settings>(GetProcAddress(HandleDll, "ClusteringElems_Set_AdditionalRadiusValue_Settings"));

            Clusterizator_New         = reinterpret_cast<_Clusterizator_New>(GetProcAddress(HandleDll, "Clusterizator_New"));
            Clusterizator_Delete      = reinterpret_cast<_Clusterizator_Delete>(GetProcAddress(HandleDll, "Clusterizator_Delete"));
            Clusterizator_Delete_All  = reinterpret_cast<_Clusterizator_Delete_All>(GetProcAddress(HandleDll, "Clusterizator_Delete_All"));

            Clusterizator_Init_Dots          = reinterpret_cast<_Clusterizator_Init_Dots>(GetProcAddress(HandleDll, "Clusterizator_Init_Dots"));
            Clusterizator_Add_Dot            = reinterpret_cast<_Clusterizator_Add_Dot>(GetProcAddress(HandleDll, "Clusterizator_Add_Dot"));
            Clusterizator_Create_Clusters_1  = reinterpret_cast<_Clusterizator_Create_Clusters_1>(GetProcAddress(HandleDll, "Clusterizator_Create_Clusters_1"));
            Clusterizator_Create_Clusters_2  = reinterpret_cast<_Clusterizator_Create_Clusters_2>(GetProcAddress(HandleDll, "Clusterizator_Create_Clusters_2"));
            Clusterizator_Create_Clusters_3  = reinterpret_cast<_Clusterizator_Create_Clusters_3>(GetProcAddress(HandleDll, "Clusterizator_Create_Clusters_3"));
            Clusterizator_Create_Clusters_4  = reinterpret_cast<_Clusterizator_Create_Clusters_4>(GetProcAddress(HandleDll, "Clusterizator_Create_Clusters_4"));

            Clusterizator_Clear_Clusters  = reinterpret_cast<_Clusterizator_Clear_Clusters>(GetProcAddress(HandleDll, "Clusterizator_Clear_Clusters"));
            Clusterizator_Clear           = reinterpret_cast<_Clusterizator_Clear>(GetProcAddress(HandleDll, "Clusterizator_Clear"));

            Clusterizator_Get_ClusteringElem_Count           = reinterpret_cast<_Clusterizator_Get_ClusteringElem_Count>(GetProcAddress(HandleDll, "Clusterizator_Get_ClusteringElem_Count"));
            Clusterizator_Get_ClusteringElem_Data_Count      = reinterpret_cast<_Clusterizator_Get_ClusteringElem_Data_Count>(GetProcAddress(HandleDll, "Clusterizator_Get_ClusteringElem_Data_Count"));
            Clusterizator_Get_All_ClusteringElem_Data_Count  = reinterpret_cast<_Clusterizator_Get_All_ClusteringElem_Data_Count>(GetProcAddress(HandleDll, "Clusterizator_Get_All_ClusteringElem_Data_Count"));
            Clusterizator_Get_Data_Count                     = reinterpret_cast<_Clusterizator_Get_Data_Count>(GetProcAddress(HandleDll, "Clusterizator_Get_Data_Count"));

            Clusterizator_Get_ClusteringElems      = reinterpret_cast<_Clusterizator_Get_ClusteringElems>(GetProcAddress(HandleDll, "Clusterizator_Get_ClusteringElems"));
            Clusterizator_Get_ClusteringElem_Data  = reinterpret_cast<_Clusterizator_Get_ClusteringElem_Data>(GetProcAddress(HandleDll, "Clusterizator_Get_ClusteringElem_Data"));

            Clusterizator_Is_ClusteringElem_Have_Tag         = reinterpret_cast<_Clusterizator_Is_ClusteringElem_Have_Tag>(GetProcAddress(HandleDll, "Clusterizator_Is_ClusteringElem_Have_Tag"));
            Clusterizator_Is_ClusteringElem_Have_Tag_Inside  = reinterpret_cast<_Clusterizator_Is_ClusteringElem_Have_Tag_Inside>(GetProcAddress(HandleDll, "Clusterizator_Is_ClusteringElem_Have_Tag_Inside"));

            if (ID_DOT &&
                ID_CLUSTER_1 &&
                ID_CLUSTER_2 &&
                ID_CLUSTER_3 &&
                ID_CLUSTER_4 &&
                Dot_Radius &&
                Cluster_1_Radius &&
                Cluster_2_Radius &&
                Cluster_3_Radius &&
                Cluster_4_Radius &&
                AdditionalRadiusValue &&
                Use_AdditionalRadiusValue &&
                ClusteringElems_Set_Default_Radius &&
                ClusteringElems_Set_Radius &&
                ClusteringElems_Set_Default_AdditionalRadiusValue_Settings &&
                ClusteringElems_Set_AdditionalRadiusValue_Settings &&
                Clusterizator_New &&
                Clusterizator_Delete &&
                Clusterizator_Delete_All &&
                Clusterizator_Init_Dots &&
                Clusterizator_Add_Dot &&
                Clusterizator_Create_Clusters_1 &&
                Clusterizator_Create_Clusters_2 &&
                Clusterizator_Create_Clusters_3 &&
                Clusterizator_Create_Clusters_4 &&
                Clusterizator_Clear_Clusters &&
                Clusterizator_Clear &&
                Clusterizator_Get_ClusteringElem_Count &&
                Clusterizator_Get_ClusteringElem_Data_Count &&
                Clusterizator_Get_All_ClusteringElem_Data_Count &&
                Clusterizator_Get_Data_Count &&
                Clusterizator_Get_ClusteringElems &&
                Clusterizator_Get_ClusteringElem_Data &&
                Clusterizator_Is_ClusteringElem_Have_Tag &&
                Clusterizator_Is_ClusteringElem_Have_Tag_Inside)
            {
                return true;
            }
        }
    }
    catch (...) {}

    return false;
}
/****************************************************************************************************/
