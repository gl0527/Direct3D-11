#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>

#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/../libs/unordered/examples/case_insensitive.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/tuple/to_list.hpp>
#include <boost/preprocessor/list/for_each.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/selection/max.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/logical/or.hpp>

#include "EnumFramework_Config.h"
#include "EnumFramework_Magic.h"
#include "EnumFramework_Custom.h"