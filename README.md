# printstl

The single file header provides a convinent mechanism to dump a C++ standard library to either a console or a file.

To use the functionality, include the header `#include "pprintstl.h"` in one of the top level headers and then anywhere in your code, call `pprint` in namespace `pprint`.

## Example Code

```
#include <vector>
#include <memory>
#include <string>
#include <future>
#include <algorithm>
#include <initializer_list>
#include <unordered_map>
#include "pprintstl.h"
int main() {
	typedef  std::vector<std::pair<int, std::shared_ptr<int>>> VEC_UNIQUEPTR_PROMISES;
	typedef std::unordered_map<std::string, VEC_UNIQUEPTR_PROMISES> elem_type;
	elem_type some_stl;
	for (auto key : { "key1", "key2", "key3" }) {
		VEC_UNIQUEPTR_PROMISES vec;

		auto nums = { 1, 2, 3 };
		vec.resize(nums.size());
		std::transform(
			std::begin(nums),
			std::end(nums),
			std::begin(vec),
			[](auto elem) {
			return std::make_pair(elem, std::make_shared< int>(elem));
		});
		some_stl[key] = vec;
	}
	std::cout << std::is_pointer<std::shared_ptr<int>>::value << std::endl;
	std::cout << pprint::pprint<elem_type , pprint::get_type_property<elem_type>()>()(some_stl) << std::endl;
}
```
