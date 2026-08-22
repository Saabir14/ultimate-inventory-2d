#pragma once

#include "godot_cpp/classes/resource.hpp"
#include "resource/inventory_item.hpp"

namespace godot {

// Resource to store a single item data in a slot
// Used to process swapping of items bitween slots and other item UI interactions
class InventorySlot : public Resource {
	GDCLASS(InventorySlot, Resource)

private:
	Ref<InventoryItem> item;

protected:
	static void _bind_methods();

public:
	void set_item(const Ref<InventoryItem> &p_item);
	Ref<InventoryItem> get_item() const;
};

} //namespace godot
