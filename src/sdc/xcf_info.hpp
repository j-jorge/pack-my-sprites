/*
  This file is part of Pack My Sprites.

  Pack My Sprites is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 3 of the License.

  Pack My Sprites is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Pack My Sprites.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * \file
 * \brief The description of the content of an xcf file.
 * \author Julien Jorge
 */
#ifndef __SDC_XCF_INFO_HPP__
#define __SDC_XCF_INFO_HPP__

#include "layer_info.hpp"

#include <map>

namespace sdc
{
  /**
   * \brief The informations of a xcf file.
   * \author Julien Jorge
   */
  class xcf_info
  {
  public:
    /**
     * \brief The map containing the informations about the layers. The key is
     *        the name of the layer.
     */
    typedef std::map<std::string, layer_info> layer_map;

  public:
    std::string get_layer_name( std::size_t index ) const;

  public:
    /** \brief The version of the file format. */
    int version;

    /** \brief The width of the image. */
    std::size_t width;

    /** \brief The height of the image. */
    std::size_t height;

    /** \brief The layers in the image. */
    layer_map layers;

  }; // struct xcf_info
} // namespace sdc

#endif // __SDC_XCF_INFO_HPP__
