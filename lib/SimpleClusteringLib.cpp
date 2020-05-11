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

#include "SimpleClusteringLib.h"

#include <windows.h>

BOOL WINAPI DllMain(HINSTANCE, DWORD, LPVOID) { return 1; }

namespace NS_Clustering
{
    const uint16_t  ID_DOT_        = ID_DOT;
    const uint16_t  ID_CLUSTER_1_  = ID_CLUSTER_1;
    const uint16_t  ID_CLUSTER_2_  = ID_CLUSTER_2;
    const uint16_t  ID_CLUSTER_3_  = ID_CLUSTER_3;
    const uint16_t  ID_CLUSTER_4_  = ID_CLUSTER_4;

    const double  Dot_Radius_        = Dot_Radius;
    const double  Cluster_1_Radius_  = Cluster_1_Radius;
    const double  Cluster_2_Radius_  = Cluster_2_Radius;
    const double  Cluster_3_Radius_  = Cluster_3_Radius;
    const double  Cluster_4_Radius_  = Cluster_4_Radius;

    const double   AdditionalRadiusValue_      = AdditionalRadiusValue;
    const int16_t  Use_AdditionalRadiusValue_  = Use_AdditionalRadiusValue;

    TMapClusterizator MapClusterizator;
}

/*** TClusterizator *********************************************************************************/
NS_Clustering::TClusterizator::TClusterizator(void) noexcept :
    Id(0)
{}

NS_Clustering::TClusterizator::TClusterizator(const uint32_t Id_) noexcept :
    Id(Id_)
{}

NS_Clustering::TClusterizator::~TClusterizator(void) noexcept
{
    Clear(); // clear all data
}

// Clear all data
void NS_Clustering::TClusterizator::Clear() noexcept
{
    Clear_Map_Of_ClusteringElem(MapCluster_4);  // clear the set of clusters_4
    Clear_Map_Of_ClusteringElem(MapCluster_3);  // clear the set of clusters_3
    Clear_Map_Of_ClusteringElem(MapCluster_2);  // clear the set of clusters_2
    Clear_Map_Of_ClusteringElem(MapCluster_1);  // clear the set of clusters_1
    Clear_Map_Of_ClusteringElem(MapDot);        // clear the set of dots
}

// Get the amount of data
std::size_t NS_Clustering::TClusterizator::Get_Count(const uint16_t ClusteringElemId, bool All) const noexcept
{
    std::size_t result = 0;

    if (All) // all clustering elements
    {
        result = ClusteringElem_Get_Count(MapDot) + ClusteringElem_Get_Count(MapCluster_1) + ClusteringElem_Get_Count(MapCluster_2) +
            ClusteringElem_Get_Count(MapCluster_3) + ClusteringElem_Get_Count(MapCluster_4);
    }
    else // specified clustering elements
    {
        switch (ClusteringElemId)
        {
            case ID_DOT:       { result = ClusteringElem_Get_Count(MapDot); break; }
            case ID_CLUSTER_1: { result = ClusteringElem_Get_Count(MapCluster_1); break; }
            case ID_CLUSTER_2: { result = ClusteringElem_Get_Count(MapCluster_2); break; }
            case ID_CLUSTER_3: { result = ClusteringElem_Get_Count(MapCluster_3); break; }
            case ID_CLUSTER_4: { result = ClusteringElem_Get_Count(MapCluster_4); break; }

            default: break;
        }
    }

    return result;
}
/****************************************************************************************************/


// Reset radius of all clustering elements to default values
void NS_Clustering::ClusteringElems_Set_Default_Radius(void) noexcept
{
    Dot_Radius        = 10.0;
    Cluster_1_Radius  = 50.0;
    Cluster_2_Radius  = 300.0;
    Cluster_3_Radius  = 1500.0;
    Cluster_4_Radius  = 10000.0;

    const_cast<double&>(Dot_Radius_)        = Dot_Radius;
    const_cast<double&>(Cluster_1_Radius_)  = Cluster_1_Radius;
    const_cast<double&>(Cluster_2_Radius_)  = Cluster_2_Radius;
    const_cast<double&>(Cluster_3_Radius_)  = Cluster_3_Radius;
    const_cast<double&>(Cluster_4_Radius_)  = Cluster_4_Radius;
}

// Set the radius of all clustering elements
int32_t NS_Clustering::ClusteringElems_Set_Radius(const double Dot_R, const double Cluster_1_R, const double Cluster_2_R, const double Cluster_3_R, const double Cluster_4_R) noexcept
{
    if (Dot_R > 0 &&
        Cluster_1_R > Dot_R &&
        Cluster_2_R > Cluster_1_R &&
        Cluster_3_R > Cluster_2_R &&
        Cluster_4_R > Cluster_3_R)
    {
        Dot_Radius        = Dot_R;
        Cluster_1_Radius  = Cluster_1_R;
        Cluster_2_Radius  = Cluster_2_R;
        Cluster_3_Radius  = Cluster_3_R;
        Cluster_4_Radius  = Cluster_4_R;

        const_cast<double&>(Dot_Radius_)        = Dot_Radius;
        const_cast<double&>(Cluster_1_Radius_)  = Cluster_1_Radius;
        const_cast<double&>(Cluster_2_Radius_)  = Cluster_2_Radius;
        const_cast<double&>(Cluster_3_Radius_)  = Cluster_3_Radius;
        const_cast<double&>(Cluster_4_Radius_)  = Cluster_4_Radius;

        return 0;
    }
    else
        return 1;
}

// Reset settings of the additional radius value to default
void NS_Clustering::ClusteringElems_Set_Default_AdditionalRadiusValue_Settings(void) noexcept
{
    AdditionalRadiusValue      = 3.0;
    Use_AdditionalRadiusValue  = 1;

    const_cast<double&>(AdditionalRadiusValue_)       = AdditionalRadiusValue;
    const_cast<int16_t&>(Use_AdditionalRadiusValue_)  = Use_AdditionalRadiusValue;
}

// Set settings of the additional radius value
void NS_Clustering::ClusteringElems_Set_AdditionalRadiusValue_Settings(const double Value, const int16_t Usage) noexcept
{
    if (Value < Dot_Radius_ && (Usage == 0 || Usage == 1))
    {
        AdditionalRadiusValue      = Value;
        Use_AdditionalRadiusValue  = Usage;

        const_cast<double&>(AdditionalRadiusValue_)       = AdditionalRadiusValue;
        const_cast<int16_t&>(Use_AdditionalRadiusValue_)  = Use_AdditionalRadiusValue;
    }
}


// Create a new clusterizator. Its code is returned
uint32_t NS_Clustering::Clusterizator_New(void) noexcept
{
    const uint32_t size = static_cast<uint32_t>(MapClusterizator.size());

    MapClusterizator[size + 1].Id = size + 1;

    return size + 1;
}

// Delete clusterizator
int32_t NS_Clustering::Clusterizator_Delete(const uint32_t ClusterizatorId) noexcept
{
    if (ClusterizatorId > 0 && MapClusterizator.find(ClusterizatorId) != MapClusterizator.end())
    {
        MapClusterizator.erase(ClusterizatorId);

        return 0;
    }
    else
        return 1;
}

// Delete all clusterizators
void NS_Clustering::Clusterizator_Delete_All(void) noexcept
{
    MapClusterizator.clear();
}


// Initialize a set of dots
int32_t NS_Clustering::Clusterizator_Init_Dots(const uint32_t ClusterizatorId, const uint32_t ArrSize, const double* X_Arr, const double* Y_Arr, const uint64_t** Address_Of_Data_Arr)
{
    if (ClusterizatorId > 0 && ArrSize > 0 && X_Arr && Y_Arr)
    {
        auto it = MapClusterizator.find(ClusterizatorId);

        if (it != MapClusterizator.end())
        {
            TClusterizator& clusterizator = it->second;

            clusterizator.Clear(); // clear all data

            if (Address_Of_Data_Arr) // array specified
            {
                for (uint32_t i = 0; i < ArrSize; ++i)
                    clusterizator.MapDot.emplace(TCoordKey(X_Arr[i], Y_Arr[i]), new TDot(X_Arr[i], Y_Arr[i], (*Address_Of_Data_Arr)[i]));
            }
            else // no array
            {
                for (uint32_t i = 0; i < ArrSize; ++i)
                    clusterizator.MapDot.emplace(TCoordKey(X_Arr[i], Y_Arr[i]), new TDot(X_Arr[i], Y_Arr[i]));
            }

            return 0;
        }
    }

    return 1;
}

// Add single dot
int32_t NS_Clustering::Clusterizator_Add_Dot(const uint32_t ClusterizatorId, const double X, const double Y, const uint64_t* Address_Of_Data, uint64_t*& Out_DotTagAddress)
{
    if (ClusterizatorId > 0)
    {
        auto it = MapClusterizator.find(ClusterizatorId);

        if (it != MapClusterizator.end())
        {
            TMapDot&           map_dot  = it->second.MapDot;              // dots
            TMapDot::iterator  dot      = map_dot.find(TCoordKey(X, Y));  // find dot with the specified coordinates

            if (dot == map_dot.end()) // not found
            {
                // Create it
                if (Address_Of_Data) // data for dot
                    dot = map_dot.emplace(TCoordKey(X, Y), new TDot(X, Y, *Address_Of_Data)).first;
                else // no data for dot
                    dot = map_dot.emplace(TCoordKey(X, Y), new TDot(X, Y)).first;
            }
            else // found
            {
                if (Address_Of_Data) // data for dot
                    dot->second->SetData.insert(*Address_Of_Data); // add data to the dataset in the dot
            }

            Out_DotTagAddress = reinterpret_cast<uint64_t*>(&dot->second->Tag); // address of dots tag

            return 0;
        }
    }

    return 1;
}

// Create set of clusters_1
int32_t NS_Clustering::Clusterizator_Create_Clusters_1(const uint32_t ClusterizatorId, const int16_t SingleDot_To_Cluster_1) noexcept
{
    if (ClusterizatorId > 0)
    {
        auto it = MapClusterizator.find(ClusterizatorId);

        if (it != MapClusterizator.end())
        {
            Create_Clusters_1(it->second.MapDot, it->second.MapCluster_1, static_cast<bool>(SingleDot_To_Cluster_1));

            return 0;
        }
    }

    return 1;
}

// Create set of clusters_2
int32_t NS_Clustering::Clusterizator_Create_Clusters_2(const uint32_t ClusterizatorId, const int16_t SingleClusteringElem_To_Cluster_2) noexcept
{
    if (ClusterizatorId > 0)
    {
        auto it = MapClusterizator.find(ClusterizatorId);

        if (it != MapClusterizator.end())
        {
            Create_Clusters_2(it->second.MapDot, it->second.MapCluster_1, it->second.MapCluster_2, static_cast<bool>(SingleClusteringElem_To_Cluster_2));

            return 0;
        }
    }

    return 1;
}

// Create set of clusters_3
int32_t NS_Clustering::Clusterizator_Create_Clusters_3(const uint32_t ClusterizatorId, const int16_t SingleClusteringElem_To_Cluster_3) noexcept
{
    if (ClusterizatorId > 0)
    {
        auto it = MapClusterizator.find(ClusterizatorId);

        if (it != MapClusterizator.end())
        {
            Create_Clusters_3(it->second.MapDot, it->second.MapCluster_1, it->second.MapCluster_2, it->second.MapCluster_3, static_cast<bool>(SingleClusteringElem_To_Cluster_3));

            return 0;
        }
    }

    return 1;
}

// Create set of clusters_4
int32_t NS_Clustering::Clusterizator_Create_Clusters_4(const uint32_t ClusterizatorId, const int16_t SingleClusteringElem_To_Cluster_4) noexcept
{
    if (ClusterizatorId > 0)
    {
        auto it = MapClusterizator.find(ClusterizatorId);

        if (it != MapClusterizator.end())
        {
            Create_Clusters_4(it->second.MapDot, it->second.MapCluster_1, it->second.MapCluster_2, it->second.MapCluster_3, it->second.MapCluster_4, static_cast<bool>(SingleClusteringElem_To_Cluster_4));

            return 0;
        }
    }

    return 1;
}


// Clear all sets of clusters
int32_t NS_Clustering::Clusterizator_Clear_Clusters(const uint32_t ClusterizatorId) noexcept
{
    if (ClusterizatorId > 0)
    {
        auto it = MapClusterizator.find(ClusterizatorId);

        if (it != MapClusterizator.end())
        {
            Clear_Map_Of_ClusteringElem(it->second.MapCluster_4);
            Clear_Map_Of_ClusteringElem(it->second.MapCluster_3);
            Clear_Map_Of_ClusteringElem(it->second.MapCluster_2);
            Clear_Map_Of_ClusteringElem(it->second.MapCluster_1);

            for (auto& dot : it->second.MapDot)
                dot.second->InCluster = false;

            return 0;
        }
    }

    return 1;
}

// Clear clusterizator
int32_t NS_Clustering::Clusterizator_Clear(const uint32_t ClusterizatorId) noexcept
{
    if (ClusterizatorId > 0)
    {
        auto it = MapClusterizator.find(ClusterizatorId);

        if (it != MapClusterizator.end())
        {
            it->second.Clear();

            return 0;
        }
    }

    return 1;
}


// Get the number of clustering elements of the specified id
uint64_t NS_Clustering::Clusterizator_Get_ClusteringElem_Count(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId) noexcept
{
    if (ClusterizatorId > 0)
    {
        const auto cit = MapClusterizator.find(ClusterizatorId);

        if (cit != MapClusterizator.cend())
        {
            switch (ClusteringElemId)
            {
                case ID_DOT:       return cit->second.MapDot.size();
                case ID_CLUSTER_1: return cit->second.MapCluster_1.size();
                case ID_CLUSTER_2: return cit->second.MapCluster_2.size();
                case ID_CLUSTER_3: return cit->second.MapCluster_3.size();
                case ID_CLUSTER_4: return cit->second.MapCluster_4.size();

                default: break;
            }
        }
    }

    return 0;
}

// Get the amount of data for clustering element of the specified id
uint64_t NS_Clustering::Clusterizator_Get_ClusteringElem_Data_Count(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, const double X, const double Y) noexcept
{
    if (ClusterizatorId > 0)
    {
        const auto cit = MapClusterizator.find(ClusterizatorId);

        if (cit != MapClusterizator.cend())
        {
            switch (ClusteringElemId)
            {
                case ID_DOT:       return Clusterizator_Get_ClusteringElem_Data_Count(cit->second.MapDot, X, Y);
                case ID_CLUSTER_1: return Clusterizator_Get_ClusteringElem_Data_Count(cit->second.MapCluster_1, X, Y);
                case ID_CLUSTER_2: return Clusterizator_Get_ClusteringElem_Data_Count(cit->second.MapCluster_2, X, Y);
                case ID_CLUSTER_3: return Clusterizator_Get_ClusteringElem_Data_Count(cit->second.MapCluster_3, X, Y);
                case ID_CLUSTER_4: return Clusterizator_Get_ClusteringElem_Data_Count(cit->second.MapCluster_4, X, Y);

                default: break;
            }
        }
    }

    return 0;
}

// Get the amount of data for all clustering elements of the specified id
uint64_t NS_Clustering::Clusterizator_Get_All_ClusteringElem_Data_Count(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId) noexcept
{
    if (ClusterizatorId > 0)
    {
        const auto cit = MapClusterizator.find(ClusterizatorId);

        if (cit != MapClusterizator.cend())
            return cit->second.Get_Count(ClusteringElemId, false);
    }

    return 0;
}

// Get the amount of data for all clustering elements of clusterizator
uint64_t NS_Clustering::Clusterizator_Get_Data_Count(const uint32_t ClusterizatorId) noexcept
{
    if (ClusterizatorId > 0)
    {
        const auto cit = MapClusterizator.find(ClusterizatorId);

        if (cit != MapClusterizator.cend())
            return cit->second.Get_Count(0, true);
    }

    return 0;
}


// Get a set of specified clustering elements
int32_t NS_Clustering::Clusterizator_Get_ClusteringElems(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, double* X_Arr, double* Y_Arr, int16_t** Address_Of_InCluster_Arr, uint64_t** Address_Of_DataCount_Arr, uint64_t** Address_Of_TagAddress_Arr) noexcept
{
    if (ClusterizatorId > 0 && X_Arr && Y_Arr)
    {
        const auto cit = MapClusterizator.find(ClusterizatorId);

        if (cit != MapClusterizator.cend() && !cit->second.MapDot.empty())
        {
            switch (ClusteringElemId)
            {
                case ID_DOT:       { Clusterizator_Get_ClusteringElems(cit->second.MapDot, X_Arr, Y_Arr, Address_Of_InCluster_Arr, Address_Of_DataCount_Arr, Address_Of_TagAddress_Arr); return 0; }
                case ID_CLUSTER_1: { Clusterizator_Get_ClusteringElems(cit->second.MapCluster_1, X_Arr, Y_Arr, Address_Of_InCluster_Arr, Address_Of_DataCount_Arr, Address_Of_TagAddress_Arr); return 0; }
                case ID_CLUSTER_2: { Clusterizator_Get_ClusteringElems(cit->second.MapCluster_2, X_Arr, Y_Arr, Address_Of_InCluster_Arr, Address_Of_DataCount_Arr, Address_Of_TagAddress_Arr); return 0; }
                case ID_CLUSTER_3: { Clusterizator_Get_ClusteringElems(cit->second.MapCluster_3, X_Arr, Y_Arr, Address_Of_InCluster_Arr, Address_Of_DataCount_Arr, Address_Of_TagAddress_Arr); return 0; }
                case ID_CLUSTER_4: { Clusterizator_Get_ClusteringElems(cit->second.MapCluster_4, X_Arr, Y_Arr, Address_Of_InCluster_Arr, Address_Of_DataCount_Arr, Address_Of_TagAddress_Arr); return 0; }

                default: break;
            }
        }
    }

    return 1;
}

// Get data for all dots for specified clustering element
int32_t NS_Clustering::Clusterizator_Get_ClusteringElem_Data(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, const double X, const double Y, uint64_t* Data_Arr) noexcept
{
    if (ClusterizatorId > 0 && Data_Arr)
    {
        const auto cit = MapClusterizator.find(ClusterizatorId);

        if (cit != MapClusterizator.cend() && !cit->second.MapDot.empty())
        {
            switch (ClusteringElemId)
            {
                case ID_DOT:       return Clusterizator_Get_ClusteringElem_Data(cit->second.MapDot, X, Y, Data_Arr);
                case ID_CLUSTER_1: return Clusterizator_Get_ClusteringElem_Data(cit->second.MapCluster_1, X, Y, Data_Arr);
                case ID_CLUSTER_2: return Clusterizator_Get_ClusteringElem_Data(cit->second.MapCluster_2, X, Y, Data_Arr);
                case ID_CLUSTER_3: return Clusterizator_Get_ClusteringElem_Data(cit->second.MapCluster_3, X, Y, Data_Arr);
                case ID_CLUSTER_4: return Clusterizator_Get_ClusteringElem_Data(cit->second.MapCluster_4, X, Y, Data_Arr);

                default: break;
            }
        }
    }

    return 1;
}


// Does the specified clustering element contain the specified tag (1 - yes, 0 - no)
int16_t NS_Clustering::Clusterizator_Is_ClusteringElem_Have_Tag(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, const double X, const double Y, const uint64_t Tag) noexcept
{
    if (ClusterizatorId > 0)
    {
        const auto cit = MapClusterizator.find(ClusterizatorId);

        if (cit != MapClusterizator.cend() && !cit->second.MapDot.empty())
        {
            switch (ClusteringElemId)
            {
                case ID_DOT:       return Clusterizator_Is_ClusteringElem_Have_Tag(cit->second.MapDot, X, Y, Tag);
                case ID_CLUSTER_1: return Clusterizator_Is_ClusteringElem_Have_Tag(cit->second.MapCluster_1, X, Y, Tag);
                case ID_CLUSTER_2: return Clusterizator_Is_ClusteringElem_Have_Tag(cit->second.MapCluster_2, X, Y, Tag);
                case ID_CLUSTER_3: return Clusterizator_Is_ClusteringElem_Have_Tag(cit->second.MapCluster_3, X, Y, Tag);
                case ID_CLUSTER_4: return Clusterizator_Is_ClusteringElem_Have_Tag(cit->second.MapCluster_4, X, Y, Tag);

                default: break;
            }
        }
    }

    return 0;
}

/*** Does the specified clustering element contain the specified tag among all its clustering elements (1 - yes, 0 - no) ***/
namespace NS_Clustering
{
    int16_t Clusterizator_Is_MapDot_Have_Tag_Inside(const TMapCDot& MapDot, const uint64_t Tag) noexcept
    {
        for (const auto& dot : MapDot)
        {
            if (dot.second->Tag == Tag)
                return 1;
        }

        return 0;
    }

    int16_t Clusterizator_Is_MapCluster_1_Have_Tag_Inside(const TMapCCluster_1& MapCluster_1, const uint64_t Tag) noexcept
    {
        int16_t tag_found = 0;

        for (const auto& c1 : MapCluster_1)
        {
            if (c1.second->Tag == Tag)
                tag_found = 1;
            else
                tag_found = Clusterizator_Is_MapDot_Have_Tag_Inside(c1.second->MapDot, Tag);

            if (tag_found == 1)
                break;
        }

        return tag_found;
    }

    int16_t Clusterizator_Is_MapCluster_2_Have_Tag_Inside(const TMapCCluster_2& MapCluster_2, const uint64_t Tag) noexcept
    {
        int16_t tag_found = 0;

        for (const auto& c2 : MapCluster_2)
        {
            if (c2.second->Tag == Tag)
                tag_found = 1;
            else
            {
                tag_found = Clusterizator_Is_MapCluster_1_Have_Tag_Inside(c2.second->MapCluster_1, Tag);

                if (tag_found != 1)
                    tag_found = Clusterizator_Is_MapDot_Have_Tag_Inside(c2.second->MapDot, Tag);
            }

            if (tag_found == 1)
                break;
        }

        return tag_found;
    }

    int16_t Clusterizator_Is_MapCluster_3_Have_Tag_Inside(const TMapCCluster_3& MapCluster_3, const uint64_t Tag) noexcept
    {
        int16_t tag_found = 0;

        for (const auto& c3 : MapCluster_3)
        {
            if (c3.second->Tag == Tag)
                tag_found = 1;
            else
            {
                tag_found = Clusterizator_Is_MapCluster_2_Have_Tag_Inside(c3.second->MapCluster_2, Tag);

                if (tag_found != 1)
                    tag_found = Clusterizator_Is_MapCluster_1_Have_Tag_Inside(c3.second->MapCluster_1, Tag);

                if (tag_found != 1)
                    tag_found = Clusterizator_Is_MapDot_Have_Tag_Inside(c3.second->MapDot, Tag);
            }

            if (tag_found == 1)
                break;
        }

        return tag_found;
    }
}

int16_t NS_Clustering::Clusterizator_Is_ClusteringElem_Have_Tag_Inside(const uint32_t ClusterizatorId, const uint16_t ClusteringElemId, const double X, const double Y, const uint64_t Tag) noexcept
{
    if (ClusterizatorId > 0)
    {
        const auto cit = MapClusterizator.find(ClusterizatorId);

        if (cit != MapClusterizator.cend() && !cit->second.MapDot.empty())
        {
            switch (ClusteringElemId)
            {
                case ID_DOT: return Clusterizator_Is_ClusteringElem_Have_Tag(cit->second.MapDot, X, Y, Tag);

                case ID_CLUSTER_1:
                {
                    const auto cit_c1 = cit->second.MapCluster_1.find(TCoordKey(X, Y)); // find specified cluster_1

                    if (cit_c1 != cit->second.MapCluster_1.cend()) // found
                    {
                        int16_t tag_found = 0;

                        if (cit_c1->second->Tag == Tag)
                            tag_found = 1;
                        else
                            tag_found = Clusterizator_Is_MapDot_Have_Tag_Inside(cit_c1->second->MapDot, Tag);

                        return tag_found;
                    }

                    break;
                }

                case ID_CLUSTER_2:
                {
                    const auto cit_c2 = cit->second.MapCluster_2.find(TCoordKey(X, Y)); // find specified cluster_2

                    if (cit_c2 != cit->second.MapCluster_2.cend()) // found
                    {
                        int16_t tag_found = 0;

                        if (cit_c2->second->Tag == Tag)
                            tag_found = 1;
                        else
                        {
                            tag_found = Clusterizator_Is_MapCluster_1_Have_Tag_Inside(cit_c2->second->MapCluster_1, Tag);

                            if (tag_found != 1)
                                tag_found = Clusterizator_Is_MapDot_Have_Tag_Inside(cit_c2->second->MapDot, Tag);
                        }

                        return tag_found;
                    }

                    break;
                }

                case ID_CLUSTER_3:
                {
                    const auto cit_c3 = cit->second.MapCluster_3.find(TCoordKey(X, Y)); // find specified cluster_3

                    if (cit_c3 != cit->second.MapCluster_3.cend()) // found
                    {
                        int16_t tag_found = 0;

                        if (cit_c3->second->Tag == Tag)
                            tag_found = 1;
                        else
                        {
                            tag_found = Clusterizator_Is_MapCluster_2_Have_Tag_Inside(cit_c3->second->MapCluster_2, Tag);

                            if (tag_found != 1)
                                tag_found = Clusterizator_Is_MapCluster_1_Have_Tag_Inside(cit_c3->second->MapCluster_1, Tag);

                            if (tag_found != 1)
                                tag_found = Clusterizator_Is_MapDot_Have_Tag_Inside(cit_c3->second->MapDot, Tag);
                        }

                        return tag_found;
                    }

                    break;
                }

                case ID_CLUSTER_4:
                {
                    const auto cit_c4 = cit->second.MapCluster_4.find(TCoordKey(X, Y)); // find specified cluster_4

                    if (cit_c4 != cit->second.MapCluster_4.cend()) // found
                    {
                        int16_t tag_found = 0;

                        if (cit_c4->second->Tag == Tag)
                            tag_found = 1;
                        else
                        {
                            tag_found = Clusterizator_Is_MapCluster_3_Have_Tag_Inside(cit_c4->second->MapCluster_3, Tag);

                            if (tag_found != 1)
                                tag_found = Clusterizator_Is_MapCluster_2_Have_Tag_Inside(cit_c4->second->MapCluster_2, Tag);

                            if (tag_found != 1)
                                tag_found = Clusterizator_Is_MapCluster_1_Have_Tag_Inside(cit_c4->second->MapCluster_1, Tag);

                            if (tag_found != 1)
                                tag_found = Clusterizator_Is_MapDot_Have_Tag_Inside(cit_c4->second->MapDot, Tag);
                        }

                        return tag_found;
                    }

                    break;
                }

                default: break;
            }
        }
    }

    return 0;
}
/***************************************************************************************************************************/
