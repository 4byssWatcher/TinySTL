#include "polymorphic_allocator.h"

namespace TinySTL
{
	memory_resource* memory_resource::default_resource(nullptr);

	default_allocator_resource default_singleton;

	memory_resource* get_default_resource()
	{
		memory_resource* ret =
			memory_resource::default_resource;
		if (nullptr == ret)
			ret = set_default_resource(nullptr);
		return ret;
	}

	memory_resource* set_default_resource(memory_resource* r)
	{
		if (nullptr == r)
			r = addressof(default_singleton);
		memory_resource::default_resource = r;
		return r;
	}
}