#pragma once

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "inventory_item.hpp"

namespace godot {

class Inventory : public Resource {
	GDCLASS(Inventory, Resource)

private:
	TypedArray<Ref<InventoryItem>> items;

protected:
	static void _bind_methods();

public:
	void set_items(const TypedArray<Ref<InventoryItem>> &p_items);
	TypedArray<Ref<InventoryItem>> get_items() const;
};

} //namespace godot
