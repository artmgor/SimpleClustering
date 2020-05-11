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

#ifndef SimpleClusteringH
#define SimpleClusteringH

#include <set>
#include <map>

#include <cstdint>

namespace NS_Clustering
{
    // Cluster object identifiers
    static const uint16_t  ID_BASE_CLASS  = 0xFFFF;
    static const uint16_t  ID_DOT         = 0;
    static const uint16_t  ID_CLUSTER_1   = 1;
    static const uint16_t  ID_CLUSTER_2   = 2;
    static const uint16_t  ID_CLUSTER_3   = 3;
    static const uint16_t  ID_CLUSTER_4   = 4;

    extern double  Dot_Radius;
    extern double  Cluster_1_Radius;
    extern double  Cluster_2_Radius;
    extern double  Cluster_3_Radius;
    extern double  Cluster_4_Radius;

    /* An additional part of the radius - is added to the radius of the clustering object when checking the possibility of merging.
    It may be necessary when graphically displaying clustering elements so that it does not seem that the center of any element
    located on the edge of another element. */
    extern double AdditionalRadiusValue;

    // Whether to use the additional part of the radius when checking the possibility of merging clustering objects
    extern int16_t Use_AdditionalRadiusValue;


    class TCoordKey // coordinate key for containers
    {
    public:
        double  X;
        double  Y;


        TCoordKey(void) noexcept;
        TCoordKey(const double X_, const double Y_) noexcept;

        bool operator == (const TCoordKey& Obj) const noexcept;
        bool operator != (const TCoordKey& Obj) const noexcept;
        bool operator < (const TCoordKey& Obj) const noexcept;
    };

    class TClusteringElem // base class for all classes of clustering elements
    {
    protected:
        uint16_t ClusteringElem_Id; // unique identifier for classes of clustering elements

    public:
        double    X;
        double    Y;
        double    R;          // radius
        bool      InCluster;  // whether the item is in any cluster
        uint64_t  Tag;        // label (variable for any user data, for example, the address of any data)


        TClusteringElem(void) noexcept;
        explicit TClusteringElem(const uint16_t ClusteringElem_Id_) noexcept;
        TClusteringElem(const uint16_t ClusteringElem_Id_, const double X_, const double Y_, const double R_ = 0.0, const bool InCluster_ = false, const uint64_t Tag_ = 0xFFFFFFFFFFFFFFFFULL) noexcept;
        virtual ~TClusteringElem(void) noexcept;

        // Get the unique identifier of the clustering element
        uint16_t Get_ClusteringElem_Id(void) const noexcept;

        // Set label, if not already set (0xFFFFFFFFFFFFFFFFULL)
        void Set_Tag_If_Empty(const uint64_t Tag_) noexcept;
    };

    class TDot : public TClusteringElem
    {
    public:
        std::set<uint64_t> SetData; // dataset for dot (for example, the addresses of any data)


        TDot(void) noexcept;
        TDot(const double X_, const double Y_) noexcept;
        TDot(const double X_, const double Y_, const uint64_t Data) noexcept;

        // Get the amount of data
        std::size_t Get_Count(void) const noexcept;

        // Get data for all dots
        std::set<uint64_t> Get_Data(void) const noexcept;
    };

    using TMapDot   = std::map<TCoordKey, TDot*>;
    using TMapCDot  = std::map<TCoordKey, const TDot*>;

    class TCluster_1 : public TClusteringElem
    {
    public:
        TMapCDot MapDot; // dots


        TCluster_1(void) noexcept;
        explicit TCluster_1(TDot* const Dot) noexcept;
        TCluster_1(TDot* const Dot1, TDot* const Dot2) noexcept;

        // Calculate center coordinates
        void Set_Center(void) noexcept;

        // Get the amount of data
        std::size_t Get_Count(void) const noexcept;

        // Get data for all dots
        std::set<uint64_t> Get_Data(void) const noexcept;

        // Check if it is worth adding a dot
        bool Check_Dot(const TDot* const Dot) const noexcept;

        // Add dot
        void Add_Dot(TDot* const Dot) noexcept;

        // Check if it is worth adding a cluster_1
        bool Check_Cluster_1(const TCluster_1* const Cluster_1) const noexcept;

        // Absorb cluster_1
        void Eat_Cluster_1(TCluster_1*& Cluster_1) noexcept;
    };

    using TMapCluster_1   = std::map<TCoordKey, TCluster_1*>;
    using TMapCCluster_1  = std::map<TCoordKey, const TCluster_1*>;

    class TCluster_2 : public TClusteringElem
    {
    public:
        TMapCDot        MapDot;        // dots
        TMapCCluster_1  MapCluster_1;  // clusters_1


        TCluster_2(void) noexcept;
        TCluster_2(TDot* const Dot1, TDot* const Dot2) noexcept;
        explicit TCluster_2(TClusteringElem* const ClusteringElem) noexcept;
        TCluster_2(TClusteringElem* const ClusteringElem1, TClusteringElem* const ClusteringElem2) noexcept;

        // Calculate center coordinates
        void Set_Center(void) noexcept;

        // Get the amount of data
        std::size_t Get_Count(void) const noexcept;

        // Get data for all dots
        std::set<uint64_t> Get_Data(void) const noexcept;

        // Check if it is worth adding a clustering element
        bool Check_ClusteringElem(const TClusteringElem* const ClusteringElem) const noexcept;

        // Add clustering element
        void Add_ClusteringElem(TClusteringElem* const ClusteringElem) noexcept;

        // Check if it is worth adding a cluster_2
        bool Check_Cluster_2(const TCluster_2* const Cluster_2) const noexcept;

        // Absorb cluster_2
        void Eat_Cluster_2(TCluster_2*& Cluster_2) noexcept;
    };

    using TMapCluster_2   = std::map<TCoordKey, TCluster_2*>;
    using TMapCCluster_2  = std::map<TCoordKey, const TCluster_2*>;

    class TCluster_3 : public TClusteringElem
    {
    public:
        TMapCDot        MapDot;        // dots
        TMapCCluster_1  MapCluster_1;  // clusters_1
        TMapCCluster_2  MapCluster_2;  // clusters_2


        TCluster_3(void) noexcept;
        TCluster_3(TDot* const Dot1, TDot* const Dot2) noexcept;
        explicit TCluster_3(TClusteringElem* const ClusteringElem) noexcept;
        TCluster_3(TClusteringElem* const ClusteringElem1, TClusteringElem* const ClusteringElem2) noexcept;

        // Calculate center coordinates
        void Set_Center(void) noexcept;

        // Get the amount of data
        std::size_t Get_Count(void) const noexcept;

        // Get data for all dots
        std::set<uint64_t> Get_Data(void) const noexcept;

        // Check if it is worth adding a clustering element
        bool Check_ClusteringElem(const TClusteringElem* const ClusteringElem) const noexcept;

        // Add clustering element
        void Add_ClusteringElem(TClusteringElem* const ClusteringElem) noexcept;

        // Check if it is worth adding a cluster_3
        bool Check_Cluster_3(const TCluster_3* const Cluster_3) const noexcept;

        // Absorb cluster_3
        void Eat_Cluster_3(TCluster_3*& Cluster_3) noexcept;
    };

    using TMapCluster_3   = std::map<TCoordKey, TCluster_3*>;
    using TMapCCluster_3  = std::map<TCoordKey, const TCluster_3*>;

    class TCluster_4 : public TClusteringElem
    {
    public:
        TMapCDot        MapDot;        // dots
        TMapCCluster_1  MapCluster_1;  // clusters_1
        TMapCCluster_2  MapCluster_2;  // clusters_2
        TMapCCluster_3  MapCluster_3;  // clusters_3


        TCluster_4(void) noexcept;
        TCluster_4(TDot* const Dot1, TDot* const Dot2) noexcept;
        explicit TCluster_4(TClusteringElem* const ClusteringElem) noexcept;
        TCluster_4(TClusteringElem* const ClusteringElem1, TClusteringElem* const ClusteringElem2) noexcept;

        // Calculate center coordinates
        void Set_Center(void) noexcept;

        // Get the amount of data
        std::size_t Get_Count(void) const noexcept;

        // Get data for all dots
        std::set<uint64_t> Get_Data(void) const noexcept;

        // Check if it is worth adding a clustering element
        bool Check_ClusteringElem(const TClusteringElem* const ClusteringElem) const noexcept;

        // Add clustering element
        void Add_ClusteringElem(TClusteringElem* const ClusteringElem) noexcept;

        // Check if it is worth adding a cluster_4
        bool Check_Cluster_4(const TCluster_4* const Cluster_4) const noexcept;

        // Absorb cluster_4
        void Eat_Cluster_4(TCluster_4*& Cluster_4) noexcept;
    };

    using TMapCluster_4   = std::map<TCoordKey, TCluster_4*>;
    using TMapCCluster_4  = std::map<TCoordKey, const TCluster_4*>;

    // Get the amount of data for clustering element
    template <typename Type> inline std::size_t ClusteringElem_Get_Count(const std::map<TCoordKey, Type*>& MapClusteringElem) noexcept
    {
        std::size_t result = 0;

        for (auto cit = MapClusteringElem.cbegin(), cend = MapClusteringElem.cend(); cit != cend; ++cit)
            result += cit->second->Get_Count();

        return result;
    }

    // Clear set of clustering elements
    template <typename Type> inline void Clear_Map_Of_ClusteringElem(std::map<TCoordKey, Type*>& MapClusteringElem) noexcept
    {
        for (auto it = MapClusteringElem.begin(), end = MapClusteringElem.end(); it != end; ++it)
            delete it->second;

        MapClusteringElem.clear();
    }

    // Create set of clusters_1
    void Create_Clusters_1(
        TMapDot&        MapDot,
        TMapCluster_1&  MapCluster_1,
        const bool      SingleDot_To_Cluster_1 = false);

    // Create set of clusters_2
    void Create_Clusters_2(
        TMapDot&        MapDot,
        TMapCluster_1&  MapCluster_1,
        TMapCluster_2&  MapCluster_2,
        const bool      SingleClusteringElem_To_Cluster_2 = false);

    // Create set of clusters_3
    void Create_Clusters_3(
        TMapDot&        MapDot,
        TMapCluster_1&  MapCluster_1,
        TMapCluster_2&  MapCluster_2,
        TMapCluster_3&  MapCluster_3,
        const bool      SingleClusteringElem_To_Cluster_3 = false);

    // Create set of clusters_4
    void Create_Clusters_4(
        TMapDot&        MapDot,
        TMapCluster_1&  MapCluster_1,
        TMapCluster_2&  MapCluster_2,
        TMapCluster_3&  MapCluster_3,
        TMapCluster_4&  MapCluster_4,
        const bool      SingleClusteringElem_To_Cluster_4 = false);
}

#endif
