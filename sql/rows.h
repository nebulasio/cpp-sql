//
//  Created by Xuepeng Fan on 08/11/2017.
//

#pragma once
#include "sql/common.h"
#include "sql/ntobject.h"
#include "sql/traits.h"
#include "sql/type_list.h"

namespace neb {
namespace sql {

template <typename... ARGS>
using row_type_base = ntobject<ARGS...>;

template <typename... ARGS>
using row_collection = ntarray<ARGS...>;
}  // namespace sql
} // namespace neb
