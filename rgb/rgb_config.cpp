/*	
	Copyright(c) 2010 Johannes Jordan <johannes.jordan@cs.fau.de>.

	This file may be licensed under the terms of of the GNU General Public
	License, version 3, as published by the Free Software Foundation. You can
	find it here: http://www.gnu.org/licenses/gpl.html
*/

#include "rgb_config.h"

using namespace boost::program_options;

namespace rgb {

ENUM_MAGIC(rgb, algorithm)

RGBConfig::RGBConfig(const std::string& p)
 : Config(p), input(prefix + "input")
#ifdef WITH_SOM
 , som(prefix + "som")
#endif
{
	algo = COLOR_XYZ;
	pca_stretch = false;
	som_depth = 5;
	som_linear = false;

	#ifdef WITH_BOOST_PROGRAM_OPTIONS
		initBoostOptions();
	#endif // WITH_BOOST
}

#ifdef WITH_BOOST_PROGRAM_OPTIONS
void RGBConfig::initBoostOptions()
{
	if (!prefix_enabled) { // input/output options only with prefix
		options.add(input.options);
		options.add_options()
			(key("output,O"), value(&output_file)->default_value("output_rgb.png"),
			 "Output file name")
			;
	}
	options.add_options()
		(key("algo"), value(&algo)->default_value(algo),
		                   "Algorithm to employ: XYZ true color,\n"
		                   "PCA or SOM false-color")
		(key("pca_stretch"), bool_switch(&pca_stretch)->default_value(pca_stretch),
		                   "In PCA case: "
		                   "Maximize contrast in each channel individually")
		(key("somDepth"), value(&som_depth)->default_value(som_depth),
		                   "In SOM case: "
		                   "number of best matching neurons to incorporate")
		(key("somLinear"), bool_switch(&som_linear)->default_value(som_linear),
		                   "In SOM case: "
		                   "Use linear BMU mixing instead of weighting scheme")
		;

#ifdef WITH_SOM
	options.add(som.options);
#endif
}
#endif // WITH_BOOST_PROGRAM_OPTIONS

std::string RGBConfig::getString() const {
	std::stringstream s;

	if (prefix_enabled) {
		s << "[" << prefix << "]" << std::endl;
	} else {
		s << input.getString()
		  << "output=" << output_file << "\t# Working directory" << std::endl
			;
	}
	s	<< "algo=" << algo << "\t# Algorithm" << std::endl
		<< "pca_stretch=" << (pca_stretch ? "true" : "false") << std::endl
		<< "somDepth=" << som_depth << "\t# SOM depth" << std::endl
		<< "somLinear=" << (som_linear ? "true" : "false") << std::endl
		;
#ifdef WITH_SOM
	s << som.getString();
#endif
	return s.str();
}

}
