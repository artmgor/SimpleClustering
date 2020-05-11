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

#include "SimpleClustering.h"

#include <list>

#include <cmath>

namespace NS_Clustering
{
    double  Dot_Radius        = 10.0;
    double  Cluster_1_Radius  = 50.0;
    double  Cluster_2_Radius  = 300.0;
    double  Cluster_3_Radius  = 1500.0;
    double  Cluster_4_Radius  = 10000.0;

    double   AdditionalRadiusValue      = 3.0;
    int16_t  Use_AdditionalRadiusValue  = 1;

    // Check if the coordinate is in a circle
    bool Coord_In_Circle(const double C_X, const double C_Y, const double C_Radius, const double X, const double Y) noexcept
    {
        return std::sqrt((C_X - X) * (C_X - X) + (C_Y - Y) * (C_Y - Y)) <= C_Radius + (Use_AdditionalRadiusValue ? AdditionalRadiusValue : 0);
    }
}

/*** TCoordKey **************************************************************************************/
NS_Clustering::TCoordKey::TCoordKey(void) noexcept :
    X(0.0),
    Y(0.0)
{}

NS_Clustering::TCoordKey::TCoordKey(const double X_, const double Y_) noexcept :
    X(X_),
    Y(Y_)
{}

bool NS_Clustering::TCoordKey::operator == (const TCoordKey& Obj) const noexcept
{
    return X == Obj.X && Y == Obj.Y;
}

bool NS_Clustering::TCoordKey::operator != (const TCoordKey& Obj) const noexcept
{
    return X != Obj.X || Y != Obj.Y;
}

bool NS_Clustering::TCoordKey::operator < (const TCoordKey& Obj) const noexcept
{
    return X < Obj.X ||
        (X == Obj.X && Y < Obj.Y);
}
/****************************************************************************************************/


/*** TClusteringElem ********************************************************************************/
NS_Clustering::TClusteringElem::TClusteringElem(void) noexcept :
    ClusteringElem_Id(ID_BASE_CLASS),
    X                (0.0),
    Y                (0.0),
    R                (0.0),
    InCluster        (false),
    Tag              (0xFFFFFFFFFFFFFFFFULL)
{}

NS_Clustering::TClusteringElem::TClusteringElem(const uint16_t ClusteringElem_Id_) noexcept :
    ClusteringElem_Id(ClusteringElem_Id_),
    X                (0.0),
    Y                (0.0),
    R                (0.0),
    InCluster        (false),
    Tag              (0xFFFFFFFFFFFFFFFFULL)
{}

NS_Clustering::TClusteringElem::TClusteringElem(const uint16_t ClusteringElem_Id_, const double X_, const double Y_, const double R_, const bool InCluster_, const uint64_t Tag_) noexcept :
    ClusteringElem_Id(ClusteringElem_Id_),
    X                (X_),
    Y                (Y_),
    R                (R_),
    InCluster        (InCluster_),
    Tag              (Tag_)
{}

NS_Clustering::TClusteringElem::~TClusteringElem(void) noexcept {}

// Get the unique identifier of the clustering element
uint16_t NS_Clustering::TClusteringElem::Get_ClusteringElem_Id(void) const noexcept
{
    return ClusteringElem_Id;
}

// Set label, if not already set (0xFFFFFFFFFFFFFFFFULL)
void NS_Clustering::TClusteringElem::Set_Tag_If_Empty(const uint64_t Tag_) noexcept
{
    if (Tag == 0xFFFFFFFFFFFFFFFFULL)
        Tag = Tag_;
}
/****************************************************************************************************/


/*** TDot *******************************************************************************************/
NS_Clustering::TDot::TDot(void) noexcept :
    TClusteringElem(ID_DOT, 0.0, 0.0, Dot_Radius, false)
{}

NS_Clustering::TDot::TDot(const double X_, const double Y_) noexcept :
    TClusteringElem(ID_DOT, X_, Y_, Dot_Radius, false)
{}

NS_Clustering::TDot::TDot(const double X_, const double Y_, const uint64_t Data) noexcept :
    TClusteringElem(ID_DOT, X_, Y_, Dot_Radius, false)
{
    SetData.insert(Data);
}

// Get the amount of data
std::size_t NS_Clustering::TDot::Get_Count(void) const noexcept
{
    return SetData.size();
}

// Get data for all dots
std::set<uint64_t> NS_Clustering::TDot::Get_Data(void) const noexcept
{
    return SetData;
}
/****************************************************************************************************/


/*** TCluster_1 *************************************************************************************/
NS_Clustering::TCluster_1::TCluster_1(void) noexcept :
    TClusteringElem(ID_CLUSTER_1, 0.0, 0.0, Cluster_1_Radius, false)
{}

NS_Clustering::TCluster_1::TCluster_1(TDot* const Dot) noexcept :
    TClusteringElem(ID_CLUSTER_1, 0.0, 0.0, Cluster_1_Radius, false)
{
    MapDot[TCoordKey(Dot->X, Dot->Y)]  = Dot;   // add dot
    Dot->InCluster                     = true;  // dot is now in the cluster

    // Set center coordinates
    X  = Dot->X;
    Y  = Dot->Y;
}

NS_Clustering::TCluster_1::TCluster_1(TDot* const Dot1, TDot* const Dot2) noexcept :
    TClusteringElem(ID_CLUSTER_1, 0.0, 0.0, Cluster_1_Radius, false)
{
    MapDot[TCoordKey(Dot1->X, Dot1->Y)]  = Dot1;  // add dot 1
    Dot1->InCluster                      = true;  // dot 1 is now in the cluster
    MapDot[TCoordKey(Dot2->X, Dot2->Y)]  = Dot2;  // add dot 2
    Dot2->InCluster                      = true;  // dot 2 is now in the cluster
    Set_Center();                                 // calculate center coordinates
}

// Calculate center coordinates
void NS_Clustering::TCluster_1::Set_Center(void) noexcept
{
    if (!MapDot.empty())
    {
        const std::size_t  size   = MapDot.size();
        double             sum_x  = 0.0;
        double             sum_y  = 0.0;

        for (const auto& dot : MapDot)
        {
            sum_x  += dot.first.X;
            sum_y  += dot.first.Y;
        }

        X  = sum_x / size;
        Y  = sum_y / size;
    }
    else
    {
        X  = 0.0;
        Y  = 0.0;
    }
}

// Get the amount of data
std::size_t NS_Clustering::TCluster_1::Get_Count(void) const noexcept
{
    return ClusteringElem_Get_Count(MapDot);
}

// Get data for all dots
std::set<uint64_t> NS_Clustering::TCluster_1::Get_Data(void) const noexcept
{
    std::set<uint64_t> result;

    for (const auto& dot : MapDot)
        result.insert(dot.second->SetData.cbegin(), dot.second->SetData.cend());

    return result;
}

// Check if it is worth adding a dot
bool NS_Clustering::TCluster_1::Check_Dot(const TDot* const Dot) const noexcept
{
    return Coord_In_Circle(X, Y, R, Dot->X, Dot->Y);
}

// Add dot
void NS_Clustering::TCluster_1::Add_Dot(TDot* const Dot) noexcept
{
    MapDot[TCoordKey(Dot->X, Dot->Y)]  = Dot;   // add dot
    Dot->InCluster                     = true;  // dot is now in the cluster
    Set_Center();                               // calculate center coordinates
}

// Check if it is worth adding a cluster_1
bool NS_Clustering::TCluster_1::Check_Cluster_1(const TCluster_1* const Cluster_1) const noexcept
{
    return Coord_In_Circle(X, Y, R, Cluster_1->X, Cluster_1->Y);
}

// Absorb cluster_1
void NS_Clustering::TCluster_1::Eat_Cluster_1(TCluster_1*& Cluster_1) noexcept
{
    MapDot.insert(Cluster_1->MapDot.cbegin(), Cluster_1->MapDot.cend()); // absorb all dots

    // Cluster is absorbed
    delete Cluster_1;
    Cluster_1 = nullptr;

    Set_Center(); // calculate center coordinates
}
/****************************************************************************************************/


/*** TCluster_2 *************************************************************************************/
NS_Clustering::TCluster_2::TCluster_2(void) noexcept :
    TClusteringElem(ID_CLUSTER_2, 0.0, 0.0, Cluster_2_Radius, false)
{}

NS_Clustering::TCluster_2::TCluster_2(TDot* const Dot1, TDot* const Dot2) noexcept :
    TClusteringElem(ID_CLUSTER_2, 0.0, 0.0, Cluster_2_Radius, false)
{
    MapDot[TCoordKey(Dot1->X, Dot1->Y)]  = Dot1;  // add dot 1
    Dot1->InCluster                      = true;  // dot 1 is now in the cluster
    MapDot[TCoordKey(Dot2->X, Dot2->Y)]  = Dot2;  // add dot 2
    Dot2->InCluster                      = true;  // dot 2 is now in the cluster
    Set_Center();                                 // calculate center coordinates
}

NS_Clustering::TCluster_2::TCluster_2(TClusteringElem* const ClusteringElem) noexcept :
    TClusteringElem(ID_CLUSTER_2, 0.0, 0.0, Cluster_2_Radius, false)
{
    if (ClusteringElem->Get_ClusteringElem_Id() < ID_CLUSTER_2)
    {
        switch (ClusteringElem->Get_ClusteringElem_Id())
        {
            case ID_DOT:       { MapDot[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]        = dynamic_cast<TDot*>(ClusteringElem); break; }
            case ID_CLUSTER_1: { MapCluster_1[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]  = dynamic_cast<TCluster_1*>(ClusteringElem); break; }
            default: break;
        }

        ClusteringElem->InCluster = true; // element is now in the cluster

        // Set center coordinates
        X  = ClusteringElem->X;
        Y  = ClusteringElem->Y;
    }
}

NS_Clustering::TCluster_2::TCluster_2(TClusteringElem* const ClusteringElem1, TClusteringElem* const ClusteringElem2) noexcept :
    TClusteringElem(ID_CLUSTER_2, 0.0, 0.0, Cluster_2_Radius, false)
{
    if (ClusteringElem1->Get_ClusteringElem_Id() < ID_CLUSTER_2 && ClusteringElem2->Get_ClusteringElem_Id() < ID_CLUSTER_2)
    {
        switch (ClusteringElem1->Get_ClusteringElem_Id())
        {
            case ID_DOT:       { MapDot[TCoordKey(ClusteringElem1->X, ClusteringElem1->Y)]        = dynamic_cast<TDot*>(ClusteringElem1); break; }
            case ID_CLUSTER_1: { MapCluster_1[TCoordKey(ClusteringElem1->X, ClusteringElem1->Y)]  = dynamic_cast<TCluster_1*>(ClusteringElem1); break; }
            default: break;
        }

        switch (ClusteringElem2->Get_ClusteringElem_Id())
        {
            case ID_DOT:       { MapDot[TCoordKey(ClusteringElem2->X, ClusteringElem2->Y)]        = dynamic_cast<TDot*>(ClusteringElem2); break; }
            case ID_CLUSTER_1: { MapCluster_1[TCoordKey(ClusteringElem2->X, ClusteringElem2->Y)]  = dynamic_cast<TCluster_1*>(ClusteringElem2); break; }
            default: break;
        }

        ClusteringElem1->InCluster  = true;  // element 1 is now in the cluster
        ClusteringElem2->InCluster  = true;  // element 2 is now in the cluster
        Set_Center();                        // calculate center coordinates
    }
}

// Calculate center coordinates
void NS_Clustering::TCluster_2::Set_Center(void) noexcept
{
    if (!MapDot.empty() || !MapCluster_1.empty())
    {
        const std::size_t  size   = MapDot.size() + MapCluster_1.size();
        double             sum_x  = 0.0;
        double             sum_y  = 0.0;

        for (const auto& dot : MapDot)
        {
            sum_x  += dot.first.X;
            sum_y  += dot.first.Y;
        }

        for (const auto& c1 : MapCluster_1)
        {
            sum_x  += c1.first.X;
            sum_y  += c1.first.Y;
        }

        X  = sum_x / size;
        Y  = sum_y / size;
    }
    else
    {
        X  = 0.0;
        Y  = 0.0;
    }
}

// Get the amount of data
std::size_t NS_Clustering::TCluster_2::Get_Count(void) const noexcept
{
    return ClusteringElem_Get_Count(MapDot) + ClusteringElem_Get_Count(MapCluster_1);
}

// Get data for all dots
std::set<uint64_t> NS_Clustering::TCluster_2::Get_Data(void) const noexcept
{
    std::set<uint64_t> result;

    for (const auto& dot : MapDot)
        result.insert(dot.second->SetData.cbegin(), dot.second->SetData.cend());

    for (const auto& c1 : MapCluster_1)
    {
        const std::set<uint64_t> c1_data = c1.second->Get_Data();

        result.insert(c1_data.cbegin(), c1_data.cend());
    }

    return result;
}

// Check if it is worth adding a clustering element
bool NS_Clustering::TCluster_2::Check_ClusteringElem(const TClusteringElem* const ClusteringElem) const noexcept
{
    return ClusteringElem->Get_ClusteringElem_Id() < ID_CLUSTER_2 && Coord_In_Circle(X, Y, R, ClusteringElem->X, ClusteringElem->Y);
}

// Add clustering element
void NS_Clustering::TCluster_2::Add_ClusteringElem(TClusteringElem* const ClusteringElem) noexcept
{
    if (ClusteringElem->Get_ClusteringElem_Id() < ID_CLUSTER_2)
    {
        switch (ClusteringElem->Get_ClusteringElem_Id())
        {
            case ID_DOT:       { MapDot[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]        = dynamic_cast<TDot*>(ClusteringElem); break; }
            case ID_CLUSTER_1: { MapCluster_1[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]  = dynamic_cast<TCluster_1*>(ClusteringElem); break; }
            default: break;
        }

        ClusteringElem->InCluster = true;  // element is now in the cluster
        Set_Center();                      // calculate center coordinates
    }
}

// Check if it is worth adding a cluster_2
bool NS_Clustering::TCluster_2::Check_Cluster_2(const TCluster_2* const Cluster_2) const noexcept
{
    return Coord_In_Circle(X, Y, R, Cluster_2->X, Cluster_2->Y);
}

// Absorb cluster_2
void NS_Clustering::TCluster_2::Eat_Cluster_2(TCluster_2*& Cluster_2) noexcept
{
    MapDot.insert(Cluster_2->MapDot.cbegin(), Cluster_2->MapDot.cend());                    // absorb all dots
    MapCluster_1.insert(Cluster_2->MapCluster_1.cbegin(), Cluster_2->MapCluster_1.cend());  // absorb all clusters_1

    // Cluster is absorbed
    delete Cluster_2;
    Cluster_2 = nullptr;

    Set_Center(); // calculate center coordinates
}
/****************************************************************************************************/


/*** TCluster_3 *************************************************************************************/
NS_Clustering::TCluster_3::TCluster_3(void) noexcept :
    TClusteringElem(ID_CLUSTER_3, 0.0, 0.0, Cluster_3_Radius, false)
{}

NS_Clustering::TCluster_3::TCluster_3(TDot* const Dot1, TDot* const Dot2) noexcept :
    TClusteringElem(ID_CLUSTER_3, 0.0, 0.0, Cluster_3_Radius, false)
{
    MapDot[TCoordKey(Dot1->X, Dot1->Y)]  = Dot1;  // add dot 1
    Dot1->InCluster                      = true;  // dot 1 is now in the cluster
    MapDot[TCoordKey(Dot2->X, Dot2->Y)]  = Dot2;  // add dot 2
    Dot2->InCluster                      = true;  // dot 2 is now in the cluster
    Set_Center();                                 // calculate center coordinates
}

NS_Clustering::TCluster_3::TCluster_3(TClusteringElem* const ClusteringElem) noexcept :
    TClusteringElem(ID_CLUSTER_3, 0.0, 0.0, Cluster_3_Radius, false)
{
    if (ClusteringElem->Get_ClusteringElem_Id() < ID_CLUSTER_3)
    {
        switch (ClusteringElem->Get_ClusteringElem_Id())
        {
            case ID_DOT:       { MapDot[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]        = dynamic_cast<TDot*>(ClusteringElem); break; }
            case ID_CLUSTER_1: { MapCluster_1[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]  = dynamic_cast<TCluster_1*>(ClusteringElem); break; }
            case ID_CLUSTER_2: { MapCluster_2[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]  = dynamic_cast<TCluster_2*>(ClusteringElem); break; }
            default: break;
        }

        ClusteringElem->InCluster = true; // element is now in the cluster

        // Set center coordinates
        X  = ClusteringElem->X;
        Y  = ClusteringElem->Y;
    }
}

NS_Clustering::TCluster_3::TCluster_3(TClusteringElem* const ClusteringElem1, TClusteringElem* const ClusteringElem2) noexcept :
    TClusteringElem(ID_CLUSTER_3, 0.0, 0.0, Cluster_3_Radius, false)
{
    if (ClusteringElem1->Get_ClusteringElem_Id() < ID_CLUSTER_3 && ClusteringElem2->Get_ClusteringElem_Id() < ID_CLUSTER_3)
    {
        switch (ClusteringElem1->Get_ClusteringElem_Id())
        {
            case ID_DOT:       { MapDot[TCoordKey(ClusteringElem1->X, ClusteringElem1->Y)]        = dynamic_cast<TDot*>(ClusteringElem1); break; }
            case ID_CLUSTER_1: { MapCluster_1[TCoordKey(ClusteringElem1->X, ClusteringElem1->Y)]  = dynamic_cast<TCluster_1*>(ClusteringElem1); break; }
            case ID_CLUSTER_2: { MapCluster_2[TCoordKey(ClusteringElem1->X, ClusteringElem1->Y)]  = dynamic_cast<TCluster_2*>(ClusteringElem1); break; }
            default: break;
        }

        switch (ClusteringElem2->Get_ClusteringElem_Id())
        {
            case ID_DOT:        { MapDot[TCoordKey(ClusteringElem2->X, ClusteringElem2->Y)]        = dynamic_cast<TDot*>(ClusteringElem2); break; }
            case ID_CLUSTER_1:  { MapCluster_1[TCoordKey(ClusteringElem2->X, ClusteringElem2->Y)]  = dynamic_cast<TCluster_1*>(ClusteringElem2); break; }
            case ID_CLUSTER_2:  { MapCluster_2[TCoordKey(ClusteringElem2->X, ClusteringElem2->Y)]  = dynamic_cast<TCluster_2*>(ClusteringElem2); break; }
            default: break;
        }

        ClusteringElem1->InCluster  = true;  // element 1 is now in the cluster
        ClusteringElem2->InCluster  = true;  // element 2 is now in the cluster
        Set_Center();                        // calculate center coordinates
    }
}

// Calculate center coordinates
void NS_Clustering::TCluster_3::Set_Center(void) noexcept
{
    if (!MapDot.empty() || !MapCluster_1.empty() || !MapCluster_2.empty())
    {
        const std::size_t  size   = MapDot.size() + MapCluster_1.size() + MapCluster_2.size();
        double             sum_x  = 0.0;
        double             sum_y  = 0.0;

        for (const auto& dot : MapDot)
        {
            sum_x  += dot.first.X;
            sum_y  += dot.first.Y;
        }

        for (const auto& c1 : MapCluster_1)
        {
            sum_x  += c1.first.X;
            sum_y  += c1.first.Y;
        }

        for (const auto& c2 : MapCluster_2)
        {
            sum_x  += c2.first.X;
            sum_y  += c2.first.Y;
        }

        X  = sum_x / size;
        Y  = sum_y / size;
    }
    else
    {
        X  = 0.0;
        Y  = 0.0;
    }
}

// Get the amount of data
std::size_t NS_Clustering::TCluster_3::Get_Count(void) const noexcept
{
    return ClusteringElem_Get_Count(MapDot) + ClusteringElem_Get_Count(MapCluster_1) + ClusteringElem_Get_Count(MapCluster_2);
}

// Get data for all dots
std::set<uint64_t> NS_Clustering::TCluster_3::Get_Data(void) const noexcept
{
    std::set<uint64_t> result;

    for (const auto& dot : MapDot)
        result.insert(dot.second->SetData.cbegin(), dot.second->SetData.cend());

    for (const auto& c1 : MapCluster_1)
    {
        const std::set<uint64_t> c1_data = c1.second->Get_Data();

        result.insert(c1_data.cbegin(), c1_data.cend());
    }

    for (const auto& c2 : MapCluster_2)
    {
        const std::set<uint64_t> c2_data = c2.second->Get_Data();

        result.insert(c2_data.cbegin(), c2_data.cend());
    }

    return result;
}

// Check if it is worth adding a clustering element
bool NS_Clustering::TCluster_3::Check_ClusteringElem(const TClusteringElem* const ClusteringElem) const noexcept
{
    return ClusteringElem->Get_ClusteringElem_Id() < ID_CLUSTER_3 && Coord_In_Circle(X, Y, R, ClusteringElem->X, ClusteringElem->Y);
}

// Add clustering element
void NS_Clustering::TCluster_3::Add_ClusteringElem(TClusteringElem* const ClusteringElem) noexcept
{
    if (ClusteringElem->Get_ClusteringElem_Id() < ID_CLUSTER_3)
    {
        switch (ClusteringElem->Get_ClusteringElem_Id())
        {
            case ID_DOT:       { MapDot[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]        = dynamic_cast<TDot*>(ClusteringElem); break; }
            case ID_CLUSTER_1: { MapCluster_1[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]  = dynamic_cast<TCluster_1*>(ClusteringElem); break; }
            case ID_CLUSTER_2: { MapCluster_2[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]  = dynamic_cast<TCluster_2*>(ClusteringElem); break; }
            default: break;
        }

        ClusteringElem->InCluster = true;  // element is now in the cluster
        Set_Center();                      // calculate center coordinates
    }
}

// Check if it is worth adding a cluster_3
bool NS_Clustering::TCluster_3::Check_Cluster_3(const TCluster_3* const Cluster_3) const noexcept
{
    return Coord_In_Circle(X, Y, R, Cluster_3->X, Cluster_3->Y);
}

// Absorb cluster_3
void NS_Clustering::TCluster_3::Eat_Cluster_3(TCluster_3*& Cluster_3) noexcept
{
    MapDot.insert(Cluster_3->MapDot.cbegin(), Cluster_3->MapDot.cend());                    // absorb all dots
    MapCluster_1.insert(Cluster_3->MapCluster_1.cbegin(), Cluster_3->MapCluster_1.cend());  // absorb all clusters_1
    MapCluster_2.insert(Cluster_3->MapCluster_2.cbegin(), Cluster_3->MapCluster_2.cend());  // absorb all clusters_2

    // Cluster is absorbed
    delete Cluster_3;
    Cluster_3 = nullptr;

    Set_Center(); // calculate center coordinates
}
/****************************************************************************************************/


/*** TCluster_4 *************************************************************************************/
NS_Clustering::TCluster_4::TCluster_4(void) noexcept :
    TClusteringElem(ID_CLUSTER_4, 0.0, 0.0, Cluster_4_Radius, false)
{}

NS_Clustering::TCluster_4::TCluster_4(TDot* const Dot1, TDot* const Dot2) noexcept :
    TClusteringElem(ID_CLUSTER_4, 0.0, 0.0, Cluster_4_Radius, false)
{
    MapDot[TCoordKey(Dot1->X, Dot1->Y)]  = Dot1;  // add dot 1
    Dot1->InCluster                      = true;  // dot 1 is now in the cluster
    MapDot[TCoordKey(Dot2->X, Dot2->Y)]  = Dot2;  // add dot 2
    Dot2->InCluster                      = true;  // dot 2 is now in the cluster
    Set_Center();                                 // calculate center coordinates
}

NS_Clustering::TCluster_4::TCluster_4(TClusteringElem* const ClusteringElem) noexcept :
    TClusteringElem(ID_CLUSTER_4, 0.0, 0.0, Cluster_4_Radius, false)
{
    if (ClusteringElem->Get_ClusteringElem_Id() < ID_CLUSTER_4)
    {
        switch (ClusteringElem->Get_ClusteringElem_Id())
        {
            case ID_DOT:       { MapDot[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]        = dynamic_cast<TDot*>(ClusteringElem); break; }
            case ID_CLUSTER_1: { MapCluster_1[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]  = dynamic_cast<TCluster_1*>(ClusteringElem); break; }
            case ID_CLUSTER_2: { MapCluster_2[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]  = dynamic_cast<TCluster_2*>(ClusteringElem); break; }
            case ID_CLUSTER_3: { MapCluster_3[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]  = dynamic_cast<TCluster_3*>(ClusteringElem); break; }
            default: break;
        }

        ClusteringElem->InCluster = true; // element is now in the cluster

        // Set center coordinates
        X  = ClusteringElem->X;
        Y  = ClusteringElem->Y;
    }
}

NS_Clustering::TCluster_4::TCluster_4(TClusteringElem* const ClusteringElem1, TClusteringElem* const ClusteringElem2) noexcept :
    TClusteringElem(ID_CLUSTER_4, 0.0, 0.0, Cluster_4_Radius, false)
{
    if (ClusteringElem1->Get_ClusteringElem_Id() < ID_CLUSTER_4 && ClusteringElem2->Get_ClusteringElem_Id() < ID_CLUSTER_4)
    {
        switch (ClusteringElem1->Get_ClusteringElem_Id())
        {
            case ID_DOT:       { MapDot[TCoordKey(ClusteringElem1->X, ClusteringElem1->Y)]        = dynamic_cast<TDot*>(ClusteringElem1); break; }
            case ID_CLUSTER_1: { MapCluster_1[TCoordKey(ClusteringElem1->X, ClusteringElem1->Y)]  = dynamic_cast<TCluster_1*>(ClusteringElem1); break; }
            case ID_CLUSTER_2: { MapCluster_2[TCoordKey(ClusteringElem1->X, ClusteringElem1->Y)]  = dynamic_cast<TCluster_2*>(ClusteringElem1); break; }
            case ID_CLUSTER_3: { MapCluster_3[TCoordKey(ClusteringElem1->X, ClusteringElem1->Y)]  = dynamic_cast<TCluster_3*>(ClusteringElem1); break; }
            default: break;
        }

        switch (ClusteringElem2->Get_ClusteringElem_Id())
        {
            case ID_DOT:       { MapDot[TCoordKey(ClusteringElem2->X, ClusteringElem2->Y)]        = dynamic_cast<TDot*>(ClusteringElem2); break; }
            case ID_CLUSTER_1: { MapCluster_1[TCoordKey(ClusteringElem2->X, ClusteringElem2->Y)]  = dynamic_cast<TCluster_1*>(ClusteringElem2); break; }
            case ID_CLUSTER_2: { MapCluster_2[TCoordKey(ClusteringElem2->X, ClusteringElem2->Y)]  = dynamic_cast<TCluster_2*>(ClusteringElem2); break; }
            case ID_CLUSTER_3: { MapCluster_3[TCoordKey(ClusteringElem2->X, ClusteringElem2->Y)]  = dynamic_cast<TCluster_3*>(ClusteringElem2); break; }
            default: break;
        }

        ClusteringElem1->InCluster  = true;  // element 1 is now in the cluster
        ClusteringElem2->InCluster  = true;  // element 2 is now in the cluster
        Set_Center();                        // calculate center coordinates
    }
}

// Calculate center coordinates
void NS_Clustering::TCluster_4::Set_Center(void) noexcept
{
    if (!MapDot.empty() || !MapCluster_1.empty() || !MapCluster_2.empty() || !MapCluster_3.empty())
    {
        const std::size_t  size   = MapDot.size() + MapCluster_1.size() + MapCluster_2.size() + MapCluster_3.size();
        double             sum_x  = 0.0;
        double             sum_y  = 0.0;

        for (const auto& dot : MapDot)
        {
            sum_x  += dot.first.X;
            sum_y  += dot.first.Y;
        }

        for (const auto& c1 : MapCluster_1)
        {
            sum_x  += c1.first.X;
            sum_y  += c1.first.Y;
        }

        for (const auto& c2 : MapCluster_2)
        {
            sum_x  += c2.first.X;
            sum_y  += c2.first.Y;
        }

        for (const auto& c3 : MapCluster_3)
        {
            sum_x  += c3.first.X;
            sum_y  += c3.first.Y;
        }

        X  = sum_x / size;
        Y  = sum_y / size;
    }
    else
    {
        X  = 0.0;
        Y  = 0.0;
    }
}

// Get the amount of data
std::size_t NS_Clustering::TCluster_4::Get_Count(void) const noexcept
{
    return ClusteringElem_Get_Count(MapDot) + ClusteringElem_Get_Count(MapCluster_1) + ClusteringElem_Get_Count(MapCluster_2) + ClusteringElem_Get_Count(MapCluster_3);
}

// Get data for all dots
std::set<uint64_t> NS_Clustering::TCluster_4::Get_Data(void) const noexcept
{
    std::set<uint64_t> result;

    for (const auto& dot : MapDot)
        result.insert(dot.second->SetData.cbegin(), dot.second->SetData.cend());

    for (const auto& c1 : MapCluster_1)
    {
        const std::set<uint64_t> c1_data = c1.second->Get_Data();

        result.insert(c1_data.cbegin(), c1_data.cend());
    }

    for (const auto& c2 : MapCluster_2)
    {
        const std::set<uint64_t> c2_data = c2.second->Get_Data();

        result.insert(c2_data.cbegin(), c2_data.cend());
    }

    for (const auto& c3 : MapCluster_3)
    {
        const std::set<uint64_t> c3_data = c3.second->Get_Data();

        result.insert(c3_data.cbegin(), c3_data.cend());
    }

    return result;
}

// Check if it is worth adding a clustering element
bool NS_Clustering::TCluster_4::Check_ClusteringElem(const TClusteringElem* const ClusteringElem) const noexcept
{
    return ClusteringElem->Get_ClusteringElem_Id() < ID_CLUSTER_4 && Coord_In_Circle(X, Y, R, ClusteringElem->X, ClusteringElem->Y);
}

// Add clustering element
void NS_Clustering::TCluster_4::Add_ClusteringElem(TClusteringElem* const ClusteringElem) noexcept
{
    if (ClusteringElem->Get_ClusteringElem_Id() < ID_CLUSTER_4)
    {
        switch (ClusteringElem->Get_ClusteringElem_Id())
        {
            case ID_DOT:       { MapDot[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]        = dynamic_cast<TDot*>(ClusteringElem); break; }
            case ID_CLUSTER_1: { MapCluster_1[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]  = dynamic_cast<TCluster_1*>(ClusteringElem); break; }
            case ID_CLUSTER_2: { MapCluster_2[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]  = dynamic_cast<TCluster_2*>(ClusteringElem); break; }
            case ID_CLUSTER_3: { MapCluster_3[TCoordKey(ClusteringElem->X, ClusteringElem->Y)]  = dynamic_cast<TCluster_3*>(ClusteringElem); break; }
            default: break;
        }

        ClusteringElem->InCluster = true;  // element is now in the cluster
        Set_Center();                      // calculate center coordinates
    }
}

// Check if it is worth adding a cluster_4
bool NS_Clustering::TCluster_4::Check_Cluster_4(const TCluster_4* const Cluster_4) const noexcept
{
    return Coord_In_Circle(X, Y, R, Cluster_4->X, Cluster_4->Y);
}

// Absorb cluster_4
void NS_Clustering::TCluster_4::Eat_Cluster_4(TCluster_4*& Cluster_4) noexcept
{
    MapDot.insert(Cluster_4->MapDot.cbegin(), Cluster_4->MapDot.cend());                    // absorb all dots
    MapCluster_1.insert(Cluster_4->MapCluster_1.cbegin(), Cluster_4->MapCluster_1.cend());  // absorb all clusters_1
    MapCluster_2.insert(Cluster_4->MapCluster_2.cbegin(), Cluster_4->MapCluster_2.cend());  // absorb all clusters_2
    MapCluster_3.insert(Cluster_4->MapCluster_3.cbegin(), Cluster_4->MapCluster_3.cend());  // absorb all clusters_3

    // Cluster is absorbed
    delete Cluster_4;
    Cluster_4 = nullptr;

    Set_Center(); // calculate center coordinates
}
/****************************************************************************************************/


// Create set of clusters_1
void NS_Clustering::Create_Clusters_1(
    TMapDot&        MapDot,
    TMapCluster_1&  MapCluster_1,
    const bool      SingleDot_To_Cluster_1)
{
    std::list<TCluster_1*>  list_cluster_1;            // temporary list of clusters_1
    std::list<TDot*>        list_dot_without_cluster;  // dots without cluster
    bool                    merged = true;             // clustering elements was merged
    bool                    dot_i_removed;             // i-th dot from set_dot_without_cluster was removed (cit_dwc_i)
    bool                    c1_deleted;                // cluster_1 was absorbed by another cluster_1

    for (auto& dot : MapDot) // collect dot addresses, assuming that none of them are in any cluster
    {
        dot.second->InCluster = false;
        list_dot_without_cluster.push_back(dot.second);
    }

    while (merged) // merge into clusters_1, while at least something is merged
    {
        merged = false;

        // Pass through all dots without any cluster
        for (auto cit_dwc_i = list_dot_without_cluster.cbegin(), cend_dwc = list_dot_without_cluster.cend(); cit_dwc_i != cend_dwc;)
        {
            dot_i_removed = false;

            // Pass through all dots, starting from the next after the current
            for (auto cit_dwc_j = ++std::list<TDot*>::const_iterator(cit_dwc_i); cit_dwc_j != cend_dwc;)
            {
                if ((*cit_dwc_i)->InCluster) // current i-th dot is in some cluster
                {
                    // Remove it from the list so that it no longer checks, following becomes current now
                    cit_dwc_i = list_dot_without_cluster.erase(cit_dwc_i);

                    dot_i_removed = true; // remember that the i-th dot was deleted

                    break; // need to check the remaining dots to the right of the current i-th
                }

                if (!(*cit_dwc_j)->InCluster) // the dot with which the comparison is not located in any cluster
                {
                    // If dots can be merged into a cluster_1
                    if (Coord_In_Circle((*cit_dwc_i)->X, (*cit_dwc_i)->Y, Cluster_1_Radius, (*cit_dwc_j)->X, (*cit_dwc_j)->Y))
                    {
                        TCluster_1* cluster = new TCluster_1(*cit_dwc_i, *cit_dwc_j); // create cluster_1 from two dots

                        list_cluster_1.push_back(cluster);                           // add the created cluster_1 to the list
                        merged         = true;                                       // remember that there was a merge
                        list_dot_without_cluster.erase(cit_dwc_j);                   // remove the j-th dot from the list so as not to check it anymore
                        cit_dwc_i      = list_dot_without_cluster.erase(cit_dwc_i);  // delete the i-th dot from the list, following becomes current now
                        dot_i_removed  = true;                                       // remember that the i-th dot was deleted

                        break; // pair found, search next
                    }
                    else // merge fails
                        ++cit_dwc_j; // go to the next j-th dot
                }
                else // the dot with which the comparison is located in any cluster
                    cit_dwc_j = list_dot_without_cluster.erase(cit_dwc_j); // remove it from the list so that it no longer checks
            }

            if (!dot_i_removed) // i-th dot has not been deleted
                ++cit_dwc_i; // go to the next i-th dot
        }

        // Add dots to clusters_1, if possible
        for (auto c1 : list_cluster_1) // pass through all clusters_1
        {
            // Pass through all dots without any cluster
            for (auto cit_dwc_i = list_dot_without_cluster.cbegin(), cend_dwc = list_dot_without_cluster.cend(); cit_dwc_i != cend_dwc;)
            {
                if (c1->Check_Dot(*cit_dwc_i)) // dot can be added to the cluster
                {
                    c1->Add_Dot(*cit_dwc_i);                                 // add dot to cluster
                    cit_dwc_i  = list_dot_without_cluster.erase(cit_dwc_i);  // remove dot from list of dots without cluster and proceed to check the next
                    merged     = true;                                       // remember that there was a merge
                }
                else
                    ++cit_dwc_i; // go to the next dot
            }
        }

        // Merge clusters_1, if possible
        for (auto it_c1_i = list_cluster_1.begin(), end_c1 = list_cluster_1.end(); it_c1_i != end_c1;) // pass through all clusters_1
        {
            c1_deleted = false;

            // Pass through all clusters_1, starting from the next after the current
            for (auto it_c1_j = ++std::list<TCluster_1*>::iterator(it_c1_i); it_c1_j != end_c1; ++it_c1_j)
            {
                if ((*it_c1_j)->Check_Cluster_1(*it_c1_i)) // clusters_1 can be merged
                {
                    (*it_c1_j)->Eat_Cluster_1(*it_c1_i);          // absorb a cluster_1 on the left in the set by a cluster_1 on the right in the set
                    it_c1_i     = list_cluster_1.erase(it_c1_i);  // go to the next cluster_1, from which clusters_1 to the right will be checked
                    c1_deleted  = true;                           // remember that there was a deletion of the current cluster_1
                    merged      = true;                           // remember that there was a merge

                    break;
                }
            }

            if (!c1_deleted)
                ++it_c1_i; // go to the next cluster_1, from which clusters_1 to the right will be checked
        }
    }

    if (SingleDot_To_Cluster_1) // need to turn the remaining single dot into clusters_1
    {
        for (const auto dot : list_dot_without_cluster) // make cluster_1 from each dot and add it to the list
            list_cluster_1.push_back(new TCluster_1(dot));

        do // merge into clusters_1, while at least something is merged
        {
            merged = false;

            // Merge clusters_1, if possible
            for (auto it_c1_i = list_cluster_1.begin(), end_c1 = list_cluster_1.end(); it_c1_i != end_c1;) // pass through all clusters_1
            {
                c1_deleted = false;

                // Pass through all clusters_1, starting from the next after the current
                for (auto it_c1_j = ++std::list<TCluster_1*>::iterator(it_c1_i); it_c1_j != end_c1; ++it_c1_j)
                {
                    if ((*it_c1_j)->Check_Cluster_1(*it_c1_i)) // clusters_1 can be merged
                    {
                        (*it_c1_j)->Eat_Cluster_1(*it_c1_i);          // absorb a cluster_1 on the left in the set by a cluster_1 on the right in the set
                        it_c1_i     = list_cluster_1.erase(it_c1_i);  // go to the next cluster_1, from which clusters_1 to the right will be checked
                        c1_deleted  = true;                           // remember that there was a deletion of the current cluster_1
                        merged      = true;                           // remember that there was a merge

                        break;
                    }
                }

                if (!c1_deleted)
                    ++it_c1_i; // go to the next cluster_1, from which clusters_1 to the right will be checked
            }
        }
        while (merged);
    }

    // When merging clusters_1 and adding dots to them, their centers no longer match the key in the set, need to recreate the set
    Clear_Map_Of_ClusteringElem(MapCluster_1); // clear the set of clusters_1

    // Move clusters_1 from the temporary list to the required
    for (const auto c1 : list_cluster_1)
        MapCluster_1.emplace(TCoordKey(c1->X, c1->Y), c1);
}

// Create set of clusters_2
void NS_Clustering::Create_Clusters_2(
    TMapDot&        MapDot,
    TMapCluster_1&  MapCluster_1,
    TMapCluster_2&  MapCluster_2,
    const bool      SingleClusteringElem_To_Cluster_2)
{
    std::list<TCluster_2*>       list_cluster_2;             // temporary list of clusters_2
    std::list<TClusteringElem*>  list_elem_without_cluster;  // clustering elements without cluster
    bool                         merged = true;              // clustering elements was merged
    bool                         elem_i_removed;             // i-th element from set_elem_without_cluster was removed (cit_ewc_i)
    bool                         c2_deleted;                 // cluster_2 was absorbed by another cluster_2

    for (const auto& dot : MapDot) // collect dot addresses that are not in any cluster
    {
        if (!dot.second->InCluster)
            list_elem_without_cluster.push_back(dot.second);
    }

    for (auto& c1 : MapCluster_1) // collect cluster_1 addresses, assuming that none of them are in any cluster
    {
        c1.second->InCluster = false;
        list_elem_without_cluster.push_back(c1.second);
    }

    while (merged) // merge into clusters_2, while at least something is merged
    {
        merged = false;

        // Pass through all clustering elements without any cluster
        for (auto cit_ewc_i = list_elem_without_cluster.cbegin(), cend_ewc = list_elem_without_cluster.cend(); cit_ewc_i != cend_ewc;)
        {
            elem_i_removed = false;

            // Pass through all clustering elements, starting from the next after the current
            for (auto cit_ewc_j = ++std::list<TClusteringElem*>::const_iterator(cit_ewc_i); cit_ewc_j != cend_ewc;)
            {
                if ((*cit_ewc_i)->InCluster) // current i-th element is in some cluster
                {
                    // Remove it from the list so that it no longer checks, following becomes current now
                    cit_ewc_i = list_elem_without_cluster.erase(cit_ewc_i);

                    elem_i_removed = true; // remember that the i-th element was deleted

                    break; // need to check the remaining elements to the right of the current i-th
                }

                if (!(*cit_ewc_j)->InCluster) // the element with which the comparison is not located in any cluster
                {
                    // If elements can be merged into a cluster_2
                    if (Coord_In_Circle((*cit_ewc_i)->X, (*cit_ewc_i)->Y, Cluster_2_Radius, (*cit_ewc_j)->X, (*cit_ewc_j)->Y))
                    {
                        TCluster_2* cluster = new TCluster_2(*cit_ewc_i, *cit_ewc_j); // create cluster_2 from two elements

                        list_cluster_2.push_back(cluster);                             // add the created cluster_2 to the list
                        merged          = true;                                        // remember that there was a merge
                        list_elem_without_cluster.erase(cit_ewc_j);                    // remove the j-th element from the list so as not to check it anymore
                        cit_ewc_i       = list_elem_without_cluster.erase(cit_ewc_i);  // delete the i-th element from the list, following becomes current now
                        elem_i_removed  = true;                                        // remember that the i-th element was deleted

                        break; // pair found, search next
                    }
                    else // merge fails
                        ++cit_ewc_j; // go to the next j-th element
                }
                else // the element with which the comparison is located in any cluster
                    cit_ewc_j = list_elem_without_cluster.erase(cit_ewc_j); // remove it from the list so that it no longer checks
            }

            if (!elem_i_removed) // i-th element has not been deleted
                ++cit_ewc_i; // go to the next i-th element
        }

        // Add elements to clusters_2, if possible
        for (auto c2 : list_cluster_2) // pass through all clusters_2
        {
            // Pass through all elements without any cluster
            for (auto cit_ewc_i = list_elem_without_cluster.cbegin(), cend_ewc = list_elem_without_cluster.cend(); cit_ewc_i != cend_ewc;)
            {
                if (c2->Check_ClusteringElem(*cit_ewc_i)) // element can be added to the cluster
                {
                    c2->Add_ClusteringElem(*cit_ewc_i);                       // add element to cluster
                    cit_ewc_i  = list_elem_without_cluster.erase(cit_ewc_i);  // remove element from list of elements without cluster and proceed to check the next
                    merged     = true;                                        // remember that there was a merge
                }
                else
                    ++cit_ewc_i; // go to the next element
            }
        }

        // Merge clusters_2, if possible
        for (auto it_c2_i = list_cluster_2.begin(), end_c2 = list_cluster_2.end(); it_c2_i != end_c2;) // pass through all clusters_2
        {
            c2_deleted = false;

            // Pass through all clusters_2, starting from the next after the current
            for (auto it_c2_j = ++std::list<TCluster_2*>::iterator(it_c2_i); it_c2_j != end_c2; ++it_c2_j)
            {
                if ((*it_c2_j)->Check_Cluster_2(*it_c2_i)) // clusters_2 can be merged
                {
                    (*it_c2_j)->Eat_Cluster_2(*it_c2_i);          // absorb a cluster_2 on the left in the set by a cluster_2 on the right in the set
                    it_c2_i     = list_cluster_2.erase(it_c2_i);  // go to the next cluster_2, from which clusters_2 to the right will be checked
                    c2_deleted  = true;                           // remember that there was a deletion of the current cluster_2
                    merged      = true;                           // remember that there was a merge

                    break;
                }
            }

            if (!c2_deleted)
                ++it_c2_i; // go to the next cluster_2, from which clusters_2 to the right will be checked
        }
    }

    if (SingleClusteringElem_To_Cluster_2) // need to turn the remaining single clustering elements into clusters_2
    {
        for (const auto elem : list_elem_without_cluster) // make cluster_2 from each element and add it to the list
            list_cluster_2.push_back(new TCluster_2(elem));

        do // merge into clusters_2, while at least something is merged
        {
            merged = false;

            // Merge clusters_2, if possible
            for (auto it_c2_i = list_cluster_2.begin(), end_c2 = list_cluster_2.end(); it_c2_i != end_c2;) // pass through all clusters_2
            {
                c2_deleted = false;

                // Pass through all clusters_2, starting from the next after the current
                for (auto it_c2_j = ++std::list<TCluster_2*>::iterator(it_c2_i); it_c2_j != end_c2; ++it_c2_j)
                {
                    if ((*it_c2_j)->Check_Cluster_2(*it_c2_i)) // clusters_2 can be merged
                    {
                        (*it_c2_j)->Eat_Cluster_2(*it_c2_i);          // absorb a cluster_2 on the left in the set by a cluster_2 on the right in the set
                        it_c2_i     = list_cluster_2.erase(it_c2_i);  // go to the next cluster_2, from which clusters_2 to the right will be checked
                        c2_deleted  = true;                           // remember that there was a deletion of the current cluster_2
                        merged      = true;                           // remember that there was a merge

                        break;
                    }
                }

                if (!c2_deleted)
                    ++it_c2_i; // go to the next cluster_2, from which clusters_2 to the right will be checked
            }
        }
        while (merged);
    }

    // When merging clusters_2 and adding elements to them, their centers no longer match the key in the set, need to recreate the set
    Clear_Map_Of_ClusteringElem(MapCluster_2); // clear the set of clusters_2

    // Move clusters_2 from the temporary list to the required
    for (const auto c2 : list_cluster_2)
        MapCluster_2.emplace(TCoordKey(c2->X, c2->Y), c2);
}

// Create set of clusters_3
void NS_Clustering::Create_Clusters_3(
    TMapDot&        MapDot,
    TMapCluster_1&  MapCluster_1,
    TMapCluster_2&  MapCluster_2,
    TMapCluster_3&  MapCluster_3,
    const bool      SingleClusteringElem_To_Cluster_3)
{
    std::list<TCluster_3*>       list_cluster_3;             // temporary list of clusters_3
    std::list<TClusteringElem*>  list_elem_without_cluster;  // clustering elements without cluster
    bool                         merged = true;              // clustering elements was merged
    bool                         elem_i_removed;             // i-th element from set_elem_without_cluster was removed (cit_ewc_i)
    bool                         c3_deleted;                 // cluster_3 was absorbed by another cluster_3

    for (const auto& dot : MapDot) // collect dot addresses that are not in any cluster
    {
        if (!dot.second->InCluster)
            list_elem_without_cluster.push_back(dot.second);
    }

    for (const auto& c1 : MapCluster_1) // collect cluster_1 addresses that are not in any cluster
    {
        if (!c1.second->InCluster)
            list_elem_without_cluster.push_back(c1.second);
    }

    for (auto& c2 : MapCluster_2) // collect cluster_2 addresses, assuming that none of them are in any cluster
    {
        c2.second->InCluster = false;
        list_elem_without_cluster.push_back(c2.second);
    }

    while (merged) // merge into clusters_3, while at least something is merged
    {
        merged = false;

        // Pass through all clustering elements without any cluster
        for (auto cit_ewc_i = list_elem_without_cluster.cbegin(), cend_ewc = list_elem_without_cluster.cend(); cit_ewc_i != cend_ewc;)
        {
            elem_i_removed = false;

            // Pass through all clustering elements, starting from the next after the current
            for (auto cit_ewc_j = ++std::list<TClusteringElem*>::const_iterator(cit_ewc_i); cit_ewc_j != cend_ewc;)
            {
                if ((*cit_ewc_i)->InCluster) // current i-th element is in some cluster
                {
                    // Remove it from the list so that it no longer checks, following becomes current now
                    cit_ewc_i = list_elem_without_cluster.erase(cit_ewc_i);

                    elem_i_removed = true; // remember that the i-th element was deleted

                    break; // need to check the remaining elements to the right of the current i-th
                }

                if (!(*cit_ewc_j)->InCluster) // the element with which the comparison is not located in any cluster
                {
                    // If elements can be merged into a cluster_3
                    if (Coord_In_Circle((*cit_ewc_i)->X, (*cit_ewc_i)->Y, Cluster_3_Radius, (*cit_ewc_j)->X, (*cit_ewc_j)->Y))
                    {
                        TCluster_3* cluster = new TCluster_3(*cit_ewc_i, *cit_ewc_j); // create cluster_3 from two elements

                        list_cluster_3.push_back(cluster);                             // add the created cluster_3 to the list
                        merged          = true;                                        // remember that there was a merge
                        list_elem_without_cluster.erase(cit_ewc_j);                    // remove the j-th element from the list so as not to check it anymore
                        cit_ewc_i       = list_elem_without_cluster.erase(cit_ewc_i);  // delete the i-th element from the list, following becomes current now
                        elem_i_removed  = true;                                        // remember that the i-th element was deleted

                        break; // pair found, search next
                    }
                    else // merge fails
                        ++cit_ewc_j; // go to the next j-th element
                }
                else // the element with which the comparison is located in any cluster
                    cit_ewc_j = list_elem_without_cluster.erase(cit_ewc_j); // remove it from the list so that it no longer checks
            }

            if (!elem_i_removed) // i-th element has not been deleted
                ++cit_ewc_i; // go to the next i-th element
        }

        // Add elements to clusters_3, if possible
        for (auto c3 : list_cluster_3) // pass through all clusters_3
        {
            // Pass through all elements without any cluster
            for (auto cit_ewc_i = list_elem_without_cluster.cbegin(), cend_ewc = list_elem_without_cluster.cend(); cit_ewc_i != cend_ewc;)
            {
                if (c3->Check_ClusteringElem(*cit_ewc_i)) // element can be added to the cluster
                {
                    c3->Add_ClusteringElem(*cit_ewc_i);                       // add element to cluster
                    cit_ewc_i  = list_elem_without_cluster.erase(cit_ewc_i);  // remove element from list of elements without cluster and proceed to check the next
                    merged     = true;                                        // remember that there was a merge
                }
                else
                    ++cit_ewc_i; // go to the next element
            }
        }

        // Merge clusters_3, if possible
        for (auto it_c3_i = list_cluster_3.begin(), end_c3 = list_cluster_3.end(); it_c3_i != end_c3;) // pass through all clusters_3
        {
            c3_deleted = false;

            // Pass through all clusters_3, starting from the next after the current
            for (auto it_c3_j = ++std::list<TCluster_3*>::iterator(it_c3_i); it_c3_j != end_c3; ++it_c3_j)
            {
                if ((*it_c3_j)->Check_Cluster_3(*it_c3_i)) // clusters_3 can be merged
                {
                    (*it_c3_j)->Eat_Cluster_3(*it_c3_i);          // absorb a cluster_3 on the left in the set by a cluster_3 on the right in the set
                    it_c3_i     = list_cluster_3.erase(it_c3_i);  // go to the next cluster_3, from which clusters_3 to the right will be checked
                    c3_deleted  = true;                           // remember that there was a deletion of the current cluster_3
                    merged      = true;                           // remember that there was a merge

                    break;
                }
            }

            if (!c3_deleted)
                ++it_c3_i; // go to the next cluster_3, from which clusters_3 to the right will be checked
        }
    }

    if (SingleClusteringElem_To_Cluster_3) // need to turn the remaining single clustering elements into clusters_3
    {
        for (const auto elem : list_elem_without_cluster) // make cluster_3 from each element and add it to the list
            list_cluster_3.push_back(new TCluster_3(elem));

        do // merge into clusters_3, while at least something is merged
        {
            merged = false;

            // Merge clusters_3, if possible
            for (auto it_c3_i = list_cluster_3.begin(), end_c3 = list_cluster_3.end(); it_c3_i != end_c3;) // pass through all clusters_3
            {
                c3_deleted = false;

                // Pass through all clusters_3, starting from the next after the current
                for (auto it_c3_j = ++std::list<TCluster_3*>::iterator(it_c3_i); it_c3_j != end_c3; ++it_c3_j)
                {
                    if ((*it_c3_j)->Check_Cluster_3(*it_c3_i)) // clusters_3 can be merged
                    {
                        (*it_c3_j)->Eat_Cluster_3(*it_c3_i);          // absorb a cluster_3 on the left in the set by a cluster_3 on the right in the set
                        it_c3_i     = list_cluster_3.erase(it_c3_i);  // go to the next cluster_3, from which clusters_3 to the right will be checked
                        c3_deleted  = true;                           // remember that there was a deletion of the current cluster_3
                        merged      = true;                           // remember that there was a merge

                        break;
                    }
                }

                if (!c3_deleted)
                    ++it_c3_i; // go to the next cluster_3, from which clusters_3 to the right will be checked
            }
        }
        while (merged);
    }

    // When merging clusters_3 and adding elements to them, their centers no longer match the key in the set, need to recreate the set
    Clear_Map_Of_ClusteringElem(MapCluster_3); // clear the set of clusters_3

    // Move clusters_3 from the temporary list to the required
    for (const auto c3 : list_cluster_3)
        MapCluster_3.emplace(TCoordKey(c3->X, c3->Y), c3);
}

// Create set of clusters_4
void NS_Clustering::Create_Clusters_4(
    TMapDot&        MapDot,
    TMapCluster_1&  MapCluster_1,
    TMapCluster_2&  MapCluster_2,
    TMapCluster_3&  MapCluster_3,
    TMapCluster_4&  MapCluster_4,
    const bool      SingleClusteringElem_To_Cluster_4)
{
    std::list<TCluster_4*>       list_cluster_4;             // temporary list of clusters_4
    std::list<TClusteringElem*>  list_elem_without_cluster;  // clustering elements without cluster
    bool                         merged = true;              // clustering elements was merged
    bool                         elem_i_removed;             // i-th element from set_elem_without_cluster was removed (cit_ewc_i)
    bool                         c4_deleted;                 // cluster_4 was absorbed by another cluster_4

    for (const auto& dot : MapDot) // collect dot addresses that are not in any cluster
    {
        if (!dot.second->InCluster)
            list_elem_without_cluster.push_back(dot.second);
    }

    for (const auto& c1 : MapCluster_1) // collect cluster_1 addresses that are not in any cluster
    {
        if (!c1.second->InCluster)
            list_elem_without_cluster.push_back(c1.second);
    }

    for (const auto& c2 : MapCluster_2) // collect cluster_2 addresses that are not in any cluster
    {
        if (!c2.second->InCluster)
            list_elem_without_cluster.push_back(c2.second);
    }

    for (auto& c3 : MapCluster_3) // collect cluster_3 addresses, assuming that none of them are in any cluster
    {
        c3.second->InCluster = false;
        list_elem_without_cluster.push_back(c3.second);
    }

    while (merged) // merge into clusters_4, while at least something is merged
    {
        merged = false;

        // Pass through all clustering elements without any cluster
        for (auto cit_ewc_i = list_elem_without_cluster.cbegin(), cend_ewc = list_elem_without_cluster.cend(); cit_ewc_i != cend_ewc;)
        {
            elem_i_removed = false;

            // Pass through all clustering elements, starting from the next after the current
            for (auto cit_ewc_j = ++std::list<TClusteringElem*>::const_iterator(cit_ewc_i); cit_ewc_j != cend_ewc;)
            {
                if ((*cit_ewc_i)->InCluster) // current i-th element is in some cluster
                {
                    // Remove it from the list so that it no longer checks, following becomes current now
                    cit_ewc_i = list_elem_without_cluster.erase(cit_ewc_i);

                    elem_i_removed = true; // remember that the i-th element was deleted

                    break; // need to check the remaining elements to the right of the current i-th
                }

                if (!(*cit_ewc_j)->InCluster) // the element with which the comparison is not located in any cluster
                {
                    // If elements can be merged into a cluster_4
                    if (Coord_In_Circle((*cit_ewc_i)->X, (*cit_ewc_i)->Y, Cluster_4_Radius, (*cit_ewc_j)->X, (*cit_ewc_j)->Y))
                    {
                        TCluster_4* cluster = new TCluster_4(*cit_ewc_i, *cit_ewc_j); // create cluster_4 from two elements

                        list_cluster_4.push_back(cluster);                             // add the created cluster_4 to the list
                        merged          = true;                                        // remember that there was a merge
                        list_elem_without_cluster.erase(cit_ewc_j);                    // remove the j-th element from the list so as not to check it anymore
                        cit_ewc_i       = list_elem_without_cluster.erase(cit_ewc_i);  // delete the i-th element from the list, following becomes current now
                        elem_i_removed  = true;                                        // remember that the i-th element was deleted

                        break; // pair found, search next
                    }
                    else // merge fails
                        ++cit_ewc_j; // go to the next j-th element
                }
                else // the element with which the comparison is located in any cluster
                    cit_ewc_j = list_elem_without_cluster.erase(cit_ewc_j); // remove it from the list so that it no longer checks
            }

            if (!elem_i_removed) // i-th element has not been deleted
                ++cit_ewc_i; // go to the next i-th element
        }

        // Add elements to clusters_4, if possible
        for (auto c4 : list_cluster_4) // pass through all clusters_4
        {
            // Pass through all elements without any cluster
            for (auto cit_ewc_i = list_elem_without_cluster.cbegin(), cend_ewc = list_elem_without_cluster.cend(); cit_ewc_i != cend_ewc;)
            {
                if (c4->Check_ClusteringElem(*cit_ewc_i)) // element can be added to the cluster
                {
                    c4->Add_ClusteringElem(*cit_ewc_i);                       // add element to cluster
                    cit_ewc_i  = list_elem_without_cluster.erase(cit_ewc_i);  // remove element from list of elements without cluster and proceed to check the next
                    merged     = true;                                        // remember that there was a merge
                }
                else
                    ++cit_ewc_i; // go to the next element
            }
        }

        // Merge clusters_4, if possible
        for (auto it_c4_i = list_cluster_4.begin(), end_c4 = list_cluster_4.end(); it_c4_i != end_c4;) // pass through all clusters_4
        {
            c4_deleted = false;

            // Pass through all clusters_4, starting from the next after the current
            for (auto it_c4_j = ++std::list<TCluster_4*>::iterator(it_c4_i); it_c4_j != end_c4; ++it_c4_j)
            {
                if ((*it_c4_j)->Check_Cluster_4(*it_c4_i)) // clusters_4 can be merged
                {
                    (*it_c4_j)->Eat_Cluster_4(*it_c4_i);          // absorb a cluster_4 on the left in the set by a cluster_4 on the right in the set
                    it_c4_i     = list_cluster_4.erase(it_c4_i);  // go to the next cluster_4, from which clusters_4 to the right will be checked
                    c4_deleted  = true;                           // remember that there was a deletion of the current cluster_4
                    merged      = true;                           // remember that there was a merge

                    break;
                }
            }

            if (!c4_deleted)
                ++it_c4_i; // go to the next cluster_4, from which clusters_4 to the right will be checked
        }
    }

    if (SingleClusteringElem_To_Cluster_4) // need to turn the remaining single clustering elements into clusters_4
    {
        for (const auto elem : list_elem_without_cluster) // make cluster_4 from each element and add it to the list
            list_cluster_4.push_back(new TCluster_4(elem));

        do // merge into clusters_4, while at least something is merged
        {
            merged = false;

            // Merge clusters_4, if possible
            for (auto it_c4_i = list_cluster_4.begin(), end_c4 = list_cluster_4.end(); it_c4_i != end_c4;) // pass through all clusters_4
            {
                c4_deleted = false;

                // Pass through all clusters_4, starting from the next after the current
                for (auto it_c4_j = ++std::list<TCluster_4*>::iterator(it_c4_i); it_c4_j != end_c4; ++it_c4_j)
                {
                    if ((*it_c4_j)->Check_Cluster_4(*it_c4_i)) // clusters_4 can be merged
                    {
                        (*it_c4_j)->Eat_Cluster_4(*it_c4_i);          // absorb a cluster_4 on the left in the set by a cluster_4 on the right in the set
                        it_c4_i     = list_cluster_4.erase(it_c4_i);  // go to the next cluster_4, from which clusters_4 to the right will be checked
                        c4_deleted  = true;                           // remember that there was a deletion of the current cluster_4
                        merged      = true;                           // remember that there was a merge

                        break;
                    }
                }

                if (!c4_deleted)
                    ++it_c4_i; // go to the next cluster_4, from which clusters_4 to the right will be checked
            }
        }
        while (merged);
    }

    // When merging clusters_4 and adding elements to them, their centers no longer match the key in the set, need to recreate the set
    Clear_Map_Of_ClusteringElem(MapCluster_4); // clear the set of clusters_4

    // Move clusters_4 from the temporary list to the required
    for (const auto c4 : list_cluster_4)
        MapCluster_4.emplace(TCoordKey(c4->X, c4->Y), c4);
}
