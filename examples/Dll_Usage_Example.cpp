/* This is a simplified example of how to use the library "SimpleClustering.dll". Using the
   clustering library is slower than using "SimpleClustering.h" and "SimpleClustering.cpp"
   directly, as there are more memory copy operations. To use the library you will need to
   compile it and then add to your project the source files "SimpleClusteringLibUser.h" and
   "SimpleClusteringLibUser.cpp". */

#include "SimpleClusteringLibUser.h"

NS_Clustering::TClustering  Clustering;       // clustering functionality
uint32_t                    ClusterizatorId;  // id of the clusterizer used in the program

Function_In_Which_Сlustering_Is_Initialized()
{
    Clustering.LoadDll("<Program_Directory_Path>"); // load clustering Dll

    // Here you can reset the settings for the clusters radius to default:
    Clustering.ClusteringElems_Set_Default_Radius();
    Clustering.ClusteringElems_Set_Default_AdditionalRadiusValue_Settings();

    // Or you can set this settings with your values:
    ClusteringElems_Set_Radius(10.0, 50.0, 150.0, 1500.0, 10000.0);
    Clustering.ClusteringElems_Set_AdditionalRadiusValue_Settings(3.0, 1);

    // Next, you need to create a new clusterer, which will be used
    ClusterizatorId = Clustering.Clusterizator_New();
}

Function_In_Which_All_Dots_Are_Prepared()
{
    Clustering.Clusterizator_Clear(ClusterizatorId); // clear your clusterizator

    for (...) // the cycle for your set of dots
    {
        double     x, y;
        uint64_t   address_of_data;
        uint64_t*  tag_address;

        ...

        /* 1. Add single dot with it coordinates and uint64_t data (for example pointer for data).
              One dot may contain a data set, so you can add this different data to it several times.
              Data will be added to std::set and will be unique.
           2. If necessary, you can get the address of the additional field of dot - Tag.
              It can store information that describes the dot.
              You can change it if some of your conditions are true. For example you can update dot
              only 10 times and no more. You can store this counter in Tag field. */
        Clustering.Clusterizator_Add_Dot(ClusterizatorId, x, y, address_of_data, tag_address);

        ...
    }
}

Function_In_Which_Clustering_Occurs_Depending_On_The_Current_Zoom_Level_On_The_Map()
{
    if (...) // if need to reload clustering elements
    {
        Clustering.Clusterizator_Clear_Clusters(ClusterizatorId); // clear all sets of clusters

        // Next, you need to IN ORDER create those clusters that should be displayed at the current map zoom level:
        if (MapControl->ZoomLevel >= VISIBLE_LEVEL_CLUSTER_1_START && MapControl->ZoomLevel <= VISIBLE_LEVEL_CLUSTER_1_END)
            Clustering.Clusterizator_Create_Clusters_1(ClusterizatorId, static_cast<int16_t>(MapControl->ZoomLevel < VISIBLE_LEVEL_DOTS_START));
        if (MapControl->ZoomLevel >= VISIBLE_LEVEL_CLUSTER_2_START && MapControl->ZoomLevel <= VISIBLE_LEVEL_CLUSTER_2_END)
            Clustering.Clusterizator_Create_Clusters_2(ClusterizatorId, static_cast<int16_t>(MapControl->ZoomLevel < VISIBLE_LEVEL_CLUSTER_1_START));
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
        const uint64_t               dot_count           = Clustering.Clusterizator_Get_ClusteringElem_Count(ClusterizatorId, *Clustering.ID_DOT);
        std::unique_ptr<double[]>    x_arr(new double[dot_count]);
        std::unique_ptr<double[]>    y_arr(new double[dot_count]);
        std::unique_ptr<int16_t[]>   in_cluster_arr(new int16_t[dot_count]);
        int16_t*                     p__in_cluster_arr   = in_cluster_arr.get();
        std::unique_ptr<uint64_t[]>  data_count_arr(new uint64_t[dot_count]);
        uint64_t*                    p__data_count_arr   = data_count_arr.get();
        std::unique_ptr<uint64_t[]>  tag_address_arr(new uint64_t[dot_count]);
        uint64_t*                    p__tag_address_arr  = tag_address_arr.get();

        // Get info about all dots
        Clustering.Clusterizator_Get_ClusteringElems(ClusterizatorId, *Clustering.ID_DOT, x_arr.get(), y_arr.get(), &p__in_cluster_arr, &p__data_count_arr, &p__tag_address_arr);

        for (int32_t i = 0; i < dot_count; ++i)
        {
            if (!in_cluster_arr[i]) // dot is not in any cluster
            {
                // Your function to create ellipse from coordinates and radius
                auto ellipse = CreateEllipse(x_arr[i], y_arr[i], *Clustering.Dot_Radius);

                ... // other actions
            }
        }
    }

    // If need to display clusters_1
    if (MapControl->ZoomLevel >= VISIBLE_LEVEL_CLUSTER_1_START && MapControl->ZoomLevel <= VISIBLE_LEVEL_CLUSTER_1_END)
    {
        const uint64_t               c1_count            = Clustering.Clusterizator_Get_ClusteringElem_Count(ClusterizatorId, *Clustering.ID_CLUSTER_1);
        std::unique_ptr<double[]>    x_arr(new double[c1_count]);
        std::unique_ptr<double[]>    y_arr(new double[c1_count]);
        std::unique_ptr<int16_t[]>   in_cluster_arr(new int16_t[c1_count]);
        int16_t*                     p__in_cluster_arr   = in_cluster_arr.get();
        std::unique_ptr<uint64_t[]>  data_count_arr(new uint64_t[c1_count]);
        uint64_t*                    p__data_count_arr   = data_count_arr.get();
        std::unique_ptr<uint64_t[]>  tag_address_arr(new uint64_t[c1_count]);
        uint64_t*                    p__tag_address_arr  = tag_address_arr.get();

        // Get info about all clusters_1
        Clustering.Clusterizator_Get_ClusteringElems(ClusterizatorId, *Clustering.ID_CLUSTER_1, x_arr.get(), y_arr.get(), &p__in_cluster_arr, &p__data_count_arr, &p__tag_address_arr);

        for (int32_t i = 0; i < c1_count; ++i)
        {
            if (!in_cluster_arr[i]) // cluster_1 is not in any cluster
            {
                // Your function to create ellipse from coordinates and radius
                auto ellipse = CreateEllipse(x_arr[i], y_arr[i], *Clustering.Cluster_1_Radius);

                ... // other actions
            }
        }
    }

    // If need to display clusters_N
    if ...
}
