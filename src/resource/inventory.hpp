#pragma once

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/typed_array.hpp"

#include "resource/inventory_slot.hpp"

namespace godot {

// Inventory resource to store inventory items in a list of slots
// Use this for any inentory containing more than one item
class Inventory : public Resource {
	GDCLASS(Inventory, Resource)

private:
	// All inventory items stored in a TypedArray of Ref<InventoryItem>
	TypedArray<Ref<InventorySlot>> slots;

protected:
	static void _bind_methods();

public:
	// items set get
	void set_slots(const TypedArray<Ref<InventorySlot>> &p_slots);
	TypedArray<Ref<InventorySlot>> get_slots() const;

	// add item to inventory
};

} //namespace godot
