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
#ifndef __PMS_XCF_MAP_HPP__
#define __PMS_XCF_MAP_HPP__

#include "gimp_interface.hpp"
#include "xcf_info.hpp"

#include <string>
#include <map>

namespace pms
{
  class xcf_map
  {
  private:
    typedef std::map<std::string, xcf_info> name_to_info_type;

  public:
    xcf_map();
    xcf_map( std::string xcf_directory, gimp_interface gimp );

    void load( std::string name );

    bool has_info( std::string name ) const;
    xcf_info get_info( std::string name ) const;

    std::string to_string() const;

  private:
    std::string execute_xcfinfo_process( std::string filename ) const;

    void parse_xcf_info_header( xcf_info& info, std::string header ) const;
    void parse_xcf_info_layer( xcf_info& info, std::string layer ) const;

  private:
    name_to_info_type m_xcf_info;
    std::string m_xcf_directory;
    gimp_interface m_gimp;

  };

}

#endif
