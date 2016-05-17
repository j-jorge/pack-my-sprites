/*
  This file is part of Pack My Sprites.

  Pack My Sprites is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as published by
  the Free Software Foundation, either version 3 of the License.

  Pack My Sprites is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.
  
  You should have received a copy of the GNU Affero General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "pms/app/general_program_options.hpp"

#include "pms/app/version.hpp"

#include <claw/logger.hpp>

boost::program_options::options_description
pms::app::get_general_program_options()
{
  boost::program_options::options_description result( "General options" );
  result.add_options()
    ( "help,h", "Print this message and exit." )
    ( "version", "Print the version of the software." )
    ( "verbose", "Show informative messages during processing." );

  return result;
}

bool pms::app::parse_general_program_options
( const boost::program_options::options_description& options,
  const boost::program_options::variables_map& values,
  const std::string& program_name, const std::string& arguments_string )
{
  if ( values.count( "help" ) != 0 )
    {
      std::cout << "Usage: " << program_name << ' ' << arguments_string << '\n'
                << options;
      return false;
    }

  if ( values.count( "version" ) != 0 )
    {
      std::cout << PMS_VERSION_STRING << '\n';
      return false;
    }

  if ( values.count( "verbose" ) != 0 )
    claw::logger.set_level( claw::log_verbose );
  else
    claw::logger.set_level( claw::log_warning );
      
  return true;
}

