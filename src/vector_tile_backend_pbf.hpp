#ifndef __MAPNIK_VECTOR_TILE_BACKEND_PBF_H__
#define __MAPNIK_VECTOR_TILE_BACKEND_PBF_H__

// libprotobuf - vector tile
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include "vector_tile.pb.h"
#pragma GCC diagnostic pop

// mapnik-vector-tile
#include "vector_tile_config.hpp"
#include "vector_tile_geometry_encoder.hpp"

// mapnik
#include <mapnik/value.hpp>
#include <mapnik/geometry.hpp>

// std
#include <unordered_map>
#include <set>

namespace mapnik
{

// forward declaring
class feature_impl;

namespace vector_tile_impl
{

    struct backend_pbf
    {
        typedef std::set<std::string> layers_set;
        typedef std::map<std::string, unsigned> keys_container;
        typedef std::unordered_map<mapnik::value, unsigned> values_container;
    private:
        vector_tile::Tile & tile_;
        unsigned path_multiplier_;
        mutable vector_tile::Tile_Layer * current_layer_;
        layers_set layer_names_;
        keys_container keys_;
        values_container values_;
        int32_t x_, y_;
    public:
        mutable vector_tile::Tile_Feature * current_feature_;
        
        MAPNIK_VECTOR_INLINE explicit backend_pbf(vector_tile::Tile & _tile,
                                                  unsigned path_multiplier);

        MAPNIK_VECTOR_INLINE void add_tile_feature_raster(std::string const& image_buffer);
        
        MAPNIK_VECTOR_INLINE void stop_tile_feature();
        
        MAPNIK_VECTOR_INLINE void start_tile_feature(mapnik::feature_impl const& feature);
        
        MAPNIK_VECTOR_INLINE bool start_tile_layer(std::string const& name);
        
        MAPNIK_VECTOR_INLINE unsigned get_path_multiplier() const
        {
            return path_multiplier_;
        }

        inline void stop_tile_layer() {}

        template <typename T>
        inline bool add_path(T const& path)
        {
            if (current_feature_)
            {
                return encode_geometry(path,
                                       *current_feature_,
                                       x_,
                                       y_);
            }
            // no path was added return false
            return false;
        }
    };

} // end ns vector_tile_impl

} // end ns mapnik

#if !defined(MAPNIK_VECTOR_TILE_LIBRARY)
#include "vector_tile_backend_pbf.ipp"
#endif

#endif // __MAPNIK_VECTOR_TILE_BACKEND_PBF_H__
