/* This is a simplified example of how to use "SimpleClustering.h" and "SimpleClustering.cpp"
   directly. Direct clustering is easier and faster because fewer memory copy operations
   occur. */

#include "SimpleClustering.h"

using namespace NS_Clustering;

// To get started, you need to create variables of clustering elements:
TMapDot        MapDot;        // dots
TMapCluster_1  MapCluster_1;  // clusters_1
TMapCluster_2  MapCluster_2;  // clusters_2
TMapCluster_3  MapCluster_3;  // clusters_3
TMapCluster_4  MapCluster_4;  // clusters_4

Function_In_Which_All_Dots_Are_Prepared()
{
    // If you have a new set of dots, you first need to delete all previous clustering elements from large to smaller
    Clear_Map_Of_ClusteringElem(MapCluster_4);  // clear set of clusters_4
    Clear_Map_Of_ClusteringElem(MapCluster_3);  // clear set of clusters_3
    Clear_Map_Of_ClusteringElem(MapCluster_2);  // clear set of clusters_2
    Clear_Map_Of_ClusteringElem(MapCluster_1);  // clear set of clusters_1
    Clear_Map_Of_ClusteringElem(MapDot);        // clear set of dots

    for (...) // the cycle for your set of dots
    {
        double         x, y;
        TAnyYourClass  data_obj;
        ...

        /* Add single dot with it coordinates and uint64_t data (for example pointer for data).
           One dot may contain a data set, so you can add this different data to it several times.
           Data will be added to std::set and will be unique. */
        auto dot = MapDot.find(TCoordKey(x, y)); // find dot with specified coordinates

        if (dot == MapDot.end()) // not found
            // Create dot with data for it
            MapDot.emplace(TCoordKey(x, y), new TDot(x, y, reinterpret_cast<uint64_t>(&data_obj)));
        else // found
            // Add data to the dataset in the dot
            dot->second->SetData.insert(reinterpret_cast<uint64_t>(&data_obj));

        ...
    }
}

Function_In_Which_Clustering_Occurs_Depending_On_The_Current_Zoom_Level_On_The_Map()
{
    if (...) // if need to reload clustering elements
    {
        // Clear only clusters from large to smaller, the dots remain the same:
        Clear_Map_Of_ClusteringElem(MapCluster_4);  // clear set of clusters_4
        Clear_Map_Of_ClusteringElem(MapCluster_3);  // clear set of clusters_3
        Clear_Map_Of_ClusteringElem(MapCluster_2);  // clear set of clusters_2
        Clear_Map_Of_ClusteringElem(MapCluster_1);  // clear set of clusters_1

        // Next, you need to IN ORDER create those clusters that should be displayed at the current map zoom level:
        if (MapControl->ZoomLevel >= VISIBLE_LEVEL_CLUSTER_1_START && MapControl->ZoomLevel <= VISIBLE_LEVEL_CLUSTER_1_END)
            Create_Clusters_1(MapDot, MapCluster_1, static_cast<int16_t>(MapControl->ZoomLevel < VISIBLE_LEVEL_DOTS_START));
        if (MapControl->ZoomLevel >= VISIBLE_LEVEL_CLUSTER_2_START && MapControl->ZoomLevel <= VISIBLE_LEVEL_CLUSTER_2_END)
            Create_Clusters_2(MapDot, MapCluster_1, MapCluster_2, static_cast<int16_t>(MapControl->ZoomLevel < VISIBLE_LEVEL_CLUSTER_1_START));
        if ...

        ...
        Function_In_Which_Cluster_Information_Is_Loaded_To_Create_Graphic_Elements_For_Map();
    }
}

Function_In_Which_Cluster_Information_Is_Loaded_To_Create_Graphic_Elements_For_Map()
{
    ... // clear all graphic elements for map

    // If need to display dots
    if (MapControl->ZoomLevel >= VISIBLE_LEVEL_DOTS_START)
    {
        for (const auto& dot_pair : MapDot)
        {
            if (!dot_pair.second->InCluster) // dot is not in any cluster
            {
                // Your function to create ellipse from coordinates and radius
                auto ellipse = CreateEllipse(dot_pair.first.X, dot_pair.first.Y, Dot_Radius);

                ... // other actions
            }
        }
    }

    // If need to display clusters_1
    if (MapControl->ZoomLevel >= VISIBLE_LEVEL_CLUSTER_1_START && MapControl->ZoomLevel <= VISIBLE_LEVEL_CLUSTER_1_END)
    {
        for (const auto& c1_pair : MapCluster_1)
        {
            if (!c1_pair.second->InCluster) // cluster_1 is not in any cluster
            {
                // Your function to create ellipse from coordinates and radius
                auto ellipse = CreateEllipse(c1_pair.first.X, c1_pair.first.Y, Cluster_1_Radius);

                ... // other actions
            }
        }
    }

    // If need to display clusters_N
    if ...
}