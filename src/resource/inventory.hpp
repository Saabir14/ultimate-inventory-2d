#pragma once

#include "resource/inventory_item.hpp"
#include "resource/inventory_slot.hpp"

namespace godot {
class Inventory : public Resource {
	GDCLASS(Inventory, Resource)

private:
	Ref<InventorySlot> slot_template;
	TypedArray<Ref<InventorySlot>> slots;

protected:
	static void _bind_methods();

public:
	void set_slot_template(const Ref<InventorySlot> &p_slot);
	Ref<InventorySlot> get_slot_template() const;

	void set_items(const TypedArray<Ref<InventoryItem>>);
	TypedArray<Ref<InventoryItem>> get_items() const;

	int64_t size();

	Ref<InventorySlot> get_slot(int64_t p_index) const;

	void set_slot(int64_t p_index, const Ref<InventorySlot> &p_slot);
	Ref<InventoryItem> get_item(int64_t p_index) const;
};
} //namespace godot
